/**
 * Gaussian Service Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file Service.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-02-21
 */


#include "GaussianService.h"

#include "BusinessException.h"
#include "ConfigurationArguments.h"
#include "Exception.h"
#include "GaussianVolume.h"
#include "GaussianVolumeOverlapEvaluator.h"
#include "InterfaceAtom.h"
#include "InterfaceMolecule.h"
#include "Mathematics.h"
#include "MoleculeManager.h"
#include "PocketComboSimilarityEvaluator.h"
#include "SimplexOptimizer.h"
#include "Utility.h"

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <list>
#include <sstream>
#include <vector>


using std::auto_ptr;
using std::list;
using std::map;
using std::string;
using std::stringstream;
using std::vector;


/* Static Member: */

const int CGaussianService::_nDIMENSIONS = 6;

/* Default Values: */
const double CGaussianService::DefaultValues::dSIMPLEX_CONTRACTION_FACTOR = 0.5;
const double CGaussianService::DefaultValues::dSIMPLEX_EXTENSION_FACTOR = 3.5;
const double CGaussianService::DefaultValues::dSIMPLEX_REFLECTION_FACTOR = 1.0;
const int CGaussianService::DefaultValues::nSIMPLEX_INITIAL_SOLUTION_GROUPS_NUMBER = 16;
const int CGaussianService::DefaultValues::nSIMPLEX_MAX_ITERATIONS = 60;

/* Message texts: */
const std::string CGaussianService::MessageTexts::sBAD_CAST("Bad type cast! ");
const std::string CGaussianService::MessageTexts::sEMPTY_MOLECULE("Empty molecule! ");
const std::string CGaussianService::MessageTexts::sINVALID_ARGUMENT("Invalid argument! ");
const std::string CGaussianService::MessageTexts::sNOT_IMOLECULE_INTERFACE("Not an IMolecule interface! ");

/* Error Codes: */
const int CGaussianService::ErrorCodes::nINVALID_CONFIGURATION_ARGUMENT = 1;
const int CGaussianService::ErrorCodes::nNORMAL = 0;

/* Parameter Names: */
const std::string CGaussianService::ParameterNames::sSIMPLEX_CONTRACTION_FACTOR("SIMPLEX_CONTRACTION_FACTOR");
const std::string CGaussianService::ParameterNames::sSIMPLEX_EXTENSION_FACTOR("SIMPLEX_EXTENSION_FACTOR");
const std::string CGaussianService::ParameterNames::sSIMPLEX_INITIAL_SOLUTION_GROUPS_NUMBER("SIMPLEX_GAUSSIAN_INITIAL_SOLUTION_GROUP_NUM");
const std::string CGaussianService::ParameterNames::sSIMPLEX_MAX_ITERATIONS("SIMPLEX_MAX_ITERATION");
const std::string CGaussianService::ParameterNames::sSIMPLEX_REFLECTION_FACTOR("SIMPLEX_REFLECTION_FACTOR");


/* Public Methods: */

/**
 * Description: Ctor, using default parameters.
 */
CGaussianService::CGaussianService()
{
	initParameters();
	initialize();
}


/**
 * Description: Ctor, using specified parameters.
 * @param configArguments: (IN) Specify parameters.
 */
CGaussianService::CGaussianService(const CConfigurationArguments& configArguments)
{
	initParameters(configArguments);
	initialize();
}


/**
 * Description: Dtor.
 */
CGaussianService::~CGaussianService()
{
}


/**
 * Description:
 * @param configurationArguments: (IN)
 * @return:
 *	ErrorCodes::nNORMAL:
 *	ErrorCodes::nINVALID_CONFIGURATION_ARGUMENT:
 */
int CGaussianService::configure(const CConfigurationArguments& configurationArguments)
{
	const int nErrorCode = initParameters(configurationArguments);

	// If parameter initialization success:
	if (nErrorCode == ErrorCodes::nNORMAL)
	{
		return ErrorCodes::nNORMAL;
	}
	else
	{
		return ErrorCodes::nINVALID_CONFIGURATION_ARGUMENT;
	}
}


