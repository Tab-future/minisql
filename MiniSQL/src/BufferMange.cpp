#include "..\include\BufferMange.h"
#include <iostream>
#include <cmath>

using namespace std;


LRUCache_Block::LRUCache_Block(int Capacity)
{
    this -> Capacity = Capacity;
    Head = NULL;
    Tail = NULL;
}
        
void LRUCache_Block::Deal_Block(Block block)
{
    block.dirty_bit = true;
    if(block.IfLoad == true)
    {
        auto result = cache.find(block);
        CacheNode *node = result -> second;
        Remove(node);
        Move_To_Head(node);
    }
    else
    {
        block.IfLoad == true;
        if(cache.size() < Capacity)
        {
            CacheNode * newnode = new CacheNode(block);
            Move_To_Head(newnode);
            cache[block] = newnode;
        }
        else
        {
            auto result = cache.find(Tail -> block);
            CacheNode *node = result -> second;
            while(!node)
            {
                if((node -> block).pin)
                    node = node -> pre;
                else
                {
                    Remove(node);
                    auto result2 = cache.find(node -> block);
                    Write_To_Disk(node -> block);
                    cache.erase(result2);
                    break;
                }
            }
            if(node == NULL)
            {
                cout << "Error: The block cache is full, and all the blocks are locked!" << endl;
                return;
            }
            CacheNode* newnode = new CacheNode(block);
            Move_To_Head(newnode);
            cache[block] = newnode;
        }
    }
}

void LRUCache_Block::Move_To_Head(CacheNode* node)
{
    node -> next = Head;
    node -> pre = NULL;

    if (Head != NULL)
    {
        Head -> pre = node;
    }
    Head = node;
    if (Tail == NULL)
    {
        Tail = Head;
    }
}

void LRUCache_Block::Remove(CacheNode* node)
{
    if (node -> pre != NULL)
    {
        node -> pre -> next = node -> next;
    }
    else
    {
        Head = node -> next;
    }
    if (node -> next != NULL)
    {
        node -> next -> pre = node -> pre;
    }
    else
    {
        Tail = node -> pre;
    }
}

void LRUCache_Block::Delete_For_BT(Block* block)
{
    CacheNode * node = new CacheNode(*block);
    Remove(node);
    auto result = cache.find(node -> block);
    cache.erase(result);
}

void LRUCache_Block::Write_To_Disk(Block block)
{
    if(block.dirty_bit == false)
        return;
    else
    {
        string path = "../File/" + block.path + ".dat";
        ofstream os(path, ios::binary);
        os.seekp(BLOCK_SIZE * (block.Block_index - 1), ios::beg);
        os.write(reinterpret_cast<char*>(block.Content), BLOCK_SIZE * sizeof(char));
    }
}

LRUCache_File::LRUCache_File(int Capacity)
{
    this -> Capacity = Capacity;
    Head = NULL;
    Tail = NULL;
}

void LRUCache_File::Deal_File(File file)
{
    file.dirty_bit = true;
    if(file.IfLoad == true)
    {
        auto result = cache.find(file);
        CacheNode2 *node = result -> second;
        Remove(node);
        Move_To_Head(node);
    }
    else
    {
        file.IfLoad == true;
        if(cache.size() < Capacity)
        {
            CacheNode2 * newnode = new CacheNode2(file);
            Move_To_Head(newnode);
            cache[file] = newnode;
        }
        else
        {
            auto result = cache.find(Tail -> file);
            CacheNode2 *node = result -> second;
            while(!node)
            {
                if((node -> file).pin)
                    node = node -> pre;
                else
                {
                    Remove(node);
                    auto result2 = cache.find(node -> file);
                    Write_To_Disk(node -> file);
                    cache.erase(result2);
                    break;
                }
            }
            if(node == NULL)
            {
                cout << "Error: The file cache is full, and all the files are locked!" << endl;
                return;
            }
            CacheNode2* newnode = new CacheNode2(file);
            Move_To_Head(newnode);
            cache[file] = newnode;
        }
    }
}

void LRUCache_File::Move_To_Head(CacheNode2* node)
{
    node -> next = Head;
    node -> pre = NULL;

    if (Head != NULL)
    {
        Head -> pre = node;
    }
    Head = node;
    if (Tail == NULL)
    {
        Tail = Head;
    }
}

void LRUCache_File::Remove(CacheNode2* node)
{
    if (node -> pre != NULL)
    {
        node -> pre -> next = node -> next;
    }
    else
    {
        Head = node -> next;
    }
    if (node -> next != NULL)
    {
        node -> next -> pre = node -> pre;
    }
    else
    {
        Tail = node -> pre;
    }
}

void LRUCache_File::Write_To_Disk(File file)
{
    if(file.dirty_bit == false)
        return;
    else
    {
        ofstream os(file.path, ios::binary);
        File_Head_Read head;
        head.Free_Record = file.Free_Record;
        head.Num_Of_Blocks = file.Num_Of_Blocks;
        head.Num_Of_Records = file.Num_Of_Records;
        head.Record_size = file.Record_size;
        os.write(reinterpret_cast<char*>(&head), sizeof(File_Head_Read));
    }
}

BufferManager::~BufferManager()
{

}

void BufferManager::Deal_Block(Block block)
{
    cache.Deal_Block(block);
}

void BufferManager::Deal_File(File file)
{
    cache1.Deal_File(file);
}

void BufferManager::Create_Category_File()
{
    fstream fs;
    fs.open("../File/File_Category.dat", ios::binary | ios::trunc | ios::out);
    fs.close();
}

