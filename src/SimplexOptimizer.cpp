/**
 * Simplex Optimizer Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file SimplexOptimizer.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-10-18
 */


#include "Mathematics.h"
#include "SimplexOptimizer.h"
#include "Utility.h"

#include <climits>
#include <limits>


using std::numeric_limits;
using std::vector;


/* Implementation for CSimplexOptimizer class: */

/* Static members: */
const int CSimplexOptimizer::ErrorCodes::nNORMAL = 0;

const double CSimplexOptimizer::DefaultValues::dCONTRACTION_FACTOR = 0.5;
const double CSimplexOptimizer::DefaultValues::dEXTENSION_FACTOR = 2.0;
const double CSimplexOptimizer::DefaultValues::dREDUCTION_FACTOR = 0.5;
const double CSimplexOptimizer::DefaultValues::dREFLECTION_FACTOR = 1.0;

const string CSimplexOptimizer::MessageTexts::sDIMENSION_NOT_MATCH = string("Dimension not match. ");
const string CSimplexOptimizer::MessageTexts::sPARAMETER_OUT_OF_RANGE = string("Parameter out of range. ");


/**
 * Description: Ctor.
 */
CSimplexOptimizer::CSimplexOptimizer(IFunctionValueEvaluator& functionValueEvaluator, const std::vector<std::vector< std::vector<double> > >& initialFeasibleSolutions)
	:
	_INITIAL_FEASIBLE_SOLUTIONS(initialFeasibleSolutions),
	_dHighestSolutionValue(-numeric_limits<double>::max()),
	_dLowestSolutionValue(numeric_limits<double>::max()),
	_dSecondHighestSolutionValue(-numeric_limits<double>::max()),
	_functionValueEvaluator(functionValueEvaluator),
	_nHighestSolutionId(-1),
	_nLowestSolutionId(-1),
	_nPROBLEM_DIMENSION(initialFeasibleSolutions[0].size() - 1),
	_nSecondHighestSolutionId(-1)
{
	/* Validate dimensions of the initial feasible solutions. */
	const int nPROBLEM_DIMENSION = initialFeasibleSolutions[0].size() - 1;
	// For each group of solutions:
	FOREACH(iterSolutionsGroup, initialFeasibleSolutions, vector<vector<vector<double> > >::const_iterator)
	{
		const vector<vector<double> >& SOLUTIONS_GROUP = *iterSolutionsGroup;
		
		if (static_cast<int>(SOLUTIONS_GROUP.size()) != nPROBLEM_DIMENSION + 1)
		{
			std::stringstream msgStream;
			msgStream
				<< LOCATION_STREAM_INSERTION
				<< "Invalid parameter: "
				<< "dimension not match! ";
			throw CInvalidArgumentException(msgStream.str());
		}

		// For each solution:
		FOREACH(iterSolution, SOLUTIONS_GROUP, vector<vector<double> >::const_iterator)
		{
			if (static_cast<int>(iterSolution->size()) != nPROBLEM_DIMENSION)
			{
				std::stringstream msgStream;
				msgStream
					<< LOCATION_STREAM_INSERTION
					<< "Invalid parameter: "
					<< "dimension not match! ";
				throw CInvalidArgumentException(msgStream.str());
			}
		}
	}

	/* Initialization. */
	_reflectedPoint.assign(_nPROBLEM_DIMENSION, 0.0);
	_reflectionCentroid.assign(_nPROBLEM_DIMENSION, 0.0);
	_currentFunctionValues.assign(_nPROBLEM_DIMENSION + 1, 0.0);

	setContractionFactor(DefaultValues::dCONTRACTION_FACTOR);
	setExtensionFactor(DefaultValues::dEXTENSION_FACTOR);
	setReductionFactor(DefaultValues::dREDUCTION_FACTOR);
	setReflectionFactor(DefaultValues::dREFLECTION_FACTOR);

	//_dContractionFactor = DefaultValues::dCONTRACTION_FACTOR;
	//_dExtensionFactor = DefaultValues::dEXTENSION_FACTOR;
	//_dReductionFactor = DefaultValues::dREDUCTION_FACTOR;
	//_dReflectionFactor = DefaultValues::dREFLECTION_FACTOR;

	//_nHighestSolutionId = -1;
	//_nLowestSolutionId = -1;
	//_nSecondHighestSolutionId = -1;

	//_dHighestSolutionValue = -numeric_limits<double>::max();
	//_dLowestSolutionValue = numeric_limits<double>::max();
	//_dSecondHighestSolutionValue = -numeric_limits<double>::max();
}


/**
 * Description: Dtor.
 */
CSimplexOptimizer::~CSimplexOptimizer()
{
}


/**
 * Description:
 */
