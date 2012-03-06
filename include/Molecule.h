/**
 * Molecule Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file Molecule.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-09-26
 */


#ifndef MOLECULE_INCLUDE_H
#define MOLEUCLE_INCLUDE_H
//


#include "InterfaceMolecule.h"
#include "Utility.h"

#include <list>
#include <map>
#include <math.h>
#include <string>
#include <vector>


class IResidue;


/**
 * Description: An implementation of abstract molecule.
 */
class CMolecule : public IMolecule
{
	/* data: */
public:
	/* error code */
	struct ErrorCodes
	{
		static const int nNORMAL;

	private:
		ErrorCodes();
	};


private:
	/* message text */
	struct MessageTexts
	{
		static const std::string sDYNAMIC_CAST_ERROR;
		static const std::string sIVALID_PARAM;

	private:
		MessageTexts(){};
	};


	// atoms list
	std::list<IAtom*> _atomsList;
	// bonds list
	std::list<IBond*> _bondsList;
	// a flag specifying whether we need updating centroid coordinate
	mutable bool _bDirtyCentroid;
	// centroid coordinate array, in X, Y, Z order.
	mutable std::vector<double> _centroid;
	// residues map (key: residue ID; value: IResidue instance)
	std::map<int, IResidue*> _residuesMap;
	// molecular name
	std::string _sMolecularName;

	/* method: */
public:
	CMolecule();
	CMolecule(const CMolecule& mol);
	virtual ~CMolecule();

	/* Implementation for IMolecule interface: */
	virtual int addAtom(const IAtom& atom);
	virtual int addBond(const IBond& bond);
	virtual void move(double dX, double dY, double dZ);
	virtual void moveToCentroid();
	virtual void clear();
	virtual void rotateXYZ(double dRadianX, double dRadianY, double dRadianZ);

	virtual std::auto_ptr<IAtomIterator> beginAtomsIterator();
	virtual std::auto_ptr<IAtomIterator> endAtomsIterator();

	virtual IResidue* findResidue(int iId) const;
	virtual int getAtomsCount() const;
	virtual std::list<IAtom*> getAtomsList() const;
	virtual int getBondsCount() const;
	virtual std::list<IBond*> getBondsList() const;
	virtual const std::vector<double>& getCentroid() const;
	virtual const std::string& getMolecularName() const;

	virtual void setMolecularName(const std::string& sName);

	/* Implementation for ICloneable interface: */
	virtual ICloneable* clone() const;

	/* operators: */
	const CMolecule& operator=(const CMolecule& mol);
private:
};


//
#endif
