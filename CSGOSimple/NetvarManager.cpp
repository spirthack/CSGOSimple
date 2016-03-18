#include "NetvarManager.hpp"

NetvarManager::NetvarManager() {
}

NetvarManager::~NetvarManager() {
     for(auto& entry : m_TableList) {
          for(auto& table : entry.second.m_ChildTables) {
               delete table.second;
          }
     }
}

void NetvarManager::CreateDatabase() {
     auto pClientDll = SourceEngine::Interfaces::Client();
     if(pClientDll) {
          m_TableList.clear();
          //Iterate over the class list
          for(auto pClientClass = pClientDll->GetAllClasses(); pClientClass; pClientClass = pClientClass->m_pNext) {
               if(pClientClass->m_pRecvTable) {
                    //If the class has a RecvTable, load it
                    Load_Internal(pClientClass->m_pRecvTable);
                    m_tableCount++;
               }
          }
     }
}

void NetvarManager::DestroyDatabase() {
     m_TableList.clear();
}

void NetvarManager::LoadClass(const std::string& szClass) {
     auto pClientDll = SourceEngine::Interfaces::Client();
     if(pClientDll) {
          //Iterate over the class list
          for(auto pClientClass = pClientDll->GetAllClasses(); pClientClass; pClientClass = pClientClass->m_pNext) {
               //If the class name matches and it hasnt been loaded, load it
               if(pClientClass->m_pRecvTable && (szClass == pClientClass->m_pNetworkName || szClass == pClientClass->m_pRecvTable->m_pNetTableName)) {
                    if(!HasBeenLoaded(pClientClass->m_pRecvTable->m_pNetTableName)) {
                         Load_Internal(pClientClass->m_pRecvTable);
                         m_tableCount++;
                    }
               }
          }
     }
}

bool NetvarManager::HasBeenLoaded(const std::string& szClass) {
     for(auto& table : m_TableList) {
          if(table.first == szClass) return true;
     }
     return false;
}

int NetvarManager::GetOffset(const std::string& szTableName, const std::initializer_list<std::string>& props) {
     typedef std::unordered_map<std::string, DataTable*> TableMap;
     typedef std::unordered_map<std::string, SourceEngine::RecvProp*> PropMap;

     auto table = m_TableList.find(szTableName);
     if(table == m_TableList.end()) return -1;

     int tableOffset = table->second.m_iTableOffset;
     if(props.size() == 0) return tableOffset;

     int totalOffset = tableOffset;

     DataTable* curTable = &table->second;

     for(uint32_t i = 0; i < props.size(); i++) {
          std::string propName = *(props.begin() + i);

          if(i + 1 < props.size()) {//This index is not the last one
               auto childTable = curTable->m_ChildTables.find(propName);
               if(childTable == curTable->m_ChildTables.end()) return -1;

               totalOffset += childTable->second->m_iTableOffset;

               curTable = childTable->second;

          } else { //Last index, retrieve prop instead of table
               auto childProp = curTable->m_ChildProps.find(propName);
               if(childProp == curTable->m_ChildProps.end()) return -1;

               totalOffset += childProp->second->m_Offset;
          }
     }

     return totalOffset;
}

void NetvarManager::Dump(std::ostream& output) {
     for(auto& entry : m_TableList) {
          if(entry.second.m_ChildProps.size() == 0 &&
               entry.second.m_ChildTables.size() == 0) //empty table
               continue;
          output << entry.first << std::endl;
          Dump(output, entry.second, 1);
          output << "==============================================================" << std::endl;
     }
}

void NetvarManager::Dump(const std::string& file) {
     std::ofstream output(file);
     Dump(output);
}

void NetvarManager::DumpTable(const std::string& szTable, std::ostream& stream) {
     for(auto& entry : m_TableList) {
          if(entry.first == szTable) {
               stream << entry.first << std::endl;
               Dump(stream, entry.second, 0);
               break;
          }
     }
}

