/**
 * Debug Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file Debug.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-09-26
 */


#ifndef DEBUG_INCLUDE_H
#define DEBUG_INCLUDE_H
//


#include "InterfaceFunctionValueEvaluator.h"


/**
 * Description: An simple fitness evaluator just for test.
 */
class CTestFitnessEvaluator : public IFunctionValueEvaluator
{
public:
	virtual double getFunctionValue(const std::vector<double>& params);
};


/**
 * Description:
 */
class CTestFunctionValueEvaluator : public IFunctionValueEvaluator
{
public:
	virtual double getFunctionValue(const std::vector<double>& params);
};

int alignMolecule();

int debug();

int stabilityTest();

//


//
#endif
