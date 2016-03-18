#pragma once

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <utility>
#include "SourceEngine/SDK.hpp"
#include "XorStr.hpp"

class DataTable {
public:
	std::unordered_map<std::string, SourceEngine::RecvProp*> m_ChildProps;
	std::unordered_map<std::string, DataTable*> m_ChildTables;
	uint32_t m_iTableOffset;
	SourceEngine::RecvTable* m_pRecvTable;
};

class NetvarManager {
private:
	NetvarManager();
	~NetvarManager();

	NetvarManager(const NetvarManager&) = delete;

public:
	///<summary>
	/// Retrieves the instance of this Singleton
	///</summary>
	static NetvarManager* Instance() {
		static NetvarManager* inst = new NetvarManager;
		return inst;
	}

	///<summary>
	/// Creates the netvar database, loading all game classes
	///</summary>
	void		CreateDatabase();

	///<summary>
	/// Erases the database
	///</summary>
	void		DestroyDatabase();

	///<summary>
	/// Loads a specific class into the database
	///</summary>
	///<param name="szClass">The class name (e.g. C_CSPlayer)</param>
	void		LoadClass(const std::string& szClass);

	///<summary>
	/// Writes a text representation of the database to the stream
	///</summary>
	///<param name="stream">The stream</param>
	void		Dump(std::ostream& stream);

	///<summary>
	/// Writes a text representation of the database to the file
	///</summary>
	///<param name="file">The file path</param>
	void		Dump(const std::string& file);

	///<summary>
	/// Checks wether the table has been loaded
	///</summary>
	///<param name="szTableName">The table name</param>
	///<returns>true if the table was loaded, false otherwise</returns>
	bool		HasBeenLoaded(const std::string& szTableName);

	///<summary>
	/// Retrieves the offset for the given prop on the table
	///</summary>
	///<param name="szTableName">The table name</param>
	///<param name="props">
	///	The prop name. 
	/// Supports multi-level search.
	/// e.g. {"DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack"}</param>
	///<returns>The prop offset if it exists, -1 otherwise</returns>
	int			GetOffset(const std::string& szTableName, const std::initializer_list<std::string>& props);
		
	///<summary>
	/// Retrieves the offset for the given prop on the table
	///</summary>
	///<param name="szTableName">The table name</param>
	///<param name="args">
	///	A variadic list with the prop name(s)
	/// Supports multi-level search.</param>
	///<returns>The prop offset if it exists, -1 otherwise</returns>
	template<typename ...Args>
	int			GetOffset(const std::string& szTableName, Args&&... args) {
		return GetOffset(szTableName, {std::forward<Args>(args)...});
	}

	int			GetNetvarCount() { return m_netvarCount; }
	int			GetTableCount() { return m_tableCount; }

private:
	void		Dump(std::ostream& output, DataTable& table, int level);
	void		DumpTable(const std::string& szTable, std::ostream& stream);
	void		DumpTable(const std::string& szTable, const std::string& file);
	void		Load_Internal(SourceEngine::RecvTable* pTable);
	DataTable*	GetNestedTable(SourceEngine::RecvTable* pTable, int offset);

	std::unordered_map<std::string, DataTable> m_TableList;

	uint32_t m_tableCount = 0;
	uint32_t m_netvarCount = 0;
};

#define GET_NETVAR(table, ...) NetvarManager::Instance()->GetOffset(table, __VA_ARGS__)
