/**
 * Molecule Manager Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file MoleculeManager.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-03-30
 */


#include "MoleculeManager.h"

#include "Molecule.h"


using std::auto_ptr;


/* Implementation for CMoleculeManager: */

/**
 * Description: Ctor.
 */
CMoleculeManager::CMoleculeManager()
{
}


/**
 * Description: Dtor.
 */
CMoleculeManager::~CMoleculeManager()
{
}


/**
 * Description: Create a new empty molecule.
 */
std::auto_ptr<IMolecule> CMoleculeManager::getMolecule()
{
	return auto_ptr<IMolecule>(new CMolecule());
}
