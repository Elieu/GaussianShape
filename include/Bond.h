/**
 * Bond Modle
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file Bond.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-10-01
 */


#ifndef BOND_INCLUDE_H
#define BOND_INCLUDE_H


#include "InterfaceBond.h"

#include <string>


class CBond : public IBond
{
	/* data: */
public:
private:
	int _nBondedAtomXId;
	int _nBondedAtomYId;
	int _nBondId;
	std::string _sBondType;

	/* method: */
public:
	CBond();
	~CBond();

	/* Implementation for IBond interface: */
	virtual int getBondedAtomXId() const;
	virtual int getBondedAtomYId() const;
	virtual int getBondId() const;
	virtual const std::string& getBondType() const;
	virtual void setBondedAtomXId(int nId);
	virtual void setBondedAtomYId(int nId);
	virtual void setBondId(int nId);
	virtual void setBondType(const std::string& sType);

	/* Implementation for ICloneable interface: */
	virtual ICloneable* clone() const;

private:
};


#endif
