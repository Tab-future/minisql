#pragma once
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

#define SQL_int 0x10000
#define SQL_char(N) (0x20000 | N)
#define SQL_float 0x30000
#define SQL_point 0x40000

const int MAX_BLOCKS = 1000;
const int MAX_FILES = 1000;
const int BLOCK_SIZE = 4096;



struct FreeLists
{
    char* content;
    unsigned short record_offset;
};

struct File_Head_Read
{
    unsigned int Num_Of_Blocks;
    unsigned int Num_Of_Records;
    unsigned int Record_size;
    unsigned int Free_Record; //>=1
};

struct File_Category_Head
{
    int Num;
};


class Record
{
    public:
        unsigned int Index;
        unsigned short Type;
        unsigned short Size;
};

class Block
{
    friend class BufferManager;
    friend class LRUCache_Block;
    //friend class File;
    public:
        Block() = default;
        friend bool operator < (const Block &block1, const Block &block2);
        friend bool operator == (const Block &block1, const Block &block2);
        std::string path;
        unsigned short Filetype;
        unsigned int Block_index; //每次只有被载入buffer的时候才会读取index，也可以通过ifload判断。>=1
        //char *First_Vaild_Record;
        //std::vector<char*> FreeLists;

    private:
        //unsigned int Next_Free_Block; 
        bool IfLoad;
        bool dirty_bit;
        bool pin;
        //unsigned short Used_Size;
        char* Content;
};


class File
{
    friend class BufferManager;
    friend class LRUCache_File;
    friend class Record_Manager;
    public:
        File() = default;
        unsigned short type;
        std::string path;
        friend bool operator < (const File &file1, const File &file2);
        friend bool operator == (const File &file1, const File &file2);
    private:
        std::vector<Block> BlockArray;
        bool IfDrop;
        bool IfLoad;
        bool pin;
        bool dirty_bit;
        unsigned int Num_Of_Blocks;
        unsigned int Num_Of_Records;
        unsigned int Record_size;
        unsigned int Free_Record;  // >=1
};


struct CacheNode
{
    Block block;
    CacheNode *pre, *next;
    CacheNode(Block _block): block(_block), pre(NULL), next(NULL) {}
};

struct CacheNode2
{
    File file;
    CacheNode2 *pre, *next;
    CacheNode2(File _file): file(_file), pre(NULL), next(NULL) {}
};


class LRUCache_Block
{
    friend class BufferManager;
    public:
        LRUCache_Block(int Capacity);
        void Deal_Block(Block block);
        void Delete_For_BT(Block* block);

    private:
        int Capacity;
        CacheNode *Head, *Tail;
        std::map<Block, CacheNode*> cache;
        void Move_To_Head(CacheNode* node);
        void Remove(CacheNode* node);
        void Write_To_Disk(Block block);
};

class LRUCache_File
{
    friend class BufferManager;
    public:
        LRUCache_File(int Capacity);
        void Deal_File(File file);
        
    private:
        int Capacity;
        CacheNode2 *Head, *Tail;
        std::map<File, CacheNode2*> cache;
        void Move_To_Head(CacheNode2* node);
        void Remove(CacheNode2* node);
        void Write_To_Disk(File file);
};

class BufferManager final 
{
    friend class Record_Manager;
    public:
        BufferManager() = default;
        ~BufferManager();
        void Create_Category_File();
        void Read_Category_File();
        void Update_Category_File(unsigned short type);
        unsigned short Register_Type(std::vector<int> type);

        File Find_File(unsigned short type);
        void Read_File_Head(unsigned short type);
        void Add_Table(File file);

        char* Find_Pos_In_Block(unsigned short type, unsigned int index, unsigned int size);
        unsigned int Find_Block(unsigned short type, unsigned int index, unsigned int size);
        void Read_Block_From_Disk(File file, unsigned int block_index, unsigned int size);

        void Deal_Block(Block block);
        void Deal_File(File file);

        unsigned int Find_Pos_To_Insert(unsigned short type,  unsigned int size);

    private:
        int Total_file;
        LRUCache_Block cache = LRUCache_Block(MAX_BLOCKS);
        LRUCache_File cache1 = LRUCache_File(MAX_FILES);
        std::vector<File> ArrayFile;

        void Initial_Block(Block block, File file);
        //unsigned int Find_block_offset(File file, unsigned int index, unsigned int size);
        //unsigned short Find_record_offset(File file, unsigned int index, unsigned int size);
        //Block Blockpool[MAX_BLOCKS];
        //void Block_Initial(Block &block);
        //void Block_Delete(Block &block);
        
        // File* Find_File(const string filename);
        //Block* CreateBlock(const std::string filename);

};

