#pragma once
#include <algorithm>
#include <map>
#include <typeinfo>
#include "RandomNodeGenerator.h"

class ExpressionTree
{
friend class RandomTreeGenerator;
public:
    ExpressionTree() : root(nullptr) {}
    ExpressionTree(const ExpressionTree& other);
    //ExpressionTree(ExpressionTree&& other);
    ~ExpressionTree();

    void operator=(const ExpressionTree& other);
    //void operator=(ExpressionTree&& other);
    static void swap(ExpressionTree *left, ExpressionTree *right);
    void createTree(const std::vector<std::string>& strVec);
    void setVariablesValues(const std::vector<double>& valuesVec);

    ExpressionTree operator+(const ExpressionTree& other) const;
    std::string variablesToString() const;
    double getResult() const;
    unsigned int getNumberOfVariables() const;
	void changeLastNode(AbstractExpressionNode * newNode);
    std::vector<std::string> toStringVec() const;
    std::string toString() const;
    void mutate();
    unsigned getTreeSize() const;


	void ExpressionTree::lastNodeRec(AbstractExpressionNode** root, AbstractExpressionNode** lastNode);

    AbstractExpressionNode* root;
private:
    std::map<const std::string, double> variablesMap;

    void toStringVecRec(const AbstractExpressionNode* root, std::vector<std::string>& strVec) const;
    AbstractExpressionNode* getFulfillConstant() const;

    void clearTree();
    std::map<const std::string, double> getVariablesMap() const;
    void fillVariablesMap();
    void fillVariablesMapRec(const AbstractExpressionNode* root);
    void updateVarsValuesRec(AbstractExpressionNode* root);

    AbstractExpressionNode*** getRandomNode();
	void getRandomNodeRec(AbstractExpressionNode ** root, int & nodeNumber, AbstractExpressionNode *** randomNode);
    AbstractExpressionNode* createNode(int& it, const int end, const std::vector<std::string>& strVec) const;

    static void getTreeSizeRec(const AbstractExpressionNode* root, unsigned& size)
    {
        if(root)
        {
            ++size;
            for(auto& child : root->getChilds())
                getTreeSizeRec(child, size);
        }
    }
public:
    static void crossOver(ExpressionTree &otherA, ExpressionTree &otherB)
    {
		AbstractExpressionNode*** randA = otherA.getRandomNode();
        AbstractExpressionNode*** randB = otherB.getRandomNode();

        AbstractExpressionNode* temp = **randA;
        **randA = **randB;
        **randB = temp;

        delete randA;
        delete randB;
    }
    static ExpressionTree generateRandomTree(const unsigned& varsNumber, const unsigned& minNodesNumber, const unsigned& maxNodesNumber)
    {
        ExpressionTree expTree;
        const std::map<const std::string, double>& variablesMap = RandomNodeGenerator::generateVariablesMap(varsNumber);
        int nodesNumber = Helper::getRandomNumber()%(maxNodesNumber - minNodesNumber) + minNodesNumber;

        expTree.root = RandomNodeGenerator::getRandomTree(variablesMap, nodesNumber);
        expTree.fillVariablesMap();

        return expTree;
    }
};
