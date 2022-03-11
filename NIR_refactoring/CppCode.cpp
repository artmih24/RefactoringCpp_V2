// файл CppCode.cpp - реализация методов класса CppCode

#include "CppCode.h"

CppCode::CppCode() {
    this->cppCode = "";
    this->lexemes = {};
    this->methods = {};
    this->types = {};
    this->keyWordsSet = {};
    this->specSymbolsSet = {};
}

/// <summary>Конструктор класса</summary>
CppCode::CppCode(string cppCode) {
    this->cppCode = cppCode;
    this->lexemes = this->GetLexemes();
    this->methods = this->GetMethods();
    this->types = this->GetStdTypes();
    this->keyWordsSet = {
        "#include", "include", "export", "module",
        "#define", "#undef",
        "#ifdef", "ifndef", "#if", "#else", "#endif", "#elifdef", "#elifndef", "#elif",
        "#pragma",
        "using",
        "namespace",
        "struct", "union", "enum", "class", "typedef",
        "template", "typename",
        "static", "volatile", "const", "extern", 
        "virtual", "friend",
        "private", "public", "protected",
        "if", "else",
        "while", "do",
        "for", 
        "switch", "case", "default",
        "try", "catch", "throw",
        "break", "continue", "return",
        "this", "new", "delete",
        "asm", "_asm", "__asm"
    };
    this->specSymbolsSet = {"<", ">", "\"", "\'", ".", " ", "\n", "\t", ";", ":", ",", "(", ")", "[", "]", "&", "|", "!", "?", "/", "\\", "-", "+", "*", "&", "%", "=", "~"};
}

vector<string> CppCode::GetStdTypes() {
    return {
        "bool",
        "char", "__int8", "int8_t",
        "signed char",
        "unsigned char", "unsigned __int8", "uint8_t",
        "wchar_t", "__wchar_t",
        "char16_t",
        "char32_t",
        "short", "short int", "signed short int", "signed short", "__int16", "int16_t",
        "unsigned short", "unsigned short int", "unsigned __int16", "uint16_t",
        "int", "signed int", "signed", "__int32", "int32_t",
        "unsigned int", "unsigned", "unsigned __int32", "uint32_t",
        "long", "long int", "signed long int", "signed long",
        "unsigned long", "unsigned long int",
        "long long", "long long int", "signed long long int", "signed long long", "__int64", "int64_t",
        "unsigned long long", "unsigned long long int", "unsigned __int64", "uint64_t",
        "float",
        "double",
        "long double",
        "size_t", "uintptr_t",
        "ptrdiff_t", "intptr_t",
        "std::byte",
        "string",
        "void",
        "auto"
    };
}

/// <summary>Дополнительные действия для разбиения кода на лексемы</summary>
string CppCode::Modify() {
    string modCode = this->cppCode;
    for (int i = 0; i < modCode.length(); i++) {
        if (((modCode[i + 1] == '(') || (modCode[i + 1] == ')') ||
            (modCode[i + 1] == '{') || (modCode[i + 1] == '}') ||
            (modCode[i + 1] == ';') ||
            (modCode[i + 1] == ',') ||
            (modCode[i + 1] == '=') || (modCode[i + 1] == '+') || (modCode[i + 1] == '-') ||
            (modCode[i + 1] == '*') || (modCode[i + 1] == '/') || (modCode[i + 1] == '%') ||
            (modCode[i + 1] == '&') || (modCode[i + 1] == '|') || (modCode[i + 1] == '^')) &&
            (i < (modCode.length() - 1)))
            modCode.insert(++i, " ");
        if (((modCode[i] == '(') || (modCode[i] == ')') ||
            (modCode[i] == '{') || (modCode[i] == '}') ||
            (modCode[i] == ';') || (modCode[i] == ':') ||
            (modCode[i] == ',') ||
            (modCode[i] == '=') || (modCode[i] == '+') || (modCode[i] == '-') ||
            (modCode[i] == '*') || (modCode[i] == '/') || (modCode[i] == '%') ||
            (modCode[i] == '&') || (modCode[i] == '|') || (modCode[i] == '^')) ||
            (((modCode[i] == '<') || (modCode[i] == '>')) &&
                (modCode[i - 1] == modCode[i]) && (i > 0)))
            modCode.insert((i + 1), " ");
        if (((modCode[i + 1] == '<') || (modCode[i + 1] == '>')) &&
            (modCode[i + 2] == modCode[i + 1]) && (i < (modCode.length() - 2)))
            modCode.insert((i += 2), " ");
        if (modCode[i] == '\n')
            modCode.insert(++i, " ₴₽₽. ");
        if (modCode[i] == '\t')
            modCode.insert(++i, " ₴₽₴₽. ");
    }
    return modCode;
}

