//
// Created by hp on 2017/5/29.
//
#include <ctype.h>
#include "..\include\Interpreter.h"
#include "stdlib.h"
#include <string.h>
#include <stdio.h>
#include <fstream>

bool Interpreter::parseSQLStatement(const char * str)
{
    this->str = str;
    tokens.clear();
    this->tokenize();
    pos = 0;
    Token token = tokens[pos];
    pos++;
    if(tokencmp(token, "select"))
        return parseSelectStatement();
    else if(tokencmp(token, "drop"))
        return parseDropStatement();
    else if(tokencmp(token, "delete"))
        return parseDeleteStatement();
    else if(tokencmp(token, "insert"))
        return parseInsertStatement();
    else if(tokencmp(token, "create"))
        return parseCreateStatement();
    else if(tokencmp(token, "show")){
        token  = tokens[pos++];
        if(tokencmp(token, "tables" )&& tokens.size() == 3){
            return sqlSession->ShowTables();
        } else {
            return SQLSyntaxException(2, "syntax error");
        }
    }else if(tokencmp(token, "describe")){
        token = tokens[pos++];
        if(tokencmp(token, "table"))
        {
            token = tokens[pos++];
            string name(str, token.begin, token.end-token.begin+1);
            if(token.type == TokenType::Name && tokens.size() == 4){
                return sqlSession->DescribleTable(name);
            } else
                return SQLSyntaxException(2, "syntax error");
        } else
            return SQLSyntaxException(2, "syntax error, it must be \'table\'");
	}
	else if (tokencmp(token, "quit") && tokencmp(tokens[pos], ";")) {
		map<string, IndexManager<int> >::iterator iter_1;
		for (iter_1 = sqlSession->int_IM.begin(); iter_1 != sqlSession->int_IM.end(); iter_1++) {
			iter_1->second.write();
		}
		map<string, IndexManager<float> >::iterator iter_2;
		for (iter_2 = sqlSession->float_IM.begin(); iter_2 != sqlSession->float_IM.end(); iter_2++) {
			iter_2->second.write();
		}
		map<string, IndexManager<string> >::iterator iter_3;
		for (iter_3 = sqlSession->string_IM.begin(); iter_3 != sqlSession->string_IM.end(); iter_3++) {
			iter_3->second.write();
		}
		exit(0);
	}
    else if(tokencmp(token, "execfile")){
        token = tokens[pos];
        pos++;
        string filename(str, token.begin, token.end-token.begin + 1);
        if(tokencmp(tokens[pos], ";") && tokens.size() == 3)
        {
			cout << " execute the file\'" << filename << endl;
            return parseExecFileStatement(filename);
        }else return SQLSyntaxException(2, "Syntax error");
    }else return SQLSyntaxException(2, "Syntax error");

        return SQLSyntaxException(2, "keyword error");


}

