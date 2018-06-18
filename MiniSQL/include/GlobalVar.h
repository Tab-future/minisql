#ifndef GLOBALVAR
#define GLOBALVAR
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <cstring>
#include <stdlib.h>
using namespace std;


#define MAX_ATTR_NUM 32
#define MAX_CHAR_NUM 255
#define MAX_PAGE_SIZE 4096

enum Operator{
    EQUAL,
    NOT_EQ,
    LESS,
    MORE,
    LESS_EQ,
    MORE_EQ
};
enum Type{
    INT_,
    FLOAT_,
    CHAR_
};

struct PageNode{
public:
    bool Pin;
    bool State; // if state == 1, then it is the bottom of the file node;
    // else state == 0;
    int OffsetNum;
    char *FileName;
    bool Dirty;
    friend class BufferManager;

private:
    char *ContentAddr;
    size_t UsedBytes;
    PageNode *PrePage;
    PageNode *NextPage;

};

struct FileNode
{
    char *FileName;
    bool Pin; // the flag that this file is locked
	int Page_Num;
	size_t File_Size;
    PageNode *PageHead;
};

class Cond{
public:
    string AttrName;
    string Value;
    Operator Op;

    Cond(string name = "", string value = "", Operator o = EQUAL):AttrName(name), Value(value), Op(o){};

    inline bool Check(double target){
        double temp = atof(this->Value.c_str());
        switch (Op){
            case EQUAL:
                return target == temp;
            case NOT_EQ:
                return target != temp;
            case LESS:
                return target < temp;
            case MORE:
                return target > temp;
            case LESS_EQ:
                return target <= temp;
            case MORE_EQ:
                return target >= temp;
        }
    }

    inline bool Check(int target){
        int temp = atoi(this->Value.c_str());
        switch (Op){
            case EQUAL:
                return target == temp;
            case NOT_EQ:
                return target != temp;
            case LESS:
                return target < temp;
            case MORE:
                return target > temp;
            case LESS_EQ:
                return target <= temp;
            case MORE_EQ:
                return target >= temp;
        }
    }

    inline bool Check(string target){
        switch (Op){
            case EQUAL:
                return target == Value;
            case NOT_EQ:
                return target != Value;
            case LESS:
                return target < Value;
            case MORE:
                return target > Value;
            case LESS_EQ:
                return target <= Value;
            case MORE_EQ:
                return target >= Value;
        }
    }

};

class Attribute {
public:
	string AttrName;
	Type type;
	bool Unique;
	string indexname;
	int size;
	bool primary;
	char Attr_name_c[100];
	char Index_name_c[100];

	Attribute(string n = "", Type t = CHAR_, bool state = false, string i = "", int s = 0,bool state2 = false) :AttrName(n), type(t), Unique(state), indexname(i), size(s),primary(state2) 
	{
		memset(Attr_name_c, 0, sizeof(Attr_name_c));
		memcpy(Attr_name_c, AttrName.c_str(), AttrName.size());
		memset(Index_name_c, 0, sizeof(Index_name_c));
		memcpy(Index_name_c, indexname.c_str(), indexname.size());
	};
};
#endif //GLOBALVAR
