#pragma once
#include "ExpressionTree.h"
#include "CommandValidator.h"
#include "ErrorsMsg.h"

class IExpressionTree
{
public:
    IExpressionTree() { Logger::debugPrintsOn = false;}
    ~IExpressionTree() {}
    void run();//infinite loop
private:
    std::string getCommand() const;

    void processCommand(std::vector<std::string>& strVec);
    void commandEnter(const std::vector<std::string> &strVec);
    void commandJoin(const std::vector<std::string>& strVec);
    void commandComp(const std::vector<std::string> &strVec);
    void commandPrint() const;
    void commandVars() const;
    void commandHelp() const;
    void commandCrossOver() const;
    void commandMutate();
    void commandDebug();
    void wasExprFixed(const std::vector<std::string> &input, const std::vector<std::string> &output) const;

    ExpressionTree expTree;
    CommandValidator cmdValidator;
};

