/**
 * Spherical Harmonic Service Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file SphericalHarmonicService.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-02-25
 */


#include "SphericalHarmonicService.h"

#include "ConfigurationArguments.h"
#include "InterfaceAtom.h"
#include "InterfaceMolecule.h"
#include "Mathematics.h"
#include "SphericalHarmonic.h"
#include "Utility.h"

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <list>
#include <string>


using std::list;
using std::string;
using std::vector;


/* Static members: */

/* Error codes: */
const int CSphericalHarmonicService::ErrorCodes::nFAILURE = -1;
const int CSphericalHarmonicService::ErrorCodes::nIVALID_CONFIGURATION_ARGUMENT = 1;
const int CSphericalHarmonicService::ErrorCodes::nNORMAL = 0;

/* Message texts: */
const std::string CSphericalHarmonicService::MessageTexts::sINVALID_NEGATIVE_VALUE("Invalid negative value! ");

/* Default values: */
const double CSphericalHarmonicService::DefaultValues::dSH_SURFACE_PROBE_RADIUS = 1.4;
const int CSphericalHarmonicService::DefaultValues::nSH_MAX_BANS = 7;
const int CSphericalHarmonicService::DefaultValues::nSH_SURFACE_POINTS_COUNT = 70 * 70;

/* Parameter names: */
const std::string CSphericalHarmonicService::ParameterNames::sSH_MAX_BANDS("SH_MAX_BANDS");
const std::string CSphericalHarmonicService::ParameterNames::sSH_SURFACE_POINTS_COUNT("SH_SURFACE_POINTS_COUNT");
const std::string CSphericalHarmonicService::ParameterNames::sSH_SURFACE_PROBE_RADIUS("SH_SURFACE_PROBE_RADIUS");


/**
 * Description: Ctor.
 */
CSphericalHarmonicService::CSphericalHarmonicService()
{
	initParameters();
}


/**
 * Description: Ctor.
 */
CSphericalHarmonicService::CSphericalHarmonicService(const CConfigurationArguments& configurationArguments)
{
	initParameters(configurationArguments);
}


/**
 * Description: Dtor.
 */
CSphericalHarmonicService::~CSphericalHarmonicService()
{
}


/**
 * Description:
 * @param molecule: (IN) Molecule whose surface points to be calculated.
 * @param surfacePoints: (IN, OUT) Each point is represented as an vector, [Theta, Phi, R]. Theta denotes the polar angle from z-axis, range [0..PI]; 
 *	Phi denotes the azimuthal angle in the xy-plane, range [0..2PI]; R denotes the radius component. R value will be calculated according to given Theta and Phi.
 * @return: Number of surface points been calculated.
 */
