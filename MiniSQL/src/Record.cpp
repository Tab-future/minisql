#include "..\include\Record.h"

using namespace std;

Record_Manager::~Record_Manager()
{

}

File Record_Manager::Create_TableFile(unsigned short type, unsigned int Record_size)
{
    string type_To_String;
    ostringstream convert;
    convert << type;
    type_To_String = convert.str();
    fstream fs;
    string path = "../File/" + type_To_String + ".dat";
    fs.open(path, ios::binary | ios::trunc | ios::out);
    fs.close();
    ofstream os(path, ios::binary);
    File_Head_Read file_head;
    file_head.Free_Record = 0;
    file_head.Num_Of_Blocks = 0;
    file_head.Num_Of_Records = 0;
    file_head.Record_size = Record_size;
    os.write(reinterpret_cast<char*>(&file_head), sizeof(File_Head_Read));
    File file;
    file.IfDrop = false;
    file.IfLoad = false;
    file.dirty_bit = false;
    file.pin = false;
    file.Num_Of_Blocks = 0;
    file.Num_Of_Records = 0;
    file.Free_Record = 0;
    file.Record_size = Record_size;
    file.type = type;
    file.path = path;
    return file;
}

void Record_Manager::Delete_From_Table(unsigned short type)
{

}

void Record_Manager::Drop_TableFile(unsigned short type)
{
    //Delete_From_Table(type);
    File file = buffer.ArrayFile[type];
    file.IfDrop = true;
    
}

void Record_Manager::Insert_Into_Table(unsigned short type, char* data, unsigned int size)
{
    File file = buffer.ArrayFile[type];
    unsigned int index =  buffer.Find_Pos_To_Insert(type, size);
    //unsigned int block_index = buffer.Find_Block(type, index, size);
    char* pos = buffer.Find_Pos_In_Block(type, index, size);

}