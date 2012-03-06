/**
 * Molecule Reader Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file MoleculeReader.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-09-26
 */


#ifndef IO_INCLUDE_H
#define IO_INCLUDE_H
//


#include "Exception.h"
#include "InterfaceAtom.h"
#include "InterfaceBond.h"
#include "InterfaceMolecule.h"
#include "InterfaceMoleculeReader.h"
#include "InterfaceResidue.h"
#include "Reference.h"
#include "Utility.h"

#include <fstream>
#include <iomanip>
#include <list>
#include <set>
#include <sstream>
#include <string>

using std::endl;
using std::list;
using std::set;
using std::string;
using std::stringstream;


/**
 * Description: For reading molecules from MOL2 file.
 * @template TAtom: An implementation of IAtom interface which is responsible for storing atom information.
 * @template TBond: An implementation of IBond interface which is responsible for storing bond information.
 */
template <typename TAtom, typename TBond>
class CMol2Reader : public IMoleculeReader
{
	/* data: */
public:
	/* Error codes. */
	struct ErrorCodes : public IMoleculeReader::ErrorCodes
	{
		// no more element
		static const int nNO_MORE;
		// logical error
		static const int nLOGICAL_ERROR;

	private:
		ErrorCodes() {};
	};


private:
	/* Predefined tag string. */
	struct TagTexts
	{
		// ATOM tag
		static const std::string sATOM_TAG;
		// BOND tag
		static const std::string sBOND_TAG;
		// MOLECULE tag
		static const std::string sMOLECULE_TAG;
		// TRIPOS tag
		static const std::string sTRIPOS_TAG;
		// TRIPOS ATOM tag
		static const std::string sTRIPOS_ATOM_TAG;
		// TRIPOS BOND tag
		static const std::string sTRIPOS_BOND_TAG;
		// TRIPOS MOLECULES tag
		static const std::string sTRIPOS_MOLECULE_TAG;

	private:
		TagTexts() {};
	};


	/* Message string. */
	struct MessageTexts
	{
		static const std::string sBAD_FORMAT;
		static const std::string sCANNOT_OPEN;
		static const std::string sCANNOT_READ;
		static const std::string sEMPTY_FIELD;
		static const std::string sFIELD_MOLECULAR_NAME;
		static const std::string sIO_ERROR;
		static const std::string sINVALID_INDEX;

	private:
		MessageTexts() {};
	};


	/* Default Value. */
	struct DefaultValues
	{
		static const bool bREAD_HYDROGEN_FLAG;
		
	private:
		DefaultValues() {};
	};


	// maximum buffer size used when reading MOL2 file
	static const int _nBUFFER_SIZE = 512;
	// an flag indicating whether the next MOLECULE tag has been located.
	bool _bNextMolecule;
	// a flag indicating whether to read hydrogen atom
	bool _bReadHydrogenFlag;
	// MOL2 file stream
	std::fstream _mol2Stream;
	// MOL2 file name
	std::string _sMol2FileName;

	/* method: */
public:
	CMol2Reader(const std::string& sMol2FileName);
	virtual ~CMol2Reader();

	bool getReadHydrogenFlag() const;
	bool isOpen();
	int locateMolecule(int nMoleculeIndex);
	int openFile(const std::string& sMol2FileName);
	int readMolecule(IMolecule& mol);
	void reset();
	void setReadHydrogenFlag(bool bReadHydrogenFlag);

private:
	int locateNextTagLine(const std::string& sTag);
	int locateNextTagLine(const std::string& sTag, std::string& sRemain);
};


/* Template implementation for CMol2Reader class: */

/* Static member initialization: */

/* Tag texts: */
template <typename TAtom, typename TBond>
const string CMol2Reader<TAtom, TBond>::TagTexts::sATOM_TAG("ATOM");
template <typename TAtom, typename TBond>
const string CMol2Reader<TAtom, TBond>::TagTexts::sBOND_TAG("BOND");
template <typename TAtom, typename TBond>
const string CMol2Reader<TAtom, TBond>::TagTexts::sMOLECULE_TAG("MOLECULE");
template <typename TAtom, typename TBond>
const string CMol2Reader<TAtom, TBond>::TagTexts::sTRIPOS_TAG("@<TRIPOS>");
template <typename TAtom, typename TBond>
const string CMol2Reader<TAtom, TBond>::TagTexts::sTRIPOS_ATOM_TAG("@<TRIPOS>ATOM");
template <typename TAtom, typename TBond>
const string CMol2Reader<TAtom, TBond>::TagTexts::sTRIPOS_BOND_TAG("@<TRIPOS>BOND");
template <typename TAtom, typename TBond>
const string CMol2Reader<TAtom, TBond>::TagTexts::sTRIPOS_MOLECULE_TAG("@<TRIPOS>MOLECULE");

