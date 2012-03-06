/**
 * Reference Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file Reference.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-09-27
 */


#ifndef REFERENCE_INCLUDE_H
#define REFERENCE_INCLUDE_H
//


#include <map>
#include <string>


/**
 * Description:
 */
class CAtomRadiusReference
{
	/* data: */
public:
private:
	// atom radius map
	static std::map<std::string, double> _atomRadiusMap;
	// default radius if atom not found
	static const double _dDEFAULT_RADIUS;
	// singleton pointer
	static CAtomRadiusReference* _pSingleton;

	/* method: */
public:
	static int destroy();
	double getAtomRadius(const std::string& sAtomType) const;
	static CAtomRadiusReference& getInstance();
private:
	int setupRadiusMap();
	// Prevent constructing from outside.
	CAtomRadiusReference();
	// Prevent copying
	CAtomRadiusReference(const CAtomRadiusReference& reference);
	// Prevent deleting from outside.
	~CAtomRadiusReference();
};


//*****************************************************************************************

/**
 * Description:
 */
class CAminoAcidNameReference
{
	/* data: */
public:
private:

	/* method: */
public:
private:
	// Prevent from constructing from outside.
	CAminoAcidNameReference();
	// Prevent from copying.
	CAminoAcidNameReference(const CAminoAcidNameReference& reference);
	// Prevent from deleting from outside.
	~CAminoAcidNameReference();
};


//*****************************************************************************************

/**
 * Description:
 */
class CBlosum45MatrixReference
{
	/* data: */
public:
private:
	// BLOSUM45 matrix
	static const int _aiBlosumMatrix[25][25];
	// mapping amino acid name to index of BLOSUM45 matrix
	static std::map<std::string, int> _aminoAcidNameToIndexMap;
	// singleton pointer
	static CBlosum45MatrixReference* _pSingleton;

	/* method: */
public:
	void destory();
	const CBlosum45MatrixReference& getInstance();
	int getMatchScore(const std::string& sAminoAcidNameA, const std::string& sAminoAcidNameB) const;
private:
	// Prevent from constructing from outside.
	CBlosum45MatrixReference();
	// Prevent from copying.
	CBlosum45MatrixReference(const CBlosum45MatrixReference& referenc);
	// Prevent from deleting from outside.
	~CBlosum45MatrixReference();

	int setupIndexMap();
};


//
#endif
