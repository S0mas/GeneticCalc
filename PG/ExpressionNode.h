#pragma once

#include "Helper.h"
#include "KnownOperators.h"
#include "Logger.h"
#include <typeinfo>

class AbstractExpressionNode
{
public:
    AbstractExpressionNode() 
	{
		++counterAbstractCreated;
	}
    static std::vector<std::string> deletedAddresses;
    AbstractExpressionNode(const AbstractExpressionNode& other)
    {
		++counterAbstractCreated;
        for(const AbstractExpressionNode* child : childs)
        {
            if(child)
            {
                delete child;
            }

        }
        childs.clear();

        for(const AbstractExpressionNode* child : other.childs)
            childs.push_back(child->getCopy());
    }
	virtual ~AbstractExpressionNode()								{ ++counterAbstractDestroyed; }

    virtual double getValue() const = 0;
    virtual std::vector<AbstractExpressionNode*> getChilds() const  { return childs;}
    virtual bool isVariable() const                                 { return false;}
    virtual std::string toString() const = 0;
    virtual std::string toStringTree() const = 0;
    virtual std::string toAddressTree() const                       { return "ABSTRACT";}
    virtual AbstractExpressionNode* getCopy() const = 0;

	static  unsigned counterAbstractDestroyed;
	static  unsigned counterAbstractCreated;
    std::vector<AbstractExpressionNode*> childs;
};

//No childs
class Constant : public AbstractExpressionNode
{
public:
    Constant(const double& value)
        :  value(value) 
	{
		++counterConstantCreated;
	}

    Constant(const Constant& other)
        : AbstractExpressionNode(other), value(other.value)
	{
		++counterConstantCreated;
	}

	virtual ~Constant()											{ ++counterConstantDestroyed; }
    virtual double getValue() const override                    { return value; }
    virtual std::string toString() const override               { return std::to_string(value); }
    virtual std::string toStringTree() const override           { return toString();}
    virtual AbstractExpressionNode* getCopy() const override    { return new Constant(value); }
    virtual std::string toAddressTree() const  override         { return "Constant: " +Helper::converAddressToString(this);}
	static unsigned counterConstantDestroyed;
	static unsigned counterConstantCreated;
private:
    const double value;
};

//No childs
class Variable : public AbstractExpressionNode
{
public:
    Variable(const std::string& name)
        :  name(name), value(1) 
	{
		++counterVariableCreated;
	}
    Variable(const std::string& name, double value)
        :  name(name), value(value) 
	{
		++counterVariableCreated;
	}
    Variable(const Variable& other)
        :  AbstractExpressionNode(other), name(other.name), value(other.value) 
	{
		++counterVariableCreated;
	}

	virtual ~Variable()											{ ++counterVariableDestroyed; }
    virtual double getValue() const override                    { return value; }
    virtual std::string toString() const override               { return name; }
    virtual std::string toStringTree() const override           { return toString();}
    std::string getName() const                                 { return name; }
    void setValue(const double& newValue)                       { value = newValue; }
    virtual bool isVariable() const override                    { return true;}
    virtual AbstractExpressionNode* getCopy() const override    { return new Variable(name, value); }
    virtual std::string toAddressTree() const  override			{ return "Var: " +Helper::converAddressToString(this);}
	static unsigned counterVariableDestroyed;
	static unsigned counterVariableCreated;
private:
    const std::string name;
    double value;
};


class AbstractOperatorNode : public AbstractExpressionNode
{
public:
    AbstractOperatorNode(const std::string& type) : type(type)
	{
		++counterAbstractOpCreated;
	}

    AbstractOperatorNode(const AbstractOperatorNode& other)
        : AbstractExpressionNode(other), type(other.type) 
	{
		++counterAbstractOpDestroyed;
    }

    virtual ~AbstractOperatorNode() 
	{
		++counterAbstractOpDestroyed;
	}
    virtual std::string toString() const override       { return type; }
    const std::string type;

	static unsigned counterAbstractOpDestroyed;
	static unsigned counterAbstractOpCreated;
};

//1 child
class Operator1Arg : public AbstractOperatorNode
{
public:
    Operator1Arg(const std::string& type, AbstractExpressionNode* arg1) :
        AbstractOperatorNode(type)
    {
		++counterOp1Created;
        childs.push_back(arg1);
    }

    Operator1Arg(const Operator1Arg& other) :
        AbstractOperatorNode(other)
	{
		++counterOp1Created;
    }

    virtual ~Operator1Arg()
    {
		++counterOp1Destroyed;
        for(const AbstractExpressionNode*  child : childs)
        {
            if(child)
                delete child;
        }
        childs.clear();
    }
    virtual double getValue() const override                    { return KnownOperators::getKnownOperators()->getFunction1Arg(type)(childs[0]->getValue()); }
    virtual std::string toStringTree() const override           { return toString() + " " + childs[0]->toStringTree(); }
    virtual AbstractExpressionNode* getCopy() const override    { return new Operator1Arg(type, childs[0]->getCopy()); }
    virtual std::string toAddressTree() const  override         { return "Op1: " +Helper::converAddressToString(this) + " ---> " + childs[0]->toAddressTree();}
	static unsigned counterOp1Destroyed;
	static unsigned counterOp1Created;
};

//2 childs
class Operator2Arg : public AbstractOperatorNode
{
public:
    Operator2Arg(const std::string& type, AbstractExpressionNode* arg1, AbstractExpressionNode* arg2)
       : AbstractOperatorNode(type)
    {
		++counterOp2Created;
        childs.push_back(arg1);
        childs.push_back(arg2);
    }

    Operator2Arg(const Operator2Arg& other)
        : AbstractOperatorNode(other)
    {
		++counterOp2Created;
    }

    virtual ~Operator2Arg()
    {
		++counterOp2Destroyed;
        for(const AbstractExpressionNode*  child : childs)
        {
            if(child)
                delete child;
        }
        childs.clear();
    }
    virtual double getValue() const override                    { return KnownOperators::getKnownOperators()->getFunction2Args(type)(childs[0]->getValue(), childs[1]->getValue()); }
    virtual std::string toStringTree() const override           { return toString() + " " + childs[0]->toStringTree() + " " + childs[1]->toStringTree();}
    virtual AbstractExpressionNode* getCopy() const override    { return new Operator2Arg(type, childs[0]->getCopy(), childs[1]->getCopy()); }
    virtual std::string toAddressTree() const  override         { return "Op2: " +Helper::converAddressToString(this) + " ---> " + childs[0]->toAddressTree() + " ---> " + childs[1]->toAddressTree();}
	static unsigned counterOp2Destroyed;
	static unsigned counterOp2Created;
};