/* Message texts: */
template <typename TAtom, typename TBond>
const string CMol2Reader<TAtom, TBond>::MessageTexts::sBAD_FORMAT("Bad MOL2 file format! ");
template <typename TAtom, typename TBond>
const string CMol2Reader<TAtom, TBond>::MessageTexts::sCANNOT_OPEN("Can not open MOL2 file! ");
template <typename TAtom, typename TBond>
const string CMol2Reader<TAtom, TBond>::MessageTexts::sCANNOT_READ("Can not read MOL2 file! ");
template <typename TAtom, typename TBond>
const string CMol2Reader<TAtom, TBond>::MessageTexts::sEMPTY_FIELD("Empty field! ");
template <typename TAtom, typename TBond>
const string CMol2Reader<TAtom, TBond>::MessageTexts::sFIELD_MOLECULAR_NAME("Field: Molecular Name. ");
template <typename TAtom, typename TBond>
const string CMol2Reader<TAtom, TBond>::MessageTexts::sINVALID_INDEX("Invalid index! ");
template <typename TAtom, typename TBond>
const string CMol2Reader<TAtom, TBond>::MessageTexts::sIO_ERROR("IO error! ");

/* Error codes: */
template <typename TAtom, typename TBond>
const int CMol2Reader<TAtom, TBond>::ErrorCodes::nLOGICAL_ERROR = -1001;
template <typename TAtom, typename TBond>
const int CMol2Reader<TAtom, TBond>::ErrorCodes::nNO_MORE = 1001;

/* Default values: */
template <typename TAtom, typename TBond>
const bool CMol2Reader<TAtom, TBond>::DefaultValues::bREAD_HYDROGEN_FLAG = false;


/* Public Methods: */

/**
 * Description: Ctor.
 */
template <typename TAtom, typename TBond>
CMol2Reader<TAtom, TBond>::CMol2Reader(const std::string& sMol2FileName)
	:
	_bNextMolecule(false),
	_sMol2FileName(sMol2FileName)
{
	/* Open file stream. */
	_mol2Stream.open(sMol2FileName.c_str(), std::ios::in);
	if (_mol2Stream.good())
	{
		setReadHydrogenFlag(DefaultValues::bREAD_HYDROGEN_FLAG);
	}
	// If IO problem:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sCANNOT_READ
			<< sMol2FileName;
		throw CFileOpenException(msgStream.str());
	}
}


/**
 * Description: Dtor.
 */
template <typename TAtom, typename TBond>
CMol2Reader<TAtom, TBond>::~CMol2Reader()
{
	if (_mol2Stream.is_open())
	{
		_mol2Stream.close();
	}
}


/**
 * Description:
 */
template <typename TAtom, typename TBond>
bool CMol2Reader<TAtom, TBond>::getReadHydrogenFlag() const
{
	return _bReadHydrogenFlag;
}


/**
 * Description:
 * @return:
 */
template <typename TAtom, typename TBond>
bool CMol2Reader<TAtom, TBond>::isOpen()
{
	return _mol2Stream.is_open();
}


/**
 * Description:
 * @param nMoleculeIndex: (IN)
 */
template <typename TAtom, typename TBond>
int CMol2Reader<TAtom, TBond>::locateMolecule(int nMoleculeIndex)
{
	// Check parameters.
	if (nMoleculeIndex >= 0)
	{
		reset();

		// locate each TRIPOS MOLECULE tag.
		for (int iId = 0; iId <= nMoleculeIndex; ++iId)
		{
			int nLocateResult = locateNextTagLine(TagTexts::sTRIPOS_MOLECULE_TAG);
			// If molecule not found:
			if (nLocateResult != ErrorCodes::nNORMAL)
			{
				return ErrorCodes::nNOT_FOUND;
			}
		}

		/* Molecule found. */
		// Set the _bNextMolecule flag.
		_bNextMolecule = true;

		return ErrorCodes::nNORMAL;
	}
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sINVALID_INDEX
			<< nMoleculeIndex;
		throw CInvalidArgumentException(msgStream.str());
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description:
 * @param sMol2FileName:
 * @return:
 */
template <typename TAtom, typename TBond>
int CMol2Reader<TAtom, TBond>::openFile(const std::string& sMol2FileName)
{
	/* Close previous file stream. */
	_mol2Stream.clear();
	if (_mol2Stream.is_open())
	{
		_mol2Stream.close();
	}

	/* Reset internal state. */
	_bNextMolecule = false;

	/* Open file stream. */
	_mol2Stream.open(sMol2FileName.c_str(), std::ios::in);
	if (_mol2Stream.good())
	{
		_sMol2FileName = sMol2FileName;

		return ErrorCodes::nNORMAL;
	}
	// If IO problem:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sCANNOT_READ
			<< sMol2FileName;
		throw CFileOpenException(msgStream.str());
	}
}


/**
 * Description:
 */
template <typename TAtom, typename TBond>
void CMol2Reader<TAtom, TBond>::reset()
{
	_mol2Stream.clear();
	_mol2Stream.seekg(0, std::ios::beg);

	_bNextMolecule = false;
}


/**
 * Description:
 * @param mol:
 * @return:
 */
