/**
 * Genetic Optimizer Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file GeneticOptimizer.cpp
 * @author Chaoqian Cai
 * @version v2.0
 * @date 2010-10-11
 */


#include "GeneticOptimizer.h"

#include "Exception.h"
#include "Utility.h"

#include <cstdlib>
#include <ctime>
#include <limits>
#include <cmath>
#include <sstream>
#include <stdexcept>


using std::abs;
using std::string;
using std::numeric_limits;
using std::vector;


/* Implementation for CGeneticOptimizer class: */

/* Static members: */
const int CGeneticOptimizer::ErrorCodes::nNORMAL = 0;

const string CGeneticOptimizer::MessageTexts::sINVALID_FITNESS("Invalid fitness. ");
const string CGeneticOptimizer::MessageTexts::sINVALID_INIT_POPULATION("Invalid initial population. ");
const string CGeneticOptimizer::MessageTexts::sINVALID_PARAMETER("Invalid parameter. ");
const string CGeneticOptimizer::MessageTexts::sINVALID_PERCENTAGE("Invalid percentage. ");
const string CGeneticOptimizer::MessageTexts::sINVALID_PROBABILITY("Invalid probability. ");

const double CGeneticOptimizer::DefaultValues::dCROSSOVER_PROBABILITY = 0.75;
const double CGeneticOptimizer::DefaultValues::dDEFAULT_MUTATION_MAGNITUDE_PERCENTAGE = 0.3;
const int CGeneticOptimizer::DefaultValues::nMAX_ITERATIONS = 200;
const double CGeneticOptimizer::DefaultValues::dMUTATION_PROBABILITY = 0.05;


/**
 * Description: Ctor.
 * @param fitnessEvaluator: Fitness function evaluator which implements the IFunctonValueEvaluator.
 * @param initialPopulation: Initial population.
 */
CGeneticOptimizer::CGeneticOptimizer(IFunctionValueEvaluator& fitnessEvaluator, const std::vector<std::vector<double> >& initialPopulation)
	:
	_INITIAL_POPULATION(initialPopulation),
	_currentPopulation(initialPopulation),
	_dTotalFitness(0.0),
	_fitnessEvaluator(fitnessEvaluator),
	_nGENE_AMOUNT_PER_CHROMOSOME(initialPopulation.front().size()),
	_nPOPULATION_SIZE(initialPopulation.size()),
	_newPopulation(_newPopulation)
{
	/* Validate dimensions of the initial population. */
	FOREACH(iterChromosome, initialPopulation, vector<vector<double> >::const_iterator)
	{
		// If the dimension of chromosome do not match:
		if (static_cast<int>(iterChromosome->size()) != _nGENE_AMOUNT_PER_CHROMOSOME)
		{
			std::stringstream msgStream;
			msgStream
				<< LOCATION_STREAM_INSERTION
				<< "Invalid parameter: "
				<< "initialPopulation: population size not consistent! ";
			throw CInvalidArgumentException(msgStream.str());
		}
	}

	/* Initialization. */
	_populationFitness.assign(_nPOPULATION_SIZE, 0.0);

	setCrossoverProbability(DefaultValues::dCROSSOVER_PROBABILITY);
	setMaxIterations(DefaultValues::nMAX_ITERATIONS);
	setMutationProbability(DefaultValues::dMUTATION_PROBABILITY);
	setMutationMagnitudeByPercentage(DefaultValues::dDEFAULT_MUTATION_MAGNITUDE_PERCENTAGE);

	srand(static_cast<unsigned int>(time(NULL)));
}


/**
 * Description: Dtor.
 */
CGeneticOptimizer::~CGeneticOptimizer()
{
}


/**
 * Description:
 * @param resultants: (OUT)
 * @param fitness: (OUT)
 * @param nMaxGenerations:
 */
