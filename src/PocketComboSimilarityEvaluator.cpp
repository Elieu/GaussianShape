/**
 * Pocket Combo Similarity Evaluator Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file PocketComboSimilarityEvaluator.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-06-30
 */


#include "PocketComboSimilarityEvaluator.h"

#include "AssignmentSolver.h"
#include "Exception.h"
#include "InterfaceAtom.h"
#include "InterfaceMolecule.h"
#include "InterfaceResidue.h"
#include "Mathematics.h"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <vector>


using std::auto_ptr;
using std::list;
using std::map;
using std::string;
using std::vector;


/**
 * Description: Constructor.
 */
CPocketComboSimilarityEvaluator::CPocketComboSimilarityEvaluator(
	const IMolecule& pocketVolumeRef,
	const IMolecule& pocketRef,
	const IMolecule& pocketVolumeFit,
	const IMolecule& pocketFit
	) :
	_pocketFitPtr(dynamic_cast<IMolecule*>(pocketFit.clone())),
	_pocketRefPtr(dynamic_cast<IMolecule*>(pocketRef.clone())),
	_pocketVolumeFitPtr(dynamic_cast<IMolecule*>(pocketVolumeFit.clone())),
	_pocketVolumeRefPtr(dynamic_cast<IMolecule*>(pocketVolumeRef.clone()))
{
	// If clone OK:
	if (_pocketFitPtr.get())
	{
		/* Get all atoms of reference pocket. */
		/* Note: The reference molecule remains still in the whole optimization process, so deal with it before hand. */
		const list<IAtom*> refPocketAtomsList = _pocketRefPtr->getAtomsList();
		std::copy(refPocketAtomsList.begin(), refPocketAtomsList.end(), std::back_inserter(_refPocketAtomsVector));
	}
}


/**
 * Description: Destructor.
 */
CPocketComboSimilarityEvaluator::~CPocketComboSimilarityEvaluator()
{
}


/**
 * Description:
 * @exception:
 * 	CBadCastException:
 */
double CPocketComboSimilarityEvaluator::getFunctionValue(const std::vector<double>& params)
{
	// If dynamic cast OK:
	if (_pocketFitPtr.get() && _pocketRefPtr.get() && _pocketVolumeFitPtr.get() && _pocketVolumeRefPtr.get())
	{
		/* Clone fit molecules. */
		auto_ptr<IMolecule> pocketFitClonePtr(dynamic_cast<IMolecule*>(_pocketFitPtr->clone()));
		auto_ptr<IMolecule> pocketVolumeFitClonePtr(dynamic_cast<IMolecule*>(_pocketVolumeFitPtr->clone()));

		// If clone OK:
		if (pocketFitClonePtr.get() && pocketVolumeFitClonePtr.get())
		{
			/* Apply transformation to fit molecules. */
			/**
			 * Note: Pay attention to the transformation sequence: translation first or rotation first.
			 */
			pocketFitClonePtr->rotateXYZ(
				params[3],
				params[4],
				params[5]
				);
			pocketFitClonePtr->move(
				params[0],
				params[1],
				params[2]
				);

			pocketVolumeFitClonePtr->rotateXYZ(
				params[3],
				params[4],
				params[5]
				);
			pocketVolumeFitClonePtr->move(
				params[0],
				params[1],
				params[2]
				);

			/* Deal with pocket (pseudo molecule representation). */
			/* Get all atoms of fit pocket. */
			const list<IAtom*> fitPocketAtomsList = pocketFitClonePtr->getAtomsList();
			vector<IAtom*> fitPocketAtomsVector;
			std::copy(fitPocketAtomsList.begin(), fitPocketAtomsList.end(), std::back_inserter(fitPocketAtomsVector));
			/* Construct distance matrix (as cost matrix for solving assignment problem). */
			// distance matrix (row: reference atom; column: fit atom)
			vector<vector<double> > distanceMatrix(_refPocketAtomsVector.size());
			// For each atom in reference molecule:
			for (int iRefAtom = 0; iRefAtom < static_cast<int>(_refPocketAtomsVector.size()); ++ iRefAtom)
			{
				IAtom& refAtom = *_refPocketAtomsVector[iRefAtom];

				// For each atom in fit molecule:
				for (int iFitAtom = 0; iFitAtom < static_cast<int>(fitPocketAtomsVector.size()); ++ iFitAtom)
				{
					IAtom& fitAtom = *fitPocketAtomsVector[iFitAtom];

					double dDistance = CMathematics::pointToPointSquareDistance(refAtom.getPosition(), fitAtom.getPosition());
					// If residue name not match:
					if (refAtom.getResidue()->getName().compare(fitAtom.getResidue()->getName()))
					{
						dDistance *= 4;
					}

					distanceMatrix[iRefAtom].push_back(dDistance);
				}
			}

			/* Calculate optimal assignments. */
			CAssignmentSolver assignmentSolver(distanceMatrix);
			// optimal assignments (key: row ID; value: column ID)
			map<int, int> optimalAssignmentsMap;
			const double dCostTotal = assignmentSolver.evaluateOptimalAssignments(optimalAssignmentsMap);

			/* Calculate RMSD. */
			double dRmsd = dCostTotal / optimalAssignmentsMap.size();
			dRmsd = std::sqrt(dRmsd);

			return dRmsd;
		}
		// If clone fails:
		else
		{
			std::stringstream msgStream;
			msgStream
				<< LOCATION_STREAM_INSERTION
				<< "Bad cast! "
				<< "The clones of fit molecules can not clone an instance of IMolecule interface! ";
			throw CBadCastException(msgStream.str());
		}
	}
	// If dynamic cast fails:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "Bad cast! "
			<< "The arguments provided in constructor can not clone an instance of IMolecule interface! ";
		throw CBadCastException(msgStream.str());
	}
}