/// <summary>Разбиение кода на лексемы</summary>
vector<string> CppCode::GetLexemes() {
    setlocale(LC_ALL, "Russian");
    vector<string> vec_lexemes = {};
    const char* separators = " \n\t";
    string modCppCode = this->Modify();
    istringstream ssCppCode(modCppCode + " ₽₴.");
    while (1) {
        string lexeme = "";
        ssCppCode >> lexeme;
        if (lexeme == "₽₴.") {
            break;
        }
        else {
            if (lexeme == "₴₽₽.")
                vec_lexemes.push_back("\n");
            else
                if (lexeme == "₴₽₴₽.")
                    vec_lexemes.push_back("\t");
                else
                    if (lexeme != "")
                        vec_lexemes.push_back(lexeme);
        }
    }
    return vec_lexemes;
}

/// <summary>Разбиение кода на лексемы</summary>
vector<string> CppCode::GetLexemesV2() {
    vector<string> vec_lexemes = {};
    string cppCode = this->cppCode;
    int lenOfCppCode = cppCode.length();
    for (int i = 0; i < lenOfCppCode; i++) {
        for (int j = i + 1; j < lenOfCppCode - i - 1; j++) {
            string codeSubStr = cppCode.substr(i, j);
            for (string str : this->keyWordsSet) {
                if (codeSubStr == str) {
                    vec_lexemes.push_back(codeSubStr);
                    i = j;
                    break;
                }
            }
            for (string str : this->types) {
                if (codeSubStr == str) {
                    vec_lexemes.push_back(codeSubStr);
                    i = j;
                    break;
                }
            }
            for (string str : this->specSymbolsSet) {
                if (codeSubStr == str) {
                    vec_lexemes.push_back(codeSubStr);
                    i = j;
                    break;
                }
            }
            if (regex_match(codeSubStr, regex("[a-zA-Z0-9_]+"))) {
                vec_lexemes.push_back(codeSubStr);
                i = j;
                break;
            }
        }
    }
    return {};
}

/// <summary>Получение типов данных, описанных в коде</summary>
/// <returns>- вектор пользовательских типов данных</returns>
vector<string> CppCode::GetTypes(string type) {
    vector<string> types = {},
        lexemes = this->lexemes;
    int lexemesSize = lexemes.size();
    for (int i = 0; i < lexemesSize; i++) {
        if (lexemes[i] == type) {
            types.push_back(lexemes[i + (type == "typedef" ? 2 : 1)]);
        }
    }
    return types;
}

/// <summary>Получение вектора методов</summary>
/// <returns>- вектор методов в коде</returns>
vector<CppMethod> CppCode::GetMethods() {
    vector<CppMethod> methods = {};
    vector<string> types = this->GetStdTypes(),
        lexemes = this->lexemes,
        typedefs = this->GetTypes("typedef"),
        structs = this->GetTypes("struct"),
        unions = this->GetTypes("union"),
        enums = this->GetTypes("enum"),
        usings = this->GetTypes("using"),
        classes = this->GetTypes("class");
    for (int i = 0; i < typedefs.size(); i++)
        types.push_back(typedefs[i]);
    for (int i = 0; i < structs.size(); i++)
        types.push_back(structs[i]);
    for (int i = 0; i < unions.size(); i++)
        types.push_back(unions[i]);
    for (int i = 0; i < enums.size(); i++)
        types.push_back(enums[i]);
    for (int i = 0; i < usings.size(); i++)
        types.push_back(usings[i]);
    for (int i = 0; i < classes.size(); i++)
        types.push_back(classes[i]);
    int typesSize = types.size(),
        lexemesSize = lexemes.size();
    for (int i = 0; i < lexemesSize; i++) {
        vector<string> curMethod = {};
        bool isType = false;
        for (int j = 0; j < typesSize && !isType; j++) {
            isType = ((lexemes[i] == types[j]) &&
                (i < lexemesSize - 2) &&
                (lexemes[i + 2] == "("));
        }
        if (isType) {
            int depth = 0,
                k = 0;
            for (k = i; k < lexemesSize; k++) {
                if (lexemes[k] == "{")
                    depth++;
                if (lexemes[k] == "}")
                    if (depth > 1)
                        depth--;
                    else
                        break;
            }
            for (int l = i; l < k; l++) {
                curMethod.push_back(lexemes[l]);
            }
            curMethod.push_back("}");
            i = k;
            methods.push_back(CppMethod(curMethod));
        }
    }
    return methods;
}