int CGeneticOptimizer::runOptimization(std::vector<double>& resultants, double& dBestFitness)
{
	// For each generation:
	for (int iGeneration = 0; iGeneration < _nMaxIterations; iGeneration++)
	{
		updatePopulationFitness();

		// For each couple:
		for (int iCouple = 0; iCouple <= _nPOPULATION_SIZE / 2 - 1; iCouple++)
		{
			/* Selection. */
			int nParentIdA = randomCompetitionSelectIndividual();
			int nParentIdB = randomCompetitionSelectIndividual();

			/* Crossover takes place by probability. */
			double dProbability = rand() / static_cast<double>(RAND_MAX);
			if (dProbability <= _dCrossoverProbability)
			{
				crossover(_currentPopulation[nParentIdA], _currentPopulation[nParentIdB], _newPopulation[2 * iCouple], _newPopulation[2 * iCouple + 1]);
			}
			else
			{
				_newPopulation[2 * iCouple] = _currentPopulation[nParentIdA];
				_newPopulation[2 * iCouple + 1] = _currentPopulation[nParentIdB];
			}
		}
		// If one more offspring is needed:
		if (_nPOPULATION_SIZE % 2)
		{
			int nParentIdA = randomCompetitionSelectIndividual();
			int nParentIdB = randomCompetitionSelectIndividual();

			double dProbability = rand() / static_cast<double>(RAND_MAX);
			if (dProbability <= _dCrossoverProbability)
			{
				crossover(_currentPopulation[nParentIdA], _currentPopulation[nParentIdB], _newPopulation[_nPOPULATION_SIZE - 1], _newPopulation[_nPOPULATION_SIZE - 1]);
			}
			else
			{
				_newPopulation[_nPOPULATION_SIZE - 1] = _currentPopulation[nParentIdA];
			}
		}

		/* Mutation. */
		mutateNewPopulation(1.0);

		replaceCurrentPopulation();
	} // for

	/* Get best individual. */
	updatePopulationFitness();
	int nBestIndividualId = getBestIndividualId();
	resultants = _currentPopulation[nBestIndividualId];
	dBestFitness = _populationFitness[nBestIndividualId];

	return ErrorCodes::nNORMAL;
}


/**
 * Description:
 * @param trajectory: (OUT)
 * @param nMaxGeneration:
 */
int CGeneticOptimizer::traceOptimization(std::vector<std::vector<double> >& trajectory)
{
	// For each generation:
	for (int iGeneration = 0; iGeneration < _nMaxIterations; iGeneration++)
	{
		updatePopulationFitness();

		/* Gather trajectory information for current generation. */
		// average fitness of current population
		double dAverageFitness = _dTotalFitness / _nPOPULATION_SIZE;
		int nBestIndividualId = getBestIndividualId();
		// best fitness of current population
		double dBestFitness = _populationFitness[nBestIndividualId];

		/* Store trajectory information. */
		vector<double> trajectoryNode;
		trajectoryNode.push_back(dAverageFitness);
		trajectoryNode.push_back(dBestFitness);
		trajectory.push_back(trajectoryNode);

		// For each couple:
		for (int iCouple = 0; iCouple <= _nPOPULATION_SIZE / 2 - 1; ++ iCouple)
		{
			/* Selection. */
			int nParentIdA = randomCompetitionSelectIndividual();
			int nParentIdB = randomCompetitionSelectIndividual();

			/* Crossover takes place by probability. */
			double dProbability = rand() / static_cast<double>(RAND_MAX);
			if (dProbability <= _dCrossoverProbability)
			{
				crossover(_currentPopulation[nParentIdA], _currentPopulation[nParentIdB], _newPopulation[2 * iCouple], _newPopulation[2 * iCouple + 1]);
			}
			else
			{
				_newPopulation[2 * iCouple] = _currentPopulation[nParentIdA];
				_newPopulation[2 * iCouple + 1] = _currentPopulation[nParentIdB];
			}
		}
		// If one more offspring is needed:
		if (_nPOPULATION_SIZE % 2)
		{
			int nParentIdA = randomCompetitionSelectIndividual();
			int nParentIdB = randomCompetitionSelectIndividual();

			double dProbability = rand() / static_cast<double>(RAND_MAX);
			if (dProbability <= _dCrossoverProbability)
			{
				crossover(_currentPopulation[nParentIdA], _currentPopulation[nParentIdB], _newPopulation[_nPOPULATION_SIZE - 1], _newPopulation[_nPOPULATION_SIZE - 1]);
			}
			else
			{
				_newPopulation[_nPOPULATION_SIZE - 1] = _currentPopulation[nParentIdA];
			}
		}

		/* Mutation. */
		mutateNewPopulation(1.0);

		replaceCurrentPopulation();
	} // for

	return ErrorCodes::nNORMAL;
}


/**
 * Description:
 * @param oldChromosome1:
 * @param oldChromosome2:
 * @param newChromosome1:
 * @param newChromosome2:
 */
