/**
 * USR Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file Usr.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-05-12
 */


#ifndef USR_INCLUDE_H
#define USR_INCLUDE_H
//


#include <string>
#include <vector>


/**
 * Description: Ultra-fast Shape Recognition Algorithm.
 */
class CUsr
{
	/* data: */
public:
	/* Error Codes. */
	struct ErrorCodes
	{
		static const int nNORMAL;
		static const int nEMPTY_CONTAINER;

	private:
		ErrorCodes() {};
	};


	/* Message Texts. */
	struct MessageTexts
	{
		static const std::string sDIMENSION_NOT_MATCH;
		
	private:
		MessageTexts() {};
	};


private:

	/* method: */
public:
	CUsr();
	~CUsr();

	static int calculateMoments(const std::vector<std::vector<double> >& coordinates, std::vector<double>& moments);
private:
	static int calculateCentroid(const std::vector<std::vector<double> >& coordinates, std::vector<double>& centroid);
	static double calculateMu1(const std::vector<double>& distances);
	static double calculateMu2(const std::vector<double>& distances, double dMean);
	static double calculateMu3(const std::vector<double>& distances, double dMean, double dSigma);
	static bool validateCoordinates(const std::vector<std::vector<double> >& coordinates);
};


//
#endif
