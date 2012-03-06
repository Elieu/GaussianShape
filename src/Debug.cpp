/**
 * Debug Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file Debug.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-09-26
 */


#include "AssignmentSolver.h"
#include "Atom.h"
#include "AtomIterator.h"
#include "Bond.h"
#include "Debug.h"
#include "Exception.h"
#include "GaussianVolume.h"
#include "GaussianVolumeOverlapEvaluator.h"
#include "GeneticOptimizer.h"
#include "MoleculeReader.h"
#include "InterfaceMoleculeReader.h"
#include "Mathematics.h"
#include "Molecule.h"
#include "MoleculeReaderManager.h"
#include "GaussianService.h"
#include "SimplexOptimizer.h"
#include "Utility.h"

#include <ctime>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>


using std::auto_ptr;
using std::cout;
using std::endl;
using std::map;
using std::set;
using std::string;
using std::vector;


double CTestFitnessEvaluator::getFunctionValue(const std::vector<double>& params)
{
	double dValue = 0.0;
	for (int i = 0; i < static_cast<int>(params.size()); i++)
	{
		dValue += (params[i] - i) * (params[i] - i);
	}

	return (100 - dValue > 0.0001) ? 100 - dValue : 0.0001;
}


double CTestFunctionValueEvaluator::getFunctionValue(const std::vector<double>& params)
{
	double dValue = 0.0;
	for (int i = 0; i < static_cast<int>(params.size()); i++)
	{
		dValue += (params[i] - i) * (params[i] - i);
	}

	return dValue;
}

void testMove(CMolecule mol)
{
	mol.move(500, 500, 500);
}


int debug()
{
	string s(__MY_FUNCTION__);
	return 0;
}


int stabilityTest()
{
	/* Stability test. */
	std::fstream outStability("D:\\temp\\stability.txt", std::ios::out);

	CMol2Reader<CAtom, CBond> reader1(string("D:\\Temp\\tmp1.mol2"));
	//reader1.setReadHydrogenFlag(true);
	CMol2Reader<CAtom, CBond> reader2(string("D:\\Temp\\tmp1.mol2"));
	CMolecule mol1, mol2;
	reader1.readMolecule(mol1);
	reader2.readMolecule(mol2);
	mol1.moveToCentroid();
	mol2.moveToCentroid();

	CGaussianVolumeOverlapEvaluator gaussianOverlap(mol1, mol2);
	gaussianOverlap.setNegativeOverlapFlag(true);

	srand(static_cast<unsigned int>(time(NULL)));

	for (int iTest = 0; iTest < 100; iTest++)
	{
		/* Construct initial feasible solutions. */
		const int nMAX_GROUP = 32;
		const int nDIMENSION = 6;
		vector<vector<vector<double> > > initialSolutionsGroup;
		// For each group:
		for (int iGroup = 0; iGroup < nMAX_GROUP; iGroup++)
		{
			// current group
			vector<vector<double> > currentGroup;

			// For each solution:
			for (int iSolution = 0; iSolution < nDIMENSION + 1; iSolution++)
			{
				// current solution
				vector<double> currentSolution;

				// For each dimension:
				for (int iDimension = 0; iDimension <nDIMENSION; iDimension++)
				{
					double dRandom = 0.0; 
					if (iDimension < 3)
					{
						dRandom = 2 * (rand() / static_cast<double>(RAND_MAX)) - 1;
						dRandom *= 4;
					}
					else
					{
						dRandom = 2 * (rand() / static_cast<double>(RAND_MAX)) - 1;
						dRandom *= 3.1415926;
					}
					currentSolution.push_back(dRandom);
				}
				currentGroup.push_back(currentSolution);
			}
			initialSolutionsGroup.push_back(currentGroup);
		}

		CSimplexOptimizer mySimplexOptimizer(gaussianOverlap, initialSolutionsGroup);
		mySimplexOptimizer.setReflectionFactor(1.0);
		mySimplexOptimizer.setExtensionFactor(3.5);
		mySimplexOptimizer.setContractionFactor(0.5);

		vector<double> testResultPoint;
		double dTestResultValue = 0.0;

		TIME_START();
		mySimplexOptimizer.runOptimization(testResultPoint, dTestResultValue, 50);
		TIME_SECONDS(x);

		std::cout << x << endl;

		outStability << std::abs(dTestResultValue) << endl;
	}
	outStability.close();

	return 0;
}