void BufferManager::Read_Category_File()
{
    File_Category_Head head;
    ifstream is("../File/File_Category.dat", ios::binary);
    is.read(reinterpret_cast<char*>(&head), sizeof(File_Category_Head));
    for(int i = 0; i != head.Num; i++)
    {
        unsigned short type;
        File file;
        is.read(reinterpret_cast<char*>(&type), sizeof(type));
        file.type = type;
        ArrayFile.emplace_back(file);
        //Initial_File(file);
    }
}

void BufferManager::Update_Category_File(unsigned short type)
{
    File_Category_Head head;
    head.Num = ArrayFile.size();
    ofstream os("../File/File_Category.dat", ios::binary);
    os.write(reinterpret_cast<char*>(&head), sizeof(File_Category_Head));
    for(auto &val : ArrayFile)
    {
        unsigned short type;
    }
    
    os.seekp(0, ios::end);
}

unsigned short BufferManager::Register_Type(vector<int> type)
{

}

void BufferManager::Initial_Block(Block block, File file)
{
    block.Filetype = file.type;
    block.dirty_bit = false;
    block.IfLoad = false;
    block.pin = false;
    block.Content = NULL;
    block.path = file.path;
}

File BufferManager::Find_File(unsigned short type)
{
    File file = ArrayFile[type];
    return file;
}

void BufferManager::Read_File_Head(unsigned short type)
{
    File file = ArrayFile[type];
    file.IfLoad = true;
    File_Head_Read file_head;
    ifstream is(file.path, ios::binary);
    is.read(reinterpret_cast<char*>(&file_head), sizeof(File_Head_Read));
    file.Num_Of_Blocks = file_head.Num_Of_Blocks;
    file.Num_Of_Records = file_head.Num_Of_Records;
    file.Free_Record = file_head.Free_Record;
    file.Record_size = file_head.Record_size;
    for(int i = 0; i < file.Num_Of_Blocks; i++)
    {
        Block block;
        Initial_Block(block, file);
        file.BlockArray.emplace_back(block);
    }
}

void BufferManager::Add_Table(File file)
{
    if(ArrayFile.size() == file.type)
        ArrayFile.emplace_back(file);
    else
        ArrayFile[file.type] = file;
}

char* BufferManager::Find_Pos_In_Block(unsigned short type, unsigned int index, unsigned int size)
{
    File file = ArrayFile[type];
    Block block;
    int num_of_blocks = file.Num_Of_Blocks;
    int num_of_records = file.Num_Of_Records;
    int records_each_block = floor(BLOCK_SIZE / size);
    int block_index = ceil(index / records_each_block);
    block = file.BlockArray[block_index - 1];
    int pos_in_block = index - records_each_block * (block_index - 1);
    char* pos = block.Content + size * (pos_in_block - 1);
    return pos;
}

unsigned int BufferManager::Find_Block(unsigned short type, unsigned int index, unsigned int size)
{
    File file = ArrayFile[type];
    Block block;
    int num_of_blocks = file.Num_Of_Blocks;
    int num_of_records = file.Num_Of_Records;
    int records_each_block = floor(BLOCK_SIZE / size);
    int block_index = ceil(index / records_each_block);
    //block = file.BlockArray[block_index - 1];
    return block_index;
}

void BufferManager::Read_Block_From_Disk(File file, unsigned int block_index, unsigned int size)
{
    Block block = file.BlockArray[block_index - 1];
    ifstream is(file.path, ios::binary);
    is.seekg(BLOCK_SIZE * block_index, ios::beg);
    is.read(reinterpret_cast<char*>(block.Content), BLOCK_SIZE * sizeof(char));
    block.Block_index = block_index; // >=1
    // int records_each_block = floor(BLOCK_SIZE / size);
    // int record_size = size - sizeof(unsigned int);
    // char* content;
    // unsigned int free_record;
    // for(int i = 0; i < records_each_block; i++)
    // {
    //     is.read(reinterpret_cast<char*>(content), record_size * sizeof(char));
    //     block.Content = content;
    //     is.read(reinterpret_cast<char*>(free_record), record_size * sizeof(unsigned int));

    // }
}

unsigned int BufferManager::Find_Pos_To_Insert(unsigned short type, unsigned int size)
{
    File file = ArrayFile[type];
    if(file.Free_Record == 0)
        return file.Num_Of_Records;
    else
    {
        int records_each_block = floor(BLOCK_SIZE / size);
        int block_index = ceil(file.Free_Record / records_each_block);
        Block block = file.BlockArray[block_index - 1];
        if(block.IfLoad == false)
        {
            Read_Block_From_Disk(file, block_index, size);
        }
        int pos_in_block = file.Free_Record - records_each_block * (block_index - 1);
        char* free_record = block.Content + size * (pos_in_block - 1) + (size - sizeof(unsigned int));
        unsigned int* next_free_record = reinterpret_cast<unsigned int*>(free_record);
        unsigned int return_index = file.Free_Record;
        file.Free_Record = *next_free_record;
        return return_index;
    }
}



bool operator < (const Block &block1, const Block &block2)
{
    return (block1.Filetype < block2.Filetype || (block1.Filetype == block2.Filetype && block1.Block_index < block2.Block_index));
}

bool operator == (const Block &block1, const Block &block2)
{
    return (block1.Filetype == block2.Filetype && block1.Block_index == block2.Block_index);
}

bool operator < (const File &file1, const File &file2)
{
    return (file1.type < file2.type);
}

bool operator == (const File &file1, const File &file2)
{
    return (file1.type == file2.type);
}