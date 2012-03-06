/**
 * Gaussian Volume Fitness Evaluator Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file GaussianVolumeFitnessEvaluator.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-09-26
 */


#ifndef GAUSSIAN_VOLUME_OVERLAP_EVALUATOR_INCLUDE_H
#define GAUSSIAN_VOLUME_OVERLAP_EVALUATOR_INCLUDE_H
//


#include "GaussianVolume.h"
#include "InterfaceFunctionValueEvaluator.h"

#include <set>
#include <string>
#include <vector>


class IAtom;
class IMolecule;


/**
 * Description: Gaussian volume fitness evaluator used for genetic optimization.
 */
class CGaussianVolumeOverlapEvaluator : public IFunctionValueEvaluator
{
	/* data: */
public:
private:
	/* Error codes. */
	struct ErrorCodes
	{
		static const int nNORMAL;

	private:
		ErrorCodes() {};
	};


	/* Default values. */
	struct DefaultValues
	{
		static const bool bNEGATIVE_OVERLAP;
		static const double dGAUSSIAN_CUTOFF;
		static const int nMAX_INTERSECTION_ORDER;

	private:
		DefaultValues() {};
	};


	// a flag indicating whether the initialization for GaussianVolumeBuilder has been done
	bool _bInitForGaussianVolumeBuilder;
	// a flag indicating whether to calculate the negative Gaussian overlap volume
	bool _bNegativeOverlap;
	// Gaussian cutoff
	double _dGaussianCutoff;
	// Gaussian volume builder
	CGaussianVolumeBuilder _gVolumeBuilder;
	// max intersection order to expand when calculating Gaussian volume
	int _nMaxIntersectionOrder;
	// fit molecule
	const IMolecule* _pFitMolecule;
	// reference molecule
	const IMolecule* _pRefMolecule;

	/* method: */
public:
	CGaussianVolumeOverlapEvaluator(const IMolecule& refMolecule, const IMolecule& fitMolecule);
	virtual ~CGaussianVolumeOverlapEvaluator();

	double getGaussianCutoff() const;
	int getMaxIntersectionOrder() const;
	bool getNegativeOverlapFlag() const;
	void setGaussianCutoff(const double dCutoff);
	void setMaxIntersectionOrder(const int nOrders);
	void setNegativeOverlapFlag(const bool bFlag);

	/* Implementation for IFunctionValueEvaluator interface: */
	virtual double getFunctionValue(const std::vector<double>& params);
private:
	inline int attemptInitialize();
};


//
#endif
