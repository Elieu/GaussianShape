/**
 * Function Value Evaluator Interface Definition
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file InterfaceFunctionValueEvaluator.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-10-21
 */


#ifndef INTERFACE_FUNCTION_VALUE_EVALUATOR_INCLUDE_H
#define INTERFACE_FUNCTION_VALUE_EVALUATOR_INCLUDE_H
//


#include <vector>


/**
 * Description: Evaluate the function value at a given point.
 */
class IFunctionValueEvaluator
{
public:
	/**
	 * Description: Get the function value at a specified point.
	 * @param params: Coordinate of the point at which to evaluate the function value.
	 * @return: Function value at specified point.
	 */
	virtual double getFunctionValue(const std::vector<double>& params) = 0;

	virtual ~IFunctionValueEvaluator(){};
};


//
#endif
