#pragma once
#include "Helper.h"
#include "KnownOperators.h"

class CommandValidator
{
public:
    CommandValidator(){}

    bool validateComandEnterOrJoin(const std::vector<std::string>& strVec) const;
    bool validateComandVarsOrPrint(const std::vector<std::string>& strVec) const;
    bool validateComandCompTreeExist(const std::vector<std::string>& strVec) const;
    bool validateComandCompArgsNumber(const std::vector<std::string>& strVec, const unsigned &numberOfArguments) const;
    bool validateComandCompArgs(const std::vector<std::string>& strVec) const;

};

