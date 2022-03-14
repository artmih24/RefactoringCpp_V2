// ���� RefactoringManager.cpp - ���������� ������� ������ RefactoringManager

#include "RefactoringManager.h"

/// <summary>����������� ������� ������ RefactoringManager</summary>
/// <param name="filePath">- ���� � ����� � �������� �����</param>
RefactoringManager::RefactoringManager(string filePath) {
    this->filePath = filePath;
    this->outFilePath = this->GetOutputFileName(this->filePath);
    this->fileStream = ifstream(this->filePath);
    cout << this->outFilePath << endl;
}

/// <summary>���������� ������������</summary>
void RefactoringManager::Refactoring() {
    this->GetFileContent(this->filePath);
    this->code = CppCode(this->fileContent);
    this->RemoveAssignmentsToParametersRefactoring(this->code);
    this->RemoveParametersRefactoring(this->code);
    this->fileContent = this->code.ToString();
    WriteFile(this->outFilePath);
}

/// <summary>���������� ������������ ������� �������� ������������ ����������</summary>
/// <param name="code">- �������� ��� ���������</param>
void RefactoringManager::RemoveAssignmentsToParametersRefactoring(CppCode code) {
    vector<CppMethod> methods = code.methods;
    for (int i = 0; i < methods.size(); i++) {
        methods[i] = this->RemoveAssignmentsToParameters(methods[i]);
    }
    code.UpdateMethods(methods);
    code.UpdateLexemes();
    this->code = code;
}

/// <summary>�������� ������������ ���������� ������</summary>
/// <param name="method">- ����������� �����</param>
/// <returns>����� �����</returns>
CppMethod RefactoringManager::RemoveAssignmentsToParameters(CppMethod method) {
    CppMethod newCppMethod,
        curCppMethod = method;
    // ���� �� ���� ����������
    for (int i = 0; i < curCppMethod.parameters.size(); i++) {
        newCppMethod.body = {};
        newCppMethod.parameters = curCppMethod.parameters;
        newCppMethod.type = curCppMethod.type;
        newCppMethod.name = curCppMethod.name;
        bool assignment = false;
        // �������� ���� ������ �� ������� ������������ ��������� � ���
        for (int j = 0; j < curCppMethod.body.size(); j++) {
            // ���� ���� ������������
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
        // ���� ���� ������������
        if (assignment) {
            int start = -1;
            for (int j = 0; j < curCppMethod.body.size(); j++) {
                // �������� ��� �� ����� ����������, ��������� ��������� ���� ���������� ��� ����
                if (curCppMethod.body[j] == curCppMethod.parameters[i].name)
                    newCppMethod.body.push_back("new_var_" + curCppMethod.body[j]);
                else
                    newCppMethod.body.push_back(curCppMethod.body[j]);
                // � ������ ���� ������ ��������� ����� ����������, ������� ����� �������� �������� ���������
                if ((curCppMethod.body[j] == "{") && (start == -1)) {
                    newCppMethod.body.push_back("\n\t" + curCppMethod.parameters[i].type);
                    newCppMethod.body.push_back("new_var_" + curCppMethod.parameters[i].name);
                    newCppMethod.body.push_back("=");
                    newCppMethod.body.push_back(curCppMethod.parameters[i].name);
                    newCppMethod.body.push_back(";");
                    start = j;
                }
            }
            // �������� ����� ����� ��������� ���������
            curCppMethod = newCppMethod;
        }
    }
    return curCppMethod;
}

/// <summary>���������� ������������ ������� �������� ����������</summary>
/// <param name="code">- �������� ��� ���������</param>
void RefactoringManager::RemoveParametersRefactoring(CppCode code) {
    vector<CppMethod> methods = code.GetMethods();
    for (int j = 0; j < methods.size(); j++) {
        // ����������� ���������� ������� - �������� ������ ����������
        for (int i = 0; i < methods.size(); i++) {
            methods[i] = this->RemoveParameters(methods[i]);
        }
        code.UpdateMethods(methods);
        code.UpdateLexemes();
        this->code = code;
        // ����������� �� ���� ����, ������� � ��������������� 
        // ������ - �������� ��������, �������������� � �������������� ���������
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

/// <summary>�������� ���������� ������</summary>
/// <param name="method">- ����������� �����</param>
/// <returns>����� �����</returns>
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
    // ���� �� ���� ����������
    for (int i = 0; i < parameters.size(); i++) {
        bool usingParameter = false;
        // ���������, ������������ �� �������� � ���� ������
        for (int j = 0; j < body.size(); j++) {
            // ���� ����� ������������ - � ��� ���� ���������� ����� ���� ��
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
                // ���� �������� � ������������ ����������
                for (l = j + 2; l < k + 1; l++) {
                    if (body[l] == parameters[i].name) {
                        j = k + 1;
                        break;
                    }
                }
            }
            // ���� �������� ������������
            else if (body[j] == parameters[i].name) {
                usingParameter = true;
                break;
            }
        }
        // ���� ������������, �� ��������� ��� � ������ ���������� ������ ������
        if (usingParameter) {
            newCppMethod.parameters.push_back(parameters[i]);
        }
        // �������� ����� ����� ��������� ���������
        curCppMethod = newCppMethod;
    }
    return curCppMethod;
}

/// <summary>����������� �������� �����, � ������� ����� ����������� ���������� ���</summary>
/// <param name="filePath">- ������ �������� ��������� �����</param>
/// <returns>�������� �����, � ������� ����� ����������� ���������� ���</returns>
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

/// <summary>������ �����</summary>
/// <param name="filePath">- ������ �������� �����</param>
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

/// <summary>������ � ����</summary>
/// <param name="filePath">- ������ �������� �����</param>
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