#pragma once
#include "BufferMange.h"

class Record_Manager
{
    public:
        Record_Manager() = default;
        ~Record_Manager();
        File Create_TableFile(unsigned short type, unsigned int Record_size);
        void Drop_TableFile(unsigned short type);
        void Delete_From_Table(unsigned short type);
        void Insert_Into_Table(unsigned short type, char* data, unsigned int size);
        char* Select_From_Table(unsigned short type);
    
    private:
        BufferManager buffer;
};