/**
 * Description:
 * @param molecule: (IN)
 * @return: Gaussian volume of the molecule.
 * @exception:
 * 	CEmptyMoleculeException:
 */
double CGaussianService::evaluateGaussianVolume(const IMolecule& molecule) const
{
	// If not empty molecule:
	if (molecule.getAtomsCount() > 0)
	{
		CGaussianVolume gaussianVolume;
		return gaussianVolume.getOverlapVolume(molecule, molecule);
	}
	// If empty molecule:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sEMPTY_MOLECULE
			<< "Function parameter: molecule. ";
		throw CEmptyMoleculeException(msgStream.str());
	}
}


/**
 * Description:
 * @param refMol: (IN)
 * @param fitMol: (IN)
 * @param pFitTransformations: (OUT) Transformation for the fit molecule to get the max overlap. This transformation is represented as a vector [tX, tY, tZ, rX, rY, rZ],
 *	where tX, tY and tZ correspond to the rigid transition along X, Y and Z axis, rX, rY and rZ correspond to the rigid rotation along X, Y and Z axis.
 * @return: Max Gaussian volume overlap.
 * @exception:
 *		CBadCastException:
 *		CEmptyMoleculeException:
 */
double CGaussianService::evaluateMaxGaussianVolumeOverlap(
	const IMolecule& refMol,
	const IMolecule& fitMol,
	std::vector<std::vector<double> >* pFitTransformations
	) const
{
	// If not empty molecule:
	if (refMol.getAtomsCount() > 0 && fitMol.getAtomsCount() > 0)
	{
		/* Construct initial feasible solutions. */
		// Note: For simplex optimization, there should be (number of dimension + 1) initial solutions to start the optimization.
		// initial solution group number
		const int nInitialGroups = getSimplexInitialSolutionGroupsNumber();
		vector<vector<vector<double> > > initialSolutionGroups;
		generateInitialSolutionGroups(nInitialGroups, _nDIMENSIONS + 1, initialSolutionGroups);

		/* Get molecule copies. */
		auto_ptr<IMolecule> refMoleculePtr(dynamic_cast<IMolecule*>(refMol.clone()));
		auto_ptr<IMolecule> fitMoleculePtr(dynamic_cast<IMolecule*>(fitMol.clone()));
		// If type cast success:
		if (refMoleculePtr.get() && fitMoleculePtr.get())
		{
			/* Move molecules to centroid. */
			/* Note: This is the initial point of alignment. */
			refMoleculePtr->moveToCentroid();
			fitMoleculePtr->moveToCentroid();

			/* Construct evaluator. */
			CGaussianVolumeOverlapEvaluator gaussianOverlapEvaluator(*refMoleculePtr, *fitMoleculePtr);
			gaussianOverlapEvaluator.setNegativeOverlapFlag(true);

			/* Construct simplex optimizer. */
			CSimplexOptimizer simplexOptimizer(gaussianOverlapEvaluator, initialSolutionGroups);
			simplexOptimizer.setReflectionFactor(getSimplexReflectionFactor());
			simplexOptimizer.setExtensionFactor(getSimplexExtensionFactor());
			simplexOptimizer.setContractionFactor(getSimplexContractionFactor());

			/* Do optimization. */
			// optimal transformation only for the centered reference and fit molecule
			vector<double> resultPoint;
			double dResultValue = 0.0;
			simplexOptimizer.runOptimization(resultPoint, dResultValue, getSimplexMaxIterations());

			/* Get results. */
			if (pFitTransformations)
			{
				/* Get centroid of molecule. */
				static const int nDIMENSION = 3;
				const vector<double>& refMoleculeCentroid = refMol.getCentroid();
				const vector<double>& fitMoleculeCentroid = fitMol.getCentroid();

				pFitTransformations->clear();

				/* Transformation 1 (translation): */
				pFitTransformations->push_back(vector<double>());
				for (int iDimension = 0; iDimension < nDIMENSION; ++ iDimension)
				{
					pFitTransformations->back().push_back(- fitMoleculeCentroid[iDimension]);
				}

				/* Transformation 2 (Rotation): */
				pFitTransformations->push_back(vector<double>(resultPoint.begin() + nDIMENSION, resultPoint.end()));

				/* Transformation 3 (translation): */
				pFitTransformations->push_back(vector<double>());
				for (int iDimension = 0; iDimension < nDIMENSION; ++ iDimension)
				{
					pFitTransformations->back().push_back(resultPoint[iDimension] + refMoleculeCentroid[iDimension]);
				}
			}

			return std::abs(dResultValue);
		}
		// If type cast failure:
		else
		{
			std::stringstream msgStream;
			msgStream
				<< LOCATION_STREAM_INSERTION
				<< MessageTexts::sBAD_CAST
				<< MessageTexts::sNOT_IMOLECULE_INTERFACE;
			throw CBadCastException(msgStream.str());
		}
	}
	// If empty molecule:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sEMPTY_MOLECULE
			<< "Function parameter: refMol or fitMol. ";
		throw CEmptyMoleculeException(msgStream.str());
	}
}