template <typename TAtom, typename TBond>
int CMol2Reader<TAtom, TBond>::readMolecule(IMolecule& mol)
{
	mol.clear();
	// io reading buffer
	auto_array<char> autoBuffer(new char[_nBUFFER_SIZE]);

	/* Locate next MOLECULE tag. */
	// If the next MOLECULE tag has not been located:
	if (!_bNextMolecule)
	{
		// Locate next MOLECULE tag.
		const int nNextTagErrorCode = locateNextTagLine(TagTexts::sTRIPOS_MOLECULE_TAG);
		// If not found:
		if (nNextTagErrorCode == ErrorCodes::nNOT_FOUND)
		{
			return ErrorCodes::nNOT_FOUND;
		}
	}
	// reset the _bNextMolecule flag
	_bNextMolecule = false;

	// Now, we are just right after the MOLECULE tag.
	/* Read molecular name. */
	string sMolecularName;
	_mol2Stream.getline(autoBuffer.get(), _nBUFFER_SIZE);
	// If success:
	if (_mol2Stream.gcount() > 0)
	{
		sMolecularName.assign(autoBuffer.get());
		CUtility::trimString(sMolecularName);
		if (sMolecularName.empty())
		{
			std::stringstream msgStream;
			msgStream
				<< LOCATION_STREAM_INSERTION
				<< MessageTexts::sEMPTY_FIELD
				<< MessageTexts::sFIELD_MOLECULAR_NAME;
			throw CBadFormatException(msgStream.str());
		}
	}
	// If EOF:
	else if (_mol2Stream.eof())
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sBAD_FORMAT
			<< _sMol2FileName;
		throw CBadFormatException(msgStream.str());
	}
	// If IO error:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sIO_ERROR
			<< _sMol2FileName;
		throw CIoErrorException(msgStream.str());
	}
	mol.setMolecularName(sMolecularName);


	// a set containing ID of hydrogen atom
	set<int> hydrogenAtomIdsSet;
	/* Locate each TRIPOS tag. */
	string sTagName;
	while (locateNextTagLine(TagTexts::sTRIPOS_TAG, sTagName) == ErrorCodes::nNORMAL)
	{
		CUtility::trimString(sTagName);

		// If an ATOM tag:
		if (!sTagName.compare(TagTexts::sATOM_TAG))
		{
			// Now, we are just right after the ATOM tag.
			while (_mol2Stream.peek() != '@')
			{
				// If ATOM line:
				if (_mol2Stream.good())
				{
					_mol2Stream.getline(autoBuffer.get(), _nBUFFER_SIZE);
					string sAtomLine(autoBuffer.get());
					CUtility::trimString(sAtomLine);

					// If not an empty atom line:
					if (!sAtomLine.empty())
					{
						std::stringstream atomLineStream(sAtomLine);

						/* Read atom information. */
						int nAtomId = -1;
						double dX = 0.0;
						double dY = 0.0;
						double dZ = 0.0;
						atomLineStream >> nAtomId;
						atomLineStream >> std::setw(_nBUFFER_SIZE) >> autoBuffer.get();
						string sAtomName(autoBuffer.get());
						atomLineStream >> dX >> dY >> dZ;
						atomLineStream >> std::setw(_nBUFFER_SIZE) >> autoBuffer.get();
						string sAtomType(autoBuffer.get());

						// If success:
						if (atomLineStream.good())
						{
							/* Query atom radius. */
							const size_t nDotPos = sAtomType.find_first_of('.');
							string sElementName = (nDotPos != string::npos) ? sAtomType.substr(0, nDotPos) : sAtomType;
							CAtomRadiusReference& radiusReference = CAtomRadiusReference::getInstance();
							const double dAtomRadius = radiusReference.getAtomRadius(sElementName);

							/* Store this atom. */
							TAtom tAtom;
							IAtom& atom = static_cast<IAtom&>(tAtom);

							atom.setAtomId(nAtomId);
							atom.setAtomName(sAtomName);
							atom.setAtomType(sAtomType);
							atom.setElementName(sElementName);
							atom.setMolecule(&mol);
							atom.setAtomRadius(dAtomRadius);
							atom.setPositionX(dX);
							atom.setPositionY(dY);
							atom.setPositionZ(dZ);

							// If we should skip this Hydrogen atom:
							if (!_bReadHydrogenFlag && (sAtomName[0] == 'H' || sAtomName[0] == 'h'))
							{
								hydrogenAtomIdsSet.insert(nAtomId);
							}
							else
							{
								mol.addAtom(tAtom);
							}
						}
						// If EOF:
						else if (atomLineStream.eof())
						{
						}
						// If interpretation failure:
						else if (atomLineStream.fail())
						{
							std::stringstream msgStream;
							msgStream
								<< LOCATION_STREAM_INSERTION
								<< MessageTexts::sBAD_FORMAT
								<< _sMol2FileName;
							throw CBadFormatException(msgStream.str());
						}
						// If IO error:
						else
						{
							std::stringstream msgStream;
							msgStream
								<< LOCATION_STREAM_INSERTION
								<< MessageTexts::sIO_ERROR
								<< _sMol2FileName;
							throw CIoErrorException(msgStream.str());
						}
					}
				}
				// If EOF:
				else if (_mol2Stream.eof())
				{
					return ErrorCodes::nNORMAL;
				}
				// If error:
				else
				{
					std::stringstream msgStream;
					msgStream
						<< LOCATION_STREAM_INSERTION
						<< MessageTexts::sIO_ERROR
						<< _sMol2FileName;
					throw CIoErrorException(msgStream.str());
				}
			} // while
		}
		// If a BOND tag:
		else if (!sTagName.compare(TagTexts::sBOND_TAG))
		{
			// Now, we are just right after the BOND tag.
			while (_mol2Stream.peek() != '@')
			{
				// If BOND line:
				if (_mol2Stream.good())
				{
					_mol2Stream.getline(autoBuffer.get(), _nBUFFER_SIZE);
					string sBondLine( autoBuffer.get());
					CUtility::trimString(sBondLine);
					
					// If not an empty bond line:
					if (!sBondLine.empty())
					{
						std::stringstream bondLineStream(sBondLine);

						/* Read bond information. */
						int nBondId = -1;
						int nBondedAtomXId = -1;
						int nBondedAtomYId = -1;
						bondLineStream >> nBondId;
						bondLineStream >> nBondedAtomXId;
						bondLineStream >> nBondedAtomYId;
						bondLineStream >> std::setw(_nBUFFER_SIZE) >> autoBuffer.get();
						string sBondType(autoBuffer.get());

						// If IO success:
						if (bondLineStream.good())
						{
							/* Store this bond. */
							TBond tBond;
							IBond& bond = static_cast<IBond&>(tBond);

							bond.setBondId(nBondId);
							bond.setBondedAtomXId(nBondedAtomXId);
							bond.setBondedAtomYId(nBondedAtomYId);
							bond.setBondType(sBondType);

							// If we should skip this bond connecting a hydrogen atom:
							if (!_bReadHydrogenFlag && (EXIST(nBondedAtomXId, hydrogenAtomIdsSet) || EXIST(nBondedAtomYId, hydrogenAtomIdsSet)))
							{
								continue;
							}
							else
							{
								mol.addBond(tBond);
							}
						}
						// If EOF:
						else if (bondLineStream.eof())
						{
						}
						// If interpretation failure:
						else if (bondLineStream.fail())
						{
							std::stringstream msgStream;
							msgStream
								<< LOCATION_STREAM_INSERTION
								<< MessageTexts::sBAD_FORMAT
								<< _sMol2FileName;
							throw CBadFormatException(msgStream.str());
						}
						// If IO error:
						else
						{
							std::stringstream msgStream;
							msgStream
								<< LOCATION_STREAM_INSERTION
								<< MessageTexts::sIO_ERROR
								<< _sMol2FileName;
							throw CIoErrorException(msgStream.str());
						}
					}
				}
				// If EOF:
				else if (_mol2Stream.eof())
				{
					return ErrorCodes::nNORMAL;
				}
				// If IO error:
				else
				{
					std::stringstream msgStream;
					msgStream
						<< LOCATION_STREAM_INSERTION
						<< MessageTexts::sIO_ERROR
						<< _sMol2FileName;
					throw CIoErrorException(msgStream.str());
				}
			} // while
		}
		// If a MOLECULE tag:
		else if (!sTagName.compare(TagTexts::sMOLECULE_TAG))
		{
			_bNextMolecule = true;
			return ErrorCodes::nNORMAL;
		}
	} // while

	return ErrorCodes::nNORMAL;
}


