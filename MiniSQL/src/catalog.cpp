#include"..\include\catalog.h"
schema::schema(istream& in)
{
}
schema::schema(string& fileName)
{
    string temp = "..\\src\\" + fileName + ".schema";
    ifstream file(temp);
    load(file);
    file.close();
}
schema::schema(const char fileName[])
{
    string temp(fileName);
    *this = schema(temp);
}
schema::schema(string tableName, vector<key>& attrVec)
{
    name = tableName;
    tupleSize = 0;
    keySize = attrVec.size();
    keyList = attrVec;
    recordSize = 0;
    for(int i = 0; i < keyList.size(); i++)
    {
        if(keyList[i].primary) primaryKey.push_back(keyList[i].name);
        if(keyList[i].type == INT || keyList[i].type == FLOAT) recordSize += 4;
        else recordSize += keyList[i].type - 2;
    }
    indexSize = 0;
    emptyIndex = 0;
    
}
schema::~schema()
{
}
bool schema::load(istream& in)
{
    //load head info
    in >> name >> deleted >> tupleSize >> keySize;
    //load attribute info
    recordSize = 0;
    for(int i = 0; i < keySize ; i ++)
    {
        auto temp = new key;
        in >> temp->name >> temp->type >> temp->primary >> temp->unique;
        if(temp->type == INT || temp->type == FLOAT) recordSize += 4;
        else recordSize += temp->type - 2;
        keyList.push_back(*temp);
        delete temp;

        //mark primary key
        if(keyList.back().primary) primaryKey.push_back(keyList.back().name);
    }
    in >> emptyIndex;
    //load index info
    in >> indexSize;
    for(int i = 0; i < indexSize; i++)
    {
        auto temp = new indexNode;
        in >> temp->name >> temp->size;
        temp->belongTo = type;
        for(int j = 0; j < temp->size; j++)
        {
            int keyNo;
            in >> keyNo;
            temp->no.push_back(keyNo);
            temp->keys.push_back(keyList[keyNo]);
        }
        indicies.push_back(*temp);
        delete temp;
    }
    return 1;
}
bool schema::store()
{
    string temp = "..\\src\\" + name + ".schema";
    ofstream file(temp);
    //store head info
    file << name << " "<< deleted << " " << tupleSize << " " << keySize << endl;
    //store attribute info
    for(auto i = keyList.begin();i!=keyList.end();i++)
        file << i->name << " " << i->type << " " << i->primary << " " << i->unique << endl;
    //store index info
    file << emptyIndex << " " << indexSize << endl;
    for(int i = 0; i < indexSize; i++)
    {
        file << indicies[i].name << " " << indicies[i].size << " ";
        for(int j = 0; j < indicies[i].size; j++)
            file << indicies[i].no[j] << " ";
        file << endl;
    }
    file.close();
    return 1;
}
bool schema::addKey()
{
    key newKey;
    cout << "Add a key with following input format: " << endl;
    cout << "name type(int->0, float->1, varchar(n)-> n+2) primary(0/1) unique(0/1)" << endl << endl;
    cin >> newKey.name >> newKey.type >> newKey.primary >> newKey.unique;
    return addKey(newKey);
}
bool schema::addKey(key& newKey)
{
    //if reach maximum attribute size
    if(keySize >= MAX_ATTRIBUTES) return 0;
    keyList.push_back(newKey);
    if(newKey.primary) primaryKey.push_back(newKey.name);
    keySize++;
    return 1;
}
bool schema::dropKey(string keyName)
{
    bool flag = 0;
    for(auto i = keyList.begin(); i != keyList.end(); i++)
        if(i->name == keyName && !i->primary)//index problem
        {
            keyList.erase(i);
            flag = 1;
            keySize --;
            break;
        }
    return flag;
}
indexNode& schema::createIndex(string& indexName, vector<string>& keys)
{
    errorInfo error;
    auto temp = new indexNode;
    temp->name = indexName;
    temp->belongTo = type;
    temp->size = keys.size();
    for(auto n = keys.begin(); n != keys.end(); n++)
    {
        for(int k = 0; k < keyList.size(); k++)
        {
            if(*n == keyList[k].name) {temp->no.push_back(k); temp->keys.push_back(keyList[k]);}
            if(!keyList[k].primary) {delete temp; error.info = "Using not primary attributes to create index. "; throw error;}
        }
    }
    indicies.push_back(*temp);
    delete temp;
    return indicies.back();
}
bool schema::dropIndex(string& indexName)
{
    errorInfo error;
    for(auto i = indicies.begin();i != indicies.end();i++)
        if(i->name == indexName){indicies.erase(i); return 1;}
    return 0;
}
int schema::keyOffset(string KeyName)
{
    int offset = 0;
    for(auto i = keyList.begin(); i != keyList.end(); i ++)
    {
        if(i->type == INT || i->type == FLOAT) offset += 4;
        else offset += i->type - 2;
        if(i->name == KeyName) break;
    }
    return offset;
}
dataBase::dataBase()
{
    cout << "Create a dataBase with following format: " << endl;
    cout << "dbs_name dbs_size" << endl;
    cout << "schema_1_name schema_1_deleted" << endl;
    cout << "schema_2_name schema_2_deleted" << endl;
    cout << "......" << endl;
    cin >> name;
    load(cin);
}
dataBase::dataBase(string& fileName)
{
    string temp = "..\\src\\" + fileName + ".dbs";
    ifstream file(temp);
    load(file);
    file.close();
}
dataBase::dataBase(const char fileName[])
{
    string temp(fileName);
    *this = dataBase(temp);
}
bool dataBase::load(istream& in)
{
    string schemaName;
    //load head info
    in >> name >> size;
    //load schema info
    for(int i = 0; i < size; i ++)
    {
        in >> schemaName;
        auto newSchema = new schema(schemaName);
        newSchema->type = i;
        in >> newSchema->deleted;
        table.push_back(*newSchema);
        delete newSchema;

        string& temp = table.back().name;
        schemas.push_back(temp);
        for(int j = 0; j < table.back().indexSize; j++)
            indicies.push_back(table.back().indicies[j]);
    }
    return 1;
}
bool dataBase::store()
{
    string temp = "..\\src\\" + name + ".dbs";
    ofstream hFile(temp);
    hFile << size << " " << endl;
    for(int i = 0 ; i < size ; i++)
    {
        hFile << table[i].name << " " << table[i].deleted << endl;
        table[i].store();
    }
    hFile.close();
    return 1;
}
unsigned short dataBase::createSchema(istream& in)
{
    schema newSchema = schema(in);
    newSchema.type = size;
    return createSchema(newSchema); 
}
unsigned short dataBase::createSchema(schema& newSchema)
{
    table.push_back(newSchema);
    schemas.push_back(newSchema.name);
    return size++;
}
unsigned short dataBase::createSchema(string tableName, vector<key>& attrVec)
{
    schema newSchema(tableName, attrVec);
    newSchema.type = size;
    return createSchema(newSchema);
}
bool dataBase::dropSchema(int no)
{
    table[no].deleted = 1;
    return 1;
}
bool dataBase::dropSchema(string& name)
{
    errorInfo error;
    for(int i = 0; i < size; i++)
        if(table[i].name == name) {dropSchema(i);return 1;}
    error.info = "No such schema exists. ";
    throw error;
}
bool dataBase::dropSchema(const char name[])
{
    string temp(name);
    return dropSchema(temp);
}
indexNode& dataBase::createIndex(string& indexName, string& schemaName, vector<string>& keys)
{
    unsigned short tableIndex;
    errorInfo error;
    for(auto i = table.begin(); i != table.end(); i++)
    {
        if(i->name == schemaName)
        {
            //find if name duplicates
            //if name legal
            for(unsigned short j = 0; j < i->indicies.size(); j++)
                if(indicies[j].name == indexName) {error.info = "Existed index name. "; throw error;}
            //if table legal
            for(tableIndex = 0; tableIndex < schemas.size();tableIndex++)
                if(schemas[tableIndex] == schemaName)break;
            if(tableIndex == schemas.size()) {error.info = "No such table exists. "; throw error;}
            //if primary key
            schema& thisTable = table[tableIndex];
            indicies.push_back(thisTable.createIndex(indexName, keys));
        }
    }
    return indicies.back();
}
bool dataBase::dropIndex(string& indexName)
{
    bool result = 0;
    errorInfo error;
    for(int i = 0; i < indicies.size(); i++)
        if(indicies[i].name == indexName){result = table[indicies[i].belongTo].dropIndex(indexName);}
    if(!result) {error.info = "No such index exists. ";throw error;}
    return result;
}
void dataBase::showSchemas(ostream& out)
{
    for(auto i = 0; i != schemas.size();i++)
        out << schemas[i] << " ";
    out << endl;
}