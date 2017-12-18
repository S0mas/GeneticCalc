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
    ~ExpressionTree();

    void operator=(const ExpressionTree& other);
	void createTree(const std::vector<std::string>& strVec);
	void mutate();
	void setVariablesValues(const std::vector<double>& valuesVec);
    ExpressionTree operator+(const ExpressionTree& other) const;  
    double getResult() const;
    unsigned int getNumberOfVariables() const;
    std::string toString() const;
	std::string variablesToString() const;
	std::vector<std::string> toStringVec() const;
    unsigned getTreeSize() const;

    AbstractExpressionNode* root;
private:
    std::map<const std::string, double> variablesMap;

	void changeLastNode(AbstractExpressionNode * newNode);
	void lastNodeRec(AbstractExpressionNode** root, AbstractExpressionNode** lastNode);
    void toStringVecRec(const AbstractExpressionNode* root, std::vector<std::string>& strVec) const;
    void clearTree();
    void fillVariablesMap();
    void fillVariablesMapRec(const AbstractExpressionNode* root);
    void updateVarsValuesRec(AbstractExpressionNode* root);  
	void getRandomNodeRec(AbstractExpressionNode ** root, int & nodeNumber, AbstractExpressionNode *** randomNode);
	void getTreeSizeRec(const AbstractExpressionNode* root, unsigned& size) const;
    AbstractExpressionNode* createNode(int& it, const int end, const std::vector<std::string>& strVec) const;
	AbstractExpressionNode*** getRandomNode();
	AbstractExpressionNode* getFulfillConstant() const;
public:
	static void swap(ExpressionTree *left, ExpressionTree *right)
	{
		AbstractExpressionNode* temp = left->root;
		left->root = right->root;
		right->root = temp;

		std::map<const std::string, double> tempMap = left->variablesMap;
		left->variablesMap = right->variablesMap;
		right->variablesMap = tempMap;
	}
    static void crossOver(ExpressionTree *otherA, ExpressionTree *otherB)
    {
		AbstractExpressionNode*** randA = otherA->getRandomNode();
        AbstractExpressionNode*** randB = otherB->getRandomNode();

        AbstractExpressionNode* temp = **randA;
        **randA = **randB;
        **randB = temp;

        delete randA;
        delete randB;
    }
    static ExpressionTree* generateRandomTree(const unsigned& varsNumber, const unsigned& minNodesNumber, const unsigned& maxNodesNumber)
    {
		ExpressionTree* tree = new ExpressionTree();
        const std::map<const std::string, double>& variablesMap = RandomNodeGenerator::generateVariablesMap(varsNumber);
        int nodesNumber = Helper::getRandomNumber()%(maxNodesNumber - minNodesNumber) + minNodesNumber;

		tree->root = RandomNodeGenerator::getRandomTree(variablesMap, nodesNumber);
		tree->variablesMap = variablesMap;

        return tree;
    }
};