/**
 * Description:
 */
double CGaussianService::evaluatePocketComboSimilarity(
	const IMolecule& refPocketVolume,
	const IMolecule& refPocket,
	const IMolecule& fitPocketVolume,
	const IMolecule& fitPocket,
	std::vector<std::vector<double> >* pFitTransformations
	) const
{
	/* Construct initial feasible solutions. */
	// Note: For simplex optimization, there should be (number of dimension + 1) initial solutions to start the optimization.
	// initial solution group number
	const int nInitialGroups = getSimplexInitialSolutionGroupsNumber();
	vector<vector<vector<double> > > initialSolutionGroups;
	generateInitialSolutionGroups(nInitialGroups, _nDIMENSIONS + 1, initialSolutionGroups);

	// If not empty molecules:
	if (refPocket.getAtomsCount() > 0
		&& refPocketVolume.getAtomsCount() > 0
		&& fitPocket.getAtomsCount() > 0
		&& fitPocketVolume.getAtomsCount() >0
		)
	{
		/* Get molecule copies. */
		auto_ptr<IMolecule> refPocketVolumeClonePtr(dynamic_cast<IMolecule*>(refPocketVolume.clone()));
		auto_ptr<IMolecule> fitPocketVolumeClonePtr(dynamic_cast<IMolecule*>(fitPocketVolume.clone()));

		// If clone OK:
		if (refPocketVolumeClonePtr.get() && fitPocketVolumeClonePtr.get())
		{
			/* Construct alpha carbon representation of pocket. */
			// alpha carbon representation of reference pocket
			auto_ptr<IMolecule> refPocketAlphaCPtr = CMoleculeManager::getMolecule();
			const list<IAtom*> refPocketAtomsList = refPocket.getAtomsList();
			FOREACH(iterAtom, refPocketAtomsList, list<IAtom*>::const_iterator)
			{
				IAtom& atom = **iterAtom;

				// If alpha C:
				if (!atom.getAtomName().compare("CA"))
				{
					refPocketAlphaCPtr->addAtom(atom);
				}
			}
			// alpha carbon representation of fit pocket
			auto_ptr<IMolecule> fitPocketAlphaCPtr = CMoleculeManager::getMolecule();
			const list<IAtom*> fitPocketAtomsList = fitPocket.getAtomsList();
			FOREACH(iterAtom, fitPocketAtomsList, list<IAtom*>::const_iterator)
			{
				IAtom& atom = **iterAtom;

				// If alpha C:
				if (!atom.getAtomName().compare("CA"))
				{
					fitPocketAlphaCPtr->addAtom(atom);
				}
			}

			/* Move molecules to centroid. */
			/* Note: This is the initial point of alignment. */
			// move along this vector to center the reference pocket volume
			vector<double> refPocketCentroidMove = refPocketVolume.getCentroid();
			CMathematics::opposite(refPocketCentroidMove);
			// move along this vector to center the fit pocket volume
			vector<double> fitPocketCentroidMove = fitPocketVolume.getCentroid();
			CMathematics::opposite(fitPocketCentroidMove);
			refPocketVolumeClonePtr->moveToCentroid();
			fitPocketVolumeClonePtr->moveToCentroid();
			// Reference pocket and corresponding volume should use the same centroid.
			refPocketAlphaCPtr->move(
				refPocketCentroidMove[0],
				refPocketCentroidMove[1],
				refPocketCentroidMove[2]
				);
			// Fit pocket and corresponding volume should use the same centroid.
			fitPocketAlphaCPtr->move(
				fitPocketCentroidMove[0],
				fitPocketCentroidMove[1],
				fitPocketCentroidMove[2]
				);

			/* Construct function evaluator. */
			CPocketComboSimilarityEvaluator functionEvaluator(
				*refPocketVolumeClonePtr,
				*refPocketAlphaCPtr,
				*fitPocketVolumeClonePtr,
				*fitPocketAlphaCPtr
				);

			/* Construct simplex optimizer. */
			CSimplexOptimizer simplexOptimizer(functionEvaluator, initialSolutionGroups);
			simplexOptimizer.setReflectionFactor(getSimplexReflectionFactor());
			simplexOptimizer.setExtensionFactor(getSimplexExtensionFactor());
			simplexOptimizer.setContractionFactor(getSimplexContractionFactor());

			/* Do optimization. */
			// optimal transformation only for the centered reference and fit molecule
			vector<double> resultPoint;
			double dResultValue = 0.0;
			simplexOptimizer.runOptimization(resultPoint, dResultValue, getSimplexMaxIterations());

			/* Debug. */
			//vector<vector<CSimplexOptimizer::CourseNode> > trajectories;
			//simplexOptimizer.traceOptimization(trajectories, 60);

			/* Get results. */
			if (pFitTransformations)
			{
				/* Get centroid of molecule. */
				static const int nDIMENSION = 3;
				const vector<double>& refMoleculeCentroid = refPocketVolume.getCentroid();
				const vector<double>& fitMoleculeCentroid = fitPocketVolume.getCentroid();

				pFitTransformations->clear();

				/* Transformation 1 (translation): */
				pFitTransformations->push_back(vector<double>());
				for (int iDimension = 0; iDimension < nDIMENSION; ++ iDimension)
				{
					pFitTransformations->back().push_back(- fitMoleculeCentroid[iDimension]);
				}

				/* Transformation 2 (Rotation): */
				pFitTransformations->push_back(vector<double>(resultPoint.begin() + nDIMENSION, resultPoint.end()));

				/* Transformation 3 (translation): */
				pFitTransformations->push_back(vector<double>());
				for (int iDimension = 0; iDimension < nDIMENSION; ++ iDimension)
				{
					pFitTransformations->back().push_back(resultPoint[iDimension] + refMoleculeCentroid[iDimension]);
				}
			}

			return dResultValue;
		}
		// If clone fails:
		else
		{
			std::stringstream msgStream;
			msgStream
				<< LOCATION_STREAM_INSERTION
				<< "Bad cast. "
				<< "Variable is not an instance of IMolecule interface. ";
			throw CBadCastException(msgStream.str());
		}
	}
	// If empty molecules:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "Empty molecules passed as arguments. "
			<< "Function arguments. ";
		throw CEmptyMoleculeException(msgStream.str());
	}

	return 0;
}


