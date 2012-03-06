/**
 * Gaussian Volume Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file GaussianVolume.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-09-27
 */


#include "GaussianVolume.h"

#include "Exception.h"
#include "InterfaceAtom.h"
#include "InterfaceMolecule.h"
#include "Mathematics.h"
#include "Utility.h"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <sstream>


using std::auto_ptr;
using std::list;
using std::set;
using std::string;
using std::vector;


/* Implementation for CGaussianVolume::CIntersectedAtomsFilter class: */

/**
 * Description: Constructor.
 * @param pNeighborAtomIds: (IN)
 */
CGaussianVolume::CIntersectedAtomsFilter::CIntersectedAtomsFilter(const std::vector<std::set<int> >* pNeighborAtomIds) :
	_pNeighborAtomIds(pNeighborAtomIds)
{
	// If not NULL parameter:
	if (pNeighborAtomIds)
	{
	}
	// If NULL parameter:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "Invalid parameters: "
			<< "pNeighborAtomIds = NULL. ";
		throw CInvalidArgumentException(msgStream.str());
	}
}


/**
 * Description: Filter out pair-wise intersected atoms cluster.
 * @param atomIdsSet: (IN)
 * @return:
 */
bool CGaussianVolume::CIntersectedAtomsFilter::operator()(const std::set<int>& atomIdsSet) const
{
	FOREACH(iterId, atomIdsSet, set<int>::const_iterator)
	{
		/* Validate with all IDs behind current one. */
		set<int>::const_iterator iterIdBehind = iterId;
		for (++ iterIdBehind; iterIdBehind != atomIdsSet.end(); ++ iterIdBehind)
		{
			const set<int>& neighborIdsSet = (*_pNeighborAtomIds)[*iterId];
			// If not a neighbor of current ID:
			if (!EXIST(*iterIdBehind, neighborIdsSet))
			{
				return false;
			}
		}
	}

	return true;
}


/* Implementation for CGaussianVolume class: */

/* Static members: */
const double CGaussianVolume::_dP = 2.8284271247;
const double CGaussianVolume::_dPI = 3.14159265358;
const double CGaussianVolume::_dPARTIAL_ALPHA = 2.41798793102;

const int CGaussianVolume::ErrorCodes::nNORMAL = 0;


/**
 * Description: Constructor.
 */
CGaussianVolume::CGaussianVolume()
{
}


/**
 * Description: Copy constructor.
 */
CGaussianVolume::CGaussianVolume(const CGaussianVolume& volume) :
	_dGaussianCutoff(volume._dGaussianCutoff),
	_fitAtomsPtr(volume._fitAtomsPtr),
	_neighborAtomIds(volume._neighborAtomIds),
	_pFitAtoms(volume._pFitAtoms),
	_pPrecalculationResult(volume._pPrecalculationResult),
	_pRefAtoms(volume._pRefAtoms),
	_squareDistancesMatrix(volume._squareDistancesMatrix)
{
}


/**
 * Description: Constructor.
 * @param pRefAtoms: (IN)
 * @param pFitAtoms: (IN)
 * @param pPrecalculationResult: (IN)
 */
CGaussianVolume::CGaussianVolume(
	const std::vector<IAtom*>* pRefAtoms,
	const std::vector<IAtom*>* pFitAtoms,
	const CGaussianVolume::PrecalculationResult* pPrecalculationResult
	) :
	_pFitAtoms(pFitAtoms),
	_pPrecalculationResult(pPrecalculationResult),
	_pRefAtoms(pRefAtoms)
{
	// If not NULL parameters:
	if (pRefAtoms && pFitAtoms && pPrecalculationResult)
	{
		// If valid parameters:
		if (pRefAtoms->size() == pPrecalculationResult->alphaValuesForRef.size() &&
			pRefAtoms->size() == pPrecalculationResult->neighborAtomIdsForRef.size() &&
			pRefAtoms->size() == pPrecalculationResult->squareDistancesMatrixForRef.size() &&
			pFitAtoms->size() == pPrecalculationResult->alphaValuesForFit.size() &&
			pFitAtoms->size() == pPrecalculationResult->neighborAtomIdsForFit.size() &&
			pFitAtoms->size() == pPrecalculationResult->squareDistancesMatrixForFit.size() &&
			pPrecalculationResult->intersectedAtomIdsForRef.size() == pPrecalculationResult->intersectedAtomIdsForFit.size())
		{
			initializeIntermolecularInformation();
		}
		// If invalid parameters:
		else
		{
			std::stringstream msgStream;
			msgStream
				<< LOCATION_STREAM_INSERTION
				<< "Inconsistent parameters, may be corrupted: "
				<< "Content of pPrecalculationResult.";
			throw CInvalidArgumentException(msgStream.str());
		}
	}
	// If NULL parameters:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "Invalid parameters: "
			<< "pRefAtoms, pFitAtoms or pPrecalculationResult = NULL. ";
		throw CInvalidArgumentException(msgStream.str());
	}
}


