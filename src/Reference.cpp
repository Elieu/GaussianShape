/**
 * Reference Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file Reference.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-09-27
 */


#include "Reference.h"

#include "Exception.h"

#include "sstream"


using std::map;
using std::string;


/* Implementation for CAtomRadiusReference class: */

/* Static member: */
std::map<std::string, double> CAtomRadiusReference::_atomRadiusMap;
const double CAtomRadiusReference::_dDEFAULT_RADIUS = 1.70;
CAtomRadiusReference* CAtomRadiusReference::_pSingleton = NULL;


/**
 * Description: Constructor.
 */
CAtomRadiusReference::CAtomRadiusReference()
{
	setupRadiusMap();
}


/**
 * Description: Destructor.
 */
CAtomRadiusReference::~CAtomRadiusReference()
{
	destroy();
}


/**
 * Description: Copy constructor.
 */
CAtomRadiusReference::CAtomRadiusReference(const CAtomRadiusReference& reference)
{
}


/**
 * Description: Query atom radius.
 * @param sAtomType: (IN) Atom type.
 * @return: Atom radius.
 */
double CAtomRadiusReference::getAtomRadius(const std::string& sAtomType) const
{
	map<string, double>::const_iterator finder = _atomRadiusMap.find(sAtomType);
	// If radius found:
	if (finder != _atomRadiusMap.end())
	{
		return finder->second;
	}
	else
	{
		return _dDEFAULT_RADIUS;
	}
}


/**
 * Description: Get an instance of CAtomRadiusReference class.
 */
CAtomRadiusReference& CAtomRadiusReference::getInstance()
{
	if (!_pSingleton)
	{
		_pSingleton = new CAtomRadiusReference();
	}

	return *_pSingleton;
}


/**
 * Description: Setup the atom radius map.
 */
int CAtomRadiusReference::setupRadiusMap()
{
	_atomRadiusMap["C"] = 1.70f;
	_atomRadiusMap["O"] = 1.52f;
	_atomRadiusMap["N"] = 1.55f;
	_atomRadiusMap["P"] = 1.80f;
	_atomRadiusMap["S"] = 1.80f;
	_atomRadiusMap["Cl"] = 1.75f;
	_atomRadiusMap["Br"] = 1.85f;
	_atomRadiusMap["I"] = 1.98f;
	_atomRadiusMap["F"] = 1.47f;
	_atomRadiusMap["H"] = 1.20f;

	return 0;
}


/**
 * Description: Self destroy, free resources.
 */
int CAtomRadiusReference::destroy()
{
	if (_pSingleton)
	{
		_atomRadiusMap.clear();
		delete _pSingleton;
		_pSingleton = NULL;

		return 0;
	}
	else
	{
		return 1;
	}
}

//*****************************************************************************************

/* Implementation for CAminoAcidNameReference class: */

/**
 * Description: Constructor.
 */
CAminoAcidNameReference::CAminoAcidNameReference()
{
}


/**
 * Description: Copy constructor.
 */
CAminoAcidNameReference::CAminoAcidNameReference(const CAminoAcidNameReference& reference)
{
}


/**
 * Description: Destructor.
 */
CAminoAcidNameReference::~CAminoAcidNameReference()
{
}


//*****************************************************************************************

/* Implementation for CBlosum45MatrixReference class: */

