/**
 * Spherical Harmonic Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file SphericalHarmonic.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-02-25
 */


#ifndef SPHERICAL_HARMONIC_INLUDE_H
#define SPHERICAL_HARMONIC_INLUDE_H
//


#include <vector>


/**
 * Description: Spherical harmonics related.
 */
class CSphericalHarmonic
{
	/* data: */
public:
	/* Error Codes. */
	struct ErrorCodes
	{
		static const int nNORMAL;

	private:
		ErrorCodes() {};
	};

private:

	/* method: */
public:
	CSphericalHarmonic();
	~CSphericalHarmonic();

	static int getProjection(const std::vector<std::vector<double> >& samples, int nMaxBands, std::vector<std::vector<double> >& coefficients);
	static double projectToSphericalHarmonic(const std::vector<std::vector<double> >& samples, int l, int m);
	static double sphericalHarmonicFunction(int l, int m, double dTheta, double dPhi);
private:
	static double K(int l, int m);
	static double P(int l, int m, double x);
};


//
#endif