/**
 * Description:
 */
template <typename TAtom, typename TBond>
void CMol2Reader<TAtom, TBond>::setReadHydrogenFlag(bool bReadHydrogenFlag)
{
	_bReadHydrogenFlag = bReadHydrogenFlag;
}


/* Private Methods: */

/**
 * Description:
 * @param sTag:
 * @return:
 *	ErrorCodes::nNOMAAL:
 *	ErrorCodes::nNOT_FOUND:
 *	ErrorCodes::nLOGICAL_ERROR:
 */
template <typename TAtom, typename TBond>
int CMol2Reader<TAtom, TBond>::locateNextTagLine(const std::string& sTag)
{
	string sRemain;
	return locateNextTagLine(sTag, sRemain);
}


/**
 * Description:
 * @param sTag:
 * @param sRemain:
 * @return:
 *	ErrorCodes::nNOMAAL:
 *	ErrorCodes::nNOT_FOUND:
 *	ErrorCodes::nLOGICAL_ERROR:
 */
template <typename TAtom, typename TBond>
int CMol2Reader<TAtom, TBond>::locateNextTagLine(const std::string& sTag, std::string& sRemain)
{
	// file reading buffer
	auto_array<char> autoBuffer(new char[_nBUFFER_SIZE]);
	while (true)
	{
		_mol2Stream.getline(autoBuffer.get(), _nBUFFER_SIZE);
		// If success:
		if (_mol2Stream.gcount() > 0)
		{
			string sLine(autoBuffer.get());
			CUtility::trimString(sLine);

			size_t nStartPos = sLine.find(sTag);
			// If tag found:
			if (nStartPos != string::npos)
			{
				sRemain = sLine.substr(nStartPos + sTag.size());

				return ErrorCodes::nNORMAL;
			}
		}
		// If EOF:
		else if (_mol2Stream.eof())
		{
			return ErrorCodes::nNOT_FOUND;
		}
		// If IO error:
		else
		{
			std::stringstream msgStream;
			msgStream
				<< LOCATION_STREAM_INSERTION
				<< MessageTexts::sIO_ERROR
				<< _sMol2FileName;
			throw CIoErrorException(msgStream.str());
		}
	}

	return ErrorCodes::nLOGICAL_ERROR;
}


