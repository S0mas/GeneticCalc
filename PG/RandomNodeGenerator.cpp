#include "RandomNodeGenerator.h"

const std::string& RandomNodeGenerator::getRandomOperator1Arg()
{
    const std::vector<std::string>& operators = KnownOperators::getKnownOperators()->getOperators1Arg();
    return operators[Helper::getRandomNumber()%operators.size()];
}

const std::string& RandomNodeGenerator::getRandomOperator2Args()
{
    const std::vector<std::string>& operators = KnownOperators::getKnownOperators()->getOperators2Args();
    return operators[Helper::getRandomNumber()%operators.size()];
}

AbstractExpressionNode* RandomNodeGenerator::getRandomVariable(const std::map<const std::string, double>& variablesMap)
{
    if(variablesMap.empty())
        return new (std::nothrow) Constant(Helper::getRandomNumber()%10+1);

    unsigned position = Helper::getRandomNumber() % variablesMap.size();
    unsigned i = 0;

    for(auto& pair : variablesMap)
    {
        if(i==position)
            return new (std::nothrow) Variable(pair.first, pair.second);
        i++;
    }
	std::cout << "BUG" << std::endl;
    return nullptr;//cant reach
}

AbstractExpressionNode* RandomNodeGenerator::getRandomNode(const std::map<const std::string, double>& variablesMap, int& operatorsMaxCount)
{

    if(Helper::getRandomNumber()%2)
        return new (std::nothrow) Operator1Arg(getRandomOperator1Arg(), getRandomTree(variablesMap, operatorsMaxCount));
    return new (std::nothrow) Operator2Arg(getRandomOperator2Args(), getRandomTree(variablesMap, operatorsMaxCount), getRandomTree(variablesMap, operatorsMaxCount));
}

AbstractExpressionNode* RandomNodeGenerator::getRandomLeaf(const std::map<const std::string, double>& variablesMap)
{
    if(Helper::getRandomNumber()%2)
    {
        return new (std::nothrow) Constant(Helper::getRandomNumber()%10 + 1);
    }
    return getRandomVariable(variablesMap);
}

AbstractExpressionNode* RandomNodeGenerator::getRandomTree(const std::map<const std::string, double>& variablesMap, int& operatorsMaxCount)
{
    while(operatorsMaxCount-- > 0)
        return getRandomNode(variablesMap, operatorsMaxCount);
    return getRandomLeaf(variablesMap);
}

const std::map<const std::string, double> RandomNodeGenerator::generateVariablesMap(const unsigned& varsNumber)
{
    if(varsNumber > 26)
        Logger::printError("Too much variables! Max is 26.");
    std::map<const std::string, double> variablesMap;
    int charNumber = 65;
    for(int i = 0; i < varsNumber; ++i, ++charNumber)
    {
        char ch = static_cast<char>(charNumber);
        std::string firstLetter(1, ch);

        variablesMap.insert(std::make_pair(firstLetter, 0.0));
    }

    return variablesMap;
}
