// файл CppCode.h - объявление класса CppCode

#pragma once
#include <string>
#include <vector>
#include <clocale>
#include <sstream>
#include <regex>
using namespace std;

#include "CppMethod.h"


#include <iostream> // <-- для отладки, потом убрать

/// <summary>Класс кода</summary>
class CppCode {
private:
    string cppCode;                 // строка, содержащая код

public:
    vector<string> lexemes;         // список лексем кода

    vector<CppMethod> methods;      // список методов кода

    vector<string> types;

    vector<string> keyWordsSet;

    vector<string> specSymbolsSet;

    /// <summary>Конструктор по умолчанию</summary>
    CppCode();

    /// <summary>Конструктор класса</summary>
    CppCode(string cppCode);

    /// <summary>Дополнительные действия для разбиения кода на лексемы</summary>
    string Modify();

    /// <summary>Разбиение кода на лексемы</summary>
    vector<string> GetLexemes();
    /// <summary>Разбиение кода на лексемы</summary>
    vector<string> GetLexemesV2();

    /// <summary>Получение типов данных, описанных в коде</summary>
    /// <returns>- вектор пользовательских типов данных</returns>
    vector<string> GetTypes(string type);
    vector<string> GetStdTypes();

    /// <summary>Получение вектора методов</summary>
    /// <returns>- вектор методов в коде</returns>
    vector<CppMethod> GetMethods();

    void UpdateMethodCalls(CppMethod cppMethod);

    void UpdateMethods(vector<CppMethod> methods);

    void UpdateLexemes();

    void UpdateCode(string code);

    string ToString();

    string GetSep(string curLexeme, string nxtLexeme);
};