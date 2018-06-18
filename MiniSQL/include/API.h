#pragma once
#include "IndexManager_wt.h"
#include "catalog.h"
#include "BufferMange.h"
#include "GlobalVar.h"
#include "header.h"
#include "Record.h"

class dataBase;
class Record_Manager;
class API 
{
public:
	dataBase *DB;
	Record_Manager *RM;
	map<string, IndexManager<int>> int_IM;
	map<string, IndexManager<float>> float_IM;
	map<string, IndexManager<string>> string_IM;
	
	bool CreateTable(string tableName, vector<key>& attrVec);
	bool DropTable(string tableName);
	bool ShowTables();	// Show the names of all tables
	bool GetAttr(string tableName);
	bool CreateIndex(string tableName, string indexName, key attr, bool check = true);
	bool DropIndex(string indexName);
	bool SelectRecord(string tableName, vector<Cond>& condVec);
	bool InsertRecord(string tableName, vector<string> values);
	bool DeleteRecord(string tableName, vector<Cond>& condVec);
	bool IfTableExist(string TableName);
	bool IfKeyExist(string keyName);
	bool DeleteIndex(int post1, int post2, int change, string tableName);
};
