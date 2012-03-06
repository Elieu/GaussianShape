/**
 * Gaussian Volume Fitness Evaluator Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file GaussianVolumeFitnessEvaluator.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-09-26
 */


#include "GaussianVolumeOverlapEvaluator.h"

#include "BusinessException.h"
#include "GaussianVolume.h"
#include "InterfaceAtom.h"
#include "InterfaceMolecule.h"
#include "Mathematics.h"
#include "Utility.h"

#include <algorithm>
#include <cmath>
#include <list>
#include <memory>
#include <string>
#include <sstream>


using std::auto_ptr;
using std::list;
using std::set;
using std::string;
using std::vector;


/* Implementation for CGaussianVolumeFitnessEvaluator class: */

/* Static Members: */
const int CGaussianVolumeOverlapEvaluator::ErrorCodes::nNORMAL = 0;

const bool CGaussianVolumeOverlapEvaluator::DefaultValues::bNEGATIVE_OVERLAP = false;
const double CGaussianVolumeOverlapEvaluator::DefaultValues::dGAUSSIAN_CUTOFF = 0;
const int CGaussianVolumeOverlapEvaluator::DefaultValues::nMAX_INTERSECTION_ORDER = 1;


/**
 * Description: Ctor.
 * @param refMolecule: Reference molecule, which will be fixed.
 * @param fitMolecule: Fit molecule, which will be transformed.
 */
CGaussianVolumeOverlapEvaluator::CGaussianVolumeOverlapEvaluator(const IMolecule& refMolecule, const IMolecule& fitMolecule) :
	_bInitForGaussianVolumeBuilder(false),
	_bNegativeOverlap(DefaultValues::bNEGATIVE_OVERLAP),
	_dGaussianCutoff(DefaultValues::dGAUSSIAN_CUTOFF),
	_gVolumeBuilder(&refMolecule, &fitMolecule),
	_nMaxIntersectionOrder(DefaultValues::nMAX_INTERSECTION_ORDER),
	_pFitMolecule(dynamic_cast<IMolecule*>(fitMolecule.clone())),
	_pRefMolecule(dynamic_cast<IMolecule*>(refMolecule.clone()))
{
	// If type cast success:
	if (_pFitMolecule && _pRefMolecule)
	{
	}
	// If type cast failure:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "Bad cast! "
			<< "Parameter refMol or fitMol can not be clone to IMolecule interface! ";
		throw CBadCastException(msgStream.str());
	}
}


/**
 * Description: Dtor.
 */
CGaussianVolumeOverlapEvaluator::~CGaussianVolumeOverlapEvaluator()
{
	delete _pFitMolecule;
	delete _pRefMolecule;
}


/**
 * Description:
 */
int CGaussianVolumeOverlapEvaluator::attemptInitialize()
{
	/* Attempt to initialize GaussianVolumeBuilder. */
	// If not been initialized yet:
	if (!_bInitForGaussianVolumeBuilder)
	{
		_gVolumeBuilder.setGaussianCutoff(getGaussianCutoff());
		_gVolumeBuilder.setMaxIntersectionOrder(getMaxIntersectionOrder());

		_bInitForGaussianVolumeBuilder = true;
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description: Keep the reference molecule fixed and make transformation to fit molecule, calculating the Gaussian volume overlap as fitness.
 * @param params: Transformation (Translation and rotation) parameters applied to fit molecule. params[0], params[1] and params[2] correspond to translation amount
 *		along X, Y and Z axis respectively, params[3], params[4] and params[5] correspond to rotation angles along X, Y and Z axis respectively.
 * @return: Gaussian volume overlap of the reference molecule and fit molecule.
 * @exception:
 *		CBadCastException:
 */
double CGaussianVolumeOverlapEvaluator::getFunctionValue(const std::vector<double>& params)
{
	attemptInitialize();

	// auto pointer for the clone of fit molecule
	auto_ptr<IMolecule> fitMoleculeClonePtr(dynamic_cast<IMolecule*>(_pFitMolecule->clone()));
	// If type cast success:
	if (fitMoleculeClonePtr.get())
	{
		/* Apply transformation. */
		/**
		 * Note: Pay attention to the transformation sequence: translation first or rotation first.
		 */
		IMolecule& fitMolecule = *fitMoleculeClonePtr;
		fitMolecule.rotateXYZ(params[3], params[4], params[5]);
		fitMolecule.move(params[0], params[1], params[2]);

		/* Extract fit atoms. */
		const list<IAtom*> fitAtomsList = fitMolecule.getAtomsList();
		vector<IAtom*> fitAtoms;
		fitAtoms.reserve(fitAtomsList.size());
		std::copy(fitAtomsList.begin(), fitAtomsList.end(), std::back_inserter(fitAtoms));

		/* Get overlap volume. */
//		CGaussianVolume gVolume(&_refAtoms, &fitAtoms, &_precalculationResult);
//		const double dOverlap = gVolume.getOverlapVolume();

		CGaussianVolume gaussianVolume;
		gaussianVolume.setGaussianCutoff(getGaussianCutoff());
		double dOverlap = gaussianVolume.getOverlapVolume(*_pRefMolecule, fitMolecule);

		return _bNegativeOverlap ? -1 * dOverlap : dOverlap;
	}
	// If type cast failure:
	else
	{
		std::stringstream msgStream;
		msgStream 
			<< LOCATION_STREAM_INSERTION
			<< "Bad cast! "
			<< "Member variable _pFitMolecule can not be clone to IMolecule interface! ";
		throw CBadCastException(msgStream.str());
	}
}


/**
 * Description:
 */
double CGaussianVolumeOverlapEvaluator::getGaussianCutoff() const
{
	return _dGaussianCutoff;
}


/**
 * Description:
 */
int CGaussianVolumeOverlapEvaluator::getMaxIntersectionOrder() const
{
	return _nMaxIntersectionOrder;
}


/**
 * Description:
 */
bool CGaussianVolumeOverlapEvaluator::getNegativeOverlapFlag() const
{
	return _bNegativeOverlap;
}


/**
 * Description:
 */
void CGaussianVolumeOverlapEvaluator::setGaussianCutoff(const double dCutoff)
{
	// If valid parameter:
	if (dCutoff >= 0)
	{
		_dGaussianCutoff = dCutoff;
	}
	// If invalid parameter:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "Invalid parameter: "
			<< "dCutoff = " << dCutoff;
		throw CInvalidArgumentException(msgStream.str());
	}
}


/**
 * Description:
 */
void CGaussianVolumeOverlapEvaluator::setMaxIntersectionOrder(const int nOrders)
{
	// If valid parameter:
	if (nOrders > 0)
	{
		_nMaxIntersectionOrder = nOrders;
	}
	// If invalid parameter:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "Invalid parameter: "
			<< "nOrders = " << nOrders;
		throw CInvalidArgumentException(msgStream.str());
	}
}


/**
 * Description:
 */
void CGaussianVolumeOverlapEvaluator::setNegativeOverlapFlag(const bool bFlag)
{
	_bNegativeOverlap = bFlag;
}
