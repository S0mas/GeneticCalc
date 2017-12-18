#include "GeneticAlgorithm.h"
#include <cmath>
#include <exception>


GeneticAlgorithm::Result GeneticAlgorithm::process(const Setup& setup, Data &data)
{
    dataValues = data.dataValues;
    dataResults = data.dataResults;
    Result result;
    bool foundChoosenOne = false;
    std::vector<ExpressionTree> population;
    population.reserve(setup.population*2);

    initiate(setup.population, population);
    if(!population.empty())
    {
        for(unsigned i = 0; i < setup.iteration; ++i)
        {
            foundChoosenOne = lookForGoldenChild(population, result);
            if(foundChoosenOne)
                break;
            std::cout << "\nselect" << std::endl;
            select(setup.population, population);
            std::cout << "\ncross" << std::endl;
            crossOver(setup.crossChance, population);
            std::cout << "\nmutate" << std::endl;
            mutate(setup.mutationChance, population);
        }
        //Find best result
        if(!foundChoosenOne)
        {
            int bestId = 0;
            double bestResult = evaluateTree(population[bestId]);

            for(unsigned i = 1; i < population.size(); ++i)
            {
                double temp = evaluateTree(population[i]);
                if(temp < bestResult)
                    bestResult = temp;
            }

            result.expression = population[bestId].toString();
            result.value = bestResult;
        }
    }
    else
        Logger::printError("Population is empty!");

    return result;
}

bool GeneticAlgorithm::lookForGoldenChild(std::vector<ExpressionTree>& population, Result& result) const
{
    bool found = false;
    for(auto& tree : population)
    {
        double temp = evaluateTree(tree);
        if( temp < 0.000001)
        {
            Logger::printInfo("Found ChoosenOne!");
            result.value = temp;
            result.expression = tree.toString();
            result.choosenOne = true;
            found = true;
            break;
        }
    }
    return found;
}

void GeneticAlgorithm::initiate(const unsigned &populationSize, std::vector<ExpressionTree>& population) const
{
    unsigned varsNumb = 2;
    unsigned minNodesNumber = 1;
    unsigned maxNodesNumber = 3;
    population.reserve(populationSize*2);

    for(unsigned i = 0; i < populationSize; ++i)
    {
        population.push_back(ExpressionTree::generateRandomTree(varsNumb, minNodesNumber, maxNodesNumber));
        std::cout << "RandomTreeSize: " << population.back().getTreeSize() << std::endl;
    }
}

void GeneticAlgorithm::select(const unsigned& populationSize, std::vector<ExpressionTree>& population) const
{
    std::vector<ExpressionTree> newParents;
    newParents.reserve(populationSize*2);
    for(unsigned i = 0; i < populationSize; ++i)
    {
        Logger::printInfo(std::to_string(i));
        newParents.push_back(selectBestFromRandTwo(population));
    }
    population.clear();
    population = newParents;
}

ExpressionTree GeneticAlgorithm::selectBestFromRandTwo(std::vector<ExpressionTree>& expTreesVec) const
{
    unsigned rand1 = Helper::getRandomNumber()%expTreesVec.size();
    unsigned rand2 = Helper::getRandomNumber()%expTreesVec.size();
    while(rand1 == rand2)
        rand2 = Helper::getRandomNumber()%expTreesVec.size();

    Logger::printInfo("ELO1:" + expTreesVec[rand1].root->toAddressTree());
    Logger::printInfo("ELO2:" + expTreesVec[rand2].root->toAddressTree());

    return ( evaluateTree(expTreesVec[rand1]) < evaluateTree(expTreesVec[rand2])) ? expTreesVec[rand1] : expTreesVec[rand2];
}

double GeneticAlgorithm::evaluateTree(ExpressionTree &expTree) const
{
    double evalTree = 0;
    for(unsigned i = 0; i < dataValues.size(); ++i)
    {
        expTree.setVariablesValues(dataValues[i]);
        double result = expTree.getResult();
        evalTree += pow(dataResults[i] - result, 2);
        if(std::isnan(result))
           throw std::exception();
    }
    return evalTree;
}

void GeneticAlgorithm::crossOver(const unsigned& crossOverProb, std::vector<ExpressionTree>& population) const
{
    std::vector<ExpressionTree> childrensPopulation;
    childrensPopulation.reserve(population.size()*2);
    while(!population.empty())
    {
        std::pair<ExpressionTree, ExpressionTree> expTreePair = withdrawTreesPair(population);
        if(Helper::getRandomNumber()%101 < crossOverProb)
            crossOverTreesPair(expTreePair);
        childrensPopulation.push_back(expTreePair.first);
        childrensPopulation.push_back(expTreePair.second);
    }
    population.clear();
    population =childrensPopulation;
}

void GeneticAlgorithm::crossOverTreesPair(std::pair<ExpressionTree, ExpressionTree> &expTreesPair) const
{
    ExpressionTree::crossOver(expTreesPair.first, expTreesPair.second);
}

std::pair<ExpressionTree, ExpressionTree> GeneticAlgorithm::withdrawTreesPair(std::vector<ExpressionTree> &expTreesVec) const
{
    std::pair<ExpressionTree, ExpressionTree> pair;
    pair.first = withdrawRandTree(expTreesVec);
    pair.second = withdrawRandTree(expTreesVec);
    return pair;
}

ExpressionTree GeneticAlgorithm::withdrawRandTree(std::vector<ExpressionTree> &expTreesVec) const
{
    unsigned rand = Helper::getRandomNumber()%expTreesVec.size();
    ExpressionTree randTree = std::move(expTreesVec[rand]);
    expTreesVec.erase(expTreesVec.begin()+rand);

    return randTree;
}

void GeneticAlgorithm::mutate(const unsigned& mutateProb, std::vector<ExpressionTree>& expTreesVec) const
{
    for(unsigned i = 0; i < expTreesVec.size(); ++i)
    {
        if(Helper::getRandomNumber()%101 < mutateProb)
        {
            std::cout << "To mutate: " <<Helper::converAddressToString(expTreesVec[i].root) <<std::endl;
            expTreesVec[i].mutate();
        }
    }
}
