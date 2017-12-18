#include "ExpressionTree.h"
#include <iostream>

unsigned AbstractExpressionNode::counterAbstractCreated = 0;
unsigned AbstractExpressionNode::counterAbstractDestroyed = 0;
unsigned Constant::counterConstantCreated = 0;
unsigned Constant::counterConstantDestroyed = 0;
unsigned Variable::counterVariableCreated = 0;
unsigned Variable::counterVariableDestroyed = 0;
unsigned AbstractOperatorNode::counterAbstractOpCreated = 0;
unsigned AbstractOperatorNode::counterAbstractOpDestroyed = 0;
unsigned Operator1Arg::counterOp1Destroyed = 0;
unsigned Operator1Arg::counterOp1Created = 0;
unsigned Operator2Arg::counterOp2Destroyed = 0;
unsigned Operator2Arg::counterOp2Created = 0;

ExpressionTree::ExpressionTree(const ExpressionTree& other) : root(nullptr)
{
    clearTree();
    root = other.root->getCopy();
    fillVariablesMap();
}

//ExpressionTree::ExpressionTree(ExpressionTree&& other)
//{
//    swap(this, &other);
//    other.root = nullptr;
//}

ExpressionTree::~ExpressionTree()
{
    clearTree();
}

void ExpressionTree::operator=(const ExpressionTree& other)
{
    clearTree();
    root = other.root->getCopy();
    variablesMap = other.variablesMap;
}

//void ExpressionTree::operator=(ExpressionTree&& other)
//{
//    clearTree();
//    swap(this, &other);
//}

void ExpressionTree::swap(ExpressionTree* left, ExpressionTree* right)
{
    AbstractExpressionNode* temp = left->root;
    left->root = right->root;
    right->root = temp;

    std::map<const std::string, double> tempMap = left->variablesMap;
    left->variablesMap = right->variablesMap;
    right->variablesMap = tempMap;
}

void ExpressionTree::setVariablesValues(const std::vector<double>& valuesVec)
{
    unsigned i = 0;
    for(auto& pair : variablesMap)
        pair.second = valuesVec[i++];

    updateVarsValuesRec(root);
}

unsigned int ExpressionTree::getNumberOfVariables() const
{
    return variablesMap.size();
}

void ExpressionTree::changeLastNode(AbstractExpressionNode* newNode)
{
	AbstractExpressionNode** parent = &root;
	lastNodeRec(&root, parent);

	unsigned childsSize = (*parent)->getChilds().size();
	if (childsSize > 0)
	{
		delete (*parent)->childs[childsSize - 1];
		(*parent)->childs[childsSize - 1] = newNode;
	}
	else
	{
		delete root;
		root = newNode;
	}

}

void ExpressionTree::lastNodeRec(AbstractExpressionNode** root, AbstractExpressionNode** parent)
{
	if (*root)
	{
		if((*root)->childs.size() > 0)
		{
			*parent = *root;
			lastNodeRec(&(*root)->childs[(*root)->childs.size() - 1], parent);
		}	
	}
}

ExpressionTree ExpressionTree::operator+(const ExpressionTree& other) const
{
    ExpressionTree expTree;
    expTree.root = root->getCopy();

	std::cout << 1 << std::endl;
    expTree.changeLastNode(other.root->getCopy());

    expTree.fillVariablesMap();
    return expTree;
}

std::string ExpressionTree::variablesToString() const
{
    std::string vars("");
    if(variablesMap.empty())
        vars = "No variables.  ";
    for(auto& var : variablesMap)
        vars += var.first + ": " + std::to_string(var.second) + ", ";

    //remove last 2 chars ", "
    vars.pop_back();
    vars.pop_back();
    return vars;
}

double ExpressionTree::getResult() const
{
    return root->getValue();
}

std::vector<std::string> ExpressionTree::toStringVec() const
{
    std::vector<std::string> strVec;
    toStringVecRec(root, strVec);
    return strVec;
}

