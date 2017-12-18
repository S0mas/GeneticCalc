#pragma once
#include "ExpressionNode.h"
#include "KnownOperators.h"
#include <map>

class RandomNodeGenerator
{
public:
    static AbstractExpressionNode* getRandomTree(const std::map<const std::string, double>& variablesMap, int& operatorsMaxCount);
    static const std::map<const std::string, double> generateVariablesMap(const unsigned &varsNumber);
private:
    static const std::string& getRandomOperator1Arg();
    static const std::string& getRandomOperator2Args();
    static AbstractExpressionNode* getRandomVariable(const std::map<const std::string, double> &variablesMap);
    static AbstractExpressionNode* getRandomNode(const std::map<const std::string, double>& variablesMap, int& operatorsMaxCount);
    static AbstractExpressionNode* getRandomLeaf(const std::map<const std::string, double>& variablesMap);
};

