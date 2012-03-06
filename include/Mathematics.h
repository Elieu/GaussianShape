/**
 * Mathematics Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file Mathematics.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-09-27
 */


#ifndef MATHEMATICS_INCLUDE_H
#define MATHEMATICS_INCLUDE_H
//


#include "Exception.h"

#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>


using std::string;


/**
 * Description:
 */
class CMathematics
{
	/* data: */
public:
private:
	/* Error codes: */
	struct ErrorCodes
	{
		static const int nNORMAL;

	private:
		ErrorCodes() {};
	};


	/* Message texts: */
	struct MessageTexts
	{
		static const std::string sDIMENSION_NOT_MATCH;
		static const std::string sNOT_IN_3D_SPACE;

	private:
		MessageTexts() {};
	};


	// PI value
	static const double _dPI;


	/* method: */
public:
	static int factorial(int n);
	static double getPiValue();
	static double pointToLineSquareDistance(const std::vector<double>& point, const std::vector<double>& lineVector);
	static double pointToPointSquareDistance(const std::vector<double>& position1, const std::vector<double>& position2);
	static int rectangularToSphericalCoordinate(const std::vector<double>& srcVector, std::vector<double>& dstVector);
	static int sphericalToRectanglularCoordinate(const std::vector<double>& srcVector, std::vector<double>& dstVector);

	template <typename T>
	static std::vector<T>& add(std::vector<T>& dstVector, const std::vector<T>& srcVector);

	template <typename T, typename G>
	static std::vector<T>& multiply(std::vector<T>& dstVector, const G srcFactor);

	template <typename T>
	static std::vector<T>& opposite(std::vector<T>& dstVector);

	template <typename T>
	static std::vector<T>& substract(std::vector<double>& dstVector, const std::vector<T>& srcVector);

private:

};


/* Template implementation for CMathematics class: */

/**
 * Description:
 * @param dstVector:
 * @param srcVector:
 * @return:
 * @exception:
 *	CInvalidArgumentException:
 */
template <typename T>
std::vector<T>& CMathematics::add(std::vector<T>& dstVector, const std::vector<T>& srcVector)
{
	// Dimension must match:
	if (dstVector.size() == srcVector.size())
	{
		// Do addition for each dimension:
		for (unsigned int i = 0; i < dstVector.size(); ++i)
		{
			dstVector[i] += srcVector[i];
		}
	}
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sDIMENSION_NOT_MATCH;
		throw CInvalidArgumentException(msgStream.str());
	}

	return dstVector;
}


/**
 * Description:
 * @param dstVector:
 * @param srcVector:
 * @return:
 * @exception:
 *	CInvalidArgumentException:
 */
template <typename T>
std::vector<T>& CMathematics::substract(std::vector<double>& dstVector, const std::vector<T>& srcVector)
{
	// Dimension must match:
	if (dstVector.size() == srcVector.size())
	{
		// Do subtraction for each dimension:
		for (unsigned int i = 0; i < dstVector.size(); ++i)
		{
			dstVector[i] -= srcVector[i];
		}
	}
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sDIMENSION_NOT_MATCH;
		throw CInvalidArgumentException(msgStream.str());
	}

	return dstVector;
}


/**
 * Description:
 * @param dstVector:
 * @param srcFactor:
 * @return:
 */
template <typename T, typename G>
std::vector<T>& CMathematics::multiply(std::vector<T>& dstVector, const G srcFactor)
{
	// Do multiplication for each dimension:
	for (unsigned int i = 0; i < dstVector.size(); ++i)
	{
		dstVector[i] *= srcFactor;
	}

	return dstVector;
}


/**
 * Description:
 * @param dstVector: (IN, OUT)
 * @return:
 */
template <typename T>
std::vector<T>& CMathematics::opposite(std::vector<T>& dstVector)
{
	// Do opposite for each dimension:
	for (unsigned int i = 0; i < dstVector.size(); ++ i)
	{
		dstVector[i] *= -1;
	}

	return dstVector;
}


//
#endif
