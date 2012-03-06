/**
 * Gaussian Volume Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file GaussianVolume.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-09-27
 */


#ifndef GAUSSIAN_VOLUME_INCLUDE_H
#define GAUSSIAN_VOLUME_INCLUDE_H
//


#include <list>
#include <memory>
#include <set>
#include <string>
#include <vector>


class IAtom;
class IMolecule;


/**
 * Description: Auxiliary class for calculating Gaussian volume overlap of two molecules.
 */
class CGaussianVolume
{
	/* data: */
public:
	/* Error codes. */
	struct ErrorCodes
	{
		static const int nNORMAL;

	private:
		ErrorCodes() {};
	};


	/**
	 * Description:
	 */
	struct PrecalculationResult
	{
		// alpha values for fit molecule
		std::vector<double> alphaValuesForFit;
		// alpha values for reference molecule
		std::vector<double> alphaValuesForRef;
		// specify the Gaussian cutoff value used to calculate this result
		double dGaussianCutoff;
		// IDs set for pair-wise intersected atoms of fit molecule at each order
		std::vector<std::vector<std::set<int> > > intersectedAtomIdsForFit;
		// IDs set for pair-wise intersected atoms of reference molecule at each order
		std::vector<std::vector<std::set<int> > > intersectedAtomIdsForRef;
		// square distances matrix for fit molecule
		std::vector<std::vector<double> > squareDistancesMatrixForFit;
		// square distances matrix for reference molecule
		std::vector<std::vector<double> > squareDistancesMatrixForRef;
		// neighbor atom IDs list (monotone) for fit molecule
		std::vector<std::set<int> > neighborAtomIdsForFit;
		// neighbor atom IDs list (monotone) for reference molecule
		std::vector<std::set<int> > neighborAtomIdsForRef;
	};


private:
	/**
	 * Description:
	 */
	class CIntersectedAtomsFilter
	{
		/* data: */
	public:
		CIntersectedAtomsFilter(const std::vector<std::set<int> >* pNeighborAtomIds);
	private:
		// neighbor atom IDs list
		const std::vector<std::set<int> >* _pNeighborAtomIds;

		/* method: */
	public:
		inline bool operator()(const std::set<int>& atomIdsSet) const;
	private:
	};


	// p constant
	static const double _dP;
	// partial alpha constant
	static const double _dPARTIAL_ALPHA;
	// PI constant
	static const double _dPI;

	// Gaussian cutoff value
	double _dGaussianCutoff;
	// auto storage for fit atoms, could be NULL pointer
	mutable std::auto_ptr<std::vector<IAtom*> > _fitAtomsPtr;
	// neighbor atom IDs (between reference and fit molecule, cross neighbors), index: reference atom ID, value: fit atom IDs
	std::vector<std::set<int> > _neighborAtomIds;
	// atoms for fit molecule
	const std::vector<IAtom*>* _pFitAtoms;
	// precalculation result
	const PrecalculationResult* _pPrecalculationResult;
	// atoms for reference molecule
	const std::vector<IAtom*>* _pRefAtoms;
	// square distances between reference and fit atoms, row index: reference atom ID, column index: fit atom ID
	std::vector<std::vector<double> > _squareDistancesMatrix;

	/* method: */
public:
	CGaussianVolume();
	CGaussianVolume(const CGaussianVolume& volume);
	CGaussianVolume(const std::vector<IAtom*>* pRefAtoms, const std::vector<IAtom*>* pFitAtoms, const CGaussianVolume::PrecalculationResult* pPrecalculationResult);
	CGaussianVolume(const std::vector<IAtom*>* pRefAtoms, const IMolecule* pFitMolecule, const CGaussianVolume::PrecalculationResult* pPrecalculationResult);
	~CGaussianVolume();