/**
 * Description: Constructor.
 * @param pRefAtoms: (IN)
 * @param pFitMolecule: (IN)
 * @param pPrecalculationResult: (IN)
 */
CGaussianVolume::CGaussianVolume(
	const std::vector<IAtom*>* pRefAtoms,
	const IMolecule* pFitMolecule,
	const CGaussianVolume::PrecalculationResult* pPrecalculationResult
	)
{
	// If not NULL parameter:
	if (pRefAtoms && pFitMolecule && pPrecalculationResult)
	{
		/* Construct fit atoms. */
		list<IAtom*> fitAtomsList = pFitMolecule->getAtomsList();
		vector<IAtom*>* pFitAtoms = new vector<IAtom*>();
		pFitAtoms->reserve(fitAtomsList.size());
		std::copy(fitAtomsList.begin(), fitAtomsList.end(), std::back_inserter(*pFitAtoms));

		try
		{
			new(this) CGaussianVolume(pRefAtoms, pFitAtoms, pPrecalculationResult);
			_fitAtomsPtr.reset(pFitAtoms);
		}
		catch(CInvalidArgumentException& exception)
		{
			std::stringstream msgStream;
			msgStream
				<< LOCATION_STREAM_INSERTION
				<< "Invalid parameters! "
				<< "Caused by: "
				<< exception.getErrorMessage();
			throw CInvalidArgumentException(msgStream.str());
		}
	}
	// If NULL parameter:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "Invalid parameters: "
			<< "pRefAtoms, pFitMolecule or pPrecalculationResult = NULL. ";
		throw CInvalidArgumentException(msgStream.str());
	}
}


/**
 * Description: Dtor.
 */
CGaussianVolume::~CGaussianVolume()
{
}


/**
 * Description:
 * @param refAtomIdsSet: (IN)
 * @param fitAtomIdsSet: (IN)
 * @return:
 */
double CGaussianVolume::calculateAtomIntersectionVolume(
	const std::set<int>& refAtomIdsSet,
	const std::set<int>& fitAtomIdsSet
	) const
{
	/* Calculate delta value. */
	double dDelta = 0;
	FOREACH(iterAtomId, refAtomIdsSet, set<int>::const_iterator)
	{
		dDelta += _pPrecalculationResult->alphaValuesForRef[*iterAtomId];
	}
	FOREACH(iterAtomId, fitAtomIdsSet, set<int>::const_iterator)
	{
		dDelta += _pPrecalculationResult->alphaValuesForFit[*iterAtomId];
	}

	/* Calculate K value. */
	double dK = 0;
	FOREACH(iterRefAtomId, refAtomIdsSet, set<int>::const_iterator)
	{
		set<int>::const_iterator iterRefAtomIdBehind = iterRefAtomId;
		for (++ iterRefAtomIdBehind; iterRefAtomIdBehind != refAtomIdsSet.end(); ++ iterRefAtomIdBehind)
		{
			dK +=
				_pPrecalculationResult->alphaValuesForRef[*iterRefAtomId] *
				_pPrecalculationResult->alphaValuesForRef[*iterRefAtomIdBehind] *
				_pPrecalculationResult->squareDistancesMatrixForRef[*iterRefAtomId][*iterRefAtomIdBehind];
		}
	}
	FOREACH(iterFitAtomId, fitAtomIdsSet, set<int>::const_iterator)
	{
		set<int>::const_iterator iterFitAtomIdBehind = iterFitAtomId;
		for (++ iterFitAtomIdBehind; iterFitAtomIdBehind != fitAtomIdsSet.end(); ++ iterFitAtomIdBehind)
		{
			dK +=
				_pPrecalculationResult->alphaValuesForFit[*iterFitAtomId] *
				_pPrecalculationResult->alphaValuesForFit[*iterFitAtomIdBehind] *
				_pPrecalculationResult->squareDistancesMatrixForFit[*iterFitAtomId][*iterFitAtomIdBehind];
		}
	}
	FOREACH(iterRefAtomId, refAtomIdsSet, set<int>::const_iterator)
	{
		FOREACH(iterFitAtomId, fitAtomIdsSet, set<int>::const_iterator)
		{
			dK +=
				_pPrecalculationResult->alphaValuesForRef[*iterRefAtomId] *
				_pPrecalculationResult->alphaValuesForFit[*iterFitAtomId] *
				_squareDistancesMatrix[*iterRefAtomId][*iterFitAtomId];
		}
	}
	dK = exp(- (dK / dDelta));

	// final intersection volume
	const double dIntersectionVolume =
		pow(_dP, refAtomIdsSet.size() + fitAtomIdsSet.size()) *
		dK *
		pow(_dPI / dDelta, 1.5);

	return dIntersectionVolume;
}