void CppCode::UpdateMethodCalls(CppMethod cppMethod) {
    vector<string> lexemes = this->lexemes,
        newLexemes = {};
    int lexemesSize = lexemes.size(),
        parametersSize = cppMethod.parameters.size(),
        oldParametersSize = cppMethod.oldParameters.size(),
        j = 0,
        k = 0,
        depth = 0;
    string cppMethodName = cppMethod.name,
        parameterValue = "";
    for (int i = 0; i < lexemes.size(); i++) {
        if (lexemes[i] != cppMethodName) {
            if (i < lexemes.size() - 1 && lexemes[i + 1] == cppMethodName)
                newLexemes.push_back(lexemes[i] + " " + lexemes[i + 1]);
            else
                newLexemes.push_back(lexemes[i]);
        }
        else {
            MethodCall methodCall = {};
            methodCall.methodName = cppMethodName;
            methodCall.parameterValues = {};
            depth = 0;
            lexemesSize = lexemes.size();
            for (j = i + 1; j < lexemesSize; j++) {
                if (lexemes[j] == "(")
                    depth++;
                if (lexemes[j] == ")")
                    if (depth > 1)
                        depth--;
                    else
                        break;
            }
            if (lexemes[j + 1] != "{") {
                parameterValue = "";
                for (k = i + 2; k < j + 1; k++) {
                    if (lexemes[k] != "," && k != j)
                        parameterValue += (lexemes[k] + (lexemes[k + 1] == ")" || lexemes[k + 1] == "," ? "" : " "));
                    else {
                        methodCall.parameterValues.push_back(parameterValue);
                        parameterValue = "";
                    }
                }
                newLexemes.push_back("(");
                if (parametersSize == methodCall.parameterValues.size()) {
                    for (int l = 0; l < parametersSize; l++) {
                        newLexemes.push_back(methodCall.parameterValues[l]);
                        if (l < parametersSize - 1)
                            newLexemes.push_back(",");
                    }
                }
                else {
                    int newParametersSize = methodCall.parameterValues.size(),
                        m = 0;
                    for (int l = 0; l < parametersSize; l++) {
                        if (cppMethod.parameters[l].name == cppMethod.oldParameters[m].name && m < oldParametersSize && l < parametersSize) {
                            newLexemes.push_back(methodCall.parameterValues[l]);
                            if (l < parametersSize - 1)
                                newLexemes.push_back(",");
                        }
                        else {
                            l--;
                            m++;
                        }
                    }
                }
                newLexemes.push_back(")");
                i = j;
            }
        }
    }
    this->lexemes = newLexemes;
}

void CppCode::UpdateMethods(vector<CppMethod> methods) {
    this->methods = methods;
}

void CppCode::UpdateLexemes() {
    vector<string> newLexemes = {},
        curLexemes = this->lexemes;
    vector<CppMethod> methods = this->methods;
    int methodsSize = methods.size(),
        curLexemesSize = curLexemes.size();
    for (int i = 0; i < methodsSize; i++) {
        for (int j = 0; j < curLexemes.size(); j++) {
            if ((curLexemes[j] == methods[i].type) && 
                (curLexemes[j + 1] == methods[i].name)) {
                int k = 0,
                    depth = 0;
                for (k = j + 1; k < curLexemes.size(); k++) {
                    if (curLexemes[k] == "{")
                        depth++;
                    if ((curLexemes[k] == "}") && (depth > 0)) {
                        if (depth > 1)
                            depth--;
                        else
                            break;
                    }
                }
                j = k;
                vector<string> newMethodLexemes = methods[i].ToLexemes();
                int newMethodLexemesSize = newMethodLexemes.size();
                for (int l = 0; l < newMethodLexemesSize; l++)
                    newLexemes.push_back(newMethodLexemes[l]);
            }
            else
                newLexemes.push_back(curLexemes[j]);
        }
        curLexemes = newLexemes;
        newLexemes = {};
    }
    this->lexemes = curLexemes;
}

string CppCode::ToString() {
    stringstream cppCodeStream;
    vector<string> lexemes = this->lexemes;
    int lexemesSize = lexemes.size();
    for (int i = 0; i < lexemesSize; i++) {
        string sep = (i < lexemesSize - 1 ? this->GetSep(lexemes[i], lexemes[i + 1]) : " ");
        cppCodeStream << lexemes[i] << sep;
    }
    return cppCodeStream.str();
}

string CppCode::GetSep(string curLexeme, string nxtLexeme) {
    if ((curLexeme == "/" || curLexeme == "<" || curLexeme == ">" || curLexeme == "=") && nxtLexeme == curLexeme)
        return "";
    else if (curLexeme == "if" && nxtLexeme == "(")
        return " ";
    else if (curLexeme == "\n" || curLexeme == "\t" ||
        curLexeme == "(" || nxtLexeme == "(" || nxtLexeme == ")" ||
        nxtLexeme == "," || nxtLexeme == ";")
        return "";
    else if ((curLexeme == "+" || curLexeme == "-") && nxtLexeme == "=")
        return "";
    else if (curLexeme != "\n" && nxtLexeme == "}")
        return "\n";
    else
        return " ";
}