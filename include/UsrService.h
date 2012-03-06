/**
 * USR Service Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file UsrService.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-05-12
 */


#ifndef USER_SERVICE_INCLUDE_H
#define USER_SERVICE_INCLUDE_H
//


#include <string>
#include <vector>


class IMolecule;


/**
 * Description: USR service.
 */
class CUsrService
{
	/* data: */
public:
	/* Error Codes. */
	struct ErrorCodes
	{
		static const int nNORMAL;

	private:
		ErrorCodes() {};
	};


	/* Message Texts. */
	struct MessageTexts
	{
		static const std::string sATOM_COORDINATE_DIMENSIONS_NOT_MATCH;

	private:
		MessageTexts() {};
	};


private:

	/* methods: */
public:
	CUsrService();
	~CUsrService();

	int evaluateUsrMolecularDescriptor(const IMolecule& molecule, std::vector<double>& descriptor);
private:
	int extractAtomCoordinates(const IMolecule& molecule, std::vector<std::vector<double> >& coordinates);
};


//
#endif