int CSphericalHarmonicService::calculateSphericalMolecularSurfacePoints(const IMolecule& molecule, std::vector<std::vector<double> >& surfacePoints) const
{
	// number of surface points been calculated
	int nSurfacePointsCalculated = 0;
	// If non-empty molecule:
	if (molecule.getAtomsCount() > 0)
	{
		// surface points
		vector<vector<double> > samplePoints;
		generateUniformSphericalPoints(samplePoints, getShSurfacePointsCount());

		/* TODO: Calculate surface point. */

		/**
		 * Notice:
		 * Here let "A" denote the centroid of atom under consideration, "B" denote the surface point we want to get, and "O" denote the origin that is the centroind
		 * of current molecule. Then we have |AB| = |OB - OA| = r, and B is on the sample vector. "S" denote the generated sample point, then S is on OB and |OB| = x|OS|, 
		 * where x is a scalar. It is quite easy to calculate the position of B using vector algebra.
		 * Finally we only need to solve a quadratic equation: ax^2 + bx + c = 0.
		 * If OS is a unit vector, then a = 1.
		 */

		// origin point
		const vector<double> originPoint = molecule.getCentroid();
		// For each surface point:
		for (int iPoint = 0; iPoint < static_cast<int>(samplePoints.size()); ++ iPoint)
		{
			// length of OB
			double dSurfaceDistance = 1.0;
			// current sample point (unit vector) in rectangular system
			vector<double> samplePoint(3, 0.0);
			CMathematics::sphericalToRectanglularCoordinate(samplePoints[iPoint], samplePoint);
			const list<IAtom*> atomsList = molecule.getAtomsList();
			// For each atom:
			FOREACH(atomIter, atomsList, list<IAtom*>::const_iterator)
			{
				// current atom
				const IAtom& atom = **atomIter;
				// atom position relative to the origin point
				vector<double> atomPosition = atom.getPosition();
				CMathematics::substract(atomPosition, originPoint);
				// surface radius
				const double dSurfaceRadius = atom.getAtomRadius() + getShSurfaceProbeRadius();

				// If OB went throuth this atom:
				if (CMathematics::pointToLineSquareDistance(atomPosition, samplePoint) < dSurfaceRadius * dSurfaceRadius)
				{
					/* TODO: Solve equation: ax^2 + bx + c = 0. */
					const double a = 1.0;
					const double b = 
						-2 * 
						(
						samplePoint[0] * atomPosition[0] 
						+ samplePoint[1] * atomPosition[1] 
						+ samplePoint[2] * atomPosition[2] 
						);
					const double c = 
						(
						atomPosition[0] * atomPosition[0] 
						+ atomPosition[1] * atomPosition[1] 
						+ atomPosition[2] * atomPosition[2]
						) 
						- dSurfaceRadius * dSurfaceRadius;

					const double dDelta = b * b - 4 * a * c;
					// If solution x exists:
					if (dDelta > 0)
					{
						const double dDeltaSquareRoot = std::sqrt(dDelta);
						// root1 (x1)
						const double dRoot1 = (-1 * b + dDeltaSquareRoot) / (2 * a);
						// root2 (x2)
						const double dRoot2 = (-1 * b - dDeltaSquareRoot) / (2 * a);
						// root been chosen
						double dRoot = 0;

						if (dRoot1 < 0)
						{
							// If both of B are on the oppsite direction of OS:
							if (dRoot2 < 0)
							{
								continue;
							}
							else
							{
								dRoot = dRoot2;
							}
						}
						else if (dRoot2 < 0)
						{
							dRoot = dRoot1;
						}
						// If both of B are on the same direction of OS:
						else
						{
							dRoot = (std::abs(dRoot1) > std::abs(dRoot2)) ? dRoot1 : dRoot2;
						}

						dSurfaceDistance = (dRoot > dSurfaceDistance) ? dRoot : dSurfaceDistance;
					}
					else
					{
						continue;
					}
				}
				else
				{
					continue;
				}
			}

			/* Get surface distance for this surface point. */
			// If |OB| has been solved:
			if (dSurfaceDistance > 1.0)
			{
				samplePoints[iPoint][2] = dSurfaceDistance;
				++ nSurfacePointsCalculated;
			}
		}

		// Return the calculated surface points.
		surfacePoints = samplePoints;
	}

	return nSurfacePointsCalculated;
}


/**
 * Desciption:
 * @param configurationArguments: (IN)
 * @return:
 *	ErrorCodes::nNORMAL:
 *	ErrorCodes::nINVALID_CONFIGURATION_ARGUMENT:
 */
int CSphericalHarmonicService::configure(const CConfigurationArguments& configurationArguments)
{
	const int nErrorCode = initParameters(configurationArguments);

	// If parameter initialization success:
	if (nErrorCode == ErrorCodes::nNORMAL)
	{
		return ErrorCodes::nNORMAL;
	}
	else
	{
		return ErrorCodes::nIVALID_CONFIGURATION_ARGUMENT;
	}
}


/**
 * Description:
 * @param molecule: (IN)
 * @param descriptor: (OUT)
 * @return:
 */
