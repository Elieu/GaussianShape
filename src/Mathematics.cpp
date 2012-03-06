/**
 * Mathematics Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file Mathematics.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-09-27
 */


#include "Mathematics.h"

#include <math.h>


using std::string;


/* Static Members: */

/* Error codes: */
const int CMathematics::ErrorCodes::nNORMAL = 0;

const double CMathematics::_dPI = 3.14159265358;

/* Message Texts: */
const std::string CMathematics::MessageTexts::sDIMENSION_NOT_MATCH("Dimension not match! ");
const std::string CMathematics::MessageTexts::sNOT_IN_3D_SPACE("Not in 3D space! ");

/* Public Methods: */

/**
 * Description: Calculate factorical of n.
 * @param n: Variable, non-negative integer.
 * @return: Factorical of n (return 1 if n is negative).
 */
int CMathematics::factorial(int n)
{
	// Factorial look up table, for quick calculation.
	static int anFactorial[10] = {
		1, 1, 2, 6, 24,
		120, 720, 5040, 40320, 362880
	};

	if (n < 0)
	{
		return 1;
	}
	else if (n < 10)
	{
		return anFactorial[n];
	}
	else 
	{
		return factorial(n-1) * n;
	}
}


/**
 * Description: Get PI value.
 * @return: PI value.
 */
double CMathematics::getPiValue()
{
	return _dPI;
}


/**
 * Description:
 * @param point: (IN) Point coordinate.
 * @param lineVector: (IN) Line vector.
 * @return:
 * @exception:
 *	CInvalidArgumentException:
 */
double CMathematics::pointToLineSquareDistance(const std::vector<double>& point, const std::vector<double>& lineVector)
{
	static const int nDIMENSION_3 = 3;
	// If in 3D space:
	if (static_cast<int>(point.size()) == nDIMENSION_3 && static_cast<int>(lineVector.size()) == nDIMENSION_3)
	{
		const double& dPointX = point[0];
		const double& dPointY = point[1];
		const double& dPointZ = point[2];

		const double& dVectorX = lineVector[0];
		const double& dVectorY = lineVector[1];
		const double& dVectorZ = lineVector[2];

		const double dA = dPointY * dVectorZ - dPointZ * dVectorY;
		const double dB = dPointZ * dVectorX - dPointX * dVectorZ;
		const double dC = dPointX * dVectorZ - dPointZ * dVectorX;

		return (dA * dA + dB * dB + dC * dC) / (dVectorX * dVectorX + dVectorY * dVectorY + dVectorZ * dVectorZ);
	}
	// If not in 3D space:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sNOT_IN_3D_SPACE;
		throw CInvalidArgumentException(msgStream.str());
	}
}


/**
 * Description: Calculate the square Euclidean distance between two points.
 * @param position1: (IN) Position of point 1.
 * @param position2: (IN) Position of point 2.
 * @return: Square Euclidean distance.
 * @exception:
 *	CInvalidArgumentException:
 */
double CMathematics::pointToPointSquareDistance(const std::vector<double>& position1, const std::vector<double>& position2)
{
	double dDistance = 0.0;
	// If dimension match:
	if (position1.size() == position2.size())
	{
		const int nDIMENSION = position1.size();
		for (int iDimension = 0; iDimension < nDIMENSION; iDimension++)
		{
			dDistance += (position2[iDimension] - position1[iDimension]) * (position2[iDimension] - position1[iDimension]);
		}
	}
	// If dimension not match:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sDIMENSION_NOT_MATCH;
		throw CInvalidArgumentException(msgStream.str());
	}

	return dDistance;
}


/**
 * Description:
 * @param srcVector: (IN)
 * @param dstVector: (OUT) Output spherical coordinate. Each point is represented as an vector, [Theta, Phi, R]. Theta denotes the polar angle from z-axis, range [0..PI]; 
 *	Phi denotes the azimuthal angle in the xy-plane, range [0..2PI]; R denotes the radius component.
 * @return:
 *	ErrorCodes::nNORMAL:
 */
int CMathematics::rectangularToSphericalCoordinate(const std::vector<double>& srcVector, std::vector<double>& dstVector)
{
	const double& dX = srcVector[0];
	const double& dY = srcVector[1];
	const double& dZ = srcVector[2];

	const double dR = sqrt(dX * dX + dY * dY + dZ * dZ);
	const double dPhi = atan2(dY, dX) + getPiValue();
	const double dTheta = acos(dZ / dR);

	dstVector[0] = dTheta;
	dstVector[1] = dPhi;
	dstVector[2] = dR;

	return ErrorCodes::nNORMAL;
}


/**
 * Description:
 * @param srcVector: (IN) Source spherical coordinate. Each point is represented as an vector, [Theta, Phi, R]. Theta denotes the polar angle from z-axis, range [0..PI]; 
 *	Phi denotes the azimuthal angle in the xy-plane, range [0..2PI]; R denotes the radius component.
 * @param dstVector: (OUT)
 * @return:
 *	ErrorCodes::nNORMAL:
 */
int CMathematics::sphericalToRectanglularCoordinate(const std::vector<double>& srcVector, std::vector<double>& dstVector)
{
	const double& dTheta = srcVector[0];
	const double& dPhi = srcVector[1];
	const double& dR = srcVector[2];

	const double dX = dR * cos(dPhi) * sin(dTheta);
	const double dY = dR * sin(dPhi) * sin(dTheta);
	const double dZ = dR * cos(dTheta);

	dstVector[0] = dX;
	dstVector[1] = dY;
	dstVector[2] = dZ;


	return ErrorCodes::nNORMAL;
}
