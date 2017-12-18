#pragma once
#include <vector>
#include <string>
#include <functional>
#include <cmath>
#include <map>

class KnownOperators
{
public:
    bool isKnownOperator(const std::string& str) const;
    bool isOperator2Args(const std::string& str) const;
    bool isOperator1Arg(const std::string& str) const;
    const std::vector<std::string>& getOperators1Arg() const;
    const std::vector<std::string>& getOperators2Args() const;
    const std::function<double(double)> getFunction1Arg(const std::string& op) const;
    const std::function<double(double, double)> getFunction2Args(const std::string& op) const;

    static KnownOperators* getKnownOperators()
    {
        if(!singleton)
            KnownOperators::singleton = new KnownOperators();
        return singleton;
    }
private:
    KnownOperators();

    static KnownOperators* singleton;
    std::vector<std::string> operators1Arg;
    std::vector<std::string> operators2Args;
    std::map<std::string, std::function<double(double)>> operators1ArgsFunctionMap;
    std::map<std::string, std::function<double(double, double)>> operators2ArgsFunctionMap;
};

