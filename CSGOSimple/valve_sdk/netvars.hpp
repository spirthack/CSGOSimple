#pragma once

#include <memory>
#include <iostream>
#include <unordered_map>
#include <map>

#include "Misc/Recv.hpp"
#include "misc/FnvHash.hpp"
#include "../Singleton.hpp"

class NetVarManager
    : public Singleton<NetVarManager>
{
    struct StoredPropData
    {
        RecvProp* prop_ptr;
        std::uint16_t class_relative_offset;
    };
public:
    void Initialize();
public:
    std::uint16_t GetOffset(unsigned int hash) { return m_props.at(hash).class_relative_offset; }
    RecvProp* GetPropPtr(unsigned int hash) { return m_props.at(hash).prop_ptr; }
public:
    // Prevent instruction cache pollution caused by automatic
    // inlining of Get and GetOffset every netvar usage when there
    // are a lot of netvars
    __declspec(noinline) static std::uint16_t GetOffsetByHash(const fnv::hash hash)
    {
        return Get().GetOffset(hash);
    }

    template<fnv::hash Hash>
    static auto GetOffsetByHashCached() -> std::uint16_t
    {
        static auto offset = std::uint16_t(0);
        if (!offset)
            offset = GetOffsetByHash(Hash);
        return offset;
    }

private:
    void DumpRecursive(const char* base_class, RecvTable* table, std::uint16_t offset);
private:
    std::map<fnv::hash, StoredPropData> m_props;
};

#define PNETVAR_OFFSET(type, funcname, class_name, var_name, offset) \
auto funcname() -> std::add_pointer_t<type> \
{ \
	constexpr auto hash = fnv::hash_constexpr(class_name "->" var_name); \
	const auto addr = std::uintptr_t(this) + offset + NetVarManager::GetOffsetByHashCached<hash>(); \
	return reinterpret_cast<std::add_pointer_t<type>>(addr); \
}

#define PNETVAR(type, funcname, class_name, var_name) \
	PNETVAR_OFFSET(type, funcname, class_name, var_name, 0)

#define NETVAR_OFFSET(type, funcname, class_name, var_name, offset) \
auto funcname() -> std::add_lvalue_reference_t<type> \
{ \
	constexpr auto hash = fnv::hash_constexpr(class_name "->" var_name); \
	const auto addr = std::uintptr_t(this) + offset + NetVarManager::GetOffsetByHashCached<hash>(); \
	return *reinterpret_cast<std::add_pointer_t<type>>(addr); \
}

#define NETVAR(type, funcname, class_name, var_name) \
	NETVAR_OFFSET(type, funcname, class_name, var_name, 0)

#define NETPROP(funcname, class_name, var_name) \
static auto funcname() ->  RecvProp* \
{ \
	constexpr auto hash = fnv::hash_constexpr(class_name "->" var_name); \
	static auto prop_ptr = NetVarManager::Get().GetPropPtr(hash); \
	return prop_ptr; \
}