int CSphericalHarmonicService::evaluateShMolecularDescriptor(const IMolecule& molecule, std::vector<double>& descriptor) const
{
	// molecular surface points
	vector<vector<double> > surfacePoints;
	calculateSphericalMolecularSurfacePoints(molecule, surfacePoints);

	// projection coefficients
	vector<vector<double> > coefficients;
	CSphericalHarmonic::getProjection(surfacePoints, getShMaxBands(), coefficients);

	/* Generate descriptor. */
	descriptor.clear();
	// For each SH band:
	for (int iBand = 0; iBand < static_cast<int>(coefficients.size()); ++ iBand)
	{
		// each component of descriptor
		double dDescriptorComponent = 0;
		const vector<double>& bandCoefficients = coefficients[iBand];
		// For each coefficient in this SH band:
		for (int iBandCoefficient = 0; iBandCoefficient < static_cast<int>(bandCoefficients.size()); ++ iBandCoefficient)
		{
			dDescriptorComponent += bandCoefficients[iBandCoefficient] * bandCoefficients[iBandCoefficient];
		}

		descriptor.push_back(dDescriptorComponent);
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description:
 * @return:
 */
int CSphericalHarmonicService::getShMaxBands() const
{
	return _parameterAggregation.nShMaxBands;
}


/**
 * Description:
 * @return:
 */
int CSphericalHarmonicService::getShSurfacePointsCount() const
{
	return _parameterAggregation.nShSurfacePointsCount;
}


/**
 * Description:
 * @return:
 */
double CSphericalHarmonicService::getShSurfaceProbeRadius() const
{
	return _parameterAggregation.dShSurfaceProbeRadius;
}


/**
 * Description: Generate uniformly distributed point on a unit sphere surface.
 * @param samplePoints: (OUT) Resulted uniform points vector. Each point is represented as an vector, [Theta, Phi, R]. Theta denotes the polar angle from z-axis, 
 *		range [0..PI]; Phi denotes the azimuthal angle in the xy-plane, range [0..2PI]; R denotes the radius component.
 * @param nDesiredPointsCount: Specify how many points are desired to sample.
 * @return: The number of sample points which are actually sampled.
 */
int CSphericalHarmonicService::generateUniformSphericalPoints(std::vector<std::vector<double> >& samplePoints, int nDesiredPointsCount) const
{
	/* Initialize random seed only onece. */
	static bool bRandomSeedInitialized = false;
	if (!bRandomSeedInitialized)
	{
		srand((unsigned)time(0));
		bRandomSeedInitialized = true;
	}

	/* Generate sample points. */
	samplePoints.clear();
	// square root integer of desired sample points count
	const int nSqrtSamplesCount = static_cast<int>(std::sqrt(static_cast<double>(nDesiredPointsCount)));
	for (int a = 0; a < nSqrtSamplesCount; ++ a)
	{
		for (int b = 0; b < nSqrtSamplesCount; ++ b)
		{
			// Do NOT reuse the rand result!
			double dX = rand() / static_cast<double>(RAND_MAX);
			double dY = rand() / static_cast<double>(RAND_MAX);
			dX = (a + dX) / nSqrtSamplesCount;
			dY = (b + dY) / nSqrtSamplesCount;
			// theta angle of the point
			const double dTheta = 2.0 * acos(std::sqrt(1.0 - dX));
			// phi angle of the point
			const double dPhi = 2.0 * CMathematics::getPiValue() * dY;

			// sample point which is represented as [Theta, Phi, R]
			vector<double> point;
			point.push_back(dTheta);
			point.push_back(dPhi);
			point.push_back(1.0);

			samplePoints.push_back(point);
		}
	}

	return nSqrtSamplesCount * nSqrtSamplesCount;
}


/**
 * Description: Initialize all parameters to default values.
 * @return:
 *	ErrorCodes::nNORMAL:
 */
int CSphericalHarmonicService::initParameters()
{
	setShMaxBands(DefaultValues::nSH_MAX_BANS);
	setShSurfacePointsCount(DefaultValues::nSH_SURFACE_POINTS_COUNT);
	setShSurfaceProbeRadius(DefaultValues::dSH_SURFACE_PROBE_RADIUS);

	return ErrorCodes::nNORMAL;
}


/**
 * Description: Initialize all parameters according to configuration. Parameters which does not exist in configuration file will be initialize to default value.
 * @param configArguments: (IN) Configuration arguments from configuration file.
 * @return:
 *	ErrorCodes::nNORMAL:
 */
int CSphericalHarmonicService::initParameters(const CConfigurationArguments& configurationArguments)
{
	// error code to be returned
	int nErrorCode = ErrorCodes::nNORMAL;

	initParameters();

	/* Set parameters to configured value if possible. */
	try
	{
		if (configurationArguments.existArgument(ParameterNames::sSH_MAX_BANDS))
		{
			int nMaxBands = 0;
			const int nArgumentErrorCode = configurationArguments.getArgumentValue(ParameterNames::sSH_MAX_BANDS, nMaxBands);

			// If argument conversion succeeds:
			if (nArgumentErrorCode == CConfigurationArguments::ErrorCodes::nNORMAL)
			{
				setShMaxBands(nMaxBands);
			}
			// If argument conversion fails:
			else
			{
				nErrorCode = ErrorCodes::nIVALID_CONFIGURATION_ARGUMENT;
			}
		}

		if (configurationArguments.existArgument(ParameterNames::sSH_SURFACE_POINTS_COUNT))
		{
			int nShSurfacePointsCount = 0;
			const int nArgumentErrorCode = configurationArguments.getArgumentValue(ParameterNames::sSH_SURFACE_POINTS_COUNT, nShSurfacePointsCount);

			// If argument conversion succeeds:
			if (nArgumentErrorCode == CConfigurationArguments::ErrorCodes::nNORMAL)
			{
				setShSurfacePointsCount(nShSurfacePointsCount);
			}
			// If argument conversion fails:
			else
			{
				nErrorCode = ErrorCodes::nIVALID_CONFIGURATION_ARGUMENT;
			}
		}

		if (configurationArguments.existArgument(ParameterNames::sSH_SURFACE_PROBE_RADIUS))
		{
			double dShSurfaceProbeRadius = 0;
			const int nArgumentErrorCode = configurationArguments.getArgumentValue(ParameterNames::sSH_SURFACE_PROBE_RADIUS, dShSurfaceProbeRadius);

			// If argument conversion succeeds:
			if (nArgumentErrorCode == CConfigurationArguments::ErrorCodes::nNORMAL)
			{
				setShSurfaceProbeRadius(dShSurfaceProbeRadius);
			}
			// If argument conversion fails:
			else
			{
				nErrorCode = ErrorCodes::nIVALID_CONFIGURATION_ARGUMENT;
			}
		}
	}
	// If parameter conversion succeeds but the corresponding value is invalid:
	catch(CInvalidArgumentException& exception)
	{
		nErrorCode = ErrorCodes::nIVALID_CONFIGURATION_ARGUMENT;
	}

	return nErrorCode;
}


/**
 * Description:
 * @param nMaxBands: (IN)
 * @exception:
 *	CInvalidArgumentException:
 */
void CSphericalHarmonicService::setShMaxBands(int nMaxBands)
{
	// If valid argument:
	if (nMaxBands > 0)
	{
		_parameterAggregation.nShMaxBands = nMaxBands;
	}
	// If invalid argument:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sINVALID_NEGATIVE_VALUE
			<< "nMaxBands = " << nMaxBands;
		throw CInvalidArgumentException(msgStream.str());
	}
}


/**
 * Description:
 * @param nPointsCount: (IN)
 * @exception:
 *	CInvalidArgumentException:
 */
void CSphericalHarmonicService::setShSurfacePointsCount(int nPointsCount)
{
	// If valid argument:
	if (nPointsCount > 0)
	{
		_parameterAggregation.nShSurfacePointsCount = nPointsCount;
	}
	// If invalid argument:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sINVALID_NEGATIVE_VALUE
			<< "nPointsCount = " << nPointsCount;
		throw CInvalidArgumentException(msgStream.str());
	}
}


/**
 * Description:
 * @param dRadius: (IN)
 * @exception:
 *	CInvalidArgumentException:
 */
void CSphericalHarmonicService::setShSurfaceProbeRadius(double dRadius)
{
	// If valid argument:
	if (dRadius > 0)
	{
		_parameterAggregation.dShSurfaceProbeRadius = dRadius;
	}
	// If invalid argument:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sINVALID_NEGATIVE_VALUE
			<< "dRadius = " << dRadius;
		throw CInvalidArgumentException(msgStream.str());
	}
}