//******************************************************************************

/**
 * Description: For writting MOL2 file.
 */
class CMol2Writer
{
	/* data: */
public:
private:
	// MOL2 file stream
	std::fstream _mol2Stream;
	// MOL2 file name
	std::string _sMol2FileName;
	// maximum buffer size used when writting MOL2 file
	static const int _nBUFFER_SIZE = 512;

	/* Predefined tag string. */
	struct TagTexts
	{
		static const std::string sATOM_TAG;
		static const std::string sBOND_TAG;
		static const std::string sMOLECULE_TAG;
		static const std::string sTRIPOS_TAG;
		static const std::string sTRIPOS_ATOM_TAG;
		static const std::string sTRIPOS_BOND_TAG;
		static const std::string sTRIPOS_MOLECULE_TAG;
	};

	/* Message string. */
	struct MessageTexts
	{
		static const std::string sCANNOT_WRITE;
	};

	/* method: */
public:
	CMol2Writer(const std::string& sMol2FileName);
	~CMol2Writer();

	int writeMolecule(IMolecule& mol);
private:
};


//*******************************************************************************************

/**
 * Description: For reading molecules from PDB file.
 * @template TAtom: An implementation of IAtom interface which is responsible for storing atom information.
 * @template TBond: An implementation of IBond interface which is responsible for storing bond information.
 * @template TResidue: An implementation of IResidue interface which is responsible for storing bond information.
 */
template <typename TAtom, typename TBond, typename TResidue>
class CPdbReader : public IMoleculeReader
{
	/* data: */
public:
	/* error codes */
	struct ErrorCodes : public IMoleculeReader::ErrorCodes
	{
		static const int nEOF;

	private:
		ErrorCodes() {};
	};


private:
	/* record name for each PDB file entry */
	struct PdbRecordNames
	{
		static const string sATOM;
		static const string sHEADER;
		static const string sHETATM;

	private:
		PdbRecordNames() {};
	};


	/* record of PDB filed position information */
	struct PdbFieldPosition
	{
		/* data: */
		// start position of the field
		int startPosition;
		// length of the field
		int fieldLength;

		/* method: */
		PdbFieldPosition(int nStartPosition, int nFieldLength);
	};


	/* field position for each type of PDB record */
	struct PdbFieldPositions
	{
		static const PdbFieldPosition ATOM_ALTERNATE_LOCATION;
		static const PdbFieldPosition ATOM_ELEMENT_SYMBOL;
		static const PdbFieldPosition ATOM_ID;
		static const PdbFieldPosition ATOM_NAME;
		static const PdbFieldPosition ATOM_RESIDUE_NAME;
		static const PdbFieldPosition ATOM_X_COORDINATE;
		static const PdbFieldPosition ATOM_Y_COORDINATE;
		static const PdbFieldPosition ATOM_Z_COORDINATE;
		static const PdbFieldPosition RECORD_NAME;
		static const PdbFieldPosition RESIDUE_ID;
		static const PdbFieldPosition RESIDUE_NAME;

	private:
		PdbFieldPositions() {};
	};


	/* message texts */
	struct MessageTexts
	{
		static const string sBAD_FILE_FORMAT;
		static const string sBUFFER_OVERFLOW;
		static const string sCAN_NOT_OPEN_FILE;
		static const string sINVALID_INDEX;
		static const string sIO_ERROR;

	private:
		MessageTexts() {};
	};


	/* default values */
	struct DefaultValues
	{
		static const bool bREAD_HYDROGEN_FLAG;

	private:
		DefaultValues() {};
	};


	// a flag specifies whether to read hydrogen atom
	bool _bReadHydrogenFlag;
	// buffer size for reading
	static const int _nBUFFER_SIZE;
	// length of a standard record in PDB file
	static const int _nSTANDARD_RECORD_LENGTH;

	// PDB file stream
	std::fstream _pdbStream;
	// PDB file name
	std::string _sPdbFileName;

	/* method: */
public:
	CPdbReader(const std::string& sFileName);
	virtual ~CPdbReader();

	bool getReadHydrogenFlag() const;
	bool isOpen();
	int locateMolecule(int nMoleculeIndex);
	int openFile(const std::string& sFileName);
	int readMolecule(IMolecule& mol);
	void reset();
	void setReadHydrogenFlag(bool bReadHydrogen);

private:
	int resolveAtomRecord(std::string& sRecordLine, IAtom& atom, IResidue& residue);
};


/* Template implementation for CPdbReader class: */

/* Static members: */

template <typename TAtom, typename TBond, typename TResidue>
const int CPdbReader<TAtom, TBond, TResidue>::_nBUFFER_SIZE = 512;
template <typename TAtom, typename TBond, typename TResidue>
const int CPdbReader<TAtom, TBond, TResidue>::_nSTANDARD_RECORD_LENGTH = 80;

/* error code: */
template <typename TAtom, typename TBond, typename TResidue>
const int CPdbReader<TAtom, TBond, TResidue>::ErrorCodes::nEOF = -1;

