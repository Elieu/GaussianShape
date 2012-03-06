/**
 * Molecule Reader Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file MoleculeReader.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-09-26
 */


#include "MoleculeReader.h"

#include "InterfaceAtom.h"


using std::fstream;
using std::string;
using std::stringstream;


/* Implementation for CMol2Writer class: */

/* Static member initialization: */

const string CMol2Writer::TagTexts::sATOM_TAG("ATOM");
const string CMol2Writer::TagTexts::sBOND_TAG("BOND");
const string CMol2Writer::TagTexts::sMOLECULE_TAG("MOLECULE");
const string CMol2Writer::TagTexts::sTRIPOS_TAG("@<TRIPOS>");
const string CMol2Writer::TagTexts::sTRIPOS_ATOM_TAG("@<TRIPOS>ATOM");
const string CMol2Writer::TagTexts::sTRIPOS_BOND_TAG("@<TRIPOS>BOND");
const string CMol2Writer::TagTexts::sTRIPOS_MOLECULE_TAG("@<TRIPOS>MOLECULE");

const string CMol2Writer::MessageTexts::sCANNOT_WRITE("Can not write MOL2 file! ");


/**
 * Description: Ctor.
 */
CMol2Writer::CMol2Writer(const std::string& sMol2FileName)
{
	_mol2Stream.open(sMol2FileName.c_str(), std::ios::out);
	// If file stream OK:
	if (!_mol2Stream.good())
	{
		_sMol2FileName = sMol2FileName;
	}
	// If file stream failure:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "Can not write file: "
			<< _sMol2FileName;
		throw CFileOpenException(msgStream.str());
	}
}


/**
 * Description: Dtor.
 */
CMol2Writer::~CMol2Writer()
{
	if (_mol2Stream.is_open())
	{
		_mol2Stream.close();
	}
}


/**
 * Description:
 * @param mol:
 */
int CMol2Writer::writeMolecule(IMolecule& mol)
{
	/* Output MOLECULE section: */
	_mol2Stream << TagTexts::sTRIPOS_MOLECULE_TAG << endl;
	_mol2Stream << mol.getMolecularName() << endl;
	_mol2Stream << mol.getAtomsCount() << '\t';
	_mol2Stream << mol.getBondsCount() << '\t';
	_mol2Stream << "1\t0\t0" << endl;
	_mol2Stream << "SMALL" << endl;
	_mol2Stream << "FORMAL_CHARGES" << endl;

	/* Output ATOM section: */
	_mol2Stream << TagTexts::sTRIPOS_ATOM_TAG << endl;
	const list<IAtom*> atomsList = mol.getAtomsList();
	FOREACH(iterAtom, atomsList, list<IAtom*>::const_iterator)
	{
		const IAtom& atom = **iterAtom;
		_mol2Stream << atom.getAtomId() << '\t';
		_mol2Stream << atom.getAtomName() << '\t';
		_mol2Stream << atom.getPositionX() << '\t';
		_mol2Stream << atom.getPositionY() << '\t';
		_mol2Stream << atom.getPositionZ() << '\t';
		_mol2Stream << atom.getAtomType() << endl;
	}

	/* Output BOND section: */
	_mol2Stream << TagTexts::sTRIPOS_BOND_TAG << endl;
	const list<IBond*>& BONDS_LIST = mol.getBondsList();
	FOREACH(iterBond, BONDS_LIST, list<IBond*>::const_iterator)
	{
		const IBond& bond = **iterBond;
		_mol2Stream << bond.getBondId() << '\t';
		_mol2Stream << bond.getBondedAtomXId() << '\t';
		_mol2Stream << bond.getBondedAtomYId() << '\t';
		_mol2Stream << bond.getBondType() << endl;
	}

	return 0;
}