int CGeneticOptimizer::crossover(const std::vector<double>& oldChromosome1, const std::vector<double>& oldChromosome2, std::vector<double>& newChromosome1, std::vector<double>& newChromosome2)
{
	double dRandom = rand()  / static_cast<double>(RAND_MAX);
	// crossover position, range [0, _nGENE_AMOUNT_PER_CHROMOSOME - 1]
	const int nCROSSOVER_POSITION = static_cast<int>(floor(dRandom * (_nGENE_AMOUNT_PER_CHROMOSOME - 1)));
	// crossover factor
	const double dRANDOM_FACTOR = rand() / static_cast<double>(RAND_MAX);

	// For genes crossed:
	for (int i = nCROSSOVER_POSITION; i < _nGENE_AMOUNT_PER_CHROMOSOME; i++)
	{
		newChromosome1[i] = dRANDOM_FACTOR * oldChromosome1[i] + (1 - dRANDOM_FACTOR) * oldChromosome2[i];
		newChromosome2[i] = dRANDOM_FACTOR * oldChromosome2[i] + (1 - dRANDOM_FACTOR) * oldChromosome1[i];
	}

	// For genes remained:
	for (int i = 0; i < nCROSSOVER_POSITION; i++)
	{
		newChromosome1[i] = oldChromosome1[i];
		newChromosome2[i] = oldChromosome2[i];
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description:
 */
int CGeneticOptimizer::getBestIndividualId() const
{
	int nBestIndividualId = -1;
	double dBestFitness = -numeric_limits<double>::max();
	// For each individual:
	for (int i = 0; i < _nPOPULATION_SIZE; i++)
	{
		if (_populationFitness[i] > dBestFitness)
		{
			dBestFitness = _populationFitness[i];
			nBestIndividualId = i;
		}
	}

	return nBestIndividualId;
}


/**
 * Description:
 * @param dAttenuation:
 */
int CGeneticOptimizer::mutateNewPopulation(double dAttenuation)
{
	/* Check parameters. */
	if (dAttenuation < 0)
	{
		dAttenuation = 1.0;
	}

	// For each individual in new population:
	FOREACH(iterChromosome, _newPopulation, vector<vector<double> >::iterator)
	{
		vector<double>& chromosome = *iterChromosome;
		// For each gene:
		for (int i = 0; i < _nGENE_AMOUNT_PER_CHROMOSOME; i++)
		{
			/* Mutation takes place by probability. */
			double dProbability = rand() / static_cast<double>(RAND_MAX);
			if (dProbability <= _dMutationProbability)
			{
				// range [-1, +1]
				double dPerturbation = 2 * (rand() / static_cast<double>(RAND_MAX));
				// range [-|magnitude|, +|magnitude|]
				dPerturbation *= _mutationMagnitude[i];

				chromosome[i] += dPerturbation * dAttenuation;
			}
		}
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description: Select a single chromosome by roulette method based on their fitness.
 * @return: ID of the chromosome been selected.
 */
int CGeneticOptimizer::rouletteSelectIndividual() const
{
	static const string sLOCATION("CGeneticOptimizer::rouletteSelectIndividual(): ");

	double dRandom = rand() / static_cast<double>(RAND_MAX);
	dRandom *= _dTotalFitness;

	/* Calculate accumulated sum. */
	double dFitnessSum = 0.0;
	for (int i = 0; i < _nPOPULATION_SIZE; i++)
	{
		dFitnessSum += _populationFitness[i];
		if (dFitnessSum >= dRandom)
		{
			return i;
		}
	}

	/* Exception. */
	std::stringstream msgStream;
	msgStream
		<< LOCATION_STREAM_INSERTION
		<< "Logic error: "
		<< "Invalid fitness! ";
	throw CLogicException(msgStream.str());
}


/**
 * Description: Select a single chromosome by ramdom competition method which is based on roulette selection.
 * @return: ID of the chromosome been selected.
 */
int CGeneticOptimizer::randomCompetitionSelectIndividual() const
{
	int nIndividualId1 = rouletteSelectIndividual();
	int nIndividualId2 = rouletteSelectIndividual();

	const vector<double>& chromosome1 = _currentPopulation[nIndividualId1];
	const vector<double>& chromosome2 = _currentPopulation[nIndividualId2];
	double dFitness1 = _fitnessEvaluator.getFunctionValue(chromosome1);
	double dFitness2 = _fitnessEvaluator.getFunctionValue(chromosome2);

	return (dFitness1 > dFitness2) ? nIndividualId1 : nIndividualId2;
}


/**
 * Description:
 */
int CGeneticOptimizer::replaceCurrentPopulation()
{
	_currentPopulation = _newPopulation;

	return ErrorCodes::nNORMAL;
}


/**
 * Description: Calculate current population's individual fitness and fitness sum.
 */
int CGeneticOptimizer::updatePopulationFitness()
{
	// Calculate and store fitness for each chromosome.
	_dTotalFitness = 0.0;
	for (int i = 0; i < _nPOPULATION_SIZE; i++)
	{
		const vector<double>& chromosome = _currentPopulation[i];
		double dFitness = _fitnessEvaluator.getFunctionValue(chromosome);
		_populationFitness[i] = dFitness;
		_dTotalFitness += dFitness;
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description:
 */
double CGeneticOptimizer::getCrossoverProbability() const
{
	return _dCrossoverProbability;
}


/**
 * Description:
 */
int CGeneticOptimizer::getMaxIterations() const
{
	return _nMaxIterations;
}


/**
 * Description:
 */
const std::vector<double>& CGeneticOptimizer::getMutationMagnitude() const
{
	return _mutationMagnitude;
}


/**
 * Description:
 */
double CGeneticOptimizer::getMutationProbability() const
{
	return _dMutationProbability;
}


/**
 * Description:
 * @param dProbability:
 */
void CGeneticOptimizer::setCrossoverProbability(double dProbability)
{
	/* Check parameters. */
	if (dProbability < 0 || dProbability > 1)
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sINVALID_PARAMETER
			<< "Detail: dProbability = " << dProbability;
		throw CInvalidArgumentException(msgStream.str());
	}

	_dCrossoverProbability = dProbability;
}


/**
 * Description:
 * @param mutationMagnitude:
 */
void CGeneticOptimizer::setMutationMagnitude(const std::vector<double>& mutationMagnitude)
{
	_mutationMagnitude = mutationMagnitude;

	FOREACH(iterMagnitude, _mutationMagnitude, vector<double>::iterator)
	{
		*iterMagnitude = abs(*iterMagnitude);
	}
}


/**
 * Description:
 * @param dPercentage:
 */
void CGeneticOptimizer::setMutationMagnitudeByPercentage(double dPercentage)
{
	/* Check parameters. */
	if (dPercentage < 0 || dPercentage > 1)
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sINVALID_PARAMETER
			<< "Detail: dPercentage = " << dPercentage;
		throw CInvalidArgumentException(msgStream.str());
	}

	/* Record the maximum genes in initial population. */
	_mutationMagnitude.assign(_nGENE_AMOUNT_PER_CHROMOSOME, 0.0);
	FOREACH(iterChromosome, _INITIAL_POPULATION, vector<vector<double> >::const_iterator)
	{
		const vector<double>& CHROMOSOME = *iterChromosome;
		for (int i = 0; i < _nGENE_AMOUNT_PER_CHROMOSOME; i++)
		{
			_mutationMagnitude[i] = (abs(CHROMOSOME[i]) > _mutationMagnitude[i]) ? abs(CHROMOSOME[i]) : _mutationMagnitude[i];
		}
	}

	/* Update the mutation magnitude vector. */
	FOREACH(iterMagnitude, _mutationMagnitude, vector<double>::iterator)
	{
		*iterMagnitude *= dPercentage;
	}
}


/**
 * Description:
 */
void CGeneticOptimizer::setMaxIterations(int nMaxIterations)
{
	// If valid parameter:
	if (nMaxIterations > 0)
	{
		_nMaxIterations = nMaxIterations;
	}
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sINVALID_PARAMETER
			<< "Detail: nMaxIterations = " << nMaxIterations;
		throw CInvalidArgumentException(msgStream.str());
	}
}


/**
 * Description:
 * @param dProbability:
 */
void CGeneticOptimizer::setMutationProbability(double dProbability)
{
	/* Check paramters. */
	if (dProbability >= 0 && dProbability <= 1)
	{
		_dMutationProbability = dProbability;
	}
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sINVALID_PARAMETER
			<< "Detail: dProbability = " << dProbability;
		throw CInvalidArgumentException(msgStream.str());
	}
}