/**
 * Description:
 * @param neighborAtomIds: (IN)
 * @param nIntersectedAtoms: (IN)
 * @param intersectedAtomIds: (OUT)
 */
int CGaussianVolume::enumerateIntersectedAtomIds(
	const std::vector<std::set<int> >& neighborAtomIds,
	const int nIntersectedAtoms,
	std::vector<std::set<int> >& intersectedAtomIds
	)
{
	// If valid parameter:
	if (nIntersectedAtoms >= 0)
	{
		// If special case: Do not enumerate.
		if (nIntersectedAtoms == 0)
		{
			return ErrorCodes::nNORMAL;
		}
		// If special case: Only need to get the key atom IDs.
		else if (nIntersectedAtoms == 1)
		{
			// reduce reallocation for performance
			intersectedAtomIds.reserve(neighborAtomIds.size());

			for (int iKeyId = 0; iKeyId < static_cast<int>(neighborAtomIds.size()); ++ iKeyId)
			{
				/* Add single ID set. */
				intersectedAtomIds.push_back(set<int>());
				intersectedAtomIds.back().insert(iKeyId);
			}
		}
		// If special case: Get single neighbor for each key atom ID.
		else if (nIntersectedAtoms == 2)
		{
			// reduce reallocation for performance
			intersectedAtomIds.reserve(4 * neighborAtomIds.size());

			for (int iKeyId = 0; iKeyId < static_cast<int>(neighborAtomIds.size()); ++ iKeyId)
			{
				/* Pair with each neighbor. */
				const set<int>& currentNeighborIdsSet = neighborAtomIds[iKeyId];
				FOREACH(iterNeighborId, currentNeighborIdsSet, set<int>::const_iterator)
				{
					/* Add paired IDs set. */
					intersectedAtomIds.push_back(set<int>());
					intersectedAtomIds.back().insert(iKeyId);
					intersectedAtomIds.back().insert(*iterNeighborId);
				}
			}
		}
		// If general case (to select more than 2 neighbors):
		else
		{
			// reduce reallocation for performance
			intersectedAtomIds.reserve(4 * neighborAtomIds.size());

			// to filter out pair-wise intersected atoms
			CIntersectedAtomsFilter intersectedAtomsFilter(&neighborAtomIds);

			/* Pick nIntersectedAtoms - 1 atom IDs for each key atom ID. */
			for (int iKeyId = 0; iKeyId < static_cast<int>(neighborAtomIds.size()); ++ iKeyId)
			{
				// neighbors for this key atom
				const set<int>& currentNeighborIdsSet = neighborAtomIds[iKeyId];

				// intersected atom IDs
				vector<set<int> > pickedIds;
				pickedIds.reserve(10);
				combineElements(
					currentNeighborIdsSet.begin(),
					currentNeighborIdsSet.end(),
					currentNeighborIdsSet.size(),
					nIntersectedAtoms - 1,
					intersectedAtomsFilter,
					set<int>(),
					pickedIds
					);

				/* Add key atom ID to IDs set. */
				FOREACH(iterIdsSet, pickedIds, vector<set<int> >::iterator)
				{
					iterIdsSet->insert(iKeyId);
				}

				// Store this result.
				std::copy(pickedIds.begin(), pickedIds.end(), std::back_inserter(intersectedAtomIds));
			}
		}
	}
	// If invalid parameter:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "Invalid parameters: "
			<< "nIntersectedAtoms = " << nIntersectedAtoms;
		throw CInvalidArgumentException(msgStream.str());
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description:
 * @return:
 */
double CGaussianVolume::getOverlapVolume() const
{
	// max order of intersection volume for single molecule
	const int nMAX_INTERSECTION_ORDER = _pPrecalculationResult->intersectedAtomIdsForRef.size();

	/* Calculate cross intersected volume. */
	double dOverlapVolume = 0;
	// For each order of reference molecule:
	for (int iOrderRef = 0; iOrderRef < nMAX_INTERSECTION_ORDER; ++ iOrderRef)
	{
		// For each order of fit molecule:
		for (int iOrderFit = 0; iOrderFit < nMAX_INTERSECTION_ORDER; ++ iOrderFit)
		{
			// sign for the cross product term
			const int nSign = (iOrderRef + 1 + iOrderFit + 1) % 2 == 0 ? 1 : -1;

			// intersected atom IDs for reference molecule at current order
			const vector<set<int> >& intersectedIdsCurrRef = _pPrecalculationResult->intersectedAtomIdsForRef[iOrderRef];
			// intersected atom IDs for fit molecule at current order
			const vector<set<int> >& intersectedIdsCurrFit = _pPrecalculationResult->intersectedAtomIdsForFit[iOrderFit];
			// For each term of reference molecule:
			FOREACH(iterIdsSetRef, intersectedIdsCurrRef, vector<set<int> >::const_iterator)
			{
				// For each term of fit molecule:
				FOREACH(iterIdsSetFit, intersectedIdsCurrFit, vector<set<int> >::const_iterator)
				{
					// If cross intersected:
					if (isIntersectedAtomsByCrossNeighbors(*iterIdsSetRef, *iterIdsSetFit, _neighborAtomIds))
					{
						dOverlapVolume += nSign * calculateAtomIntersectionVolume(*iterIdsSetRef, *iterIdsSetFit);
					}
				}
			}
		}
	}

	return dOverlapVolume;
}


/**
 * Description: Calculate the overlap volume of two molecules.
 * @param refMol: (IN) Reference molecule, usually the query molecule.
 * @param fitMol: (IN) Fitting molecule, usually the target molecule in database.
 * @return: Overlap volume scalar.
 */
double CGaussianVolume::getOverlapVolume(const IMolecule& refMol, const IMolecule& fitMol) const
{
	const list<IAtom*> refAtomsList = refMol.getAtomsList();
	const list<IAtom*> fitAtomsList = fitMol.getAtomsList();
	double dOverlap = 0.0;

	// For each atom in reference molecule:
	FOREACH(iterRefAtom, refAtomsList, list<IAtom*>::const_iterator)
	{
		const IAtom& refAtom = **iterRefAtom;

		const vector<double>& refAtomPosition = refAtom.getPosition();
		const double dRadiusRefAtom = refAtom.getAtomRadius();
		const double dAlphaRefAtom = _dPARTIAL_ALPHA / (dRadiusRefAtom * dRadiusRefAtom);

		// For each atom in fit molecule:
		FOREACH(iterFitAtom, fitAtomsList, list<IAtom*>::const_iterator)
		{
			const IAtom& fitAtom = **iterFitAtom;

			const vector<double>& fitAtomPosition = fitAtom.getPosition();
			const double dRadiusFitAtom = fitAtom.getAtomRadius();

			const double dR2 = CMathematics::pointToPointSquareDistance(refAtomPosition, fitAtomPosition);
			if (dR2 < (dRadiusRefAtom + dRadiusFitAtom + _dGaussianCutoff) * (dRadiusRefAtom + dRadiusFitAtom + _dGaussianCutoff))
			{
				const double dAlphaFitAtom = _dPARTIAL_ALPHA / (dRadiusFitAtom * dRadiusFitAtom);

				const double dK = exp(-(dAlphaRefAtom * dAlphaFitAtom * dR2) / (dAlphaRefAtom + dAlphaFitAtom));
				const double dV = 8 * dK * pow(_dPI / (dAlphaRefAtom + dAlphaFitAtom), 1.5);
				dOverlap += dV;
			}
		}
	}

	return dOverlap;
}


/**
 * Description:
 * @return:
 */
double CGaussianVolume::getReferenceVolume() const
{
	// max order of intersection volume for single molecule
	const int nMAX_INTERSECTION_ORDER = _pPrecalculationResult->intersectedAtomIdsForRef.size();

	/* Calculate cross intersected volume. */
	double dOverlapVolume = 0;
	// For each order of reference molecule:
	for (int iOrderOuter = 0; iOrderOuter < nMAX_INTERSECTION_ORDER; ++ iOrderOuter)
	{
		// For each order of reference molecule:
		for (int iOrderInner = 0; iOrderInner < nMAX_INTERSECTION_ORDER; ++ iOrderInner)
		{
			// sign for the cross product term
			const int nSign = (iOrderOuter + 1 + iOrderInner + 1) % 2 == 0 ? 1 : -1;

			// intersected atom IDs for reference molecule at current order
			const vector<set<int> >& intersectedIdsCurrOuter = _pPrecalculationResult->intersectedAtomIdsForRef[iOrderOuter];
			// intersected atom IDs for reference molecule at current order
			const vector<set<int> >& intersectedIdsCurrInner = _pPrecalculationResult->intersectedAtomIdsForRef[iOrderInner];
			// For each term of reference molecule:
			FOREACH(iterIdsSetOuter, intersectedIdsCurrOuter, vector<set<int> >::const_iterator)
			{
				// For each term of reference molecule:
				FOREACH(iterIdsSetInner, intersectedIdsCurrInner, vector<set<int> >::const_iterator)
				{
					// If cross intersected:
					if (isIntersectedAtomsByMonotoneNeighbors(*iterIdsSetOuter, *iterIdsSetInner, _pPrecalculationResult->neighborAtomIdsForRef))
					{
						dOverlapVolume += nSign * calculateAtomIntersectionVolume(*iterIdsSetOuter, *iterIdsSetInner);
					}
				}
			}
		}
	}

	return dOverlapVolume;
}


/**
 * Description: Get Gaussian cutoff value.
 */
double CGaussianVolume::getGaussianCutoff() const
{
	return _dGaussianCutoff;
}


/**
 * Description:
 * @param idsSetAsKeys: (IN)
 * @param idsSetAsValues: (IN)
 * @param neighborAtomIds: (IN)
 * @return:
 */
bool CGaussianVolume::isIntersectedAtomsByCrossNeighbors(
	const std::set<int>& idsSetAsKeys,
	const std::set<int>& idsSetAsValues,
	const std::vector<std::set<int> >& neighborAtomIds
	)
{
	FOREACH(iterKeyId, idsSetAsKeys, set<int>::const_iterator)
	{
		FOREACH(iterValueId, idsSetAsValues, set<int>::const_iterator)
		{
			// If current value atom is not a neighbor of current key atom:
			if (!EXIST(*iterValueId, neighborAtomIds[*iterKeyId]))
			{
				return false;
			}
		}
	}

	return true;
}


/**
 * Description:
 * @param idsSet: (IN)
 * @param neighborAtomIds: (IN)
 * @return:
 */
bool CGaussianVolume::isIntersectedAtomsByMonotoneNeighbors(
	const std::set<int>& idsSet,
	const std::vector<std::set<int> >& neighborAtomIds
	)
{
	FOREACH(iterId, idsSet, set<int>::const_iterator)
	{
		/* Validate with all IDs behind current one. */
		set<int>::const_iterator iterIdBehind = iterId;
		for (++ iterIdBehind; iterIdBehind != idsSet.end(); ++ iterIdBehind)
		{
			const set<int>& neighborIdsSet = neighborAtomIds[*iterId];
			// If not a neighbor of current ID:
			if (!EXIST(*iterIdBehind, neighborIdsSet))
			{
				return false;
			}
		}
	}

	return true;
}


/**
 * Description:
 * @param idsSetAsKeys: (IN)
 * @param idsSetAsValues: (IN)
 * @param neighborAtomIds: (IN)
 * @return:
 */
bool CGaussianVolume::isIntersectedAtomsByMonotoneNeighbors(
	const std::set<int>& idsSetAsKeys,
	const std::set<int>& idsSetAsValues,
	const std::vector<std::set<int> >& neighborAtomIds
	)
{
	FOREACH(iterKeyId, idsSetAsKeys, set<int>::const_iterator)
	{
		FOREACH(iterValueId, idsSetAsValues, set<int>::const_iterator)
		{
			// If key ID should be used as a key to determine neighbor relation:
			if (*iterKeyId < *iterValueId)
			{
				// If current value atom is not a neighbor of current key atom:
				if (!EXIST(*iterValueId, neighborAtomIds[*iterKeyId]))
				{
					return false;
				}
			}
			// If value ID should be used as a key to determine neighbor relation:
			else if (*iterKeyId > *iterValueId)
			{
				// If current key atom is not a neighbor of current value atom:
				if (!EXIST(*iterKeyId, neighborAtomIds[*iterValueId]))
				{
					return false;
				}
			}
			// If key ID is value ID itself:
			else
			{
				continue;
			}
		}
	}

	return true;
}


/**
 * Description:
 */
int CGaussianVolume::initializeIntermolecularInformation()
{
	// If not NULL member variable:
	if (_pRefAtoms && _pFitAtoms && _pPrecalculationResult)
	{
		_squareDistancesMatrix.clear();
		_squareDistancesMatrix.reserve(_pRefAtoms->size());
		_neighborAtomIds.clear();
		_neighborAtomIds.reserve(_pRefAtoms->size());

		const double dGAUSSIAN_CUTOFF = _pPrecalculationResult->dGaussianCutoff;

		FOREACH(iterRefAtom, *_pRefAtoms, vector<IAtom*>::const_iterator)
		{
			/* Preallocate an empty row in square distances matrix for performance reasons. */
			_squareDistancesMatrix.push_back(vector<double>());
			// current row in square distances matrix for this reference atom
			vector<double>& squareDistancesRow = _squareDistancesMatrix.back();
			squareDistancesRow.reserve(_pFitAtoms->size());

			/* Preallocate neighbor IDs container for current reference atom. */
			_neighborAtomIds.push_back(set<int>());
			set<int>& neighborAtomIdsSet = _neighborAtomIds.back();

			const IAtom& refAtom = **iterRefAtom;
			int iFitAtomId = 0;
			FOREACH(iterFitAtom, *_pFitAtoms, vector<IAtom*>::const_iterator)
			{
				const IAtom& fitAtom = **iterFitAtom;

				/* Deal with distance information. */
				const double dSquareDistance = CMathematics::pointToPointSquareDistance(refAtom.getPosition(), fitAtom.getPosition());
				squareDistancesRow.push_back(dSquareDistance);

				/* Deal with neighbor information. */
				// If a neighbor:
				if (dSquareDistance < pow(refAtom.getAtomRadius() + fitAtom.getAtomRadius() + dGAUSSIAN_CUTOFF, 2))
				{
					neighborAtomIdsSet.insert(iFitAtomId);
				}

				++ iFitAtomId;
			}
		}
	}
	// If NULL member variable:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "NULL member variable! "
			<< "Member variable: _pRefAtoms, _pFitAtoms, _pPrecalculationResult. ";
		throw CInvalidArgumentException(msgStream.str());
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description:
 * @param refMol: (IN)
 * @param fitMol: (IN)
 * @param dGaussianCutoff: (IN)
 * @param nMaxIntersectionOrder: (IN)
 * @param precalculationResult: (OUT)
 */
int CGaussianVolume::precalculate(
	const IMolecule& refMol,
	const IMolecule& fitMol,
	const double dGaussianCutoff,
	const int nMaxIntersectionOrder,
	PrecalculationResult& precalculationResult
	)
{
	// If valid parameter:
	if (dGaussianCutoff >= 0 && nMaxIntersectionOrder > 0)
	{
		// Precalculate reference molecule.
		precalculateMolecule(
			refMol,
			dGaussianCutoff,
			nMaxIntersectionOrder,
			precalculationResult.alphaValuesForRef,
			precalculationResult.squareDistancesMatrixForRef,
			precalculationResult.neighborAtomIdsForRef,
			precalculationResult.intersectedAtomIdsForRef
			);

		// Precalculate fit molecule.
		precalculateMolecule(
			fitMol,
			dGaussianCutoff,
			nMaxIntersectionOrder,
			precalculationResult.alphaValuesForFit,
			precalculationResult.squareDistancesMatrixForFit,
			precalculationResult.neighborAtomIdsForFit,
			precalculationResult.intersectedAtomIdsForFit
			);

		precalculationResult.dGaussianCutoff = dGaussianCutoff;
	}
	// If invalid parameter:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "Invalid parameters: "
			<< "dGaussianCutoff = " << dGaussianCutoff
			<< "nMaxIntersectionOrder = " << nMaxIntersectionOrder;
		throw CInvalidArgumentException(msgStream.str());
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description:
 * @param molecule: (IN)
 * @param dGaussianCutoff: (IN)
 * @param nMaxIntersectionOrder: (IN)
 * @param alphaValues: (OUT)
 * @param squareDistancesMatrix: (OUT)
 * @param neighborAtomIds: (OUT)
 * @param intersectedAtomIds: (OUT)
 */
int CGaussianVolume::precalculateMolecule(
	const IMolecule& molecule,
	const double dGaussianCutoff,
	const int nMaxIntersectionOrder,
	std::vector<double>& alphaValues,
	std::vector<std::vector<double> >& squareDistancesMatrix,
	std::vector<std::set<int> >& neighborAtomIds,
	std::vector<std::vector<std::set<int> > >& intersectedAtomIds
	)
{
	// If valid parameter:
	if (dGaussianCutoff >= 0 && nMaxIntersectionOrder > 0)
	{
		/* Do preallocation for performance reasons. */
		const int nATOMS_COUNT = molecule.getAtomsCount();
		alphaValues.clear();
		alphaValues.reserve(nATOMS_COUNT);
		squareDistancesMatrix.clear();
		squareDistancesMatrix.reserve(nATOMS_COUNT);
		neighborAtomIds.clear();
		neighborAtomIds.reserve(nATOMS_COUNT);
		intersectedAtomIds.clear();
		intersectedAtomIds.reserve(nMaxIntersectionOrder);

		const list<IAtom*> atomsList = molecule.getAtomsList();
		int iOuterAtomId = 0;
		FOREACH(iterAtomOuter, atomsList, list<IAtom*>::const_iterator)
		{
			const IAtom& atomOuter = **iterAtomOuter;

			/* Construct square distances container for current atom. */
			squareDistancesMatrix.push_back(vector<double>());
			vector<double>& squareDistancesRow = squareDistancesMatrix.back();
			squareDistancesRow.reserve(nATOMS_COUNT);

			/* Construct neighbor IDs container for current atom. */
			neighborAtomIds.push_back(set<int>());
			set<int>& currentNeighborIdsSet = neighborAtomIds.back();

			/* Calculate distance and neighbors. */
			// atom coordinate
			const vector<double>& outerAtomPosition = atomOuter.getPosition();
			int iInnerAtomId = 0;
			FOREACH(iterAtomInner, atomsList, list<IAtom*>::const_iterator)
			{
				const IAtom& atomInner = **iterAtomInner;
				// atom coordinate
				const vector<double>& innerAtomPosition = atomInner.getPosition();
				const double dSquareDistance = iInnerAtomId >= iOuterAtomId ?
					CMathematics::pointToPointSquareDistance(outerAtomPosition, innerAtomPosition) : squareDistancesMatrix[iOuterAtomId][iInnerAtomId];

				/* Record square distances. */
				squareDistancesRow.push_back(dSquareDistance);

				/* Record neighbor atom ID. */
				if (iInnerAtomId > iOuterAtomId && dSquareDistance < pow(atomOuter.getAtomRadius() + atomInner.getAtomRadius() + dGaussianCutoff, 2))
				{
					currentNeighborIdsSet.insert(iInnerAtomId);
				}

				++ iInnerAtomId;
			}

			/* Calculate alpha values. */
			const double dAlpha = _dPARTIAL_ALPHA / pow(atomOuter.getAtomRadius(), 2);
			alphaValues.push_back(dAlpha);

			++ iOuterAtomId;
		}

		/* Precalculate intersected atom IDs for molecule. */
		// For each order:
		for (int iOrder = 1; iOrder <= nMaxIntersectionOrder; ++ iOrder)
		{
			intersectedAtomIds.push_back(vector<set<int> >());
			vector<set<int> >& intersectedAtomIdsForCurrentOrder = intersectedAtomIds.back();
			enumerateIntersectedAtomIds(
				neighborAtomIds,
				iOrder,
				intersectedAtomIdsForCurrentOrder
				);
		}
	}
	// If invalid parameter:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "Invalid parameters: "
			<< "dGaussianCutoff = " << dGaussianCutoff
			<< "nMaxIntersectionOrder = " << nMaxIntersectionOrder;
		throw CInvalidArgumentException(msgStream.str());
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description: Set Gaussian cutoff value.
 * @param dCutoff: (IN) A non negative value representing Caussian cutoff value.
 */
void CGaussianVolume::setGaussianCutoff(double dCutoff)
{
	if (dCutoff >= 0)
	{
		_dGaussianCutoff = dCutoff;
	}
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "Invalid parameter! "
			<< "Detail: dCutoff = " << dCutoff;
		throw CInvalidArgumentException(msgStream.str());
	}
}


/* Implementation for CGaussianVolumeBuilder class: */

/* static members: */
const double CGaussianVolumeBuilder::DefaultValues::dGAUSSIAN_CUTOFF = 0;
const int CGaussianVolumeBuilder::DefaultValues::nMAX_INTERSECTION_ORDER = 1;

const int CGaussianVolumeBuilder::ErrorCodes::nNORMAL = 0;


/**
 * Description: Constructor.
 * @param pRefMolecule: (IN)
 * @param pFitMolecule: (IN)
 */
CGaussianVolumeBuilder::CGaussianVolumeBuilder(const IMolecule* pRefMolecule, const IMolecule* pFitMolecule) :
	_bInitForPrecalculationResult(false),
	_dGaussianCutoff(DefaultValues::dGAUSSIAN_CUTOFF),
	_nMaxIntersectionOrder(DefaultValues::nMAX_INTERSECTION_ORDER),
	_pFitMolecule(pFitMolecule),
	_pRefMolecule(pRefMolecule)
{
	// If NULL parameter:
	if (pRefMolecule && pFitMolecule)
	{
		/* Extract atoms of reference molecule. */
		list<IAtom*> refAtomsList = pRefMolecule->getAtomsList();
		_refAtoms.reserve(refAtomsList.size());
		std::copy(refAtomsList.begin(), refAtomsList.end(), std::back_inserter(_refAtoms));

		/* Extract atoms of fit molecule. */
		list<IAtom*> fitAtomsList = pFitMolecule->getAtomsList();
		_fitAtoms.reserve(fitAtomsList.size());
		std::copy(fitAtomsList.begin(), fitAtomsList.end(), std::back_inserter(_fitAtoms));
	}
	// If not NULL parameter:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "Invalid parameter: "
			<< "pRefMolecule = NULL or pFitMolecule = NULL. ";
		throw CInvalidArgumentException(msgStream.str());
	}
}


/**
 * Description: Destructor.
 */
CGaussianVolumeBuilder::~CGaussianVolumeBuilder()
{
}


/**
 * Description:
 */
int CGaussianVolumeBuilder::attemptInitialize()
{
	/* Attempt to perform precalculation. */
	// If precalculation has not been done:
	if (!_bInitForPrecalculationResult)
	{
		CGaussianVolume::precalculate(
			*_pRefMolecule,
			*_pFitMolecule,
			getGaussianCutoff(),
			getMaxIntersectionOrder(),
			_precalculationResult
			);

		_bInitForPrecalculationResult = true;
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description:
 * @param fitMolecule: (IN)
 * @return:
 */
CGaussianVolume CGaussianVolumeBuilder::build(const IMolecule* pFitMolecule)
{
	// If valid parameter:
	if (pFitMolecule)
	{
		attemptInitialize();

		return CGaussianVolume(&_refAtoms, pFitMolecule, &_precalculationResult);
	}
	// If invalid parameter:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "Invalid parameter: "
			<< "pFitMolecule = NULL";
		throw CInvalidArgumentException(msgStream.str());
	}
}


/**
 * Description:
 * @return:
 */
CGaussianVolume CGaussianVolumeBuilder::build()
{
	attemptInitialize();

	return CGaussianVolume(&_refAtoms, &_fitAtoms, &_precalculationResult);
}


/**
 * Description:
 * @return:
 */
double CGaussianVolumeBuilder::getGaussianCutoff() const
{
	return _dGaussianCutoff;
}


/**
 * Description:
 * @return:
 */
int CGaussianVolumeBuilder::getMaxIntersectionOrder() const
{
	return _nMaxIntersectionOrder;
}


/**
 * Description:
 * @param dCutoff: (IN)
 */
void CGaussianVolumeBuilder::setGaussianCutoff(const double dCutoff)
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
 * @param nOrder: (IN)
 */
void CGaussianVolumeBuilder::setMaxIntersectionOrder(const int nOrder)
{
	if (nOrder > 0)
	{
		_nMaxIntersectionOrder = nOrder;
	}
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "Invalid parameter: "
			<< "nOrder = " << nOrder;
		throw CInvalidArgumentException(msgStream.str());
	}
}
