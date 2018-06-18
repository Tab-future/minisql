#include "..\include\API.H"

bool API::CreateTable(string tableName, vector<key>& attrVec)
{
    //catalog begin
    DB->createSchema(tableName, attrVec);//returns the type of table
    //catalog end
}
bool API::DropTable(string tableName)
{
    //catalog begin
    if(IfTableExist(tableName)) DB->dropSchema(tableName);//return 1 if succees, else throw an error
    //catalog end
}
bool API::ShowTables()// Show the names of all tables
{
    //catalog begin
    DB->showSchemas(cout);
    //catalog end
}	
bool API::CreateIndex(string tableName, string indexName, key attr, bool check = true)
{
    //catalog begin
    vector<string> attrVec;
    attrVec.push_back(attr.name);
    DB->createIndex(indexName, tableName, attrVec);//if success, returns the resference of created index, else throw an error
    //catalog end
	//index begin
	if (check)
	{
		// check if we can create index
	}
	vector<key> attrVec;
	// I need to get the attrVec corresponding with the tableName(assign to attrVec)
	for (int i = 0; i < attrVec.size(); i++)
	{
		if (attrVec[i] == attr.name)
		{
			switch (attr.type)
			{
			case(0)://the key is int
			{
				IndexManager<int> tmp(tableName, indexName, attr.name, sizeof(int));
				int_IM.insert(make_pair(indexName, tmp));
				vector<KEY<int>> record;
				// I need to get the record corresponding with the attribute.(assign to record)
				for (int j = 0; j < record.size(); j++)
				{
					tmp.InsertElement(record[j].value, record[j].offset);
				}
				break;
			}
			case(1)://the key is float
			{
				IndexManager<float> tmp(tableName, indexName, attr.name, sizeof(float));
				float_IM.insert(make_pair(indexName, tmp));
				vector<KEY<float>> record;
				// I need to get the record corresponding with the attribute.(assign to record)
				for (int j = 0; j < record.size(); j++)
				{
					tmp.InsertElement(record[j].value, record[j].offset);
				}
				break;
			}
			default://the key is char(n)/string
			{
				IndexManager<string> tmp(tableName, indexName, attr.name, sizeof(char) * 256);
				string_IM.insert(make_pair(indexName, tmp));
				vector<KEY<string>> record;
				// I need to get the record corresponding with the attribute.(assign to record)
				for (int j = 0; j < record.size(); j++)
				{
					tmp.InsertElement(record[j].value, record[j].offset);
				}
				break;
			}
			}
			break;
		}
	}
	return true;
	//index end
}
bool API::DropIndex(string indexName)
{
    //catalog begin
    DB->dropIndex(indexName);//if success, returns 1, else throw an error
    //catalog end
	//index begin
	indexNode idxNode;//I need to get the indexNode from the catalog.h
	switch (idxNode.keys[0].type)
	{
	case(0)://the key is int
	{
		int_IM[indexName].~IndexManager();
		int_IM.erase(indexName);
	}
	case(1)://the key is float
	{
		float_IM[indexName].~IndexManager();
		float_IM.erase(indexName);
	}
	default://the key is char(n)/string
	{
		string_IM[indexName].~IndexManager();
		string_IM.erase(indexName);
	}
	break;
	}
	//index end
}
bool API::SelectRecord(string tableName, vector<Cond>& condVec)
{
}
bool API::InsertRecord(string tableName, vector<string> values)
{
}
bool API::DeleteRecord(string tableName, vector<Cond>& condVec)
{
}
bool API::IfTableExist(string TableName)
{
    //catalog begin
    for(auto i = DB->schemas.begin(); i != DB->schemas.end(); i++)
        if(*i == TableName) return 1;
    return 0;
    //catalog ends
}
bool API::IfKeyExist(string TableName, string KeyName)
{
    //catalog begin
    errorInfo error;
    vector<schema>::iterator i;
    for(i = DB->table.begin();i != DB->table.end(); i++)
        if(i->name == TableName) break;
    if(i == DB->table.end()){error.info = "Table exists, but key not exist. "; throw error;}
    schema& ThisTable = *i;
    for(auto j = ThisTable.keyList.begin(); j != ThisTable.keyList.end(); j++)
        if(j->name == KeyName) return 1;
    return 0;
    //catalog end
}
bool API::DeleteIndex(int post1, int post2, int change, string tableName)
{
}
void API::GetAttr(string& TableName, vector<key>& attr)
{
	int tableIndex = DB->FindTableIndex(TableName);
	if(tableIndex >= 0) attr = DB->table[tableIndex].keyList;
}
bool API::DescribleTable(string& tableName)
{
	int tableIndex = DB->FindTableIndex(tableName);
	if(tableIndex >= 0) return DB->table[tableIndex].DescribeTable();
	else 
	{
		cout << "No such table exists. " << endl;
		return 0;
	}
}