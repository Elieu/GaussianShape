/**
 * Pocket Combo Similarity Evaluator Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file PocketComboSimilarityEvaluator.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-06-30
 */


#ifndef POCKET_COMBO_SIMILARITY_EVALUATOR_INCLUDE_H
#define POCKET_COMBO_SIMILARITY_EVALUATOR_INCLUDE_H
//


#include "InterfaceFunctionValueEvaluator.h"

#include <memory>
#include <vector>


class IAtom;
class IMolecule;


/**
 * Description:
 */
class CPocketComboSimilarityEvaluator : public IFunctionValueEvaluator
{
	/* data: */
public:
private:
	// molecule representation of the fit pocket
	const std::auto_ptr<IMolecule> _pocketFitPtr;
	// molecule representation of the reference pocket
	const std::auto_ptr<IMolecule> _pocketRefPtr;
	// molecule representation of the volume (negative image) of fit pocket
	const std::auto_ptr<IMolecule> _pocketVolumeFitPtr;
	// molecule representation of the volume (negative image) of reference pocket
	const std::auto_ptr<IMolecule> _pocketVolumeRefPtr;
	// all atoms of reference pocket
	std::vector<IAtom*> _refPocketAtomsVector;

	/* method: */
public:
	CPocketComboSimilarityEvaluator(const IMolecule& pocketVolumeRef, const IMolecule& pocketRef, const IMolecule& pocketVolumeFit, const IMolecule& pocketFit);
	virtual ~CPocketComboSimilarityEvaluator();

	/* Implementation for IFunctionValueEvaluator interface: */
	virtual double getFunctionValue(const std::vector<double>& params);
private:
};


//
#endif
