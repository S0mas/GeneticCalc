#pragma once
#include "gtest/gtest.h"
#include <iostream>
#include "ExpressionTree.h"
#include <vector>
#include <string>
#include <time.h>

void printSpecialFunctionsCounters()
{
	std::cout << "counterAbstractCreated: " << AbstractExpressionNode::counterAbstractCreated << std::endl;
	std::cout << "counterAbstractDestroyed: " << AbstractExpressionNode::counterAbstractDestroyed << std::endl;
	std::cout << "counterConstantCreated: " << Constant::counterConstantCreated << std::endl;
	std::cout << "counterConstantDestroyed: " << Constant::counterConstantDestroyed << std::endl;
	std::cout << "counterVariableCreated: " << Variable::counterVariableCreated << std::endl;
	std::cout << "counterVariableDestroyed: " << Variable::counterVariableDestroyed << std::endl;
	std::cout << "counterAbstractOpCreated: " << AbstractOperatorNode::counterAbstractOpCreated << std::endl;
	std::cout << "counterAbstractOpDestroyed: " << AbstractOperatorNode::counterAbstractOpDestroyed << std::endl;
	std::cout << "counterOp1Created: " << Operator1Arg::counterOp1Created << std::endl;
	std::cout << "counterOp1Destroyed: " << Operator1Arg::counterOp1Destroyed << std::endl;
	std::cout << "counterOp2Created: " << Operator2Arg::counterOp2Created << std::endl;
	std::cout << "counterOp2Destroyed: " << Operator2Arg::counterOp2Destroyed << std::endl;
}

bool checkClean()
{
	if (AbstractExpressionNode::counterAbstractCreated != AbstractExpressionNode::counterAbstractDestroyed)
		return false;
	if (Constant::counterConstantCreated != Constant::counterConstantDestroyed)
		return false;
	if (Variable::counterVariableCreated != Variable::counterVariableDestroyed)
		return false;
	if (AbstractOperatorNode::counterAbstractOpCreated != AbstractOperatorNode::counterAbstractOpDestroyed)
		return false;
	if (Operator1Arg::counterOp1Created != Operator1Arg::counterOp1Destroyed)
		return false;
	if (Operator2Arg::counterOp2Created != Operator2Arg::counterOp2Destroyed)
		return false;
	return true;
}

TEST(createTree1, expTree)
{
	srand(time(0));
	ExpressionTree* expTree = new ExpressionTree();
	std::vector<std::string> expr = { "*", "+", "9", "X", "-", "Y", "4" };
	expTree->createTree(expr);
	EXPECT_EQ(expTree->toString(), "* + 9.000000 X - Y 4.000000");
	delete expTree;
	expTree = nullptr;
	EXPECT_TRUE(checkClean());

}

TEST(createTreeRepaired1, expTree)
{
	ExpressionTree* expTree = new ExpressionTree();
	std::vector<std::string> expr = { "*", "+", "9", "X", "-", "Y" };
	expTree->createTree(expr);
	EXPECT_EQ(expTree->toString(), "* + 9.000000 X - Y 1.000000");
	delete expTree;
	expTree = nullptr;
	EXPECT_TRUE(checkClean());
}

TEST(createTreeRepaired2, expTree)
{
	ExpressionTree* expTree = new ExpressionTree();
	std::vector<std::string> expr = { "*", "+", "9", "X", "-", "Y", "4", "*", "-", "Y" };
	expTree->createTree(expr);
	EXPECT_EQ(expTree->toString(), "* + 9.000000 X - Y 4.000000");
	delete expTree;
	expTree = nullptr;
	EXPECT_TRUE(checkClean());
}

