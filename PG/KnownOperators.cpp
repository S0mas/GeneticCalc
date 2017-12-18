#include "KnownOperators.h"
#include <math.h>

KnownOperators* KnownOperators::singleton;
KnownOperators::KnownOperators()
{
    KnownOperators::operators1Arg.push_back("sin");
    KnownOperators::operators1Arg.push_back("cos");

    KnownOperators::operators2Args.push_back("+");
    KnownOperators::operators2Args.push_back("-");
    KnownOperators::operators2Args.push_back("*");
    KnownOperators::operators2Args.push_back("/");

    //KnownOperators::operators1ArgsFunctionMap.insert(std::make_pair<std::string, std::function<double(double)>>("sin", std::sin));
    //KnownOperators::operators1ArgsFunctionMap.insert(std::make_pair<std::string, std::function<double(double)>>("cos", std::cos));

    KnownOperators::operators2ArgsFunctionMap.insert(std::make_pair<std::string, std::function<double(double, double)>>("+", std::plus<double>()));
    KnownOperators::operators2ArgsFunctionMap.insert(std::make_pair<std::string, std::function<double(double, double)>>("-", std::minus<double>()));
    KnownOperators::operators2ArgsFunctionMap.insert(std::make_pair<std::string, std::function<double(double, double)>>("/", std::divides<double>()));
    KnownOperators::operators2ArgsFunctionMap.insert(std::make_pair<std::string, std::function<double(double, double)>>("*", std::multiplies<double>()));
}

bool KnownOperators::isKnownOperator(const std::string& str) const
{
    return isOperator1Arg(str) || isOperator2Args(str);
}

bool KnownOperators::isOperator2Args(const std::string& str) const
{
    for(const std::string& op : operators2Args)
        if(op == str) return true;
   return false;
}

bool KnownOperators::isOperator1Arg(const std::string& str) const
{
   for(const std::string& op : operators1Arg)
       if(op == str) return true;
   return false;
}

const std::vector<std::string>& KnownOperators::getOperators1Arg() const
{
    return operators1Arg;
}

const std::vector<std::string>& KnownOperators::getOperators2Args() const
{
    return operators2Args;
}

const std::function<double(double)> KnownOperators::getFunction1Arg(const std::string& op) const
{
    return operators1ArgsFunctionMap.find(op)->second;
}

const std::function<double(double, double)> KnownOperators::getFunction2Args(const std::string& op) const
{
    return operators2ArgsFunctionMap.find(op)->second;
}
