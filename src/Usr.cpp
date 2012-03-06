/**
 * USR Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file Usr.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-05-12
 */


#include "Usr.h"

#include "Exception.h"
#include "Mathematics.h"
#include "Utility.h"

#include <cmath>
#include <limits>
#include <sstream>


using std::string;
using std::vector;


/* Static members: */

/* Error codes: */
const int CUsr::ErrorCodes::nNORMAL = 0;
const int CUsr::ErrorCodes::nEMPTY_CONTAINER = 1;

/* Message texts: */
const std::string CUsr::MessageTexts::sDIMENSION_NOT_MATCH("Coordinate dimensions not match! ");


/**
 * Description: Ctor.
 */
CUsr::CUsr()
{
}


/**
 * Description: Dtor.
 */
CUsr::~CUsr()
{
}


/**
 * Description: Calculate the centroid of a series of coordinates.
 * @param coordinates: (IN) Source 3-dimensional rectangular coordinates.
 * @param centroid: (OUT) Calculated centroid coordinate. This vector can be empty if the source coordinates array is empty.
 * @return:
 *	ErrorCodes::nNORMAL:
 *	ErrorCodes::nEMPTY_CONTAINER:
 */
int CUsr::calculateCentroid(const std::vector<std::vector<double> >& coordinates, std::vector<double>& centroid)
{
	// If not empty coordinates array:
	if (!coordinates.empty())
	{
		// coordinate dimension
		const int nDIMENSION = coordinates.front().size();
		// centroid coordinate
		vector<double> centroidCoordinate(coordinates.front().size(), 0.0);

		// For each coordinate:
		for (int iCoordinate = 0; iCoordinate < static_cast<int>(coordinates.size()); ++ iCoordinate)
		{
			const vector<double>& coordinate = coordinates[iCoordinate];

			// For each dimension:
			for (int iDimension = 0; iDimension < nDIMENSION; ++ iDimension)
			{
				centroidCoordinate[iDimension] += coordinate[iDimension];
			}
		}
		for (int iDimension = 0; iDimension < nDIMENSION; ++ iDimension)
		{
			centroidCoordinate[iDimension] /= coordinates.size();
		}

		centroid = centroidCoordinate;
		return ErrorCodes::nNORMAL;
	}
	else
	{
		centroid.clear();
		return ErrorCodes::nEMPTY_CONTAINER;
	}
}


/**
 * Description: Calculate the 12-dimensional  USR moments for a series of coordinates.
 * @param coordinates: (IN) Source 3-dimensional rectangular coordinates.
 * @param moments: (OUT) Calculated USR moments.
 * @ecception:
 *	CInvalidArgumentException:
 */