bool Interpreter::parseExecFileStatement(string Filename){

    string comand;
    ifstream fin(Filename.c_str());
    if(!fin.is_open()) {
        cout <<" failed to open the file" << endl;
		return false;
	}
    int num = 0;
    while(!fin.eof()){
        comand = ReadInput(fin);
        if(comand == "")
            break;
        if(parseSQLStatement(comand.c_str()) == false) {
            cout << "Fail to execute the file" << endl;
            return false;
        }
    }
	cout << "File\'" << Filename << "\'has been executed;" << endl;
    fin.close();
	return true;
}
string Interpreter::ReadInput(ifstream &fin){
    string temp = "";
    string sql = "";

    while(temp.find(";") == temp.npos){
        fin >> temp;
        sql += temp;
        sql += " ";
    }
    return sql;
}
string Interpreter::ReadInput() {
    string temp = "";
    string sql = "";
    while(temp.find(";") == temp.npos){
        cin >> temp;
        sql += temp;
        sql += " ";
    }

    return  sql;
}
bool Interpreter::parseDeleteStatement()
{
    bool flag;
    


    Token token = tokens[pos];
    if(!tokencmp(token, "from")) return SQLSyntaxException(2, "syntax error");
    pos ++; token = tokens[pos];//FROM
    if(token.type != TokenType::Name)
        return SQLSyntaxException(2, "syntax error");
    string TableName(str, token.begin, token.end - token.begin + 1);
    if(sqlSession->IfTableExist(TableName) == false) return SQLSyntaxException(0,"No Such Table");

    pos ++; token = tokens[pos];//tableName
    vector<Cond> condVec;
    if(str[token.begin] == ';')
        return sqlSession->DeleteRecord(TableName, condVec);
    else if(!tokencmp(token, "where")) return SQLSyntaxException(2, "Syntax error");

    vector<key> Attr;
    sqlSession->GetAttr(TableName, Attr);
    pos++; token = tokens[pos];
    while(str[token.begin] != ';') {
        flag = false;
        Cond *temp = new Cond();
        unsigned short t;
        if(token.type != TokenType::Name) return SQLSyntaxException(2, "Syntax error");
        string AName(str, token.begin, token.end - token.begin +1);
        for(int i = 0; i < Attr.size(); i++)
            if(Attr[i].name == AName)
            {
                temp->AttrName = AName;
                t = Attr[i].type;
                flag = true;
                break;
            }
        if(!flag) return SQLSyntaxException(0, "No Such Attribute");

        pos++; token = tokens[pos];
        if(token.type != TokenType::Ope)
            return SQLSyntaxException(2, "Syntax error");
        else if(tokencmp(token, "<")){
            temp->Op = LESS;
        }
        else if(tokencmp(token, ">")){
            temp->Op = MORE;
        }
        else if(tokencmp(token, ">=")){
            temp->Op = MORE_EQ;
        }
        else if(tokencmp(token, "<=")){
            temp->Op = LESS_EQ;
        }
        else if(tokencmp(token, "<>") || tokencmp(token, "!=")){
            temp->Op = NOT_EQ;
        }
        else if(tokencmp(token, "=")){
            temp->Op = EQUAL;
        }else return SQLSyntaxException(2, "illegal opes");

        pos++; token = tokens[pos];
        string tempValue(str, token.begin, token.end - token.begin +1);
        if(token.type == TokenType::Integer){
			if (t == INT_)
				temp->Value = tempValue;
			else if (t == FLOAT_)
				temp->Value = tempValue;
            else return SQLSyntaxException(1, "type error");
        }else if(token.type == TokenType::Real ){
            if(t == FLOAT_)
                temp->Value = tempValue;
            else return SQLSyntaxException(1, "type error");
        }else if(token.type == TokenType::String){
            if(t == CHAR_)
                temp->Value = tempValue.substr(1, tempValue.size()-2);
            else
                return SQLSyntaxException(1, "type error");
        }else
            return SQLSyntaxException(2, "illegal operand");
        pos++; token = tokens[pos];
		condVec.push_back(*temp);
		if (!(tokencmp(token, "and")) && !(tokencmp(token, ";")))
			return SQLSyntaxException(2, "Syntax error");
		else if (tokencmp(token, "and"))
			token = tokens[++pos];
    }
    return sqlSession->DeleteRecord(TableName, condVec);
}
bool Interpreter::parseSelectStatement(){
    Token token = tokens[pos];
    std::vector<std::string> columns;
    bool ok = false;
    bool flag = false;
    if(str[token.begin] == '*'){
        pos ++; token = tokens[pos];
        flag = true;
        if(tokencmp(token, "from")){ok = true;}
        else return SQLSyntaxException(2, "syntax error");
    }
    else{
        while(token.type == TokenType::Name){
            columns.emplace_back(str, token.begin, token.end - token.begin + 1);
            pos ++; token = tokens[pos];
            if(tokencmp(token, "FROM")){ok = true; break;}
            if(str[token.begin] != ',') return SQLSyntaxException(2, "syntax error");
            pos ++; token = tokens[pos];
        }
        if(!ok)return SQLSyntaxException(2, "syntax error");
    }
    pos ++; token = tokens[pos];//FROM
    if(token.type != TokenType::Name)
        return SQLSyntaxException(2, "syntax error");
    string TableName(str, token.begin, token.end - token.begin + 1);
    if(sqlSession->IfTableExist(TableName) == false) return SQLSyntaxException(0,"No Such Table");
    vector<key> Attr;
    sqlSession->GetAttr(TableName, Attr);
    if(flag){
        for(int i = 0; i < Attr.size(); i++)
            columns.push_back(Attr[i].name);
    }
    pos ++; token = tokens[pos];//tableName
    vector<Cond> condVec;
    if(str[token.begin] == ';')
        return sqlSession->SelectTable(TableName, columns, condVec);
    else if(!tokencmp(token, "where")) return SQLSyntaxException(2, "Syntax error");
    pos++; token = tokens[pos];
    
    while(str[token.begin] != ';') {
        flag = false;
        Cond *temp = new Cond();
        int t;
        if(token.type != TokenType::Name) return SQLSyntaxException(2, "Syntax error");
        string AName(str, token.begin, token.end - token.begin +1);
        for(int i = 0; i < Attr.size(); i++)
            if(Attr[i].name == AName)
            {
                temp->AttrName = AName;
                t = Attr[i].type;
                flag = true;
                break;
            }
        if(!flag) return SQLSyntaxException(0, "No Such Attribute");

        pos++; token = tokens[pos];
        if(token.type != TokenType::Ope)
            return SQLSyntaxException(2, "Syntax error");
        else if(tokencmp(token, "<")){
            temp->Op = LESS;
        }
        else if(tokencmp(token, ">")){
            temp->Op = MORE;
        }
        else if(tokencmp(token, ">=")){
            temp->Op = MORE_EQ;
        }
        else if(tokencmp(token, "<=")){
            temp->Op = LESS_EQ;
        }
        else if(tokencmp(token, "<>") || tokencmp(token, "!=")){
            temp->Op = NOT_EQ;
        }
        else if(tokencmp(token, "=")){
            temp->Op = EQUAL;
        }else return SQLSyntaxException(2, "illegal opes");

        pos++; token = tokens[pos];
        string tempValue(str, token.begin, token.end - token.begin +1);
        if(token.type == TokenType::Integer){
			if (t == INT_)
				temp->Value = tempValue;
			else if (t == FLOAT_)
				temp->Value = tempValue;
			else return SQLSyntaxException(1, "type error");
        }else if(token.type == TokenType::Real ){
            if(t == FLOAT_)
                temp->Value = tempValue;
            else return SQLSyntaxException(1, "type error");
        }else if(token.type == TokenType::String){
            if(t == CHAR_)
                temp->Value = tempValue.substr(1, tempValue.size()-2);
            else
                return SQLSyntaxException(1, "type error");
        }else
            return SQLSyntaxException(2, "illegal operand");
        pos++; token = tokens[pos];
		condVec.push_back(*temp);
		if (!(tokencmp(token, "and")) && !(tokencmp(token, ";")))
			return SQLSyntaxException(2, "Syntax error");
		else if (tokencmp(token, "and"))
			token = tokens[++pos];
    }
    return sqlSession->SelectTable(TableName, columns, condVec);
}

