/**
 * Atom Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file Atom.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-09-26
 */


#ifndef ATOM_INCLUDE_H
#define ATOM_INCLUDE_H
//


#include "InterfaceAtom.h"

#include <string>
#include <vector>


class IMolecule;
class IResidue;


/**
 * Description: An implementation of abstract atom.
 */
class CAtom : public IAtom
{
	// data:
public:
private:
	// hetero atom flag
	bool _bHeteroAtomFlag;
	// atom radius
	double _dAtomRadius;
	// atom ID
	int _nAtomId;
	// atom type
	std::string _sAtomType;
	// atom name
	std::string _sAtomName;
	// element name
	std::string _sElementName;
	// parent molecule
	IMolecule* _pMolecule;
	// related residue
	IResidue* _pResidue;
	// atom position
	std::vector<double> _position;

	// method:
public:
	CAtom();
	virtual ~CAtom();

	/* Implementation for IAtom interface: */
	virtual bool isHeavyAtom() const;
	virtual bool isHeteroAtom() const;

	virtual int getAtomId() const;
	virtual const std::string& getAtomType() const;
	virtual const std::string& getAtomName() const;
	virtual double getAtomRadius() const;
	virtual const std::string& getElementName() const;
	virtual IMolecule* getMolecule() const;
	virtual const std::vector<double>& getPosition() const;
	virtual double getPositionX() const;
	virtual double getPositionY() const;
	virtual double getPositionZ() const;
	virtual IResidue* getResidue() const;

	virtual void setAtomId(int nId);
	virtual void setAtomType(const std::string& sAtomType);
	virtual void setAtomName(const std::string& sAtomName);
	virtual void setAtomRadius(double dRadius);
	virtual void setElementName(const std::string& sName);
	virtual void setHeteroAtomFlag(bool bFlag);
	virtual void setMolecule(IMolecule* const pMolecule);
	virtual void setPosition(const std::vector<double>& position);
	virtual void setPositionX(double dPosX);
	virtual void setPositionY(double dPosY);
	virtual void setPositionZ(double dPosZ);
	virtual void setResidue(IResidue* pResidue);

	/* Implementation for ICloneable interface: */
	virtual ICloneable* clone() const;

private:
};


//
#endif
