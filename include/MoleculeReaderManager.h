/**
 * Molecule Reader Manager Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file MoleculeReaderManager.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-02-22
 */


#ifndef MOLECULE_READER_MANAGER_INCLUDE_H
#define MOLECULE_READER_MANAGER_INCLUDE_H
//


#include <memory>
#include <string>


class IMoleculeReader;


/**
 * Description:
 */
class CMoleculeReaderManager
{
	/* data: */
public:
private:
	/* File name extensions. */
	struct FileNameExtensions
	{
		// file name extension for MOL2 file
		static const std::string sMOL2_FILE_EXTENSION;
		// file name extension for PDB file
		static const std::string sPDB_FILE_EXTENSION;

	private:
		FileNameExtensions() {};
	};


	/* Message texts. */
	struct MessageTexts
	{
		static const std::string sCAN_NOT_CREATE_MOLECULE_READER;
		static const std::string sCAUSED_BY;
		static const std::string sFILE_NAME_EXTENSION_NOT_SUPPORTED;
		static const std::string sMISSING_FILE_NAME_EXTENSION;

	private:
		MessageTexts() {};
	};


	// file name extension delimiter character
	static const char _cFILE_NAME_EXTENSION_DELIMITER;

	/* method: */
public:
	CMoleculeReaderManager();
	~CMoleculeReaderManager();

	static std::auto_ptr<IMoleculeReader> getMoleculeReader(const std::string& sFileName);
private:
};


//
#endif