void NetvarManager::DumpTable(const std::string& szTable, const std::string& file) {
     std::ofstream output(file);
     for(auto& entry : m_TableList) {
          if(entry.first == szTable) {
               output << entry.first << std::endl;
               Dump(output, entry.second, 0);
               break;
          }
     }
}

void NetvarManager::Dump(std::ostream& output, DataTable& table, int level) {

     char line[512] = "";
     char fmt[512] = "";

     sprintf(fmt, "%%-%ds: 0x%%08X\n", 50 - level * 4);

     for(auto& prop : table.m_ChildProps) {
          for(int i = 0; i < level; i++) {
               if(i != level - 1) {
                    output << "    ";
               } else {
                    output << "|___";
               }
          }
          sprintf(line, fmt, prop.first.c_str(), prop.second->m_Offset + table.m_iTableOffset);
          output << line;

     }
     for(auto& child : table.m_ChildTables) {
          for(int i = 0; i < level; i++) {
               if(i != level - 1) {
                    output << "    ";
               } else {
                    output << "|___";
               }
          }
          sprintf(line, fmt, child.first.c_str(), child.second->m_iTableOffset);
          output << line;
          Dump(output, *child.second, level + 1);
     }

}

void NetvarManager::Load_Internal(SourceEngine::RecvTable* pTable) {
     DataTable table;
     table.m_iTableOffset = 0;

     //Parse all the props on this table
     for(int i = 0; i < pTable->m_nProps; i++) {
          SourceEngine::RecvProp* pProp = &pTable->m_pProps[i];

          //Skip trash array items
          if(!pProp || isdigit(pProp->m_pVarName[0])) continue;
          //We dont care about the base class
          if(strcmp(pProp->m_pVarName, "baseclass") == 0) continue;


          //If this prop is a table
          if(pProp->m_RecvType == SourceEngine::SendPropType::DPT_DataTable &&  //If it is a table AND
               pProp->m_pDataTable != NULL &&                                   //The DataTable isnt null AND
               pProp->m_pDataTable->m_pNetTableName[0] == 'D') {                //The Table name starts with D (this is because there are some shitty nested 
                                                                                //tables that we want to skip, and those dont start with D)

               //Load it into the children list of this table
               table.m_ChildTables.insert(std::make_pair(pProp->m_pVarName, GetNestedTable(pProp->m_pDataTable, pProp->m_Offset))); 

          } else { //Its not a table, its a regular prop
               //Append it to the prop list of this table
               table.m_ChildProps.insert(std::make_pair(pProp->m_pVarName, pProp));
          }
          m_netvarCount++;
     }
     //Append this table to the main table list
     m_TableList.insert(std::make_pair(pTable->m_pNetTableName, table));
}

DataTable* NetvarManager::GetNestedTable(SourceEngine::RecvTable* pTable, int offset) {
     DataTable* tableData = new DataTable;
     tableData->m_iTableOffset = offset;
     tableData->m_pRecvTable = pTable;

     for(int i = 0; i < pTable->m_nProps; i++) {
          SourceEngine::RecvProp* pProp = &pTable->m_pProps[i];

          if(!pProp || isdigit(pProp->m_pVarName[0])) continue;
          if(strcmp(pProp->m_pVarName, "baseclass") == 0) continue;

          if(pProp->m_RecvType == SourceEngine::SendPropType::DPT_DataTable &&
               pProp->m_pDataTable != NULL &&
               pProp->m_pDataTable->m_pNetTableName[0] == 'D') {
               tableData->m_ChildTables.insert(std::make_pair(pProp->m_pVarName, GetNestedTable(pProp->m_pDataTable, pProp->m_Offset)));
          } else {
               tableData->m_ChildProps.insert(std::make_pair(pProp->m_pVarName, pProp));
          }
          m_netvarCount++;
     }
     m_tableCount++;
     return tableData;
}