/**
 * Description:
 * @return:
 */
std::map<std::string, std::string> CGaussianService::getParametersMap() const
{
	/* Construct parameters map. */
	// parameters map
	map<string, string> parametersMap;
	parametersMap[ParameterNames::sSIMPLEX_CONTRACTION_FACTOR] = CUtility::toString(getSimplexContractionFactor());
	parametersMap[ParameterNames::sSIMPLEX_EXTENSION_FACTOR] = CUtility::toString(getSimplexExtensionFactor());
	parametersMap[ParameterNames::sSIMPLEX_INITIAL_SOLUTION_GROUPS_NUMBER] = CUtility::toString(getSimplexInitialSolutionGroupsNumber());
	parametersMap[ParameterNames::sSIMPLEX_MAX_ITERATIONS] = CUtility::toString(getSimplexMaxIterations());
	parametersMap[ParameterNames::sSIMPLEX_REFLECTION_FACTOR] = CUtility::toString(getSimplexReflectionFactor());
	
	return parametersMap;
}


/**
 * Description:
 * @return:
 */
double CGaussianService::getSimplexContractionFactor() const
{
	return _parameterAggregation.dSimplexContractionFactor;
}


/**
 * Description:
 * @return:
 */
double CGaussianService::getSimplexExtensionFactor() const
{
	return _parameterAggregation.dSimplexExtensionFactor;
}


