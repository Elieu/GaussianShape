/**
 * Simplex Optimizer Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file SimplexOptimizer.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-10-18
 */


#ifndef SIMPLEX_OPTIMIZER_INCLUDE_H
#define SIMPLEX_OPTIMIZER_INCLUDE_H
//


#include "InterfaceFunctionValueEvaluator.h"

#include <vector>


/**
 * Description:
 */
class CSimplexOptimizer
{
	/* data: */
public:
	/**
	 * Description: Struct for recording the ionformation during the optimization process. One CourseNode corresponds to one iteration.
	 */
	struct CourseNode
	{
		/**
		 * Description:
		 */
		enum OperationType
		{
			OperationType_Default,
			OperationType_Extension,
			OperationType_Replacement,
			OperationType_Contraction
		};

		/* data: */
	private:
		// the lowest point
		std::vector<double> _lowestPoint;
		// function value of the lowest value
		double _dLowestValue;
		// simplex operation type
		OperationType _operationType;

		/* method: */
	public:
		CourseNode();

		const std::vector<double>& getLowestPoint() const;
		double getLowestValue() const;
		CSimplexOptimizer::CourseNode::OperationType getOperationType() const;

		void setLowestPoint(const std::vector<double>& point);
		void setLowestValue(double dValue);
		void setOperationType(CSimplexOptimizer::CourseNode::OperationType type);
	};


	/* Error code: */
	struct ErrorCodes
	{
		static const int nNORMAL;

	private:
		ErrorCodes() {};
	};


private:
	// groups of initial feasible soulutions
	const std::vector<std::vector<std::vector<double> > > _INITIAL_FEASIBLE_SOLUTIONS;
	// current group of feasible solutions (n+1 points of current simplex)
	std::vector<std::vector<double> > _currentFeasibleSolutions;
	// corresponding function values for current feasible solutions
	std::vector<double> _currentFunctionValues;
	// coefficient for contraction operation
	double _dContractionFactor;
	// coefficient for extension operation
	double _dExtensionFactor;
	// function value of the highest solution
	double _dHighestSolutionValue;
	// function value of the lowest solution
	double _dLowestSolutionValue;
	// coefficient for reduction operation
	double _dReductionFactor;
	// function value at the reflected point
	double _dReflectedPointValue;
	// coefficient for reflection operation
	double _dReflectionFactor;
	// function value of the second highest solution
	double _dSecondHighestSolutionValue;
	// reference of an external function value evaluator
	IFunctionValueEvaluator& _functionValueEvaluator;
	// index for the hightest solution in current group
	int _nHighestSolutionId;
	// index for the lowest solution in current group
	int _nLowestSolutionId;
	// dimension of the optimization problem
	const int _nPROBLEM_DIMENSION;
	// index for the second highest solution in current group
	int _nSecondHighestSolutionId;
	// centroid point used for reflection
	std::vector<double> _reflectionCentroid;
	// reflected point
	std::vector<double> _reflectedPoint;

	/* Default value: */
	struct DefaultValues
	{
		static const double dCONTRACTION_FACTOR;
		static const double dEXTENSION_FACTOR;
		static const double dREDUCTION_FACTOR;
		static const double dREFLECTION_FACTOR;

	private:
		DefaultValues();
	};

	/* Message text: */
	struct MessageTexts
	{
		static const std::string sDIMENSION_NOT_MATCH;
		static const std::string sPARAMETER_OUT_OF_RANGE;

	private:
		MessageTexts();
	};

	/* method: */
public: 
	CSimplexOptimizer(IFunctionValueEvaluator& functionValueEvaluator, const std::vector<std::vector<std::vector<double> > >& initialFeasibleSolutions);
	~CSimplexOptimizer();

	int runOptimization(std::vector<double>& minSolution, double& dMinValue, int nMaxIterations);
	int traceOptimization(std::vector<std::vector<CSimplexOptimizer::CourseNode> >& trajectories, int nMaxIterations);

	double getContractionFactor() const;
	double getExtensionFactor() const;
	double getReductionFactor() const;
	double getReflectionFactor() const;

	void setContractionFactor(double dFactor);
	void setExtensionFactor(double dFactor);
	void setReductionFactor(double dFactor);
	void setReflectionFactor(double dFactor);
private:
	int doContraction();
	int doExtention();
	int doReduction();
	int doReflection();
	int evaluateAllCurrentSolutions();
	int updateSpecialVertices();
	int updateReflectionCentroid();
};


//
#endif