bool Interpreter::parseInsertStatement(){
    Token token = tokens[pos];pos++;

    vector<key> Attr;

    if(!tokencmp(token, "into")) return SQLSyntaxException(2, "syntax error");
    token = tokens[pos];pos++;
    if( token.type != TokenType::Name )return SQLSyntaxException(2, "syntax error");
    std::string tableName(str, token.begin, token.end - token.begin + 1);
    
    if(sqlSession->IfTableExist(tableName) == false)return SQLSyntaxException(0, "no table");
    sqlSession->GetAttr(tableName, Attr);
    token = tokens[pos]; pos++;
    if(!tokencmp(token, "values")) return SQLSyntaxException(2, "syntax error");
    token = tokens[pos];pos++;// (
    if(!tokencmp(token, "("))return SQLSyntaxException(2, "syntax error");
    token = tokens[pos];pos++;
    vector<string> value;
    int i = 0;
    while( pos < tokens.size()){
        string temp(str, token.begin, token.end-token.begin+1);
        if(i >= Attr.size())
            return SQLSyntaxException(2, "too much value!");
        if((token.type == TokenType::Real || token.type == TokenType::Integer) && Attr[i].type == FLOAT_)
            value.push_back(temp);
        else if(token.type == TokenType::Integer && Attr[i].type == INT_)
            value.push_back(temp);
        else if(token.type == TokenType::String && Attr[i].type == CHAR_)
            value.push_back(temp);
        else
            return SQLSyntaxException(2,"Wrong Attribute type!");
        i++;
        token = tokens[pos]; pos ++; // name;
        if(str[token.begin] == ')') break;
        if(str[token.begin] != ',') return SQLSyntaxException(2, "syntax error");
        token = tokens[pos]; pos ++; // ,
    }

    token = tokens[pos]; pos++;// ;
    if(str[token.begin] != ';') return SQLSyntaxException(2, "syntax error");
    return sqlSession->InsertRecord(tableName, value);

}