/**
 * Description:
 * @return:
 */
int CGaussianService::getSimplexInitialSolutionGroupsNumber() const
{
	return _parameterAggregation.nSimplexInitialSolutionGroupsNumber;
}


/**
 * Description:
 * @return:
 */
int CGaussianService::getSimplexMaxIterations() const
{
	return _parameterAggregation.nSimplexMaxIterations;
}


/**
 * Description:
 * @return:
 */
double CGaussianService::getSimplexReflectionFactor() const
{
	return _parameterAggregation.dSimplexReflectionFactor;
}


/**
 * Description:
 * @param dContractionFactor: (IN)
 * @exception:
 *	CInvalidArgumentException:
 */
void CGaussianService::setSimplexContractionFactor(double dContractionFactor)
{
	// If valid argument:
	if (dContractionFactor > 0)
	{
		_parameterAggregation.dSimplexContractionFactor = dContractionFactor;
	}
	// If invalid argument:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sINVALID_ARGUMENT
			<< "dContractionFactor = "
			<< dContractionFactor;
		throw CInvalidArgumentException(msgStream.str());
	}
}


/**
 * Description:
 * @param dExtensionFactor: (IN)
 * @exception:
 *	CInvalidArgumentException:
 */
void CGaussianService::setSimplexExtensionFactor(double dExtensionFactor)
{
	// If valid argument:
	if (dExtensionFactor > 0)
	{
		_parameterAggregation.dSimplexExtensionFactor = dExtensionFactor;
	}
	// If invalid argument:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sINVALID_ARGUMENT
			<< "dExtensionFactor = "
			<< dExtensionFactor;
		throw CInvalidArgumentException(msgStream.str());
	}
}


/**
 * Description:
 * @param nGroupsNumber: (IN)
 * @exception:
 *	CInvalidArgumentException:
 */
void CGaussianService::setSimplexInitialSolutionGroupsNumber(int nGroupsNumber)
{
	// If valid argument:
	if (nGroupsNumber > 0)
	{
		_parameterAggregation.nSimplexInitialSolutionGroupsNumber = nGroupsNumber;
	}
	// If invalid argument:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sINVALID_ARGUMENT
			<< "nGroupsNumber = "
			<< nGroupsNumber;
		throw CInvalidArgumentException(msgStream.str());
	}
}


/**
 * Description:
 * @param nMaxIteration: (IN)
 * @exception:
 *	CInvalidArgumentException:
 */