/* Static member: */
const int CBlosum45MatrixReference::_aiBlosumMatrix[25][25] =
	{
		// A  R  N  D  C  Q  E  G  H  I  L  K  M  F  P  S  T  W  Y  V  B  J  Z  X  *
		{  5,-2,-1,-2,-1,-1,-1, 0,-2,-1,-1,-1,-1,-2,-1, 1, 0,-2,-2, 0,-1,-1,-1,-1,-5 },
		{ -2, 7, 0,-1,-3, 1, 0,-2, 0,-3,-2, 3,-1,-2,-2,-1,-1,-2,-1,-2,-1,-3, 1,-1,-5 },
		{ -1, 0, 6, 2,-2, 0, 0, 0, 1,-2,-3, 0,-2,-2,-2, 1, 0,-4,-2,-3, 5,-3, 0,-1,-5 },
		{ -2,-1, 2, 7,-3, 0, 2,-1, 0,-4,-3, 0,-3,-4,-1, 0,-1,-4,-2,-3, 6,-3, 1,-1,-5 },
		{ -1,-3,-2,-3,12,-3,-3,-3,-3,-3,-2,-3,-2,-2,-4,-1,-1,-5,-3,-1,-2,-2,-3,-1,-5 },
		{ -1, 1, 0, 0,-3, 6, 2,-2, 1,-2,-2, 1, 0,-4,-1, 0,-1,-2,-1,-3, 0,-2, 4,-1,-5 },
		{ -1, 0, 0, 2,-3, 2, 6,-2, 0,-3,-2, 1,-2,-3, 0, 0,-1,-3,-2,-3, 1,-3, 5,-1,-5 },
		{  0,-2, 0,-1,-3,-2,-2, 7,-2,-4,-3,-2,-2,-3,-2, 0,-2,-2,-3,-3,-1,-4,-2,-1,-5 },
		{ -2, 0, 1, 0,-3, 1, 0,-2,10,-3,-2,-1, 0,-2,-2,-1,-2,-3, 2,-3, 0,-2, 0,-1,-5 },
		{ -1,-3,-2,-4,-3,-2,-3,-4,-3, 5, 2,-3, 2, 0,-2,-2,-1,-2, 0, 3,-3, 4,-3,-1,-5 },
		{ -1,-2,-3,-3,-2,-2,-2,-3,-2, 2, 5,-3, 2, 1,-3,-3,-1,-2, 0, 1,-3, 4,-2,-1,-5 },
		{ -1, 3, 0, 0,-3, 1, 1,-2,-1,-3,-3, 5,-1,-3,-1,-1,-1,-2,-1,-2, 0,-3, 1,-1,-5 },
		{ -1,-1,-2,-3,-2, 0,-2,-2, 0, 2, 2,-1, 6, 0,-2,-2,-1,-2, 0, 1,-2, 2,-1,-1,-5 },
		{ -2,-2,-2,-4,-2,-4,-3,-3,-2, 0, 1,-3, 0, 8,-3,-2,-1, 1, 3, 0,-3, 1,-3,-1,-5 },
		{ -1,-2,-2,-1,-4,-1, 0,-2,-2,-2,-3,-1,-2,-3, 9,-1,-1,-3,-3,-3,-2,-3,-1,-1,-5 },
		{  1,-1, 1, 0,-1, 0, 0, 0,-1,-2,-3,-1,-2,-2,-1, 4, 2,-4,-2,-1, 0,-2, 0,-1,-5 },
		{  0,-1, 0,-1,-1,-1,-1,-2,-2,-1,-1,-1,-1,-1,-1, 2, 5,-3,-1, 0, 0,-1,-1,-1,-5 },
		{ -2,-2,-4,-4,-5,-2,-3,-2,-3,-2,-2,-2,-2, 1,-3,-4,-3,15, 3,-3,-4,-2,-2,-1,-5 },
		{ -2,-1,-2,-2,-3,-1,-2,-3, 2, 0, 0,-1, 0, 3,-3,-2,-1, 3, 8,-1,-2, 0,-2,-1,-5 },
		{  0,-2,-3,-3,-1,-3,-3,-3,-3, 3, 1,-2, 1, 0,-3,-1, 0,-3,-1, 5,-3, 2,-3,-1,-5 },
		{ -1,-1, 5, 6,-2, 0, 1,-1, 0,-3,-3, 0,-2,-3,-2, 0, 0,-4,-2,-3, 5,-3, 1,-1,-5 },
		{ -1,-3,-3,-3,-2,-2,-3,-4,-2, 4, 4,-3, 2, 1,-3,-2,-1,-2, 0, 2,-3, 4,-2,-1,-5 },
		{ -1, 1, 0, 1,-3, 4, 5,-2, 0,-3,-2, 1,-1,-3,-1, 0,-1,-2,-2,-3, 1,-2, 5,-1,-5 },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-5 },
		{ -5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5, 1 }
	};
std::map<std::string, int> CBlosum45MatrixReference::_aminoAcidNameToIndexMap;
CBlosum45MatrixReference* CBlosum45MatrixReference::_pSingleton = NULL;


/**
 * Description: Constructor.
 */
CBlosum45MatrixReference::CBlosum45MatrixReference()
{
	setupIndexMap();
}


/**
 * Description: Copy constructor.
 */
CBlosum45MatrixReference::CBlosum45MatrixReference(const CBlosum45MatrixReference& referenc)
{
}


/**
 * Description: Destructor.
 */
CBlosum45MatrixReference::~CBlosum45MatrixReference()
{
	destory();
}


/**
 * Description:
 */
void CBlosum45MatrixReference::destory()
{
	_aminoAcidNameToIndexMap.clear();

	delete _pSingleton;
	_pSingleton = NULL;
}


/**
 * Description:
 * @param sAminoAcidNameA: (IN)
 * @param sAminoAcidNameB: (IN)
 * @return:
 * @exception:
 * 	CElementNotFoundException:
 */