int CSimplexOptimizer::runOptimization(std::vector<double>& minSolution, double& dMinValue, int nMaxIterations)
{
	// best solution been found so far
	vector<double> bestSolution;
	// function value corresponding to the best solution
	double dBestValue = numeric_limits<double>::max();
	// For each group of feasible solutions:
	FOREACH(iterSolutionsGroup, _INITIAL_FEASIBLE_SOLUTIONS, vector<vector<vector<double> > >::const_iterator)
	{
		_currentFeasibleSolutions = *iterSolutionsGroup;

		evaluateAllCurrentSolutions();
		// Main iteration:
		for (int iIteration = 0; iIteration < nMaxIterations; iIteration++)
		{
			/* Calculate three special vertices and one centroid. */
			updateSpecialVertices();
			updateReflectionCentroid();

			/* Reflection operation. */
			doReflection();

			// Reflection result 1:
			if (_dReflectedPointValue < _dLowestSolutionValue)
			{
				/* Extension operation. */
				doExtention();
			}
			// Reflection result 2:
			else if (_dReflectedPointValue >= _dLowestSolutionValue && _dReflectedPointValue <= _dSecondHighestSolutionValue)
			{
				/* Replace the highest point with the reflected point. */
				_currentFeasibleSolutions[_nHighestSolutionId] = _reflectedPoint;
				_currentFunctionValues[_nHighestSolutionId] = _dReflectedPointValue;
			}
			// Reflection result 3:
			else
			{
				/* Contraction operation. */
				doContraction();
			}
		}

		/* Record the best solution and value. */
		if (_currentFunctionValues[_nLowestSolutionId] < dBestValue)
		{
			bestSolution = _currentFeasibleSolutions[_nLowestSolutionId];
			dBestValue = _currentFunctionValues[_nLowestSolutionId];
		}
	} // FOREACH

	/* Return result. */
	minSolution = bestSolution;
	dMinValue = dBestValue;

	return ErrorCodes::nNORMAL;
}


/**
 * Description:
 * @param trajectories:
 * @param nMaxIterations:
 */
int CSimplexOptimizer::traceOptimization(std::vector<std::vector<CSimplexOptimizer::CourseNode> >& trajectories, int nMaxIterations)
{
	// clear output
	trajectories.clear();

	// For each group of feasible solutions:
	FOREACH(iterSolutionsGroup, _INITIAL_FEASIBLE_SOLUTIONS, vector<vector<vector<double> > >::const_iterator)
	{
		_currentFeasibleSolutions = *iterSolutionsGroup;

		// trajectory for current group of initial solutions
		vector<CourseNode> currentTrajectory;

		evaluateAllCurrentSolutions();
		// Main iteration:
		for (int iIteration = 0; iIteration < nMaxIterations; iIteration++)
		{
			// course node for recording information of this iteration
			CourseNode currentCourseNode;

			/* Calculate three special vertices and one centroid. */
			updateSpecialVertices();
			updateReflectionCentroid();

			/* Reflection operation. */
			doReflection();

			// Reflection result 1:
			if (_dReflectedPointValue < _dLowestSolutionValue)
			{
				/* Extension operation. */
				doExtention();

				// Record.
				currentCourseNode.setOperationType(CourseNode::OperationType_Extension);
			}
			// Reflection result 2:
			else if (_dReflectedPointValue >= _dLowestSolutionValue && _dReflectedPointValue <= _dSecondHighestSolutionValue)
			{
				/* Replace the highest point with the reflected point. */
				_currentFeasibleSolutions[_nHighestSolutionId] = _reflectedPoint;
				_currentFunctionValues[_nHighestSolutionId] = _dReflectedPointValue;

				// Record.
				currentCourseNode.setOperationType(CourseNode::OperationType_Replacement);
			}
			// Reflection result 3:
			else
			{
				/* Contraction operation. */
				doContraction();

				// Record.
				currentCourseNode.setOperationType(CourseNode::OperationType_Contraction);
			}

			/* Record the information of current iteration. */
			currentCourseNode.setLowestPoint(_currentFeasibleSolutions[_nLowestSolutionId]);
			currentCourseNode.setLowestValue(_currentFunctionValues[_nLowestSolutionId]);

			currentTrajectory.push_back(currentCourseNode);
		}

		/* Record the information of current trajectory. */
		trajectories.push_back(currentTrajectory);
	} // FOREACH

	return ErrorCodes::nNORMAL;
}


/**
 * Description:
 */
