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


#ifndef SPHERICAL_HARMONIC_SERVICE_INCLUDE_H
#define SPHERICAL_HARMONIC_SERVICE_INCLUDE_H
//


#include <string>
#include <vector>


class CConfigurationArguments;
class IMolecule;


/**
 * Description: Spherical harmonic service.
 */
class CSphericalHarmonicService
{
	/* data: */
public:
	/* Error Codes. */
	struct ErrorCodes
	{
		static const int nFAILURE;
		static const int nIVALID_CONFIGURATION_ARGUMENT;
		static const int nNORMAL;

	private:
		ErrorCodes() {};
	};


private:
	/* Message texts. */
	struct MessageTexts
	{
		static const std::string sINVALID_NEGATIVE_VALUE;

	private:
		MessageTexts() {};
	};


	/* Default Values. */
	struct DefaultValues
	{
		// for parameter "dShSurfaceProbeRadius"
		static const double dSH_SURFACE_PROBE_RADIUS;
		// for parameter "nShMaxBands"
		static const int nSH_MAX_BANS;
		// for parameter "nShSurfacePointsCount"
		static const int nSH_SURFACE_POINTS_COUNT;

	private:
		DefaultValues() {};
	};


	/* Aggregation of all parameters. */
	struct ParametersAggregation
	{
		// max spherical harmonic bands when performing projection
		int nShMaxBands;
		// number of surface points to do SH projection
		int nShSurfacePointsCount;
		// radius of probe used to generate spherical harmonics surface
		double dShSurfaceProbeRadius;
	};


	/**
	 * Description: Collection of parameter names to look up for specified parameter in configuration file.
	 */
	struct ParameterNames
	{
		// for parameter "nShMaxBands"
		static const std::string sSH_MAX_BANDS;
		// for parameter "nShSurfacePointsCount"
		static const std::string sSH_SURFACE_POINTS_COUNT;
		// for parameter "dShSurfaceProbeRadius"
		static const std::string sSH_SURFACE_PROBE_RADIUS;

	private:
		ParameterNames() {};
	};


	// parameters
	ParametersAggregation _parameterAggregation;

	/* method: */
public:
	CSphericalHarmonicService();
	CSphericalHarmonicService(const CConfigurationArguments& configurationArguments);
	~CSphericalHarmonicService();

	int configure(const CConfigurationArguments& configurationArguments);
	int evaluateShMolecularDescriptor(const IMolecule& molecule, std::vector<double>& descriptor) const;
	int getShMaxBands() const;
	int getShSurfacePointsCount() const;
	double getShSurfaceProbeRadius() const;
	void setShMaxBands(int nMaxBands);
	void setShSurfacePointsCount(int nPointsCount);
	void setShSurfaceProbeRadius(double dRadius);

private:
	int calculateSphericalMolecularSurfacePoints(const IMolecule& molecule, std::vector<std::vector<double> >& surfacePoints) const;
	int generateUniformSphericalPoints(std::vector<std::vector<double> >& samplePoints, int nDesiredPointsCount) const;
	int initParameters();
	int initParameters(const CConfigurationArguments& configurationArguments);
};


//
#endif
