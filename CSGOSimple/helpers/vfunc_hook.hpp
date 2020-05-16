#pragma once
#define NOMINMAX
#include <Windows.h>

#include <cstdint>
#include <stdexcept>

namespace detail
{
	class protect_guard
	{
	public:
		protect_guard(LPVOID address, uint32_t size, DWORD flags) : m_address(address), m_size(size), m_flags(0)
		{
			if (!VirtualProtect(m_address, m_size, flags, &m_flags))
				throw std::system_error(GetLastError(), std::system_category(), "Failed to protect the specified page-region");
		}
		~protect_guard()
		{
			VirtualProtect(m_address, m_size, m_flags, &m_flags);
		}

	private:
		LPVOID m_address;
		uint32_t m_size;
		DWORD m_flags;
	};
}

#define INIT_MEM_PROTECT_RW( address, size ) auto protect = detail::protect_guard( address, size, PAGE_READWRITE );

inline uint32_t get_vtable_length(uintptr_t* table)
{
	auto length = uint32_t{};

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	for (length = 0; table[length]; length++)
		if (IS_INTRESOURCE(table[length]))
			break;
#else
	/// TODO: Needs a better way
	for (length = 0; table[length]; length++)
		if (!table[length])
			break;
#endif
	return length;
}

class hook_t
{
public:
	hook_t() = default;

	hook_t(uintptr_t ptr) : m_vtable(reinterpret_cast<uintptr_t**>(ptr)), m_table_length(0), m_orig(nullptr), m_replace(nullptr) {};

	hook_t(void* ptr) : m_vtable(reinterpret_cast<uintptr_t**>(ptr)), m_table_length(0), m_orig(nullptr), m_replace(nullptr) {};

	bool init()
	{
		if (!m_vtable)
			return false;

		INIT_MEM_PROTECT_RW(m_vtable, sizeof(uintptr_t));

		/// Store old vtable
		m_orig = *m_vtable;

		m_table_length = get_vtable_length(m_orig);

		/// Either faulty vtable or function fail
		if (!m_table_length)
			return false;

		/// Allocate new vtable ( +1 for RTTI )
		m_replace = std::make_unique<uintptr_t[]>(m_table_length + 1);

		/// instantiate all values with 0
		std::memset(m_replace.get(),
			NULL,
			m_table_length * sizeof(uintptr_t) + sizeof(uintptr_t));

		/// The following two memcpy's could be just made 
		/// into 1 call but for demonstration purposes
		/// I'll leave it like that

		/// Copy old table
		/// Skip first 4 bytes to later insert RTTI there
		std::memcpy(&m_replace[1],
			m_orig,
			m_table_length * sizeof(uintptr_t));

		/// Copy RTTI
		std::memcpy(m_replace.get(),
			&m_orig[-1],
			sizeof(uintptr_t));

		/// Apply new vtable, again skipping the first 4
		/// bytes since that's where the RTTI is now located
		*m_vtable = &m_replace[1];

		return true;
	}

	void hook(const uint16_t index, void* replace_function)
	{
		/// Is index out of bounds?
		if (index < 0 || index > m_table_length)
			return;

		m_replace[index + 1] = reinterpret_cast<uintptr_t>(replace_function);
	}

	template< typename t >
	t get_original(const uint16_t index)
	{
		/// Is index out of bounds?
		if (index < 0 || index > m_table_length)
			return nullptr;

		return reinterpret_cast<t>(m_orig[index]);
	}

	void unhook(const uint16_t index)
	{
		/// Is index out of bounds?
		if (index < 0 || index > m_table_length)
			return;

		m_replace[index + 1] = m_orig[index];
	}

	void unhook()
	{
		/// Check if it was already restored
		if (!m_orig)
			return;

		INIT_MEM_PROTECT_RW(m_vtable, sizeof(uintptr_t));

		*m_vtable = m_orig;

		/// Prevent double unhook
		m_orig = nullptr;
	}

private:
	uintptr_t** m_vtable;
	uint16_t m_table_length;
	uintptr_t* m_orig;
	std::unique_ptr<uintptr_t[]> m_replace;
};