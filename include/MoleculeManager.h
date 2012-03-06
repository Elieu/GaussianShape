/**
 * Molecule Manager Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file MoleculeManager.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-03-30
 */


#include <memory>


class IMolecule;


/**
 * Description:
 */
class CMoleculeManager
{
	/* data: */
public:
private:

	/* method: */
public:
	CMoleculeManager();
	~CMoleculeManager();

	static std::auto_ptr<IMolecule> getMolecule();
private:
};