int alignMolecule()
{
	// reference molecule and fit molecule
	CMolecule refMol, fitMol;

	/* Reading reference molecule. */
	auto_ptr<IMoleculeReader> refReaderPtr = CMoleculeReaderManager::getMoleculeReader(string("D:\\temp\\PASS\\site_1.pdb"));
	refReaderPtr->setReadHydrogenFlag(true);
	refReaderPtr->readMolecule(refMol);

	/* Reading fit molecule. */
	auto_ptr<IMoleculeReader> fitReaderPtr = CMoleculeReaderManager::getMoleculeReader(string("D:\\temp\\tmp3.mol2"));
	fitReaderPtr->setReadHydrogenFlag(false);
	fitReaderPtr->readMolecule(fitMol);

	/* Centre molecules. */
	refMol.moveToCentroid();
	fitMol.moveToCentroid();

	/* Prepare function value evaluator. */
	CGaussianVolumeOverlapEvaluator gaussianOverlap(refMol, fitMol);
	//gaussianOverlap.setGaussianCutoff(1.1);
	gaussianOverlap.setNegativeOverlapFlag(true);

	/* Construct initial feasible solutions. */
	srand(static_cast<unsigned int>(time(NULL)));
	const int nMAX_GROUP = 16;
	const int nDIMENSION = 6;
	vector<vector<vector<double> > > initialSolutionsGroup;
	// For each group:
	for (int iGroup = 0; iGroup < nMAX_GROUP; iGroup++)
	{
		// current group
		vector<vector<double> > currentGroup;

		// For each solution:
		for (int iSolution = 0; iSolution < nDIMENSION + 1; iSolution++)
		{
			// current solution
			vector<double> currentSolution;

			// For each dimension:
			for (int iDimension = 0; iDimension < nDIMENSION; iDimension++)
			{
				double dRandom = 0.0; 
				if (iDimension < 3)
				{
					dRandom = 2 * (rand() / static_cast<double>(RAND_MAX)) - 1;
					dRandom *= 4;
				}
				else
				{
					dRandom = 2 * (rand() / static_cast<double>(RAND_MAX)) - 1;
					dRandom *= 3.1415926;
				}
				currentSolution.push_back(dRandom);
			}
			currentGroup.push_back(currentSolution);
		}
		initialSolutionsGroup.push_back(currentGroup);
	}

	/* Prepare optimizer. */
	CSimplexOptimizer mySimplexOptimizer(gaussianOverlap, initialSolutionsGroup);
	mySimplexOptimizer.setReflectionFactor(0.9);
	mySimplexOptimizer.setExtensionFactor(1.5);
	mySimplexOptimizer.setContractionFactor(0.5);

	/* Do optimization. */
	vector<double> resultPoint;
	double dResultValue = 0.0;

	TIME_START();
	mySimplexOptimizer.runOptimization(resultPoint, dResultValue, 50);
	TIME_SECONDS(x);

	std::cout << "Time(s):" << x << endl;

	/* Transform fit molecule to optimized orientation. */
	double& dTranslationX = resultPoint[0];
	double& dTranslationY = resultPoint[1];
	double& dTranslationZ = resultPoint[2];

	double& dRotationX = resultPoint[3];
	double& dRotationY = resultPoint[4];
	double& dRotationZ = resultPoint[5];

	fitMol.move(dTranslationX, dTranslationY, dTranslationZ);
	fitMol.rotateXYZ(dRotationX, dRotationY, dRotationZ);

	/* Output molecules. */
	CMol2Writer refMolWriter("D:\\temp\\out1.mol2");
	refMolWriter.writeMolecule(refMol);

	CMol2Writer fitMolWriter("D:\\temp\\out2.mol2");
	fitMolWriter.writeMolecule(fitMol);

	return 0;
}
