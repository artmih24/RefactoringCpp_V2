// файл RefactoringManager.cpp - реализация методов класса RefactoringManager

#include "RefactoringManager.h"

/// <summary>Конструктор объекта класса RefactoringManager</summary>
/// <param name="filePath">- путь к файлу с исходным кодом</param>
RefactoringManager::RefactoringManager(string filePath) {
    this->filePath = filePath;
    this->outFilePath = this->GetOutputFileName(this->filePath);
    this->fileStream = ifstream(this->filePath);
    cout << this->outFilePath << endl;
}

/// <summary>Проведение рефакторинга</summary>
void RefactoringManager::Refactoring() {
    this->GetFileContent(this->filePath);
    this->code = CppCode(this->fileContent);
    this->RemoveAssignmentsToParametersRefactoring(this->code);
    this->RemoveParametersRefactoring(this->code);
    this->fileContent = this->code.ToString();
    WriteFile(this->outFilePath);
}

/// <summary>Проведение рефакторинга методом удаления присваиваний параметрам</summary>
/// <param name="code">- исходный код программы</param>
void RefactoringManager::RemoveAssignmentsToParametersRefactoring(CppCode code) {
    vector<CppMethod> methods = code.methods;
    for (int i = 0; i < methods.size(); i++) {
        methods[i] = this->RemoveAssignmentsToParameters(methods[i]);
    }
    code.UpdateMethods(methods);
    code.UpdateLexemes();
    this->code = code;
}

/// <summary>Удаление присваиваний параметрам метода</summary>
/// <param name="method">- изначальный метод</param>
/// <returns>новый метод</returns>
CppMethod RefactoringManager::RemoveAssignmentsToParameters(CppMethod method) {
    CppMethod newCppMethod,
        curCppMethod = method;
    // цикл по всем параметрам
    for (int i = 0; i < curCppMethod.parameters.size(); i++) {
        newCppMethod.body = {};
        newCppMethod.parameters = curCppMethod.parameters;
        newCppMethod.type = curCppMethod.type;
        newCppMethod.name = curCppMethod.name;
        bool assignment = false;
        // проверка тела метода на наличие присваиваний параметру в нем
        for (int j = 0; j < curCppMethod.body.size(); j++) {
            // если есть присваивание
            if ((curCppMethod.body[j] == curCppMethod.parameters[i].name) &&
                ((((curCppMethod.body[j + 1] == "=") && (curCppMethod.body[j + 2] != "=")) && (j < curCppMethod.body.size() - 1)) ||
                    ((((curCppMethod.body[j + 1] == "+") || (curCppMethod.body[j + 1] == "-") ||
                        (curCppMethod.body[j + 1] == "*") || (curCppMethod.body[j + 1] == "/") ||
                        (curCppMethod.body[j + 1] == "%") || (curCppMethod.body[j + 1] == "&") ||
                        (curCppMethod.body[j + 1] == "|") || (curCppMethod.body[j + 1] == "^")) &&
                        (curCppMethod.body[j + 2] == "=")) && (j < curCppMethod.body.size() - 2)) ||
                    (((curCppMethod.body[j + 1] == "<") || (curCppMethod.body[j + 1] == ">")) &&
                        (curCppMethod.body[j + 2] == curCppMethod.body[j + 1]) &&
                        (curCppMethod.body[j + 3] == "=") && (j < curCppMethod.body.size() - 3)))) {
                assignment = true;
                break;
            }
        }
        // если есть присваивание
        if (assignment) {
            int start = -1;
            for (int j = 0; j < curCppMethod.body.size(); j++) {
                // заменяем его на новую переменную, остальные фрагменты кода записываем как есть
                if (curCppMethod.body[j] == curCppMethod.parameters[i].name)
                    newCppMethod.body.push_back("new_var_" + curCppMethod.body[j]);
                else
                    newCppMethod.body.push_back(curCppMethod.body[j]);
                // в начало тела метода добавляем новую переменную, которой затем присвоим значение параметра
                if ((curCppMethod.body[j] == "{") && (start == -1)) {
                    newCppMethod.body.push_back("\n\t" + curCppMethod.parameters[i].type);
                    newCppMethod.body.push_back("new_var_" + curCppMethod.parameters[i].name);
                    newCppMethod.body.push_back("=");
                    newCppMethod.body.push_back(curCppMethod.parameters[i].name);
                    newCppMethod.body.push_back(";");
                    start = j;
                }
            }
            // запомним метод перед следующей итерацией
            curCppMethod = newCppMethod;
        }
    }
    return curCppMethod;
}

