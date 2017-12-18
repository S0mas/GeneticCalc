#pragma once
#include <string>

class ErrorsMsg
{
public:
    ErrorsMsg(){}

    static const constexpr auto unrecognizedSymbol  = "Unrecognized symbol.";
    static const constexpr auto noExpression  = "There is no expression to calculate.";
    static const constexpr auto invalidArgsNumber   = "Invalid number of arguments.";
    static const constexpr auto invalidVariableName  = "Invalid variable name. It should contain only letters or numbers.";
    static const constexpr auto invalidValue  = "Invalid value. Only unsigned int are valid.";
    static const constexpr auto invalidCommand  = "Invalid command.";
};

