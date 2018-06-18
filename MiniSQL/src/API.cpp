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
}
bool API::DropIndex(string indexName)
{
    //catalog begin
    DB->dropIndex(indexName);//if success, returns 1, else throw an error
    //catalog end
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
