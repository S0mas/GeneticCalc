#include "GeneticAlgorithm.h"
#include <cmath>
#include <exception>


GeneticAlgorithm::Result GeneticAlgorithm::process(const Setup& setup, Data &data)
{
    dataValues = data.dataValues;
    dataResults = data.dataResults;
    Result result;
    bool foundChoosenOne = false;
    std::vector<VauluatedTree> population;
    population.reserve(setup.population*2);

    initiate(setup.population, population);
	foundChoosenOne = evalPopulation(population, result);
	for (int i = 0; i < setup.iteration && !foundChoosenOne; ++i)
		iteration(population, setup, result, foundChoosenOne);

	//Find best result
	if (!foundChoosenOne)
	{
		int bestId = 0;
		double bestResult = population.begin()->value;

		for (unsigned i = 1; i < population.size(); ++i)
		{
			const double& temp = population[i].value;
			if (temp < bestResult)
			{
				bestId = i;
				bestResult = temp;
			}			
		}

		result.expression = population[bestId].tree->toString();
		result.value = bestResult;
	}

    return result;
}

void GeneticAlgorithm::iteration(std::vector<VauluatedTree>& population, const Setup& setup, Result& result, bool& foundChoosenOne)
{
	select(setup.population, population);
	crossOver(setup.crossChance, population);
	mutate(setup.mutationChance, population);
	foundChoosenOne = evalPopulation(population, result);
	if(foundChoosenOne)
		return;
}

bool GeneticAlgorithm::evalPopulation(std::vector<VauluatedTree>& population, Result& result) const
{
    bool foundChoosenOne = false;
    for(VauluatedTree& valTree : population)
    {
		evaluateTree(valTree);
        if(valTree.value < 0.000001)
        {
            Logger::printInfo("Found ChoosenOne!");
            result.value = valTree.value;
            result.expression = valTree.tree->toString();
            result.choosenOne = true;
			foundChoosenOne = true;
            break;
        }
    }
    return foundChoosenOne;
}

void GeneticAlgorithm::initiate(const unsigned &populationSize, std::vector<VauluatedTree>& population) const
{
    unsigned varsNumb = 2;
    unsigned minNodesNumber = 0;
    unsigned maxNodesNumber = 2;
    population.reserve(populationSize*2);

    for(unsigned i = 0; i < populationSize; ++i)
    {
		ExpressionTree* newTree = ExpressionTree::generateRandomTree(varsNumb, minNodesNumber, maxNodesNumber);	
        population.push_back(VauluatedTree(0, newTree));
    }
}

void GeneticAlgorithm::select(const unsigned& populationSize, std::vector<VauluatedTree>& population) const
{
    std::vector<VauluatedTree> newParents;
    newParents.reserve(populationSize*2);
    for(unsigned i = 0; i < populationSize; ++i)
        newParents.push_back(selectBestFromRandTwo(population));
    population.clear();
    population = newParents;
}

GeneticAlgorithm::VauluatedTree GeneticAlgorithm::selectBestFromRandTwo(const std::vector<VauluatedTree> &population) const
{
    unsigned rand1 = Helper::getRandomNumber()%population.size();
    unsigned rand2 = Helper::getRandomNumber()%population.size();
    while(rand1 == rand2)
        rand2 = Helper::getRandomNumber()%population.size();

    return (population[rand1].value < population[rand1].value) ? 
		VauluatedTree(population[rand1].value, new ExpressionTree(*population[rand1].tree)) : VauluatedTree(population[rand2].value, new ExpressionTree(*population[rand2].tree));
}

void GeneticAlgorithm::evaluateTree(VauluatedTree& valTree) const
{
	valTree.value = 0;
    for(unsigned i = 0; i < dataValues.size(); ++i)
    {
		valTree.tree->setVariablesValues(dataValues[i]);
        const double& result = valTree.tree->getResult();
		valTree.value += pow(dataResults[i] - result, 2);
        if(std::isnan(result))
           throw std::exception();
    }
}

void GeneticAlgorithm::crossOver(const unsigned& crossOverProb, std::vector<VauluatedTree>& population) const
{
    std::vector<VauluatedTree> childrensPopulation;
    childrensPopulation.reserve(population.size()*2);
    while(!population.empty())
    {
        std::pair<VauluatedTree, VauluatedTree> expTreePair = withdrawTreesPair(population);
        if(Helper::getRandomNumber()%101 < crossOverProb)
            crossOverTreesPair(expTreePair);
        childrensPopulation.push_back(expTreePair.first);
        childrensPopulation.push_back(expTreePair.second);
    }
    population.clear();
    population = childrensPopulation;
}

void GeneticAlgorithm::crossOverTreesPair(std::pair<VauluatedTree, VauluatedTree> &valTreesPair) const
{
    ExpressionTree::crossOver(valTreesPair.first.tree, valTreesPair.second.tree);
}

std::pair<GeneticAlgorithm::VauluatedTree, GeneticAlgorithm::VauluatedTree> GeneticAlgorithm::withdrawTreesPair(std::vector<VauluatedTree> &population) const
{
    std::pair<VauluatedTree, VauluatedTree> pair;
    pair.first = withdrawRandTree(population);
    pair.second = withdrawRandTree(population);
    return pair;
}

GeneticAlgorithm::VauluatedTree GeneticAlgorithm::withdrawRandTree(std::vector<VauluatedTree> &population) const
{
    unsigned rand = Helper::getRandomNumber()%population.size();
	VauluatedTree randTree = population[rand];
	population.erase(population.begin()+rand);

    return randTree;
}

void GeneticAlgorithm::mutate(const unsigned& mutateProb, std::vector<VauluatedTree>& population) const
{
    for(unsigned i = 0; i < population.size(); ++i)
    {
        if(Helper::getRandomNumber()%101 < mutateProb)
			population[i].tree->mutate();
    }
}