void CGaussianService::setSimplexMaxIterations(int nMaxIterations)
{
	// If valid argument:
	if (nMaxIterations > 0)
	{
		_parameterAggregation.nSimplexMaxIterations = nMaxIterations;
	}
	// If invalid argument:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sINVALID_ARGUMENT
			<< "nMaxIterations = "
			<< nMaxIterations;
		throw CInvalidArgumentException(msgStream.str());
	}
}


/**
 * Description:
 * @param dReflectionFactor: (IN)
 * @exception:
 *	CInvalidArgumentException:
 */
void CGaussianService::setSimplexReflectionFactor(double dReflectionFactor)
{
	// If valid argument:
	if (dReflectionFactor > 0)
	{
		_parameterAggregation.dSimplexReflectionFactor = dReflectionFactor;
	}
	// If invalid argument:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sINVALID_ARGUMENT
			<< "dReflectionFactor = "
			<< dReflectionFactor;
		throw CInvalidArgumentException(msgStream.str());
	}
}


/* Private Methods: */

/**
 * Description:
 * @param initialSolutions: (OUT)
 * @return:
 */
int CGaussianService::generateInitialSolutionGroups(int nGroups, int nSolutionsPerGroup, std::vector<std::vector<std::vector<double> > >& initialSolutionGroups) const
{
	// number of groups
	const int nGROUPS = nGroups;
	// number of solutions per group
	const int nSOLUTIONS_PER_GROUP = nSolutionsPerGroup;
	// total solutions generated
	int nTotalSolutions = 0;
	
	// For each group:
	for (int iGroup = 0; iGroup < nGROUPS; ++iGroup)
	{
		// current group
		vector<vector<double> > currentGroup;

		// For each solution:
		for (int iSolution = 0; iSolution < nSOLUTIONS_PER_GROUP; ++iSolution)
		{
			// current solution
			vector<double> currentSolution;

			// For each dimension:
			for (int iDimension = 0; iDimension < _nDIMENSIONS; ++iDimension)
			{
				double dRandom = 0.0;
				// translation
				if (iDimension < 3)
				{
					dRandom = 2 * (rand() / static_cast<double>(RAND_MAX)) - 1;
					dRandom *= 4.0;
				}
				// rotation
				else
				{
					dRandom = 2 * (rand() / static_cast<double>(RAND_MAX)) - 1;
					dRandom *= 3.1415926;
				}
				currentSolution.push_back(dRandom);
				++nTotalSolutions;
			}
			currentGroup.push_back(currentSolution);
		}
		initialSolutionGroups.push_back(currentGroup);
	}

	return nTotalSolutions;
}


/**
 * Description: Common initialization.
 */
int CGaussianService::initialize()
{
	// Initialize random seed.
	srand(static_cast<unsigned int>(time(NULL)));

	/* Debug: Using a constant seed. */
	//srand(2);

	return ErrorCodes::nNORMAL;
}


/**
 * Description: Initialize all parameters to default value.
 */
int CGaussianService::initParameters()
{
	setSimplexContractionFactor(DefaultValues::dSIMPLEX_CONTRACTION_FACTOR);
	setSimplexExtensionFactor(DefaultValues::dSIMPLEX_EXTENSION_FACTOR);
	setSimplexInitialSolutionGroupsNumber(DefaultValues::nSIMPLEX_INITIAL_SOLUTION_GROUPS_NUMBER);
	setSimplexMaxIterations(DefaultValues::nSIMPLEX_MAX_ITERATIONS);
	setSimplexReflectionFactor(DefaultValues::dSIMPLEX_REFLECTION_FACTOR);

	return ErrorCodes::nNORMAL;
}


/**
 * Description: Initialize all parameters according to configuration. Parameters which does not exist in configuration file will be initialize to default value.
 * @param configArguments: (IN) Configuration arguments from configuration file.
 * @return:
 *	ErrorCodes::nNORMAL:
 *	ErrorCodes::nINVALID_CONFIGURATION_ARGUMENT:
 */