void ExpressionTree::toStringVecRec(const AbstractExpressionNode* root, std::vector<std::string>& strVec) const
{
    if(root)
    {
        strVec.push_back(root->toString());
        for(auto& child : root->getChilds())
            toStringVecRec(child, strVec);
    }
}

std::string ExpressionTree::toString() const
{
    return root->toStringTree();
}

AbstractExpressionNode*** ExpressionTree::getRandomNode()
{
    AbstractExpressionNode*** randomNode = new AbstractExpressionNode**();

    int nodeNumber = Helper::getRandomNumber() % getTreeSize();
    getRandomNodeRec(&root, nodeNumber, randomNode);

    return randomNode;
}

void ExpressionTree::getRandomNodeRec(AbstractExpressionNode** root, int& nodeNumber, AbstractExpressionNode*** randomNode)
{
    if(root && nodeNumber >= 0)
    {
        if(nodeNumber-- == 0)
        {
            *randomNode = root;
        }
        else
        {
            for(auto& child : (*root)->childs)
                getRandomNodeRec(&child, nodeNumber, randomNode);
        }
    }
}

void ExpressionTree::mutate()
{
    AbstractExpressionNode*** randomNode = getRandomNode();
    delete **randomNode;

    int mutationNodeOperatorsCount = Helper::getRandomNumber()%5;
    **randomNode = RandomNodeGenerator::getRandomTree(variablesMap, mutationNodeOperatorsCount);

    delete randomNode;
}

void ExpressionTree::createTree(const std::vector<std::string>& strVec)
{
    int it = 0;
	int end = strVec.size();
    clearTree();
    root = createNode(it, end, strVec);
	fillVariablesMap();
}

AbstractExpressionNode* ExpressionTree::createNode(int& it, const int end, const std::vector<std::string>& strVec) const
{
    AbstractExpressionNode* result = nullptr;
    if(it != end)
    {
        const std::string& element = strVec[it];
		if (KnownOperators::getKnownOperators()->isOperator2Args(element))
		{
			AbstractExpressionNode* arg1 = createNode(++it, end, strVec);
			AbstractExpressionNode* arg2 = createNode(++it, end, strVec);
			result = new Operator2Arg(element, arg1, arg2);
		}          
        else if(KnownOperators::getKnownOperators()->isOperator1Arg(element))
            result = new Operator1Arg(element, createNode(++it, end, strVec));
		else if (Helper::constainsOnlyNumbers(element))
			result = new Constant(std::stod(element));        
        else if(Helper::constainsOnlyLettersOrNumbers(element))
			result = new Variable(element);          
    }
    else
        result = getFulfillConstant();

    return result;
}

AbstractExpressionNode* ExpressionTree::getFulfillConstant() const
{
    return new Constant(1);
}

void ExpressionTree::clearTree()
{
    if(root)
    {
        delete root;
        root = nullptr;
    }
    variablesMap.clear();
}

std::map<const std::string, double> ExpressionTree::getVariablesMap() const
{
    return variablesMap;
}

void ExpressionTree::fillVariablesMap()
{
	variablesMap.clear();
    fillVariablesMapRec(root);
}

void ExpressionTree::fillVariablesMapRec(const AbstractExpressionNode* root)
{
    if(root)
    {
        if(root->isVariable())
        {
            const Variable* var = dynamic_cast<const Variable*>(root);
            variablesMap.insert(std::make_pair<const std::string, double>(var->getName(), var->getValue()));
        }
        for(const AbstractExpressionNode* child : root->getChilds())
            fillVariablesMapRec(child);
    }
}

void ExpressionTree::updateVarsValuesRec(AbstractExpressionNode* root)
{
    if(root)
    {
        if(root->isVariable())
        {
            Variable* var = dynamic_cast<Variable*>(root);
            var->setValue(variablesMap[var->getName()]);
        }

        for(AbstractExpressionNode* child : root->getChilds())
            updateVarsValuesRec(child);
    }
}


unsigned ExpressionTree::getTreeSize() const
{
    unsigned size = 0;
    getTreeSizeRec(root, size);
    return size;
}
