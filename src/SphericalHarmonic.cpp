/**
 * Spherical Harmonic Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file SphericalHarmonic.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-02-25
 */


#include "SphericalHarmonic.h"
#include "Mathematics.h"

#include <cmath>
#include <cstdlib>


using std::abs;
using std::vector;


/* Static Members: */

/* Error Codes: */
const int CSphericalHarmonic::ErrorCodes::nNORMAL = 0;


/* Public Methods: */

/**
 * Description: Ctor.
 */
CSphericalHarmonic::CSphericalHarmonic()
{
}


/**
 * Description: Dtor.
 */
CSphericalHarmonic::~CSphericalHarmonic()
{
}


/**
 * Description: Project a group of samples to a series of spherical harmonic funtions (basis functions), and output the projection coefficients.
 * @param samples: (IN) An array of sample points, which are actually points in the spherical functions. Each point is represented as an vector, [Theta, Phi, R].
 *		Theta denotes the polar angle from z-axis, range [0..PI]; Phi denotes the azimuthal angle in the xy-plane, range [0..2PI]; R denotes the radius component.
 * @param nMaxBands: (IN) Specify the maximal bandwidth of the basis functions.
 * @param coefficients: (OUT) Output vector to store the result coefficients. Coefficients for each band are grouped into separate vectors.
 * @return: Number of projection coefficient.
 */
int CSphericalHarmonic::getProjection(const std::vector<std::vector<double> >& samples, int nMaxBands, std::vector<std::vector<double> >& coefficients)
{
	// projection coefficients count
	int nCoefficientsCount = 0;

	// For each band:
	for (int iBand = 0; iBand < nMaxBands; ++ iBand)
	{
		// coefficients for this band
		vector<double> bandCoefficients;
		// For each component:
		for (int iComponent = -1 * iBand; iComponent <= iBand; ++ iComponent)
		{
			// get projection coefficient
			const double dCoefficient = projectToSphericalHarmonic(samples, iBand, iComponent);
			bandCoefficients.push_back(dCoefficient);

			++ nCoefficientsCount;
		}

		coefficients.push_back(bandCoefficients);
	}

	return nCoefficientsCount;
}


/**
 * Description: Project a group of samples to a specified spherical harmonic function, returnning the projection coefficient.
 * @param samples: (IN) An array of sample points, which are actually points in the spherical functions. Each point is represented as an vector, [Theta, Phi, R].
 *		Theta denotes the polar angle from z-axis, range [0..PI]; Phi denotes the azimuthal angle in the xy-plane, range [0..2PI]; R denotes the radius component.
 * @param l: Band index, range [0..N], specifying a spherical harmonic function.
 * @param m: The mth component inside the band, range [-l..l], specifying a spherical harmonic function.
 * @return: Coefficient of the projection.
 */
double CSphericalHarmonic::projectToSphericalHarmonic(const std::vector<std::vector<double> >& samples, int l, int m)
{
	// total samples count
	const int nSamplesCount = samples.size();
	// coefficient
	double dCoefficient = 0;

	// For each point:
	for (int iPoint = 0; iPoint < nSamplesCount; ++iPoint)
	{
		// current sample point, represented as [Theta, Phi, R]
		const vector<double>& samplePoint = samples[iPoint];
		// theta angle
		const double& dTheta = samplePoint[0];
		// phi angle
		const double& dPhi = samplePoint[1];
		// radius component
		const double& dR = samplePoint[2];

		dCoefficient += dR * sphericalHarmonicFunction(l, m, dTheta, dPhi);
 	}
	dCoefficient = (4 * CMathematics::getPiValue() * dCoefficient) / nSamplesCount;

	return dCoefficient;
}


/**
 * Description: Calculate a spherical harmonic function Y(l,m,theta,phi)
 * @param l: Band index, range [0..N];
 * @param m: The mth component inside the band, range [-l..l];
 * @param dTheta: Spherical coordinate, theta angle, namely the polar angle from z-axis, range [0..PI];
 * @param dPhi: Spherical coordinate, phi angle, namely the azimuthal angle in the xy-plane, range [0..2*PI];
 * @return: The value of spherical harmonic function Y(l,m,theta,phi).
 */
double CSphericalHarmonic::sphericalHarmonicFunction(int l, int m, double dTheta, double dPhi)
{
	const double sqrt2 = sqrt(2.0);
	if (m == 0)
	{
		return K(l, 0) * P(l, 0, cos(dTheta));
	}
	else if (m>0)
	{
		return sqrt2 * K(l, m) * cos(m * dPhi) * P(l, m, cos(dTheta));
	}
	else
	{
		return sqrt2 * K(l, -m) * sin(-m * dPhi) * P(l, -m, cos(dTheta));
	}
}


/* Private Methods: */

/**
 * Description: Calculate the factor K(l,m) used in a spherical harmonic function.
 * @param l: Band index, range [0..N];
 * @param m: The mth component inside the band, range [-l..l];
 * @return: The value of K(l,m).
 */
double CSphericalHarmonic::K(int l, int m)
{
	double dTemp = ((2.0 * l + 1.0) * CMathematics::factorial(l - abs(m))) / (4.0 * CMathematics::getPiValue() * CMathematics::factorial(l + abs(m)));
	return sqrt(dTemp);
}


/**
 * Description: Evaluate an Associated Legendre Polynomial P(l,m,x) at x
 * @param l: Band index, range [0..N];
 * @param m: The mth component inside the band, range [-l..l];
 * @param x: Independent variable of the Associated Legendre Polynomial;
 * @return: The value of Associated Legendre Polynomial.
 */
double CSphericalHarmonic::P(int l, int m, double x)
{
	double pmm = 1.0;
	if (m > 0)
	{
		double somx2 = sqrt((1.0 - x) * (1.0 + x));
		double fact = 1.0;
		for (int i = 1; i <= m; ++i)
		{
			pmm *= (-fact) * somx2;
			fact += 2.0;
		}
	}
	if (l == m) return pmm;

	double pmmp1 = x * (2.0 * m + 1.0) * pmm;
	if (l == m + 1) return pmmp1;
	double pll = 0.0;
	for (int ll = m + 2; ll <= l; ++ll)
	{
		pll = ((2.0 * ll - 1.0) * x * pmmp1 - (ll + m - 1.0) * pmm) / (ll - m);
		pmm = pmmp1;
		pmmp1 = pll;
	}
	return pll;
}
