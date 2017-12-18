#pragma once
#include "limits"
#include <vector>
#include <string>
#include "ExpressionTree.h"

class GeneticAlgorithm
{
public:
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
    void initiate(const unsigned& populationSize, std::vector<ExpressionTree>& population) const;
    void select(const unsigned& populationSize, std::vector<ExpressionTree>& population) const;
    ExpressionTree selectBestFromRandTwo(std::vector<ExpressionTree> &expTreesVec) const;
    double evaluateTree(ExpressionTree &expTree) const;
    void crossOver(const unsigned& crossOverProb, std::vector<ExpressionTree> &expTreesVec) const;
    void crossOverTreesPair(std::pair<ExpressionTree, ExpressionTree> &expTreesPair) const;
    std::pair<ExpressionTree, ExpressionTree> withdrawTreesPair(std::vector<ExpressionTree> &expTreesVec) const;
    ExpressionTree withdrawRandTree(std::vector<ExpressionTree> &expTreesVec) const;
    void mutate(const unsigned& mutateProb, std::vector<ExpressionTree> &expTreesVec) const;
    bool lookForGoldenChild(std::vector<ExpressionTree>& population, Result &result) const;

    std::vector<std::vector<double>> dataValues;
    std::vector<double> dataResults;
};

