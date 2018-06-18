//
// Created by hp on 2017/5/29.
//

#ifndef MINI_SQL_INTERPRETER_H
#define MINI_SQL_INTERPRETER_H

#include <iostream>
#include <cstring>
#include <vector>
#include <list>
#include <algorithm>
#include "API.h"
#include "GlobalVar.h"
using namespace std;

class Interpreter{
private:
    API *sqlSession;
    const char * str = nullptr;
public:
    Interpreter(API *api):sqlSession(api){};
    ~Interpreter(){
		
    }
    enum class TokenType{
        Integer, Real, Name, String, Ope, Null, __Keyword
    };
    struct Token {
        int begin, end;
        TokenType type;
        Token(int b, int e, TokenType t):begin(b), end(e), type(t){}
    };
    vector<Token> tokens;
    int pos = 0;
    inline bool IsNum(char x){
        return x >= '0' && x <= '9';
    }
    inline bool IsChar(char x){
        return (x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z')|| x == '_'|| x == '-';
    }
    string ReadInput();

    void tokenize();

    bool tokencmp(Token & token, const char * target);

    bool SQLSyntaxException(int state, string Error);

    bool parseSelectStatement();

    bool parseInsertStatement();

    bool parseCreateDefinition(vector<key> &Attr);

    bool parseCreateStatement();

    string ReadInput(ifstream &fin);

    bool parseSQLStatement(const char * str);

    bool parseDeleteStatement();

    bool parseExecFileStatement(string Filename);

    bool parseDropStatement();

    inline void test(){
        for(auto & x: tokens){
            for(int i = x.begin; i<= x.end; i++){
                putchar(str[i]);
            }
            printf("%d\n", x.end - x.begin + 1);
        }
    }
};


#endif //MINI_SQL_INTERPRETER_H
