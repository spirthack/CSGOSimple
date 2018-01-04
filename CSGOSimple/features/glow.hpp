#pragma once
#include "../singleton.hpp"

class Glow
    : public Singleton<Glow>
{
    friend class Singleton<Glow>;

    Glow();
    ~Glow();

public:
    void Run();
    void Shutdown();
};