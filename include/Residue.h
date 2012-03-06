/**
 * Residue Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file Residue.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-06-29
 */

#ifndef RESIDUE_INCLUDE_H
#define RESIDUE_INCLUDE_H
//


#include "InterfaceResidue.h"

#include <string>
#include <vector>


class IAtom;


/**
 * Description:
 */
class CResidue : public IResidue
{
	/* data: */
public:
private:
	// related atoms vector
	std::vector<const IAtom*> _relatedAtomsVector;
	// residue ID
	int _nId;
	// residue name
	std::string _sName;

	/* method: */
public:
	CResidue();
	virtual ~CResidue();

	/* Implementation for IResidue interface: */
	virtual int addRelatedAtom(const IAtom& atom);
	virtual int clearRelatedAtoms();
	virtual int getId() const;
	virtual const std::string& getName() const;
	virtual void setId(int nID);
	virtual void setName(const std::string& sName);

	/* Implementation for ICloneable interface: */
	virtual ICloneable* clone() const;
private:
};


//
#endif