int CGaussianService::initParameters(const CConfigurationArguments& configArguments)
{
	// error code to be returned
	int nErrorCode = ErrorCodes::nNORMAL;

	initParameters();

	/* Set parameters to configured value if possible. */
	try
	{
		if (configArguments.existArgument(ParameterNames::sSIMPLEX_CONTRACTION_FACTOR))
		{
			double dContractionFactor = 0;
			const int nArgumentErrorCode = configArguments.getArgumentValue(ParameterNames::sSIMPLEX_CONTRACTION_FACTOR, dContractionFactor);

			// If argument conversion succeeds:
			if (nArgumentErrorCode == CConfigurationArguments::ErrorCodes::nNORMAL)
			{
				setSimplexContractionFactor(dContractionFactor);
			}
			// If argument conversion fails:
			else
			{
				nErrorCode = ErrorCodes::nINVALID_CONFIGURATION_ARGUMENT;
			}
		}

		if (configArguments.existArgument(ParameterNames::sSIMPLEX_EXTENSION_FACTOR))
		{
			double dExtensionFactor = 0;
			const int nArgumentErrorCode = configArguments.getArgumentValue(ParameterNames::sSIMPLEX_EXTENSION_FACTOR, dExtensionFactor);

			// If argument conversion succeeds:
			if (nArgumentErrorCode == CConfigurationArguments::ErrorCodes::nNORMAL)
			{
				setSimplexExtensionFactor(dExtensionFactor);
			}
			// If argument conversion fails:
			else
			{
				nErrorCode = ErrorCodes::nINVALID_CONFIGURATION_ARGUMENT;
			}
		}

		if (configArguments.existArgument(ParameterNames::sSIMPLEX_REFLECTION_FACTOR))
		{
			double dReflectionFactor = 0;
			const int nArgumentErrorCode = configArguments.getArgumentValue(ParameterNames::sSIMPLEX_REFLECTION_FACTOR, dReflectionFactor);

			// If argument conversion succeeds:
			if (nArgumentErrorCode == CConfigurationArguments::ErrorCodes::nNORMAL)
			{
				setSimplexReflectionFactor(dReflectionFactor);
			}
			// If argument conversion fails:
			else
			{
				nErrorCode = ErrorCodes::nINVALID_CONFIGURATION_ARGUMENT;
			}
		}

		if (configArguments.existArgument(ParameterNames::sSIMPLEX_INITIAL_SOLUTION_GROUPS_NUMBER))
		{
			int nGroupNumber = 0;
			const int nArgumentErrorCode = configArguments.getArgumentValue(ParameterNames::sSIMPLEX_INITIAL_SOLUTION_GROUPS_NUMBER, nGroupNumber);

			// If argument conversion succeeds:
			if (nArgumentErrorCode == CConfigurationArguments::ErrorCodes::nNORMAL)
			{
				setSimplexInitialSolutionGroupsNumber(nGroupNumber);
			}
			// If argument conversion fails:
			else
			{
				nErrorCode = ErrorCodes::nINVALID_CONFIGURATION_ARGUMENT;
			}
		}

		if (configArguments.existArgument(ParameterNames::sSIMPLEX_MAX_ITERATIONS))
		{
			int nMaxIterations = 0;
			const int nArgumentErrorCode = configArguments.getArgumentValue(ParameterNames::sSIMPLEX_MAX_ITERATIONS, nMaxIterations);

			// If argument conversion succeeds:
			if (nArgumentErrorCode == CConfigurationArguments::ErrorCodes::nNORMAL)
			{
				setSimplexMaxIterations(nMaxIterations);
			}
			// If argument conversion fails:
			else
			{
				nErrorCode = ErrorCodes::nINVALID_CONFIGURATION_ARGUMENT;
			}
		}
	}
	// If parameter conversion succeeds but the corresponding value is invalid:
	catch(CInvalidArgumentException& exception)
	{
		nErrorCode = ErrorCodes::nINVALID_CONFIGURATION_ARGUMENT;
	}

	return nErrorCode;
}