bool Interpreter::parseCreateDefinition(vector<key> &Attr)
{
    key *object = new key();
    Token token = tokens[pos]; pos++;
    int flag = 0;

    // name;
    if( token.type != TokenType::Name ) return SQLSyntaxException(2, "syntax error");
    if( tokencmp(token, "primary") ){
        token = tokens[pos]; pos ++; //primary
        if( !tokencmp(token, "key")) return SQLSyntaxException(2, "syntax error");
        token = tokens[pos]; pos ++; //key
        if(str[token.begin] != '(') return SQLSyntaxException(2, "syntax error");
        token = tokens[pos]; pos ++; // (
        std::string name(str, token.begin, token.end - token.begin + 1);
        token = tokens[pos]; pos ++; // name
        if(str[token.begin] != ')') return SQLSyntaxException(2, "syntax error");
        for (int i = 0; i < Attr.size(); ++i) {
            if(name == Attr[i].name)
                flag = Attr[i].primary = 1;

        }
        if(flag == 0)
            return  SQLSyntaxException(2, "syntax error! No primary key");
        else
            return 1;
    }
    std::string name(str, token.begin, token.end - token.begin + 1);
    object->name = name;
	object->unique = false;
    token = tokens[pos]; pos ++; //name
    if( tokencmp(token, "int")){
        object->type = INT_;
    }
    else if( tokencmp(token, "float")){
        object->type = FLOAT_;
    }
    else if( tokencmp(token, "char")){
        token = tokens[pos]; pos ++; // (
        if(str[token.begin] != '(') return SQLSyntaxException(2, "syntax error");
        token = tokens[pos]; pos ++; // n
        if(token.type != TokenType::Integer)return SQLSyntaxException(2, "syntax error");
        int n = 0;
        sscanf(str + token.begin, "%d", &n);
        token = tokens[pos]; pos ++; // )
        if(str[token.begin] != ')') return SQLSyntaxException(2, "syntax error");
        object->type = CHAR_;
    }
    else return SQLSyntaxException(2, "syntax error");

    token = tokens[pos];

    if(tokencmp(token, "unique")){
        object->unique = 1;
        pos ++; // (
    }

    Attr.push_back(*object);

    return true;
}

bool Interpreter::parseCreateStatement(){
    Token token = tokens[pos];
    pos++;
    if( tokencmp(token, "table") ){
        token = tokens[pos]; pos ++;// table
        if(token.type != TokenType::Name) SQLSyntaxException(2, "syntax error");
        std::string name(str, token.begin, token.end - token.begin + 1);
        token = tokens[pos]; pos ++;// name
        if(str[token.begin] != '(') SQLSyntaxException(2, "syntax error");
        vector<key> Attr;
        while( pos < tokens.size() ){

            if(!parseCreateDefinition(Attr)){
                for(int i = 0; i < Attr.size(); i++)
                    delete &Attr[i];
                Attr.clear();
                return false;
            }
            token = tokens[pos]; pos++;
            if(str[token.begin] == ')') break;
            if(str[token.begin] != ',') return SQLSyntaxException(2, "syntax error");
        }
        token = tokens[pos]; pos ++;// ;
        if(str[token.begin] != ';') return SQLSyntaxException(2, "syntax error");
        return sqlSession->CreateTable(name, Attr);
    }
    else if( tokencmp(token, "index") ){
        key IndexAttr;
        token = tokens[pos]; pos ++;// index name;
        if(token.type != TokenType::Name) return SQLSyntaxException(2, "syntax error");
        std::string name(str, token.begin, token.end - token.begin + 1);
        token = tokens[pos]; pos++;
        if(!tokencmp(token, "on") ) return SQLSyntaxException(2, "syntax error");
        token = tokens[pos]; pos ++;// on
        if(token.type != TokenType::Name)return SQLSyntaxException(2, "syntax error");
        std::string table(str, token.begin, token.end - token.begin + 1);
        token = tokens[pos]; pos ++;// table
        if(str[token.begin] != '(') return SQLSyntaxException(2, "syntax error");
        token = tokens[pos]; pos ++;// (
        if(token.type != TokenType::Name)return SQLSyntaxException(2, "syntax error");
        std::string column(str, token.begin, token.end - token.begin + 1);
        token = tokens[pos]; pos ++;// column
        if(str[token.begin] != ')') return SQLSyntaxException(2, "syntax error");
        token = tokens[pos]; pos ++;// )
        if(str[token.begin] != ';') return SQLSyntaxException(2, "syntax error");
        IndexAttr.name = column;
        return sqlSession->CreateIndex(table, name, IndexAttr);
    }
    return SQLSyntaxException(2, "syntax error");
}

