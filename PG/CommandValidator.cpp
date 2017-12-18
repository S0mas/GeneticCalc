#include "CommandValidator.h"

bool CommandValidator::validateComandEnterOrJoin(const std::vector<std::string>& strVec) const
{
    for(auto& str : strVec)
    {
        if(!KnownOperators::getKnownOperators()->isKnownOperator(str) && !Helper::constainsOnlyLettersOrNumbers(str))
            return false;
    }
    return true;
}

bool CommandValidator::validateComandVarsOrPrint(const std::vector<std::string>& strVec) const
{
    return strVec.empty();
}

bool CommandValidator::validateComandCompTreeExist(const std::vector<std::string>& strVec) const
{
    return !strVec.empty();
}

bool CommandValidator::validateComandCompArgsNumber(const std::vector<std::string>& strVec, const unsigned &numberOfArguments) const
{
    return strVec.size() == numberOfArguments;
}

bool CommandValidator::validateComandCompArgs(const std::vector<std::string>& strVec) const
{
    for(auto strIt = strVec.begin(); strIt != strVec.end(); ++strIt)
    {
        if(!Helper::constainsOnlyNumbers(*strIt))
            return false;
    }
    return true;
}
