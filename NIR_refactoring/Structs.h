// файл Parameter.h - объ€вление структуры Parameter

#pragma once
#include <string>
using namespace std;

struct Parameter {
    string type;
    string name;
};

struct MethodCall {
    string methodName;
    vector<string> parameterValues;
};