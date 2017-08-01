#define NOMINMAX
#include <Windows.h>

#include "valve_sdk/sdk.hpp"
#include "Helpers/Utils.hpp"
#include "Helpers/InputSys.hpp"

#include "Hooks.hpp"
#include "Menu.hpp"
#include "Options.hpp"

DWORD WINAPI on_dll_attach(LPVOID base)
{
    // 
    // Wait at most 10s for the main game modules to be loaded.
    // 
    if(Utils::WaitForModules(10000, { L"client.dll", L"engine.dll", L"shaderapidx9.dll" }) == WAIT_TIMEOUT) {
        // One or more modules were not loaded in time
        return FALSE;
    }

    Utils::AttachConsole();

    try {
        Utils::ConsolePrint("Initializing... ");

        Interfaces::Initialize();

        NetvarSys::Get().Initialize();
        InputSys::Get().Initialize();
        Menu::Get().Initialize();

        Hooks::Initialize();

        // Register some m_Hotkeys.
        // - Note:  The function that is called when the hotkey is pressed
        //          is called from the WndProc thread, not this thread.
        // 

        // Panic button
        InputSys::Get().RegisterHotkey(VK_DELETE, [base]() {
            g_Unload = true;
        });

        // Menu Toggle
        InputSys::Get().RegisterHotkey(VK_INSERT, [base]() {
            Menu::Get().Toggle();
        });

        Utils::ConsolePrint("Finished.\n");

        while(!g_Unload) 
            Sleep(1000);

        FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);

    } catch(const std::exception& ex) {
        Utils::ConsolePrint("An error occured during initialization:\n");
        Utils::ConsolePrint("%s\n", ex.what());
        Utils::ConsolePrint("Press any key to exit.\n");
        Utils::ConsoleReadKey();
        Utils::DetachConsole();

        FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);
    }

    // unreachable
    //return TRUE;
}

BOOL WINAPI on_dll_detach()
{
    Utils::DetachConsole();
    Hooks::Shutdown();

    Menu::Get().Shutdown();
    return TRUE;
}

BOOL WINAPI DllMain(
    _In_      HINSTANCE hinstDll,
    _In_      DWORD     fdwReason,
    _In_opt_  LPVOID    lpvReserved
)
{
    switch(fdwReason) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hinstDll);
        CreateThread(nullptr, 0, on_dll_attach, hinstDll, 0, nullptr);
        return TRUE;
    case DLL_PROCESS_DETACH:
        if(lpvReserved == nullptr)
            return on_dll_detach();
        return TRUE;
    default:
        return TRUE;
    }
}