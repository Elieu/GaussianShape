/**
 * Residue Interface Definition
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file InterfaceAtom.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-09-26
 */

#ifndef INTERFACE_RESIDUE_INCLUDE_H
#define INTERFACE_RESIDUE_INCLUDE_H
//


#include "InterfaceCloneable.h"

#include <string>


class IAtom;


/**
 * Description:
 */
class IResidue : public ICloneable
{
public:
	virtual ~IResidue() {};

	/**
	 * Description:
	 */
	virtual int addRelatedAtom(const IAtom& atom) = 0;

	/**
	 * Description:
	 */
	virtual int clearRelatedAtoms() = 0;

	/**
	 * Description:
	 */
	virtual int getId() const = 0;

	/**
	 * Description:
	 */
	virtual const std::string& getName() const = 0;

	/**
	 * Description:
	 */
	virtual void setId(int nId) = 0;

	/**
	 * Description:
	 */
	virtual void setName(const std::string& sName) = 0;
};


//
#endif
