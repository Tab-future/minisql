#pragma once
#include "header.h"
using namespace std;
#define INT 0
#define FLOAT 1
#define VARCHAR(N) (N+2)
#define MAX_ATTRIBUTES 32
struct key{
    string name;
    unsigned short type;
    bool primary;
    bool unique;
};
struct indexNode{
    string name;
    unsigned int belongTo;//which schema this index belongs to
    int size;
    vector<int> no;//key's order number in all keys
    vector<key> keys;//keys to build this index
};
class errorInfo
{
public:
    bool fail;
    string info;
    errorInfo():fail(1){};
    ~errorInfo() = default;
};
class schema{
public:
    unsigned short type;
    string name;
    bool deleted;
    int tupleSize;//how many records it has
    int keySize;
    vector<key> keyList;
    vector<string> primaryKey;
    unsigned short recordSize;
    unsigned int emptyIndex;
    int indexSize;
    vector<indexNode> indicies;
    schema() = default;
    schema(istream& in);//create schema from a istream
    schema(string& fileName);//create schema from file
    schema(string tableName, vector<key>& attrVec);
    schema(const char fileName[]);
    ~schema();
    bool load(istream& in);//load schema from istream
    bool store();//store schema into file
    bool addKey();//add a key by cin
    bool addKey(key& newKey);//add a key
    bool dropKey(string keyName);//drop a key
    indexNode& createIndex(string& indexName, vector<string>& keys);//add an index
    bool dropIndex(string& indexName);//drop an index
    int keyOffset(string KeyName);
};
class dataBase{
public:
    string name;//database's name
    int size;//size of schemas
    vector<string> schemas;//name of all schemas
    vector<schema> table;//the instances of all schemas
    vector<indexNode> indicies;//indicies of all schemas
    dataBase();//create database from cin
    dataBase(string& fileName);//create database from file
    dataBase(const char fileName[]);
    ~dataBase() = default;
    bool load(istream& in);
    bool store();//store database info into a file
    unsigned short createSchema(istream& in);//create a schema from an istream
    unsigned short createSchema(schema& newSchema);//copy a shcema to the database
    unsigned short createSchema(string tableName, vector<key>& attrVec);
    bool dropSchema(int no);//drop the No.no table
    bool dropSchema(string& name);//drop a schema called 'name'
    bool dropSchema(const char name[]);
    indexNode& createIndex(string& indexName, string& schemaName, vector<string>& keys);//create index
    bool dropIndex(string& indexName);//drop index
    void showSchemas(ostream& out);
};