bool Interpreter::parseDropStatement(){
    Token token = tokens[pos]; pos ++;
    if( tokencmp(token, "table") ){
        token = tokens[pos]; pos ++;// table;
        if(token.type != TokenType::Name)return SQLSyntaxException(2, "syntax error");
        std::string name(str, token.begin, token.end - token.begin + 1);
        token = tokens[pos]; pos ++;// ;
        if(str[token.begin] != ';') return SQLSyntaxException(2, "syntax error");
        return sqlSession->DropTable(name);
    }
    else if( tokencmp(token, "index") ){
        token = tokens[pos]; pos ++;// database;
        if(token.type != TokenType::Name)return SQLSyntaxException(2, "syntax error");
        std::string name(str, token.begin, token.end - token.begin + 1);
        token = tokens[pos]; pos ++;//
        if(str[token.begin] != ';') return SQLSyntaxException(2, "syntax error");
        return sqlSession->DropIndex(name);
    }
    return SQLSyntaxException(2, "syntax error!\'table\' or \'index\'");

}

void Interpreter::tokenize() {
    TokenType status = TokenType::Null;
    int saved_pos = 0;

    for (int i = 0; str[i] != 0; i++) {
        switch (status) {
            case TokenType::Null:
                if (str[i] == '\'' || str[i] == '\"') {
                    status = TokenType::String;
                }
                else if (str[i] == '`') {
                    status = TokenType::Name;
                }
                else if (IsNum(str[i]) || str[i] == '-') {
                    status = TokenType::Integer;
                }
                else if (IsChar(str[i])) {
                    status = TokenType::__Keyword;
                }
                else if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\r') {
                    break;
                }
                else {
                    int st = i;
                    if (str[i] == '!' && str[i + 1] == '=') {
                        i++;
                    }
                    else if (str[i] == '<' && str[i + 1] == '>') {
                        i++;
                    }
                    else if (str[i] == '<' && str[i + 1] == '=') {
                        i++;
                    }
                    else if (str[i] == '>' && str[i + 1] == '=') {
                        i++;
                    }
                    tokens.emplace_back(st, i, TokenType::Ope);
                    break;
                }
                saved_pos = i;
                break;
            case TokenType::String:
                if ((str[i] == '\"' || str[i] == '\'') && str[i - 1] != '\\') {
                    tokens.emplace_back(saved_pos, i, TokenType::String);
                    status = TokenType::Null;
                }break;
            case TokenType::Name:
                if (str[i] == '`'  && str[i - 1] != '\\') {
                    tokens.emplace_back(saved_pos, i - 1, TokenType::String);
                    status = TokenType::Null;
                }
                break;
            case TokenType::Integer:
                if( (str[i] == '.' && IsNum(str[i+1]))|| str[i] == 'e' ) {
                    status = TokenType::Real;
                    if (str[i] == 'e' && str[i] == '-')i++;
                    break;
                }else if(str[i] == '.' && IsChar(str[i+1]))
                {
                    status = TokenType::__Keyword;
                    break;
                }
                if (!IsNum(str[i])) {
                    tokens.emplace_back(saved_pos, i - 1, TokenType::Integer);
                    status = TokenType::Null;
                    i--;
                }
                break;
            case TokenType::Real:
                if (!IsNum(str[i])) {
                    tokens.emplace_back(saved_pos, i - 1, TokenType::Real);
                    status = TokenType::Null;
                    i--;
                }
                break;
            case TokenType::__Keyword:
                if (!IsChar(str[i]) && !IsNum(str[i]) && str[i]!='.') {
                    tokens.emplace_back(saved_pos, i - 1, TokenType::Name);
                    status = TokenType::Null;
                    i--;
                }
                break;
            default: break;
        }
    }

}

bool strncasecmp(const char* s1, const char* s2, int len) {
	string S1 = s1, S2 = s2;
	if (S1.size()<len || S2.size()<len) {
		if (S1.size() != S2.size()) return false;
		for (int i = 0; i < S1.size(); i++) {
		}
		return !(S1 == S2);
	}
	S1 = S1.substr(0, len);
	S2 = S2.substr(0, len);
	for (int i = 0; i < len; i++) {
		if (isupper(S1[i])) S1[i] = tolower(S1[i]);
		if (isupper(S2[i])) S2[i] = tolower(S2[i]);
	}
	return !(S1 == S2);
}

bool Interpreter::tokencmp(Interpreter::Token &token, const char *target) {
    int len1 = token.end - token.begin + 1;
    int len2 = (int)strlen(target);
    if( len1 != len2 ) return false;
    return strncasecmp(str + token.begin, target, (int)len1) == 0;
}

bool  Interpreter::SQLSyntaxException(int state, string Error){
    if(state == 2)
    {
        cout << Error << endl;
    }else if(state == 0)
    {
        cout << "Sorry, there is " << Error << endl;
    }else if(state == 1)
    {
        cout << "the Attribute has " << Error << endl;
    }


    return true;
}