TEST(copy_assignment, expTree)
{
	ExpressionTree* expTree = new ExpressionTree();
	std::vector<std::string> expr = { "*", "+", "9", "X", "-", "Y", "4"};
	expTree->createTree(expr);

	ExpressionTree* expTree2 = new ExpressionTree();
	std::vector<std::string> expr2 = { "+", "-", "X", "Y", "-", "X", "Y" };
	expTree2->createTree(expr2);
	
	*expTree = *expTree2;
	EXPECT_EQ(expTree->toString(), expTree2->toString());
	EXPECT_FALSE(expTree->root == expTree2->root);

	delete expTree2;
	expTree2 = nullptr;

	EXPECT_EQ(expTree->toString(), "+ - X Y - X Y");

	delete expTree;
	expTree = nullptr;
	EXPECT_TRUE(checkClean());
}

TEST(copy_construct, expTree)
{
	ExpressionTree* expTree = new ExpressionTree();
	std::vector<std::string> expr = { "*", "+", "9", "X", "-", "Y", "4" };
	expTree->createTree(expr);

	ExpressionTree* expTree2 = new ExpressionTree(*expTree);

	EXPECT_EQ(expTree2->toString(), expTree->toString());
	EXPECT_FALSE(expTree2->root == expTree->root);

	delete expTree;
	expTree = nullptr;

	EXPECT_EQ(expTree2->toString(), "* + 9.000000 X - Y 4.000000");

	delete expTree2;
	expTree2 = nullptr;
	EXPECT_TRUE(checkClean());
}

TEST(getNumberOfVariables, expTree)
{
	ExpressionTree* expTree = new ExpressionTree();
	std::vector<std::string> expr = { "*", "+", "9", "X", "-", "Y", "4" };
	expTree->createTree(expr);

	EXPECT_EQ(expTree->getNumberOfVariables(), 2);
	delete expTree;
	expTree = nullptr;
	EXPECT_TRUE(checkClean());
}

TEST(getResult, expTree)
{
	ExpressionTree* expTree = new ExpressionTree();
	std::vector<std::string> expr = { "*", "+", "9", "2", "-", "3", "4" };
	expTree->createTree(expr);

	EXPECT_EQ(expTree->getResult(), -11);
	delete expTree;
	expTree = nullptr;
	EXPECT_TRUE(checkClean());
}

TEST(getTreeSize, expTree)
{
	ExpressionTree* expTree = new ExpressionTree();
	std::vector<std::string> expr = { "*", "+", "9", "2", "-", "3", "4" };
	expTree->createTree(expr);

	EXPECT_EQ(expTree->getTreeSize(), 7);
	delete expTree;
	expTree = nullptr;
	EXPECT_TRUE(checkClean());
}

TEST(toStringVec, expTree)
{
	ExpressionTree* expTree = new ExpressionTree();
	const std::vector<std::string> expr = { "*", "+", "X", "Y", "-", "Y", "X" };
	expTree->createTree(expr);

	EXPECT_EQ(expTree->toStringVec(), expr);
	delete expTree;
	expTree = nullptr;
	EXPECT_TRUE(checkClean());
}

TEST(variablesToString, expTree)
{
	ExpressionTree* expTree = new ExpressionTree();
	const std::vector<std::string> expr = { "*", "+", "X", "Y", "-", "Y", "X" };
	expTree->createTree(expr);

	ExpressionTree* expTree2 = new ExpressionTree();
	const std::vector<std::string> expr2 = { "*", "+", "4", "5", "-", "6", "7" };
	expTree2->createTree(expr2);

	EXPECT_EQ(expTree->variablesToString(), "X: 1.000000, Y: 1.000000");
	EXPECT_EQ(expTree2->variablesToString(), "No variables.");

	delete expTree;
	expTree = nullptr;

	delete expTree2;
	expTree2 = nullptr;
	EXPECT_TRUE(checkClean());
}

TEST(setVariablesValues, expTree)
{
	ExpressionTree* expTree = new ExpressionTree();
	const std::vector<std::string> expr = { "*", "+", "X", "Y", "-", "Y", "X" };
	expTree->createTree(expr);

	std::vector<double> values;
	values.push_back(5.57);
	values.push_back(-2.37);
	EXPECT_EQ(expTree->variablesToString(), "X: 1.000000, Y: 1.000000");
	expTree->setVariablesValues(values);
	EXPECT_EQ(expTree->variablesToString(), "X: 5.570000, Y: -2.370000");

	delete expTree;
	expTree = nullptr;

	EXPECT_TRUE(checkClean());
}