int CSimplexOptimizer::doContraction()
{
	/* Record the lower point between the highest point and the reflected point. */
	vector<double> lowerPoint;
	double dLowerPointValue;
	if (_currentFunctionValues[_nHighestSolutionId] < _dReflectedPointValue)
	{
		lowerPoint = _currentFeasibleSolutions[_nHighestSolutionId];
		dLowerPointValue = _currentFunctionValues[_nHighestSolutionId];
	}
	else
	{
		lowerPoint = _reflectedPoint;
		dLowerPointValue = _dReflectedPointValue;
	}

	/* Calculate the contracted point. */
	// contracted point
	vector<double> contractedPoint = lowerPoint;
	CMathematics::substract(contractedPoint, _reflectionCentroid);
	CMathematics::multiply(contractedPoint, _dContractionFactor);
	CMathematics::add(contractedPoint, _reflectionCentroid);
	// function value at the contracted point
	const double dContractedPointValue = _functionValueEvaluator.getFunctionValue(contractedPoint);

	/* Assess the contraction operation. */
	// If contraction success:
	if (dContractedPointValue <= dLowerPointValue)
	{
		/* Replace the highest point with the contracted point. */
		_currentFeasibleSolutions[_nHighestSolutionId] = contractedPoint;
		_currentFunctionValues[_nHighestSolutionId] = dContractedPointValue;
	}
	// If contraction failure:
	else
	{
		// Do reduction operation.
		doReduction();
		// All solutions have been changed, so re-evaluate them.
		evaluateAllCurrentSolutions();
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description:
 */
int CSimplexOptimizer::doExtention()
{
	/* Calculate extended point. */
	// extended point
	vector<double> extendedPoint = _reflectedPoint;
	CMathematics::substract(extendedPoint, _reflectionCentroid);
	CMathematics::multiply(extendedPoint, _dExtensionFactor);
	CMathematics::add(extendedPoint, _reflectionCentroid);
	// function value at the extended point
	const double dExtendedPointValue = _functionValueEvaluator.getFunctionValue(extendedPoint);

	/* Assess the extension operation. */
	// If extension success:
	if (dExtendedPointValue < _dReflectedPointValue)
	{
		/* Replace the highest point with the extended point. */
		_currentFeasibleSolutions[_nHighestSolutionId] = extendedPoint;
		_currentFunctionValues[_nHighestSolutionId] = dExtendedPointValue;
	}
	// If extension failure:
	else
	{
		/* Replace the highest point with the reflected point. */
		_currentFeasibleSolutions[_nHighestSolutionId] = _reflectedPoint;
		_currentFunctionValues[_nHighestSolutionId] = _dReflectedPointValue;
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description:
 */
int CSimplexOptimizer::doReduction()
{
	const vector<double> lowestPoint = _currentFeasibleSolutions[_nLowestSolutionId];

	// Apply reduction to every current feasible solution.
	FOREACH(iterSolution, _currentFeasibleSolutions, vector<vector<double> >::iterator)
	{
		// original point (solution)
		vector<double>& oldPoint = *iterSolution;
		// corresponding new point after reduction
		vector<double> newPoint = lowestPoint;
		CMathematics::substract(newPoint, oldPoint);
		CMathematics::multiply(newPoint, _dReductionFactor);
		CMathematics::add(newPoint, oldPoint);

		oldPoint = newPoint;
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description:
 */
int CSimplexOptimizer::doReflection()
{
	/* Calculate the reflected result point. */
	// reflected point
	_reflectedPoint = _reflectionCentroid;
	CMathematics::substract(_reflectedPoint, _currentFeasibleSolutions[_nHighestSolutionId]);
	CMathematics::multiply(_reflectedPoint, _dReductionFactor);
	CMathematics::add(_reflectedPoint, _reflectionCentroid);
	// function value at reflected point
	_dReflectedPointValue = _functionValueEvaluator.getFunctionValue(_reflectedPoint);

	return ErrorCodes::nNORMAL;
}


/**
 * Description:
 */
int CSimplexOptimizer::evaluateAllCurrentSolutions()
{
	// Evaluate all current feasible solutions.
	for (int i = 0; i < static_cast<int>(_currentFeasibleSolutions.size()); i++)
	{
		_currentFunctionValues[i] = _functionValueEvaluator.getFunctionValue(_currentFeasibleSolutions[i]);
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description: Calculate the highest, second highest and lowest verticies of current simplex.
 * @Note: Before calling this method, all current feasible solutions must be evaluated.
 */
int CSimplexOptimizer::updateSpecialVertices()
{
	/* Reset records. */
	_nHighestSolutionId = -1;
	_nLowestSolutionId = -1;
	_nSecondHighestSolutionId = -1;

	_dHighestSolutionValue = -numeric_limits<double>::max();
	_dLowestSolutionValue = numeric_limits<double>::max();
	_dSecondHighestSolutionValue = -numeric_limits<double>::max();

	/* Get some special solutions. */
	for (int i = 0; i < static_cast<int>(_currentFunctionValues.size()); i++)
	{
		/* Get the highest and second highest solution. */
		if (_currentFunctionValues[i] > _dHighestSolutionValue)
		{
			_dSecondHighestSolutionValue = _dHighestSolutionValue;
			_nSecondHighestSolutionId = _nHighestSolutionId;

			_dHighestSolutionValue = _currentFunctionValues[i];
			_nHighestSolutionId = i;
		}
		else if (_currentFunctionValues[i] > _dSecondHighestSolutionValue)
		{
			_dSecondHighestSolutionValue = _currentFunctionValues[i];
			_nSecondHighestSolutionId = i;
		}

		/* Get the lowest solution. */
		if (_currentFunctionValues[i] < _dLowestSolutionValue)
		{
			_dLowestSolutionValue = _currentFunctionValues[i];
			_nLowestSolutionId = i;
		}
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description: Calculate the reflection centroid point.
 * @Note: Before calling this method, the highest, second highest and lowest verticies must be calculated.
 */
int CSimplexOptimizer::updateReflectionCentroid()
{
	/* Calculate centroid point. */
	_reflectionCentroid.assign(_nPROBLEM_DIMENSION, 0.0);
	FOREACH(iterSolution, _currentFeasibleSolutions, std::vector<std::vector<double> >::const_iterator)
	{
		CMathematics::add(_reflectionCentroid, *iterSolution);
	}
	CMathematics::substract(_reflectionCentroid, _currentFeasibleSolutions[_nHighestSolutionId]);
	CMathematics::multiply(_reflectionCentroid, 1 / static_cast<double>(_nPROBLEM_DIMENSION));

	return ErrorCodes::nNORMAL;
}


/**
 * Description:
 */
double CSimplexOptimizer::getContractionFactor() const
{
	return _dContractionFactor;
}


/**
 * Description:
 */
double CSimplexOptimizer::getExtensionFactor() const
{
	return _dExtensionFactor;
}


/**
 * Description:
 */
double CSimplexOptimizer::getReductionFactor() const
{
	return _dReductionFactor;
}


/**
 * Description:
 */
double CSimplexOptimizer::getReflectionFactor() const
{
	return _dReflectionFactor;
}


/**
 * Description:
 */
void CSimplexOptimizer::setContractionFactor(double dFactor)
{
	// Check parameter:
	if (dFactor > 0 && dFactor < 1)
	{
		_dContractionFactor = dFactor;
	}
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "Invalid parameter: "
			<< "dFactor = " << dFactor;
		throw CInvalidArgumentException(msgStream.str());
	}
}


/**
 * Description:
 */
void CSimplexOptimizer::setExtensionFactor(double dFactor)
{
	// Check parameter:
	if (dFactor > 1.0)
	{
		_dExtensionFactor = dFactor;
	}
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "Invalid parameter: "
			<< "dFactor = " << dFactor;
		throw CInvalidArgumentException(msgStream.str());
	}
}


/**
 * Description:
 */
void CSimplexOptimizer::setReductionFactor(double dFactor)
{
	// Check parameter:
	if (dFactor > 0 && dFactor < 1)
	{
		_dReductionFactor = dFactor;
	}
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "Invalid parameter: "
			<< "dFactor = " << dFactor;
		throw CInvalidArgumentException(msgStream.str());
	}
}


/**
 * Description:
 */
void CSimplexOptimizer::setReflectionFactor(double dFactor)
{
	_dReflectionFactor = dFactor;
}


/* Implementation for CSimplexOptimizer::CourseNode struct: */

/**
 * Description: Ctor.
 */
CSimplexOptimizer::CourseNode::CourseNode() :
	_dLowestValue(0.0),
	_operationType(OperationType_Default)
{
}


/**
 * Description:
 */
const std::vector<double>& CSimplexOptimizer::CourseNode::getLowestPoint() const
{
	return _lowestPoint;
}


/**
 * Description:
 */
double CSimplexOptimizer::CourseNode::getLowestValue() const
{
	return _dLowestValue;
}


/**
 * Description:
 */
CSimplexOptimizer::CourseNode::OperationType CSimplexOptimizer::CourseNode::getOperationType() const
{
	return _operationType;
}


/**
 * Description:
 */
void CSimplexOptimizer::CourseNode::setLowestPoint(const std::vector<double>& point)
{
	_lowestPoint = point;
}


/**
 * Description:
 */
void CSimplexOptimizer::CourseNode::setLowestValue(double dValue)
{
	_dLowestValue = dValue;
}


/**
 * Description:
 */
void CSimplexOptimizer::CourseNode::setOperationType(CSimplexOptimizer::CourseNode::OperationType type)
{
	_operationType = type;
}
