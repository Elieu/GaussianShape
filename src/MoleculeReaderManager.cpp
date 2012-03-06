/**
 * Molecule Reader Manager Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file MoleculeReaderManager.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-02-22
 */


#include "MoleculeReaderManager.h"

#include "Atom.h"
#include "Bond.h"
#include "BusinessException.h"
#include "InterfaceMoleculeReader.h"
#include "MoleculeReader.h"
#include "Residue.h"
#include "Utility.h"

#include <sstream>


using std::auto_ptr;
using std::string;


/* Static members: */

/* File name extensions: */
const std::string CMoleculeReaderManager::FileNameExtensions::sMOL2_FILE_EXTENSION("MOL2");
const std::string CMoleculeReaderManager::FileNameExtensions::sPDB_FILE_EXTENSION("PDB");

/* Message texts: */
const std::string CMoleculeReaderManager::MessageTexts::sCAN_NOT_CREATE_MOLECULE_READER("Can not create molecule reader! ");
const std::string CMoleculeReaderManager::MessageTexts::sCAUSED_BY("Caused by: ");
const std::string CMoleculeReaderManager::MessageTexts::sFILE_NAME_EXTENSION_NOT_SUPPORTED("File name extenson not suppported! ");
const std::string CMoleculeReaderManager::MessageTexts::sMISSING_FILE_NAME_EXTENSION("Missing file name extension! ");

const char CMoleculeReaderManager::_cFILE_NAME_EXTENSION_DELIMITER = '.';


/**
 * Description: Ctor.
 */
CMoleculeReaderManager::CMoleculeReaderManager()
{
}


/**
 * Description: Dtor.
 */
CMoleculeReaderManager::~CMoleculeReaderManager()
{
}


/**
 * Description:
 * @param sFileName: (IN)
 * @return:
 * @exception:
 *	CFileNotSupportedException:
 */
std::auto_ptr<IMoleculeReader> CMoleculeReaderManager::getMoleculeReader(const std::string& sFileName)
{
	size_t nDilimiterPosition = sFileName.find_last_of(_cFILE_NAME_EXTENSION_DELIMITER);
	// If file name extension exists:
	if (nDilimiterPosition != string::npos)
	{
		/* Get file name extension. */
		string sFileNameExtension = sFileName.substr(nDilimiterPosition + 1);
		CUtility::stringToUpper(sFileNameExtension);

		// If MOL2 file:
		if (!sFileNameExtension.compare(FileNameExtensions::sMOL2_FILE_EXTENSION))
		{
			CMol2Reader<CAtom, CBond>* pMoleculeReader = NULL;
			try
			{
				pMoleculeReader = new CMol2Reader<CAtom, CBond>(sFileName);
			}
			// If file access failure:
			catch(CException& exception)
			{
				std::stringstream msgStream;
				msgStream
					<< LOCATION_STREAM_INSERTION
					<< MessageTexts::sCAN_NOT_CREATE_MOLECULE_READER
					<< MessageTexts::sCAUSED_BY
					<< exception.getErrorMessage();
				throw CFileIoException(msgStream.str());
			}

			return auto_ptr<IMoleculeReader>(pMoleculeReader);
		}
		// If PDB file:
		else if (!sFileNameExtension.compare(FileNameExtensions::sPDB_FILE_EXTENSION))
		{
			CPdbReader<CAtom, CBond, CResidue>* pMoleculeReader = NULL;
			try
			{
				pMoleculeReader = new CPdbReader<CAtom, CBond, CResidue>(sFileName);
			}
			// If file access failure:
			catch(CException& exception)
			{
				std::stringstream msgStream;
				msgStream
					<< LOCATION_STREAM_INSERTION
					<< MessageTexts::sCAN_NOT_CREATE_MOLECULE_READER
					<< MessageTexts::sCAUSED_BY
					<< exception.getErrorMessage();
				throw CFileIoException(msgStream.str());
			}

			return auto_ptr<IMoleculeReader>(pMoleculeReader);
		}
		// If other file:
		else
		{
			std::stringstream msgStream;
			msgStream
				<< LOCATION_STREAM_INSERTION
				<< MessageTexts::sFILE_NAME_EXTENSION_NOT_SUPPORTED
				<< sFileName;
			throw CFileNotSupportedException(msgStream.str());
		}
	}
	// If file name extension missing:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sMISSING_FILE_NAME_EXTENSION
			<< sFileName;
		throw CFileNotSupportedException(msgStream.str());
	}
}