int CUsr::calculateMoments(const std::vector<std::vector<double> >& coordinates, std::vector<double>& moments)
{
	// If not empty coordinates array:
	if (!coordinates.empty())
	{
		// If valid coordinates:
		if (validateCoordinates(coordinates))
		{
			// centroid point
			vector<double> ctdCoordinate;
			// closest point to centroid
			vector<double> cstCoordinate;
			// farthest point to centroid
			vector<double> fctCoordinate;
			// farthest point to fct
			vector<double> ftfCoordinate;

			/* distances arrays */
			vector<double> distancesToCtd;
			vector<double> distancesToCst;
			vector<double> distancesToFct;
			vector<double> distancesToFtf;

			calculateCentroid(coordinates, ctdCoordinate);

			// min distance to centroid
			double dMinDistanceToCtd = std::numeric_limits<double>::max();
			// max distance to centroid
			double dMaxDistanceToCtd = std::numeric_limits<double>::min();

			/* Calculate distances to ctd. */
			FOREACH(coordinateIter, coordinates, vector<vector<double> >::const_iterator)
			{
				double dDistance = CMathematics::pointToPointSquareDistance(*coordinateIter, ctdCoordinate);
				dDistance = std::sqrt(dDistance);
				distancesToCtd.push_back(dDistance);

				/* Find cst point. */
				if (dDistance < dMinDistanceToCtd)
				{
					dMinDistanceToCtd = dDistance;
					cstCoordinate = *coordinateIter;
				}

				/* Find fct point. */
				if (dDistance > dMaxDistanceToCtd)
				{
					dMaxDistanceToCtd = dDistance;
					fctCoordinate = *coordinateIter;
				}
			}

			/* Calculate distances to cst. */
			FOREACH(coordinateIter, coordinates, vector<vector<double> >::const_iterator)
			{
				double dDistance = CMathematics::pointToPointSquareDistance(*coordinateIter, cstCoordinate);
				dDistance = std::sqrt(dDistance);
				distancesToCst.push_back(dDistance);
			}

			// max distance to fct
			double dMaxDistanceToFct = std::numeric_limits<double>::min();

			/* Calculate distances to fct. */
			FOREACH(coordinateIter, coordinates, vector<vector<double> >::const_iterator)
			{
				double dDistance = CMathematics::pointToPointSquareDistance(*coordinateIter, fctCoordinate);
				dDistance = std::sqrt(dDistance);
				distancesToFct.push_back(dDistance);

				/* Find ftf point. */
				if (dDistance > dMaxDistanceToFct)
				{
					dMaxDistanceToFct = dDistance;
					ftfCoordinate = *coordinateIter;
				}
			}

			/* Calculate distances to ftf. */
			FOREACH(coordinateIter, coordinates, vector<vector<double> >::const_iterator)
			{
				double dDistance = CMathematics::pointToPointSquareDistance(*coordinateIter, ftfCoordinate);
				dDistance = std::sqrt(dDistance);
				distancesToFtf.push_back(dDistance);
			}

			/* Calculate 3 moments for each four special positions. */
			moments.clear();

			// mu1 (mean)
			double dMean = 0.0;
			// mu2 (squared sigma)
			double dSigma2 = 0.0;
			// mu3 (skewness)
			double dSkewness = 0.0;

			/* Calculate moments for ctd point. */
			dMean = calculateMu1(distancesToCtd);
			dSigma2 = calculateMu2(distancesToCtd, dMean);
			dSkewness = calculateMu3(distancesToCtd, dMean, std::sqrt(dSigma2));
			moments.push_back(dMean);
			moments.push_back(dSigma2);
			moments.push_back(dSkewness);

			/* Calculate moments for cst point. */
			dMean = calculateMu1(distancesToCst);
			dSigma2 = calculateMu2(distancesToCst, dMean);
			dSkewness = calculateMu3(distancesToCst, dMean, std::sqrt(dSigma2));
			moments.push_back(dMean);
			moments.push_back(dSigma2);
			moments.push_back(dSkewness);

			/* Calculate moments for fct point. */
			dMean = calculateMu1(distancesToFct);
			dSigma2 = calculateMu2(distancesToFct, dMean);
			dSkewness = calculateMu3(distancesToFct, dMean, std::sqrt(dSigma2));
			moments.push_back(dMean);
			moments.push_back(dSigma2);
			moments.push_back(dSkewness);

			/* Calculate moments for ftf point. */
			dMean = calculateMu1(distancesToFtf);
			dSigma2 = calculateMu2(distancesToFtf, dMean);
			dSkewness = calculateMu3(distancesToFtf, dMean, std::sqrt(dSigma2));
			moments.push_back(dMean);
			moments.push_back(dSigma2);
			moments.push_back(dSkewness);
		}
		// If invalid coordinates:
		else
		{
			std::stringstream msgStream;
			msgStream
				<< LOCATION_STREAM_INSERTION
				<< MessageTexts::sDIMENSION_NOT_MATCH;
			throw CInvalidArgumentException(msgStream.str());
		}
	}
	// If empty coodinates array:
	else
	{
		// return zero moments
		moments = vector<double>(12, 0.0);
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description: Calculate 1st moment of USR.
 * @param distances: (IN) Distances to a fixed point.
 * @return: 1st moment of USR.
 */
double CUsr::calculateMu1(const std::vector<double>& distances)
{
	// If not empty distances array:
	if (!distances.empty())
	{
		double dSum = 0.0;

		// For each distance:
		for (int iDistance = 0; iDistance < static_cast<int>(distances.size()); ++ iDistance)
		{
			dSum += distances[iDistance];
		}

		return dSum / distances.size();
	}
	// If empty distances array:
	else
	{
		return 0;
	}
}


/**
 * Description: Calculate 2nd moment of USR.
 * @param distances: (IN) Distances to a fixed point.
 * @param dMean: (IN) Mean value of the corresponding distances. This value can be calculated by calculateM1().
 * @return: 2nd moment of USR.
 */
double CUsr::calculateMu2(const std::vector<double>& distances, double dMean)
{
	// If not empty or single element distances array:
	if (distances.size() > 1)
	{
		double dSum = 0.0;

		// For each distance:
		for (int iDistance = 0; iDistance < static_cast<int>(distances.size()); ++ iDistance)
		{
			const double& dDistance = distances[iDistance];
			dSum += (dDistance - dMean) * (dDistance - dMean);
		}

		return dSum / (distances.size() - 1);
	}
	// If empty or single element distances array:
	else
	{
		return 0;
	}
}


/**
 * Description: Calculate 3rd moment of USR.
 * @param distances: (IN) Distances to a fixed point.
 * @param dMean: (IN) Mean value of the corresponding distances. This value can be calculated by calculateM1().
 * @param dSigma: (IN) Sigma value of the corresponding distances. This value can be calculated by calculateM2().
 * @return: 3rd moment of USR.
 */
double CUsr::calculateMu3(const std::vector<double>& distances, double dMean, double dSigma)
{
	// If valid arguments:
	if (!distances.empty() && dSigma != 0)
	{
		double dSum = 0.0;

		// For each distance:
		for (int iDistance = 0; iDistance < static_cast<int>(distances.size()); ++ iDistance)
		{
			const double& dDistance = distances[iDistance];
			dSum += ((dDistance - dMean) / dSigma) * ((dDistance - dMean) / dSigma) * ((dDistance - dMean) / dSigma);
		}

		return dSum / distances.size();
	}
	// If invalid arguments:
	else
	{
		return 0;
	}
}


/**
 * Description: Validate a series of coordinates making sure that they are all in equal dimension.
 * @param coordinates: (IN) Array of coordinates to be validated.
 * @return: Return ture for valid coordinates and false otherwise.
 */
bool CUsr::validateCoordinates(const std::vector<std::vector<double> >& coordinates)
{
	// If not empty coordinates array:
	if (!coordinates.empty())
	{
		// coordinates dimension
		const unsigned int nDIMENSION = coordinates.front().size();

		// For each coordinate:
		FOREACH(coordinateIter, coordinates, vector<vector<double> >::const_iterator)
		{
			if (coordinateIter->size() != nDIMENSION)
			{
				return false;
			}
		}
		
		return true;
	}
	// If empty coordinates array:
	else
	{
		return true;
	}
}