	static int precalculate(const IMolecule& refMol, const IMolecule& fitMol, const double dGaussianCutoff, const int nMaxIntersectionOrder, PrecalculationResult& precalculationResult);
	double getOverlapVolume(const IMolecule& refMol, const IMolecule& fitMol) const;
	double getOverlapVolume() const;
	double getReferenceVolume() const;
	double getGaussianCutoff() const;
	void setGaussianCutoff(double dCutoff);
private:
	inline double calculateAtomIntersectionVolume(const std::set<int>& refAtomIdsSet, const std::set<int>& fitAtomIdsSet) const;
	template <typename TFilter>
	static int combineElements(const std::set<int>::const_iterator iterCurrent, const std::set<int>::const_iterator& iterEnd, const int nElementsLeft, const int nElementsToSelect, const TFilter& filter, const std::set<int>& currentCombination, std::vector<std::set<int> >& resultCombinations);
	static int enumerateIntersectedAtomIds(const std::vector<std::set<int> >& neighborAtomIds, const int nIntersectedAtoms, std::vector<std::set<int> >& intersectedAtomIds);
	inline static bool isIntersectedAtomsByCrossNeighbors(const std::set<int>& idsSetAsKeys, const std::set<int>& idsSetAsValues, const std::vector<std::set<int> >& neighborAtomIds);
	inline static bool isIntersectedAtomsByMonotoneNeighbors(const std::set<int>& idsSet, const std::vector<std::set<int> >& neighborAtomIds);
	inline static bool isIntersectedAtomsByMonotoneNeighbors(const std::set<int>& idsSetAsKeys, const std::set<int>& idsSetAsValues, const std::vector<std::set<int> >& neighborAtomIds);
	int initializeIntermolecularInformation();
	static int precalculateMolecule(const IMolecule& molecule, const double dGaussianCutoff, const int nMaxIntersectionOrder, std::vector<double>& alphaValues, std::vector<std::vector<double> >& squareDistancesMatrix, std::vector<std::set<int> >& neighborAtomIds, std::vector<std::vector<std::set<int> > >& intersectedAtomIds);
};


/**
 * Description:
 */
class CGaussianVolumeBuilder
{
	/* data: */
public:
	/* Error codes. */
	struct ErrorCodes
	{
		static const int nNORMAL;

	private:
		ErrorCodes() {};
	};


private:
	/* Default values. */
	struct DefaultValues
	{
		static const double dGAUSSIAN_CUTOFF;
		static const int nMAX_INTERSECTION_ORDER;

	private:
		DefaultValues() {};
	};


	// a flag indicating whether precalculation has been done
	bool _bInitForPrecalculationResult;
	// Gaussian cutoff applied to calculate the Gaussian volume
	double _dGaussianCutoff;
	// atoms for fit molecule
	std::vector<IAtom*> _fitAtoms;
	// max intersection order to expand when calculating Gaussian volume
	int _nMaxIntersectionOrder;
	// fit molecule
	const IMolecule* _pFitMolecule;
	// reference molecule
	const IMolecule* _pRefMolecule;
	// precalculation result
	CGaussianVolume::PrecalculationResult _precalculationResult;
	// atoms for reference molecule
	std::vector<IAtom*> _refAtoms;

	/* method: */
public:
	CGaussianVolumeBuilder(const IMolecule* pRefMolecule, const IMolecule* pFitMolecule);
	~CGaussianVolumeBuilder();

	inline CGaussianVolume build();
	inline CGaussianVolume build(const IMolecule* pFitMolecule);
	double getGaussianCutoff() const;
	int getMaxIntersectionOrder() const;
	void setGaussianCutoff(const double dCutoff);
	void setMaxIntersectionOrder(const int nOrder);
private:
	inline int attemptInitialize();
};


/* Template implementation for CGaussianVolume class: */

/**
 * Description:
 * @template TFilter:
 */
template <typename TFilter>
int CGaussianVolume::combineElements(
	const std::set<int>::const_iterator iterCurrent,
	const std::set<int>::const_iterator& iterEnd,
	const int nElementsLeft,
	const int nElementsToSelect,
	const TFilter& filter,
	const std::set<int>& currentCombination,
	std::vector<std::set<int> >& resultCombinations
	)
{
	// If not enough elements to select
	if (nElementsLeft < nElementsToSelect)
	{
		return 1;
	}
	// If enough elements to select
	else
	{
		// If selection done:
		if (nElementsToSelect == 0)
		{
			/* Add current combination to result set. */
			if (filter(currentCombination))
			{
				resultCombinations.push_back(currentCombination);
				return 0;
			}
		}
		// If need to select more:
		else
		{
			for (std::set<int>::const_iterator iterStart = iterCurrent; iterStart != iterEnd;)
			{
				/* Select current element and search more. */
				std::set<int> combinationCopy = currentCombination;
				combinationCopy.insert(*iterStart);

				combineElements(
					++ iterStart,
					iterEnd,
					nElementsLeft - 1,
					nElementsToSelect -1,
					filter,
					combinationCopy,
					resultCombinations
					);
			}
		}
	}

	return 1;
}


//
#endif