/* PDB record name: */
template <typename TAtom, typename TBond, typename TResidue>
const string CPdbReader<TAtom, TBond, TResidue>::PdbRecordNames::sATOM("ATOM  ");
template <typename TAtom, typename TBond, typename TResidue>
const string CPdbReader<TAtom, TBond, TResidue>::PdbRecordNames::sHEADER("HEADER");
template <typename TAtom, typename TBond, typename TResidue>
const string CPdbReader<TAtom, TBond, TResidue>::PdbRecordNames::sHETATM("HETATM");

/* PDB field position: */
template <typename TAtom, typename TBond, typename TResidue>
const typename CPdbReader<TAtom, TBond, TResidue>::PdbFieldPosition CPdbReader<TAtom, TBond, TResidue>::PdbFieldPositions::ATOM_ELEMENT_SYMBOL(76, 2);
template <typename TAtom, typename TBond, typename TResidue>
const typename CPdbReader<TAtom, TBond, TResidue>::PdbFieldPosition CPdbReader<TAtom, TBond, TResidue>::PdbFieldPositions::ATOM_ID(6, 5);
template <typename TAtom, typename TBond, typename TResidue>
const typename CPdbReader<TAtom, TBond, TResidue>::PdbFieldPosition CPdbReader<TAtom, TBond, TResidue>::PdbFieldPositions::ATOM_NAME(12, 4);
template <typename TAtom, typename TBond, typename TResidue>
const typename CPdbReader<TAtom, TBond, TResidue>::PdbFieldPosition CPdbReader<TAtom, TBond, TResidue>::PdbFieldPositions::ATOM_X_COORDINATE(30, 8);
template <typename TAtom, typename TBond, typename TResidue>
const typename CPdbReader<TAtom, TBond, TResidue>::PdbFieldPosition CPdbReader<TAtom, TBond, TResidue>::PdbFieldPositions::ATOM_Y_COORDINATE(38, 8);
template <typename TAtom, typename TBond, typename TResidue>
const typename CPdbReader<TAtom, TBond, TResidue>::PdbFieldPosition CPdbReader<TAtom, TBond, TResidue>::PdbFieldPositions::ATOM_Z_COORDINATE(46, 8);
template <typename TAtom, typename TBond, typename TResidue>
const typename CPdbReader<TAtom, TBond, TResidue>::PdbFieldPosition CPdbReader<TAtom, TBond, TResidue>::PdbFieldPositions::RECORD_NAME(0, 6);
template <typename TAtom, typename TBond, typename TResidue>
const typename CPdbReader<TAtom, TBond, TResidue>::PdbFieldPosition CPdbReader<TAtom, TBond, TResidue>::PdbFieldPositions::RESIDUE_ID(22, 6);
template <typename TAtom, typename TBond, typename TResidue>
const typename CPdbReader<TAtom, TBond, TResidue>::PdbFieldPosition CPdbReader<TAtom, TBond, TResidue>::PdbFieldPositions::RESIDUE_NAME(17, 3);

/* default value: */
template <typename TAtom, typename TBond, typename TResidue>
const bool CPdbReader<TAtom, TBond, TResidue>::DefaultValues::bREAD_HYDROGEN_FLAG = false;

/* message text: */
template <typename TAtom, typename TBond, typename TResidue>
const string CPdbReader<TAtom, TBond, TResidue>::MessageTexts::sBAD_FILE_FORMAT("Bad file format. ");
template <typename TAtom, typename TBond, typename TResidue>
const string CPdbReader<TAtom, TBond, TResidue>::MessageTexts::sBUFFER_OVERFLOW("Buffer overflows. ");
template <typename TAtom, typename TBond, typename TResidue>
const string CPdbReader<TAtom, TBond, TResidue>::MessageTexts::sCAN_NOT_OPEN_FILE("Can not open file. ");
template <typename TAtom, typename TBond, typename TResidue>
const string CPdbReader<TAtom, TBond, TResidue>::MessageTexts::sINVALID_INDEX("Invalid index. ");
template <typename TAtom, typename TBond, typename TResidue>
const string CPdbReader<TAtom, TBond, TResidue>::MessageTexts::sIO_ERROR("IO error. ");


/**
 * Description: Constructor.
 */
template <typename TAtom, typename TBond, typename TResidue>
CPdbReader<TAtom, TBond, TResidue>::PdbFieldPosition::PdbFieldPosition(int nStartPosition, int nFieldLength)
	:
	startPosition(nStartPosition),
	fieldLength(nFieldLength)
{
}


/**
 * Description: Constructor.
 */
template <typename TAtom, typename TBond, typename TResidue>
CPdbReader<TAtom, TBond, TResidue>::CPdbReader(const std::string& sFileName)
	:
	_sPdbFileName(sFileName)
{
	_pdbStream.open(sFileName.c_str(), std::ios::in);
	// If file stream OK:
	if (_pdbStream.good())
	{
		setReadHydrogenFlag(DefaultValues::bREAD_HYDROGEN_FLAG);
	}
	// If file stream failure:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sCAN_NOT_OPEN_FILE
			<< sFileName;
		throw CFileOpenException(msgStream.str());
	}
}


