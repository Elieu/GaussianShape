/**
 * Command Line Service Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file CommandLineService.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-02-28
 */


#ifndef COMMAND_LINE_SERVICE_INLUDE_H
#define COMMAND_LINE_SERVICE_INLUDE_H
//


#include "ConfigurationArguments.h"

#include <string>


class CCommandLineArguments;


/**
 * Description:
 */
class CCommandLineService
{
	/* data: */
public:
	/* Error codes. */
	struct ErrorCodes
	{
		static const int nNORMAL;

	private:
		ErrorCodes() {};
	};


private:
	/* Default values. */
	struct DefaultValues
	{
	private:
		DefaultValues() {};
	};


	/* Message texts. */
	struct MessageTexts
	{
		static const std::string sCAN_NOT_WRITE_FILE;
		static const std::string sEMPTY_COMMAND_LINE_SWITCH;
		static const std::string sINVALID_COMMAND_LINE_SWITCH;
		static const std::string sINVALID_COMMAND_LINE_SWITCH_VALUE;
		static const std::string sMISSING_COMMAND_LINE_SWITCH_VALUES;

	private:
		MessageTexts() {};
	};


	/**
	 * Description: Tag texts used for output.
	 */
	struct TagTexts
	{
		static const std::string sCOMMENT_INDICATOR;
		static const std::string sHEADER;
		static const std::string sQUERY;
		static const std::string sTIME_PER_CONFORMER;
		static const std::string sTOTAL_MOLECULES;
		static const std::string sTOTAL_TIME;

	private:
		TagTexts() {};
	};


	/**
	 * Description: Aggregation of all parameters.
	 */
	struct ParametersAggregation
	{
	private:
		ParametersAggregation() {};
	};


	/**
	 * Description: Collection of parameter names to look up for specified parameter in configuration file.
	 */
	struct ParameterNames
	{
	private:
		ParameterNames() {};
	};


	/**
	 * Description: Switch names.
	 */
	struct SwitchNames
	{
		static const std::string sDATABASE;
		static const std::string sDB_RANGE;
		static const std::string sFIT;
		static const std::string sGAUSSIAN_VOLUME;
		static const std::string sOUTPUT;
		static const std::string sPOCKET;
		static const std::string sQUERY;
		static const std::string sREFERENCE;
		static const std::string sSH_DESCRIPTOR;
		static const std::string sUSR_DESCRIPTOR;

	private:
		SwitchNames() {};
	};


	// configuration arguments
	CConfigurationArguments _configurationArguments;

	/* method: */
public:
	CCommandLineService(const CConfigurationArguments& configurationArguments);
	~CCommandLineService();

	int startFromCommandLine(const CCommandLineArguments& commandLineArguments);
private:
	const CConfigurationArguments& getConfigurationArguments() const;
};


//
#endif
