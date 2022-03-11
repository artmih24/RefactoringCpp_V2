// файл CppMethod.cpp - реализация методов класса CppMethod

#include "CppMethod.h"

CppMethod::CppMethod() {
    lexemes = {};
    parameters = {};
    oldParameters = {};
    body = {};
    type = "";
    name = "";
}

CppMethod::CppMethod(vector<string> lexemes_) {
    lexemes = lexemes_;
    parameters = this->GetMethodParameters();
    oldParameters = this->GetMethodParameters();
    body = this->GetMethodBody();
    type = this->GetMethodType();
    name = this->GetMethodName();
}

vector<Parameter> CppMethod::GetMethodParameters() {
    vector<string> lexemes = this->lexemes;
    vector<Parameter> parameters = {};
    bool params = false;
    int lexemesSize = lexemes.size(),
        start = -1,
        j = 0;
    for (int i = 0; i < lexemesSize; i++) {
        if (lexemes[i] == "(" && !params) {
            start = i;
            params = true;
        }
        if (lexemes[i] == ")") {
            j = i;
            break;
        }
    }
    for (int i = start + 1; i < j - 1; i += 3) {
        if (lexemes[i] != ")") {
            Parameter parameter = {};
            parameter.type = lexemes[i];
            parameter.name = lexemes[i + 1];
            parameters.push_back(parameter);
        }
    }
    return parameters;
}

vector<string> CppMethod::GetMethodBody() {
    vector<string> lexemes = this->lexemes,
        bodyLexemes = {};
    int lexemesSize = lexemes.size(),
        depth = 0, 
        i = 0, 
        start = -1;
    if (lexemesSize != 0) {
        // crashes
        for (i = 0; i < lexemesSize; i++) {
            if (lexemes[i] == "{") {
                depth++;
                if (start == -1)
                    start = i;
            }
            if (lexemes[i] == "}")
                if (depth > 1)
                    depth--;
                else
                    break;
        }
        // crashes
        if (start >= 0) {
            for (int j = start; j < i; j++) {
                bodyLexemes.push_back(lexemes[j]);
            }
            bodyLexemes.push_back("}");
            return bodyLexemes;
        }
    }
    return {};
}

string CppMethod::GetMethodType() {
    return this->lexemes[0];
}

string CppMethod::GetMethodName() {
    return this->lexemes[1];
}

vector<string> CppMethod::ToLexemes() {
    vector<string> lexemes = {},
        body = this->body;
    vector<Parameter> parameters = this->parameters;
    int parametersSize = parameters.size(),
        bodySize = body.size();
    lexemes.push_back(this->type);
    lexemes.push_back(this->name);
    lexemes.push_back("(");
    for (int i = 0; i < parametersSize; i++) {
        lexemes.push_back(parameters[i].type);
        lexemes.push_back(parameters[i].name);
        if (i < parametersSize - 1)
            lexemes.push_back(",");
    }
    lexemes.push_back(")");
    for (int i = 0; i < bodySize; i++) {
        lexemes.push_back(body[i]);
    }
    return lexemes;
}