/**
 * Description: Destructor.
 */
template <typename TAtom, typename TBond, typename TResidue>
CPdbReader<TAtom, TBond, TResidue>::~CPdbReader()
{
	/* Close file stream. */
	if (_pdbStream.is_open())
	{
		_pdbStream.close();
	}
}


/**
 * Description:
 */
template <typename TAtom, typename TBond, typename TResidue>
bool CPdbReader<TAtom, TBond, TResidue>::getReadHydrogenFlag() const
{
	return _bReadHydrogenFlag;
}


/**
 * Description:
 * @return:
 */
template <typename TAtom, typename TBond, typename TResidue>
bool CPdbReader<TAtom, TBond, TResidue>::isOpen()
{
	return _pdbStream.is_open();
}


/**
 * Description:
 * @param nMoleculeIndex: (IN)
 */
template <typename TAtom, typename TBond, typename TResidue>
int CPdbReader<TAtom, TBond, TResidue>::locateMolecule(int nMoleculeIndex)
{
	// Check parameters.
	if (nMoleculeIndex >= 0)
	{
		/*
		Note that PDB file can only contain one molecule.
		*/
		if (nMoleculeIndex == 0)
		{
			return ErrorCodes::nNORMAL;
		}
		else
		{
			return ErrorCodes::nNOT_FOUND;
		}
	}
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sINVALID_INDEX
			<< nMoleculeIndex;
		throw CInvalidArgumentException(msgStream.str());
	}
}


/**
 * Description:
 * @param sFileName: (IN)
 */
template <typename TAtom, typename TBond, typename TResidue>
int CPdbReader<TAtom, TBond, TResidue>::openFile(const std::string& sFileName)
{
	/* Close current file stream first. */
	if (_pdbStream.is_open())
	{
		_pdbStream.close();
	}

	/* Open a new file stream. */
	_pdbStream.open(sFileName.c_str(), std::ios::in);
	// If file stream OK:
	if (_pdbStream.good())
	{
		_sPdbFileName = sFileName;

		return ErrorCodes::nNORMAL;
	}
	// If file stream error:
	else
	{
		stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sCAN_NOT_OPEN_FILE
			<< sFileName;
		throw CIoErrorException(msgStream.str());
	}
}


/**
 * Description:
 */
template <typename TAtom, typename TBond, typename TResidue>
void CPdbReader<TAtom, TBond, TResidue>::reset()
{
	_pdbStream.clear();
	_pdbStream.seekg(0, std::ios::beg);
}


/**
 * Description:
 * @param sRecordLine: (IN)
 * @param atom: (OUT)
 * @exception:
 * 	CBadFormatException:
 */
template <typename TAtom, typename TBond, typename TResidue>
int CPdbReader<TAtom, TBond, TResidue>::resolveAtomRecord(std::string& sRecordLine, IAtom& atom, IResidue& residue)
{
	// If record line length OK (containing necessary fields):
	if (static_cast<int>(sRecordLine.size()) >= PdbFieldPositions::ATOM_Z_COORDINATE.startPosition + 1)
	{
		/* The following fields are necessary. */
		/* Get record name. */
		string sRecordName = sRecordLine.substr(
			PdbFieldPositions::RECORD_NAME.startPosition,
			PdbFieldPositions::RECORD_NAME.fieldLength
			);
		// whether this atom is a hetero atom
		CUtility::trimString(sRecordName);
		bool bHeteroAtomFlag = (!sRecordName.compare(PdbRecordNames::sHETATM)) ? true : false;

		/* Get atom ID. */
		stringstream atomIdStream(
			sRecordLine.substr(
				PdbFieldPositions::ATOM_ID.startPosition,
				PdbFieldPositions::ATOM_ID.fieldLength
				)
			);
		int iAtomId = -1;
		atomIdStream >> iAtomId;

		/* Get atom name. */
		string sAtomName = sRecordLine.substr(
			PdbFieldPositions::ATOM_NAME.startPosition,
			PdbFieldPositions::ATOM_NAME.fieldLength
			);
		CUtility::trimString(sAtomName);

		/* Get residue name. */
		string sResidueName = sRecordLine.substr(
			PdbFieldPositions::RESIDUE_NAME.startPosition,
			PdbFieldPositions::RESIDUE_NAME.fieldLength
			);
		CUtility::trimString(sResidueName);

		/* Get residue ID. */
		stringstream residueIdStream(
			sRecordLine.substr(
				PdbFieldPositions::RESIDUE_ID.startPosition,
				PdbFieldPositions::RESIDUE_ID.fieldLength
				)
			);
		int iResidueId;
		residueIdStream >> iResidueId;

		/* Get atom X coordinate. */
		stringstream atomXCoordinateStream(
			sRecordLine.substr(
				PdbFieldPositions::ATOM_X_COORDINATE.startPosition,
				PdbFieldPositions::ATOM_X_COORDINATE.fieldLength
				)
			);
		double dX = 0.0;
		atomXCoordinateStream >> dX;

		/* Get atom Y coordinate. */
		stringstream atomYCoordinateStream(
			sRecordLine.substr(
				PdbFieldPositions::ATOM_Y_COORDINATE.startPosition,
				PdbFieldPositions::ATOM_Y_COORDINATE.fieldLength
				)
			);
		double dY = 0.0;
		atomYCoordinateStream >> dY;

		/* Get atom Z coordinate. */
		stringstream atomZCoordinateStream(
			sRecordLine.substr(
				PdbFieldPositions::ATOM_Z_COORDINATE.startPosition,
				PdbFieldPositions::ATOM_Z_COORDINATE.fieldLength
				)
			);
		double dZ = 0.0;
		atomZCoordinateStream >> dZ;

		/* The following fields may be missing from source PDB record. */
		/* Get atom element symbol. */
		string sElementSymbol;
		if (static_cast<int>(sRecordLine.size()) > PdbFieldPositions::ATOM_ELEMENT_SYMBOL.startPosition + 1)
		{
			sElementSymbol = sRecordLine.substr(
				PdbFieldPositions::ATOM_ELEMENT_SYMBOL.startPosition,
				PdbFieldPositions::ATOM_ELEMENT_SYMBOL.fieldLength
				);
			CUtility::trimString(sElementSymbol);
		}

		/* Look up for atom radius. */
		CAtomRadiusReference& atomRadiusReference = CAtomRadiusReference::getInstance();
		double dAtomRadius = atomRadiusReference.getAtomRadius(sElementSymbol);

		/* Store atom information. */
		atom.setAtomId(iAtomId);
		atom.setAtomName(sAtomName);
		atom.setPositionX(dX);
		atom.setPositionY(dY);
		atom.setPositionZ(dZ);
		atom.setAtomRadius(dAtomRadius);
		atom.setElementName(sElementSymbol);
		atom.setHeteroAtomFlag(bHeteroAtomFlag);

		/* Store residue information. */
		residue.setId(iResidueId);
		residue.setName(sResidueName);

		/* Set atom and residue relations. */
		atom.setResidue(&residue);

		return ErrorCodes::nNORMAL;
	}
	// If record line too short:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sBAD_FILE_FORMAT
			<< "Record line too short: "
			<< sRecordLine
			<< _sPdbFileName;
		throw CBadFormatException(msgStream.str());
	}
}


