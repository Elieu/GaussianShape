/**
 * Genetic Optimizer Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file GeneticOptimizer.h
 * @author Chaoqian Cai
 * @version v2.0
 * @date 2010-10-11
 */


#ifndef GENETIC_OPTIMIZER_INCLUDE_H
#define GENETIC_OPTIMIZER_INCLUDE_H
//


#include "InterfaceFunctionValueEvaluator.h"

#include <string>
#include <vector>


/**
 * Description:
 */
class CGeneticOptimizer
{
	/* data: */
public:
	/* Error code. */
	struct ErrorCodes
	{
		static const int nNORMAL;

	private:
		ErrorCodes();
	};


private:
	// individuals of initial population
	const std::vector<std::vector<double> > _INITIAL_POPULATION;
	// individuals of current population
	std::vector<std::vector<double> > _currentPopulation;
	// crossover probability
	double _dCrossoverProbability;
	// mutation probability
	double _dMutationProbability;
	// fitness sum of all individuals in current population
	double _dTotalFitness;
	// reference of an external fitness evaluator
	IFunctionValueEvaluator& _fitnessEvaluator;
	// mutation magnitude for each gene
	std::vector<double> _mutationMagnitude;
	// gene number per chromosome
	const int _nGENE_AMOUNT_PER_CHROMOSOME;
	// population size (amount of individuals per generation)
	const int _nPOPULATION_SIZE;
	// individuals of new population
	std::vector<std::vector<double> > _newPopulation;
	// fitness of individuals in current population
	std::vector<double> _populationFitness;
	// upper limit for iterations
	int _nMaxIterations;

	
	/* Message string: */
	struct MessageTexts
	{
		static const std::string sINVALID_FITNESS;
		static const std::string sINVALID_INIT_POPULATION;
		static const std::string sINVALID_PARAMETER;
		static const std::string sINVALID_PERCENTAGE;
		static const std::string sINVALID_PROBABILITY;

	private:
		MessageTexts();
	};

	/* Default values: */
	struct DefaultValues
	{
		// crossover probability
		static const double dCROSSOVER_PROBABILITY;
		// percentage of mutation magnitude
		static const double dDEFAULT_MUTATION_MAGNITUDE_PERCENTAGE;
		// mutation probability
		static const double dMUTATION_PROBABILITY;
		//
		static const int nMAX_ITERATIONS;

	private:
		DefaultValues();
	};

	/* method: */
public:
	CGeneticOptimizer(IFunctionValueEvaluator& fitnessEvaluator, const std::vector<std::vector<double> >& initialPopulation);
	~CGeneticOptimizer();

	int runOptimization(std::vector<double>& resultants, double& dBestFitness);
	int traceOptimization(std::vector<std::vector<double> >& trajectory);

	double getCrossoverProbability() const;
	const std::vector<double>& getMutationMagnitude() const;
	int getMaxIterations() const;
	double getMutationProbability() const;

	void setCrossoverProbability(double dProbability);
	void setMaxIterations(int nMaxIterations);
	void setMutationMagnitude(const std::vector<double>& mutationMagnitude);
	void setMutationMagnitudeByPercentage(double dPercentage);
	void setMutationProbability(double dProbability);
private:
	int crossover(const std::vector<double>& oldChromosome1, const std::vector<double>& oldChromosome2, std::vector<double>& newChromosome1, std::vector<double>& newChromosome2);
	int getBestIndividualId() const;
	int mutateNewPopulation(double dAttenuation);
	int randomCompetitionSelectIndividual() const;
	int replaceCurrentPopulation();
	int rouletteSelectIndividual() const;
	int updatePopulationFitness();
};


//
#endif
