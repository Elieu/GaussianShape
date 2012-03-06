/**
 * USR Service Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file UsrService.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-05-12
 */


#include "UsrService.h"

#include "Exception.h"
#include "InterfaceMolecule.h"
#include "InterfaceAtom.h"
#include "Usr.h"
#include "Utility.h"

#include <list>
#include <sstream>


using std::list;
using std::string;
using std::vector;


/* Static members: */

/* Error codes: */
const int CUsrService::ErrorCodes::nNORMAL = 0;

/* Message texts: */
const std::string CUsrService::MessageTexts::sATOM_COORDINATE_DIMENSIONS_NOT_MATCH("Atom coordinates are not in equal dimensions! ");


/**
 * Description: Ctor.
 */
CUsrService::CUsrService()
{
}


/**
 * Description: Dtor.
 */
CUsrService::~CUsrService()
{
}


/**
 * Description: Evaluate the USR molecular descriptor.
 * @param molecule: (IN) Source molecule to calculate descriptor.
 * @param descriptor: (OUT) Vector to hold the descriptor (12 elements).
 * @exception:
 *	CInvalidArgumentException: Atom coordinates are not in equal dimensions.
 */
int CUsrService::evaluateUsrMolecularDescriptor(const IMolecule& molecule, std::vector<double>& descriptor)
{
	/* Get atom coordinates. */
	vector<vector<double> > atomCoordinates;
	extractAtomCoordinates(molecule, atomCoordinates);

	/* Calculate descriptor. */
	try
	{
		CUsr::calculateMoments(atomCoordinates, descriptor);
	}
	// Atom coordinates are not in equal dimensions.
	catch(CInvalidArgumentException& exception)
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sATOM_COORDINATE_DIMENSIONS_NOT_MATCH
			<< "Caused by: "
			<< exception.getErrorMessage();
		throw CInvalidArgumentException(msgStream.str());
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description: Extract all atom coordinates.
 * @param molecule: (IN) Source molecule to extract atom coordinates.
 * @param coordinates: (OUT) Vector to hold atom coordinates. Eech coordinate is represented as a vector with 3 elements (X, Y and Z coordinates).
 */
int CUsrService::extractAtomCoordinates(const IMolecule& molecule, std::vector<std::vector<double> >& coordinates)
{
	coordinates.clear();

	list<IAtom*> atomsList = molecule.getAtomsList();
	// For each atom:
	FOREACH(atomIter, atomsList, list<IAtom*>::const_iterator)
	{
		IAtom& atom = **atomIter;
		coordinates.push_back(atom.getPosition());
	}

	return ErrorCodes::nNORMAL;
}