TEST(swap, expTree)
{
	ExpressionTree* expTree = new ExpressionTree();
	const std::vector<std::string> expr = { "*", "+", "X", "Y", "-", "Y", "X" };
	expTree->createTree(expr);

	ExpressionTree* expTree2 = new ExpressionTree();
	std::vector<std::string> expr2 = { "+", "-", "A", "B", "/", "B", "A" };
	expTree2->createTree(expr2);

	EXPECT_EQ(expTree->toString(), "* + X Y - Y X");
	EXPECT_EQ(expTree2->toString(), "+ - A B / B A");

	ExpressionTree::swap(expTree, expTree2);

	EXPECT_EQ(expTree2->toString(), "* + X Y - Y X");
	EXPECT_EQ(expTree->toString(), "+ - A B / B A");

	delete expTree;
	expTree = nullptr;
	delete expTree2;
	expTree2 = nullptr;

	EXPECT_TRUE(checkClean());
}

TEST(operator_plus, expTree)
{
	ExpressionTree* expTree = new ExpressionTree();
	const std::vector<std::string> expr = { "*", "X", "Y"};
	expTree->createTree(expr);

	ExpressionTree* expTree2 = new ExpressionTree();
	std::vector<std::string> expr2 = { "+", "A", "B"};
	expTree2->createTree(expr2);

	ExpressionTree* expTree3 = new ExpressionTree();
	*expTree3 = *expTree + *expTree2;

	EXPECT_EQ(expTree->toString(), "* X Y");
	EXPECT_EQ(expTree2->toString(), "+ A B");
	EXPECT_EQ(expTree3->toString(), "* X + A B");

	EXPECT_EQ(expTree3->getNumberOfVariables(), 3);
	EXPECT_EQ(expTree3->getResult(), 2);

	delete expTree;
	expTree = nullptr;
	delete expTree2;
	expTree2 = nullptr;
	delete expTree3;
	expTree3 = nullptr;

	EXPECT_TRUE(checkClean());
}

TEST(mutate, expTree)
{
	ExpressionTree* expTree = new ExpressionTree();
	const std::vector<std::string> expr = { "*", "X", "Y" };
	expTree->createTree(expr);

	ExpressionTree* expTreeCopy = new ExpressionTree(*expTree);

	EXPECT_EQ(expTree->toString(), expTreeCopy->toString());

	expTree->mutate();

	EXPECT_FALSE(expTree->toString() == expTreeCopy->toString());

	delete expTree;
	expTree = nullptr;
	delete expTreeCopy;
	expTreeCopy = nullptr;

	EXPECT_TRUE(checkClean());
}

TEST(crossOver, expTree)
{
	ExpressionTree* expTree = new ExpressionTree();
	const std::vector<std::string> expr = { "*", "X", "+", "-", "Y", "X", "1" };
	expTree->createTree(expr);

	ExpressionTree* expTree2 = new ExpressionTree();
	const std::vector<std::string> expr2 = { "/", "A", "*", "*", "B", "A", "8" };
	expTree2->createTree(expr2);

	ExpressionTree* expTreeCopy = new ExpressionTree(*expTree);
	ExpressionTree* expTreeCopy2 = new ExpressionTree(*expTree2);

	ExpressionTree::crossOver(expTree, expTree2);

	EXPECT_FALSE(expTree->toString() == expTreeCopy->toString());
	EXPECT_FALSE(expTree2->toString() == expTreeCopy2->toString());

	delete expTree;
	expTree = nullptr;
	delete expTreeCopy;
	expTreeCopy = nullptr;
	delete expTree2;
	expTree2 = nullptr;
	delete expTreeCopy2;
	expTreeCopy2 = nullptr;

	EXPECT_TRUE(checkClean());
}


TEST(testMath, myCubeTest2)
{
	int a;
	std::cin >> a;
}

