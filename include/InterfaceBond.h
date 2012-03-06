/**
 * Bond Interface Definition
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file InterfaceBond.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-10-01
 */


#ifndef INTERFACE_BOND_INCLUDE_H
#define INTERFACE_BOND_INCLUDE_H
//


#include "InterfaceCloneable.h"

#include <string>


/**
 * Description: Abstract bond interface.
 */
class IBond : public ICloneable
{
public:
	/**
	 * Description: Dtor.
	 */
	virtual ~IBond() {};

	virtual int getBondedAtomXId() const = 0;
	virtual int getBondedAtomYId() const = 0;
	virtual int getBondId() const = 0;
	virtual const std::string& getBondType() const = 0;
	virtual void setBondedAtomXId(int nId) = 0;
	virtual void setBondedAtomYId(int nId) = 0;
	virtual void setBondId(int nId) = 0;
	virtual void setBondType(const std::string& sType) = 0;
};


//
#endif
