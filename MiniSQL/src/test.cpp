#include "Record.h"
#include <cstdio>
#include <iostream>
int main()
{
    // BufferManager buffermanager;
    // Record_Manager recordmanger;
    // File file = recordmanger.Create_TableFile(5, 12);
    // buffermanager.Add_Table(file);
    std::string path = "../File/5.dat";
    const char* x = path.c_str();
    if(remove(x) == 0)
     std::cout << "sunccess" << std::endl;
    else
        std::cout << "fuck" << std::endl;
}