int CBlosum45MatrixReference::getMatchScore(const std::string& sAminoAcidNameA, const std::string& sAminoAcidNameB) const
{
	map<string, int>::const_iterator iterFindAminoAcid = _aminoAcidNameToIndexMap.find(sAminoAcidNameA);
	// If amino acid A found:
	if (iterFindAminoAcid != _aminoAcidNameToIndexMap.end())
	{
		// index for amino acid A
		const int& iIndexA = iterFindAminoAcid->second;

		iterFindAminoAcid = _aminoAcidNameToIndexMap.find(sAminoAcidNameB);
		// If amino acid B found:
		if (iterFindAminoAcid != _aminoAcidNameToIndexMap.end())
		{
			// index for amino acid B
			const int& iIndexB = iterFindAminoAcid->second;

			return _aiBlosumMatrix[iIndexA][iIndexB];
		}
		// If amino acid B not found:
		else
		{
			std::stringstream sMsgStream;
			sMsgStream
				<< LOCATION_STREAM_INSERTION
				<< "Specified amino acid not found! "
				<< "Amino acid name = "
				<< sAminoAcidNameB;
			throw CElementNotFoundException(sMsgStream.str());
		}
	}
	// If amino acid not A found:
	else
	{
		std::stringstream sMsgStream;
		sMsgStream
			<< LOCATION_STREAM_INSERTION
			<< "Specified amino acid not found! "
			<< "Amino acid name = "
			<< sAminoAcidNameA;
		throw CElementNotFoundException(sMsgStream.str());
	}

	return 0;
}


/**
 * Description: Get an instance of CBlosum45MatrixReferenc class.
 */
const CBlosum45MatrixReference& CBlosum45MatrixReference::getInstance()
{
	// If an singleton instance already exists:
	if (_pSingleton)
	{
		return *_pSingleton;
	}
	// If no singleton instance exists:
	else
	{
		_pSingleton = new CBlosum45MatrixReference();
		return *_pSingleton;
	}
}


/**
 * Description:
 */
int CBlosum45MatrixReference::setupIndexMap()
{
	_aminoAcidNameToIndexMap["A"] = 0;
	_aminoAcidNameToIndexMap["ALA"] = 0;
	_aminoAcidNameToIndexMap["R"] = 1;
	_aminoAcidNameToIndexMap["ARG"] = 1;
	_aminoAcidNameToIndexMap["N"] = 2;
	_aminoAcidNameToIndexMap["ASN"] = 2;
	_aminoAcidNameToIndexMap["D"] = 3;
	_aminoAcidNameToIndexMap["ASP"] = 3;
	_aminoAcidNameToIndexMap["C"] = 4;
	_aminoAcidNameToIndexMap["CYS"] = 4;
	_aminoAcidNameToIndexMap["Q"] = 5;
	_aminoAcidNameToIndexMap["GLN"] = 5;
	_aminoAcidNameToIndexMap["E"] = 6;
	_aminoAcidNameToIndexMap["GLU"] = 6;
	_aminoAcidNameToIndexMap["G"] = 7;
	_aminoAcidNameToIndexMap["GLY"] = 7;
	_aminoAcidNameToIndexMap["H"] = 8;
	_aminoAcidNameToIndexMap["HIS"] = 8;
	_aminoAcidNameToIndexMap["I"] = 9;
	_aminoAcidNameToIndexMap["ILE"] = 9;
	_aminoAcidNameToIndexMap["L"] = 10;
	_aminoAcidNameToIndexMap["LEU"] = 10;
	_aminoAcidNameToIndexMap["K"] = 11;
	_aminoAcidNameToIndexMap["LYS"] = 11;
	_aminoAcidNameToIndexMap["M"] = 12;
	_aminoAcidNameToIndexMap["MET"] = 12;
	_aminoAcidNameToIndexMap["F"] = 13;
	_aminoAcidNameToIndexMap["PHE"] = 13;
	_aminoAcidNameToIndexMap["P"] = 14;
	_aminoAcidNameToIndexMap["PRO"] = 14;
	_aminoAcidNameToIndexMap["S"] = 15;
	_aminoAcidNameToIndexMap["SER"] = 15;
	_aminoAcidNameToIndexMap["T"] = 16;
	_aminoAcidNameToIndexMap["THR"] = 16;
	_aminoAcidNameToIndexMap["W"] = 17;
	_aminoAcidNameToIndexMap["TRP"] = 17;
	_aminoAcidNameToIndexMap["Y"] = 18;
	_aminoAcidNameToIndexMap["TYR"] = 18;
	_aminoAcidNameToIndexMap["V"] = 19;
	_aminoAcidNameToIndexMap["VAL"] = 19;
	_aminoAcidNameToIndexMap["B"] = 20;
	_aminoAcidNameToIndexMap["J"] = 21;
	_aminoAcidNameToIndexMap["Z"] = 22;
	_aminoAcidNameToIndexMap["X"] = 23;
	_aminoAcidNameToIndexMap["*"] = 24;

	return 0;
}
