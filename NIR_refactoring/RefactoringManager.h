// ���� RefactoringManager.h - ���������� ������ RefactoringManager

#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include "CppCode.h"
#include <iostream>
using namespace std;

/// <summary>�����, ��������������� ��� ���������� ������������</summary>
class RefactoringManager {
private:
    string filePath,            // ������ ���� � ��������� �����
        outFilePath,            // ������ ���� � ������ �����
        fileContent;            // ������, ���������� ���

    ifstream fileStream;        // ����� ������ �� �����

    ofstream outFileStream;     // ����� ������ � ����

    stringstream stringStream,  // ����� ��� ����������� ����������
        outStringStream;        // ����� ��� ���������� ������

    CppCode code;               // ��� �� �/�++, ����������� �������� ��������� ��������

public:
    /// <summary>����������� ������� ������ RefactoringManager</summary>
    /// <param name="filePath">- ���� � ����� � �������� �����</param>
    RefactoringManager(string filePath);

    /// <summary>���������� ������������</summary>
    void Refactoring();

    /// <summary>���������� ������������ ������� �������� ����������� ����������</summary>
    /// <param name="code">- �������� ��� ���������</param>
    void RemoveAssignmentsToParametersRefactoring(CppCode code);

    /// <summary>�������� ������������ ���������� ������</summary>
    /// <param name="method">- ����������� �����</param>
    /// <returns>����� �����</returns>
    CppMethod RemoveAssignmentsToParameters(CppMethod method);

    /// <summary>���������� ������������ ������� �������� ����������</summary>
    /// <param name="code">- �������� ��� ���������</param>
    void RemoveParametersRefactoring(CppCode code);

    /// <summary>�������� ���������� ������</summary>
    /// <param name="method">- ����������� �����</param>
    /// <returns>����� �����</returns>
    CppMethod RemoveParameters(CppMethod method);

    /// <summary>����������� �������� �����, � ������� ����� ����������� ���������� ���</summary>
    /// <param name="filePath">- ������ �������� ��������� �����</param>
    /// <returns>�������� �����, � ������� ����� ����������� ���������� ���</returns>
    string GetOutputFileName(string filePath);

    /// <summary>������ �����</summary>
    /// <param name="filePath">- ������ �������� �����</param>
    void GetFileContent(string filePath);

    /// <summary>������ � ����</summary>
    /// <param name="filePath">- ������ �������� �����</param>
    void WriteFile(string filePath);
};