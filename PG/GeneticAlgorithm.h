#pragma once
#include "limits"
#include <vector>
#include <string>
#include "ExpressionTree.h"

class GeneticAlgorithm
{
public:
	struct VauluatedTree
	{
		VauluatedTree() : value(std::numeric_limits<double>::max()), tree(nullptr) {}
		VauluatedTree(double value, ExpressionTree* treePtr) : value(value), tree(treePtr) {}
		double value;
		ExpressionTree* tree;
	};

    struct Data
    {
        Data(){}
        std::vector<std::vector<double>> dataValues;
        std::vector<double> dataResults;
    };

    struct Setup
    {
        Setup(unsigned pop, unsigned it, unsigned cross, unsigned mutate, int id)
            : population(pop), iteration(it), crossChance(cross), mutationChance(mutate), id(id){}
        unsigned population;
        unsigned iteration;
        unsigned crossChance;
        unsigned mutationChance;
        int id;

        std::string toString()
        {
            return "Setup: population:" + std::to_string(population) + " iteration:" + std::to_string(iteration)
                    + " crossChance:" + std::to_string(crossChance) + " mutationChance:" + std::to_string(mutationChance);
        }
    };

    struct Result
    {
        Result() : expression(""), value(std::numeric_limits<double>::max()), choosenOne(false) {}

        std::string expression;
        double value;
        bool choosenOne;
        std::string toString()
        {
            return std::string("Result: Value "+std::to_string(value)+" Expression: "+expression);
        }
    };

    GeneticAlgorithm(){}

    Result process(const Setup& setup, Data &data);

private:
    void initiate(const unsigned& populationSize, std::vector<VauluatedTree>& population) const;
	void iteration(std::vector<VauluatedTree>& population, const Setup& setup, Result& result, bool& foundChoosenOne);
    void select(const unsigned& populationSize, std::vector<VauluatedTree>& population) const;
	VauluatedTree selectBestFromRandTwo(const std::vector<VauluatedTree> &population) const;
	void evaluateTree(VauluatedTree& expTree) const;
    void crossOver(const unsigned& crossOverProb, std::vector<VauluatedTree> &expTreesVec) const;
    void crossOverTreesPair(std::pair<VauluatedTree, VauluatedTree> &expTreesPair) const;
    std::pair<VauluatedTree, VauluatedTree> withdrawTreesPair(std::vector<VauluatedTree> &expTreesVec) const;
	VauluatedTree withdrawRandTree(std::vector<VauluatedTree> &expTreesVec) const;
    void mutate(const unsigned& mutateProb, std::vector<VauluatedTree> &expTreesVec) const;
    bool evalPopulation(std::vector<VauluatedTree>& population, Result &result) const;

    std::vector<std::vector<double>> dataValues;
    std::vector<double> dataResults;
};

