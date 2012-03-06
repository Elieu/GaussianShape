/**
 * Molecule Reader Interface Definition
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file InterfaceMoleculeReader.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-02-22
 */


#ifndef INTERFACE_MOLECULE_READER_INCLUDE_H
#define INTERFACE_MOLECULE_READER_INCLUDE_H
//


#include <string>


class IMolecule;


/**
 * Description:
 */
class IMoleculeReader
{
	/* data: */
public:
	/* Error codes. */
	struct ErrorCodes
	{
		// success
		static const int nNORMAL = 0;
		// element not found
		static const int nNOT_FOUND = 1;

	private:
		ErrorCodes() {};
	};

private:

	/* method: */
public:
	/**
	 * Description: Destructor.
	 */
	virtual ~IMoleculeReader() {};

	/**
	 * Description:
	 * @return:
	 */
	virtual bool getReadHydrogenFlag() const = 0;

	/**
	 * Description:
	 * @return:
	 */
	virtual bool isOpen() = 0;

	/**
	 * Description:
	 * @param nMoleculeIndex:
	 * @return:
	 *	ErrorCodes::nNORMAL:
	 *	ErrorCodes::nNOT_FOUND:
	 */
	virtual int locateMolecule(int nMoleculeIndex) = 0;

	/**
	 * Description:
	 * @param sFileName: (IN)
	 * @return:
	 */
	virtual int openFile(const std::string& sFileName) = 0;

	/**
	 * Description:
	 */
	virtual void reset() = 0;

	/**
	 * Description:
	 * @param molecule: (OUT)
	 * @return:
	 *	ErrorCodes::nNORMARL:
	 *	ErrorCodes::nNOT_FOUND:
	 * @exception:
	 *	CBadFormatException:
	 *	CIoErrorException:
	 */
	virtual int readMolecule(IMolecule& molecule) = 0;

	/**
	 * Description:
	 * @param bFlag:
	 */
	virtual void setReadHydrogenFlag(bool bFlag) = 0;
private:
};


//
#endif