/// <summary>Проведение рефакторинга методом удаления параметров</summary>
/// <param name="code">- исходный код программы</param>
void RefactoringManager::RemoveParametersRefactoring(CppCode code) {
    vector<CppMethod> methods = code.GetMethods();
    for (int j = 0; j < methods.size(); j++) {
        // рефакторинг реализаций методов - удаление лишних параметров
        for (int i = 0; i < methods.size(); i++) {
            methods[i] = this->RemoveParameters(methods[i]);
        }
        code.UpdateMethods(methods);
        code.UpdateLexemes();
        this->code = code;
        // рефакторинг во всем коде, включая и преобразованные 
        // методы - удаление значений, передававшихся в неиспользуемые параметры
        for (int i = 0; i < methods.size(); i++) {
            code.UpdateMethodCalls(methods[i]);
        }
        code.UpdateMethods(methods);
        code.UpdateLexemes();
        this->code = code;
        string codeString = code.ToString();
        code.UpdateCode(codeString);
        code.lexemes = code.GetLexemes();
        methods = code.GetMethods();
    }
    this->code = code;
}

/// <summary>Удаление параметров метода</summary>
/// <param name="method">- изначальный метод</param>
/// <returns>новый метод</returns>
CppMethod RefactoringManager::RemoveParameters(CppMethod method) {
    CppMethod newCppMethod,
        curCppMethod = method;
    vector<Parameter> parameters = curCppMethod.parameters;
    vector<string> body = curCppMethod.body;
    int parametersSize = parameters.size(),
        bodySize = body.size(),
        depth = 0,
        k = 0,
        l = 0;
    string parameterValue;
    newCppMethod.parameters = {};
    newCppMethod.oldParameters = parameters;
    newCppMethod.body = body;
    newCppMethod.type = curCppMethod.type;
    newCppMethod.name = curCppMethod.name;
    // цикл по всем параметрам
    for (int i = 0; i < parameters.size(); i++) {
        bool usingParameter = false;
        // проверяем, используется ли параметр в теле метода
        for (int j = 0; j < body.size(); j++) {
            // если метод реккурсивный - в его теле происходит вызов него же
            if (body[j] == curCppMethod.name) {
                MethodCall methodCall = {};
                methodCall.methodName = curCppMethod.name;
                methodCall.parameterValues = {};
                depth = 0;
                for (k = j + 1; k < body.size(); k++) {
                    if (body[k] == "(")
                        depth++;
                    if (body[k] == ")")
                        if (depth > 1)
                            depth--;
                        else
                            break;
                }
                parameterValue = "";
                // ищем параметр в передаваемых параметрах
                for (l = j + 2; l < k + 1; l++) {
                    if (body[l] == parameters[i].name) {
                        j = k + 1;
                        break;
                    }
                }
            }
            // если параметр используется
            else if (body[j] == parameters[i].name) {
                usingParameter = true;
                break;
            }
        }
        // если используется, то добавляем его в список параметров нового метода
        if (usingParameter) {
            newCppMethod.parameters.push_back(parameters[i]);
        }
        // запомним метод перед следующей итерацией
        curCppMethod = newCppMethod;
    }
    return curCppMethod;
}

/// <summary>Составление названия файла, в котором будет содержаться измененный код</summary>
/// <param name="filePath">- полное название исходного файла</param>
/// <returns>название файла, в котором будет содержаться измененный код</returns>
string RefactoringManager::GetOutputFileName(string filePath) {
    if (filePath.substr(filePath.length() - 4, filePath.length()) == ".cpp") {
        filePath.resize(filePath.length() - 4);
        filePath += "_refactored.cpp";
    }
    else if (filePath.substr(filePath.length() - 2, filePath.length()) == ".c") {
        filePath.resize(filePath.length() - 2);
        filePath += "_refactored.c";
    }
    return filePath;
}

/// <summary>Чтение файла</summary>
/// <param name="filePath">- полное название файла</param>
void RefactoringManager::GetFileContent(string filePath) {
    string fileString = "";
    char buff[2000];
    ifstream fileStream(filePath);
    if (fileStream.is_open()) {
        while (getline(fileStream, fileString)) {
            this->stringStream << fileString << endl;
        }
    }
    else {
        strerror_s(buff, errno);
        cerr << "Error: " << buff << endl;
    }
    this->fileContent = this->stringStream.str();
    fileStream.close();
}

/// <summary>Запись в файл</summary>
/// <param name="filePath">- полное название файла</param>
void RefactoringManager::WriteFile(string filePath) {
    string outString = "";
    this->outFileStream.open(filePath, ios::out);
    this->outStringStream << this->fileContent;
    while (!outStringStream.eof()) {
        getline(this->outStringStream, outString);
        this->outFileStream << outString << " " << endl;
    }
    this->outFileStream.close();
}