/**
 * Description:
 */
template <typename TAtom, typename TBond, typename TResidue>
void CPdbReader<TAtom, TBond, TResidue>::setReadHydrogenFlag(bool bReadHydrogen)
{
	_bReadHydrogenFlag = bReadHydrogen;
}


/**
 * Description:
 * @param mol: (OUT)
 * @exception:
 * 	CBufferOverflowException:
 * 	CIoErrorException:
 */
template <typename TAtom, typename TBond, typename TResidue>
int CPdbReader<TAtom, TBond, TResidue>::readMolecule(IMolecule& mol)
{
	mol.clear();

	// reading buffer
	auto_array<char> buffer(new char[_nBUFFER_SIZE]);
	// string wrapper of reading buffer
	string sBuffer;

	while (true)
	{
		_pdbStream.getline(*buffer, _nBUFFER_SIZE);
		sBuffer.assign(*buffer);
		/**
		 * Bug fixed at 2011-06-20
		 * Note: Record line of PDB file should not be trimmed.
		 */
		//CUtility::trimString(sBuffer);
		// If reading succeeds:
		if (_pdbStream.gcount() > 0)
		{
			// If not empty buffer:
			if (!sBuffer.empty())
			{
				// record name
				string sRecordName = sBuffer.substr(
						PdbFieldPositions::RECORD_NAME.startPosition,
						PdbFieldPositions::RECORD_NAME.fieldLength
						);

				// If an ATOM record:
				if (!sRecordName.compare(PdbRecordNames::sATOM) || !sRecordName.compare(PdbRecordNames::sHETATM))
				{
					/* Store this atom. */
					TAtom atom;
					TResidue residue;
					resolveAtomRecord(sBuffer, atom, residue);

					if (getReadHydrogenFlag())
					{
						mol.addAtom(atom);
					}
					else if (atom.isHeavyAtom())
					{
						mol.addAtom(atom);
					}
				}
				// If a HEADER record:
				else if (!sRecordName.compare(PdbRecordNames::sHEADER))
				{
				}
			}
			// If empty buffer:
			else
			{
				continue;
			}
		}
		// If EOF, nothing been read:
		else if (_pdbStream.eof())
		{
			if (mol.getAtomsCount() > 0)
			{
				return ErrorCodes::nNORMAL;
			}
			else
			{
				return ErrorCodes::nNOT_FOUND;
			}
		}
		// If buffer overflows:
		else if (_pdbStream.fail())
		{
			std::stringstream msgStream;
			msgStream
				<< LOCATION_STREAM_INSERTION
				<< MessageTexts::sBUFFER_OVERFLOW
				<< _sPdbFileName;
			throw CBufferOverflowException(msgStream.str());
		}
		// If IO error:
		else
		{
			std::stringstream msgStream;
			msgStream
				<< LOCATION_STREAM_INSERTION
				<< MessageTexts::sIO_ERROR
				<< _sPdbFileName;
			throw CIoErrorException(msgStream.str());
		}
	} // while

	return ErrorCodes::nNORMAL;
}


//
#endif
