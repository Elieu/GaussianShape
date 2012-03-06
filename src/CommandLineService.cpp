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



#include "CommandLineService.h"

#include "BusinessException.h"
#include "CommandLineArguments.h"
#include "GaussianService.h"
#include "InterfaceMolecule.h"
#include "InterfaceMoleculeReader.h"
#include "MoleculeManager.h"
#include "MoleculeReaderManager.h"
#include "SphericalHarmonicService.h"
#include "UsrService.h"
#include "Utility.h"

#include <fstream>
#include <limits>
#include <sstream>
#include <vector>


using std::auto_ptr;
using std::endl;
using std::string;
using std::vector;


/* Static Members: */

/* Error codes: */
const int CCommandLineService::ErrorCodes::nNORMAL = 0;

/* Message texts: */
const std::string CCommandLineService::MessageTexts::sCAN_NOT_WRITE_FILE("Can not write file! ");
const std::string CCommandLineService::MessageTexts::sEMPTY_COMMAND_LINE_SWITCH("Empty command line switch! ");
const std::string CCommandLineService::MessageTexts::sINVALID_COMMAND_LINE_SWITCH("Invalid command line switch! ");
const std::string CCommandLineService::MessageTexts::sINVALID_COMMAND_LINE_SWITCH_VALUE("Invalid command line swtich value! ");
const std::string CCommandLineService::MessageTexts::sMISSING_COMMAND_LINE_SWITCH_VALUES("Missing command line switch values! ");

/* Tag texts: */
const std::string CCommandLineService::TagTexts::sCOMMENT_INDICATOR("#");
const std::string CCommandLineService::TagTexts::sHEADER("{MoleculeName}; {QueryVolume}; {DbMoleculeVolume}; {OverlapVolume}");
const std::string CCommandLineService::TagTexts::sQUERY("@QUERY");
const std::string CCommandLineService::TagTexts::sTIME_PER_CONFORMER("@TIME_PER_CONFORMER");
const std::string CCommandLineService::TagTexts::sTOTAL_MOLECULES("@TOTAL_MOLECULES");
const std::string CCommandLineService::TagTexts::sTOTAL_TIME("@TOTAL_TIME");

/* Switch names: */
const std::string CCommandLineService::SwitchNames::sDATABASE("-db");
const std::string CCommandLineService::SwitchNames::sDB_RANGE("-dbRange");
const std::string CCommandLineService::SwitchNames::sFIT("-fit");
const std::string CCommandLineService::SwitchNames::sGAUSSIAN_VOLUME("-gVolume");
const std::string CCommandLineService::SwitchNames::sOUTPUT("-output");
const std::string CCommandLineService::SwitchNames::sPOCKET("-pocket");
const std::string CCommandLineService::SwitchNames::sQUERY("-query");
const std::string CCommandLineService::SwitchNames::sREFERENCE("-ref");
const std::string CCommandLineService::SwitchNames::sSH_DESCRIPTOR("-shDesc");
const std::string CCommandLineService::SwitchNames::sUSR_DESCRIPTOR("-usrDesc");


/**
 * Description: Ctor.
 * @param configurationArguments: (IN)
 */
CCommandLineService::CCommandLineService(const CConfigurationArguments& configurationArguments)
	: _configurationArguments(configurationArguments)
{
}


/**
 * Description: Dtor.
 */
CCommandLineService::~CCommandLineService()
{
}


/**
 * Description: Start main operation from command line.
 * @param commandLineArguments: (IN)
 */
int CCommandLineService::startFromCommandLine(const CCommandLineArguments& commandLineArguments)
{
	/* TODO: Do Gaussian volume overlap evaluation. */
	if (commandLineArguments.existSwitch(SwitchNames::sGAUSSIAN_VOLUME))
	{
		// If necessary command line switches exist:
		if (!commandLineArguments.isEmptySwitch(SwitchNames::sQUERY) 
			&& !commandLineArguments.isEmptySwitch(SwitchNames::sDATABASE) 
			&& !commandLineArguments.isEmptySwitch(SwitchNames::sOUTPUT))
		{
			// start ID limit for database molecule
			int nDbMoleculeStartIdLimit = 0;
			// end ID limit for database molecule
			int nDbMoleculeEndIdLimit = std::numeric_limits<int>::max();

			/* Handle DB_RANGE switch. */
			// If specified switch (database molecule ID range limit) exists:
			if (commandLineArguments.existSwitch(SwitchNames::sDB_RANGE))
			{
				// If valid switch value number:
				if (commandLineArguments.getArgumentsCount(SwitchNames::sDB_RANGE) >= 2)
				{
					const vector<string> dbMoleculeIdLimits = commandLineArguments.getArguments(SwitchNames::sDB_RANGE);
					const int nConversionError_0 = CUtility::parseString(dbMoleculeIdLimits[0], nDbMoleculeStartIdLimit);
					const int nConversionError_1 = CUtility::parseString(dbMoleculeIdLimits[1], nDbMoleculeEndIdLimit);
					// If conversion success:
					if (nConversionError_0 == CUtility::ErrorCodes::nNORMAL && nConversionError_1 == CUtility::ErrorCodes::nNORMAL)
					{
						// If switch values are logical:
						if (nDbMoleculeEndIdLimit >= nDbMoleculeStartIdLimit && nDbMoleculeStartIdLimit >= 0)
						{
						}
						// If swtich values are illogical:
						else
						{
							std::stringstream msgStream;
							msgStream 
								<< LOCATION_STREAM_INSERTION
								<< MessageTexts::sINVALID_COMMAND_LINE_SWITCH_VALUE
								<< SwitchNames::sDB_RANGE << " "
								<< dbMoleculeIdLimits[0] << " "
								<< dbMoleculeIdLimits[1];
							throw CInvalidCommandLineSwitchException(msgStream.str());
						}
					}
					// If conversion failure:
					else
					{
						std::stringstream msgStream;
						msgStream 
							<< LOCATION_STREAM_INSERTION
							<< MessageTexts::sINVALID_COMMAND_LINE_SWITCH_VALUE
							<< SwitchNames::sDB_RANGE << " "
							<< dbMoleculeIdLimits[0] << " "
							<< dbMoleculeIdLimits[1];
						throw CInvalidCommandLineSwitchException(msgStream.str());
					}
				}
				// If invalid switch value number:
				else
				{
					std::stringstream msgStream;
					msgStream 
						<< LOCATION_STREAM_INSERTION
						<< MessageTexts::sMISSING_COMMAND_LINE_SWITCH_VALUES
						<< SwitchNames::sDB_RANGE;
					throw CInvalidCommandLineSwitchException(msgStream.str());
				}
			}

			/* Construct output file stream. */
			const string sOutputFileName = commandLineArguments.getArguments(SwitchNames::sOUTPUT)[0];
			std::fstream outputStream(sOutputFileName.c_str(), std::ios_base::out);

			// If output stream OK:
			if (outputStream.good())
			{
				/* Construct reader for query molecule. */
				const string sQueryFileName = commandLineArguments.getArguments(SwitchNames::sQUERY)[0];
				auto_ptr<IMoleculeReader> queryMoleculeReaderPtr = CMoleculeReaderManager::getMoleculeReader(sQueryFileName);
				queryMoleculeReaderPtr->setReadHydrogenFlag(true);

				/* Construct reader for database molecule. */
				const string sDbFileName = commandLineArguments.getArguments(SwitchNames::sDATABASE)[0];
				auto_ptr<IMoleculeReader> dbMoleculeReaderPtr = CMoleculeReaderManager::getMoleculeReader(sDbFileName);
				dbMoleculeReaderPtr->setReadHydrogenFlag(false);
				dbMoleculeReaderPtr->locateMolecule(nDbMoleculeStartIdLimit);

				// for storing query molecule
				auto_ptr<IMolecule> queryMoleculePtr = CMoleculeManager::getMolecule();
				// for storing database molecule
				auto_ptr<IMolecule> dbMoleculePtr = CMoleculeManager::getMolecule();

				// main service
				CGaussianService gaussianService(getConfigurationArguments());
				// For each query molecule:
				while (queryMoleculeReaderPtr->readMolecule(*queryMoleculePtr) == IMoleculeReader::ErrorCodes::nNORMAL)
				{
					const double dQueryMoleculeVolume = gaussianService.evaluateGaussianVolume(*queryMoleculePtr);

					/* Output information for query molecule. */
					outputStream 
						<< TagTexts::sCOMMENT_INDICATOR << " "
						<< TagTexts::sQUERY << " "
						<< queryMoleculePtr->getMolecularName() << endl;
					outputStream
						<< TagTexts::sCOMMENT_INDICATOR << " "
						<< TagTexts::sHEADER << endl;

					// ID of current database molecule
					int nDbMoleculeId = nDbMoleculeStartIdLimit;
					double dTimeTotal = 0;
					// For each database molecule:
					while (dbMoleculeReaderPtr->readMolecule(*dbMoleculePtr) == IMoleculeReader::ErrorCodes::nNORMAL)
					{
						// If current database molecule is still in range limit:
						if (nDbMoleculeId <= nDbMoleculeEndIdLimit)
						{
							TIME_START();
							const double dDbMoleculeVolume = gaussianService.evaluateGaussianVolume(*dbMoleculePtr);
							const double dOverlapVolume = gaussianService.evaluateMaxGaussianVolumeOverlap(*queryMoleculePtr, *dbMoleculePtr);
							TIME_TICKS(dTicks);

							++ nDbMoleculeId;
							dTimeTotal += dTicks;

							/* Output for each database molecule. */
							outputStream 
								<< dbMoleculePtr->getMolecularName() << "; "
								<< dQueryMoleculeVolume << "; "
								<< dDbMoleculeVolume << "; "
								<< dOverlapVolume << endl;
						}
						// If current database molecule is out of range limit:
						else
						{
							break;
						}
					}

					/* Get statistics. */
					// computation time in seconds.
					dTimeTotal /= CLOCKS_PER_SEC;
					// total database molecules
					const int nTotalDbMolecules = nDbMoleculeId - nDbMoleculeStartIdLimit + 1;

					/* Output statistics for this query. */
					outputStream
						<< TagTexts::sCOMMENT_INDICATOR << " "
						<< TagTexts::sTOTAL_MOLECULES << " "
						<< nTotalDbMolecules << endl;
					outputStream
						<< TagTexts::sCOMMENT_INDICATOR << " "
						<< TagTexts::sTOTAL_TIME << " "
						<< dTimeTotal << endl;
					outputStream
						<< TagTexts::sCOMMENT_INDICATOR << " "
						<< TagTexts::sTIME_PER_CONFORMER << " "
						<< dTimeTotal / nTotalDbMolecules << endl;
				}
			}
			// If output stream failure:
			else
			{
				std::stringstream msgStream;
				msgStream
					<< LOCATION_STREAM_INSERTION
					<< MessageTexts::sCAN_NOT_WRITE_FILE
					<< sOutputFileName;
				throw CFileIoException(msgStream.str());
			}
		}
		// If not enough command line switches:
		else
		{
			std::stringstream msgStream;
			msgStream
				<< LOCATION_STREAM_INSERTION
				<< MessageTexts::sEMPTY_COMMAND_LINE_SWITCH
				<< SwitchNames::sQUERY
				<< "; "
				<< SwitchNames::sDATABASE;
			throw CInvalidCommandLineSwitchException(msgStream.str());
		}
	}

	/* TODO: Do SH descriptors generation. */
	if (commandLineArguments.existSwitch(SwitchNames::sSH_DESCRIPTOR))
	{
		// If necessary command line switches exist:
		if (!commandLineArguments.isEmptySwitch(SwitchNames::sDATABASE)
			&& !commandLineArguments.isEmptySwitch(SwitchNames::sOUTPUT))
		{
			// start ID limit for database molecule
			int nDbMoleculeStartIdLimit = 0;
			// end ID limit for database molecule
			int nDbMoleculeEndIdLimit = std::numeric_limits<int>::max();

			/* Handle DB_RANGE switch. */
			// If specified switch (database molecule ID range limit) exists:
			if (commandLineArguments.existSwitch(SwitchNames::sDB_RANGE))
			{
				// If valid switch value number:
				if (commandLineArguments.getArgumentsCount(SwitchNames::sDB_RANGE) >= 2)
				{
					const vector<string> dbMoleculeIdLimits = commandLineArguments.getArguments(SwitchNames::sDB_RANGE);
					const int nConversionError_0 = CUtility::parseString(dbMoleculeIdLimits[0], nDbMoleculeStartIdLimit);
					const int nConversionError_1 = CUtility::parseString(dbMoleculeIdLimits[1], nDbMoleculeEndIdLimit);
					// If conversion success:
					if (nConversionError_0 == CUtility::ErrorCodes::nNORMAL && nConversionError_1 == CUtility::ErrorCodes::nNORMAL)
					{
						// If switch values are logical:
						if (nDbMoleculeEndIdLimit >= nDbMoleculeStartIdLimit && nDbMoleculeStartIdLimit >= 0)
						{
						}
						// If switch values are illogical:
						else
						{
							std::stringstream msgStream;
							msgStream 
								<< LOCATION_STREAM_INSERTION
								<< MessageTexts::sINVALID_COMMAND_LINE_SWITCH_VALUE
								<< SwitchNames::sDB_RANGE << " "
								<< dbMoleculeIdLimits[0] << " "
								<< dbMoleculeIdLimits[1];
							throw CInvalidCommandLineSwitchException(msgStream.str());
						}
					}
					// If conversion failure:
					else
					{
						std::stringstream msgStream;
						msgStream 
							<< LOCATION_STREAM_INSERTION
							<< MessageTexts::sINVALID_COMMAND_LINE_SWITCH_VALUE
							<< SwitchNames::sDB_RANGE << " "
							<< dbMoleculeIdLimits[0] << " "
							<< dbMoleculeIdLimits[1];
						throw CInvalidCommandLineSwitchException(msgStream.str());
					}
				}
				// If invalid switch value number:
				else
				{
					std::stringstream msgStream;
					msgStream 
						<< LOCATION_STREAM_INSERTION
						<< MessageTexts::sMISSING_COMMAND_LINE_SWITCH_VALUES
						<< SwitchNames::sDB_RANGE;
					throw CInvalidCommandLineSwitchException(msgStream.str());
				}
			}

			/* Construct output file stream. */
			const string sOutputFileName = commandLineArguments.getArguments(SwitchNames::sOUTPUT)[0];
			std::fstream outputStream(sOutputFileName.c_str(), std::ios_base::out);

			// If output stream OK:
			if (outputStream.good())
			{
				/* Construct reader for database molecule. */
				const string sDbFileName = commandLineArguments.getArguments(SwitchNames::sDATABASE)[0];
				auto_ptr<IMoleculeReader> dbMoleculeReaderPtr = CMoleculeReaderManager::getMoleculeReader(sDbFileName);
				dbMoleculeReaderPtr->setReadHydrogenFlag(false);
				dbMoleculeReaderPtr->locateMolecule(nDbMoleculeStartIdLimit);

				// for storing database molecule
				auto_ptr<IMolecule> dbMoleculePtr = CMoleculeManager::getMolecule();

				// ID of current database molecule
				int nDbMoleculeId = nDbMoleculeStartIdLimit;
				double dTimeTotal = 0;
				// main service
				CSphericalHarmonicService sphericalHarmonicService(getConfigurationArguments());
				// For each database molecule:
				while (dbMoleculeReaderPtr->readMolecule(*dbMoleculePtr) == IMoleculeReader::ErrorCodes::nNORMAL)
				{
					// If current database molecule is still in range limit:
					if (nDbMoleculeId <= nDbMoleculeEndIdLimit)
					{
						// spherical harmonic descriptor for this molecule
						vector<double> shDescriptor;
						TIME_START();
						sphericalHarmonicService.evaluateShMolecularDescriptor(*dbMoleculePtr, shDescriptor);
						TIME_TICKS(dTicks);

						++ nDbMoleculeId;
						dTimeTotal += dTicks;

						/* Output for each database molecule. */
						outputStream 
							<< dbMoleculePtr->getMolecularName();
						for (int iComponent = 0; iComponent < static_cast<int>(shDescriptor.size()); ++ iComponent)
						{
							outputStream
								<< "; "
								<< shDescriptor[iComponent];
						}
						outputStream 
							<< endl;
					}
					// If current database molecule is out of range limit:
					else
					{
						break;
					}
				}

				/* Get statistics. */
				// computation time in seconds.
				dTimeTotal /= CLOCKS_PER_SEC;
				// total database molecules
				const int nTotalDbMolecules = nDbMoleculeId - nDbMoleculeStartIdLimit + 1;

				/* Output statistics. */
				outputStream
					<< TagTexts::sCOMMENT_INDICATOR << " "
					<< TagTexts::sTOTAL_MOLECULES << " "
					<< nTotalDbMolecules << endl;
				outputStream
					<< TagTexts::sCOMMENT_INDICATOR << " "
					<< TagTexts::sTOTAL_TIME << " "
					<< dTimeTotal << endl;
				outputStream
					<< TagTexts::sCOMMENT_INDICATOR << " "
					<< TagTexts::sTIME_PER_CONFORMER << " "
					<< dTimeTotal / nTotalDbMolecules << endl;
			}
			// If output stream fails:
			else
			{
				std::stringstream msgStream;
				msgStream
					<< LOCATION_STREAM_INSERTION
					<< MessageTexts::sCAN_NOT_WRITE_FILE
					<< sOutputFileName;
				throw CFileIoException(msgStream.str());
			}
		}
		// If not enough command line switches
		else
		{
			std::stringstream msgStream;
			msgStream
				<< LOCATION_STREAM_INSERTION
				<< MessageTexts::sEMPTY_COMMAND_LINE_SWITCH
				<< SwitchNames::sDATABASE
				<< "; "
				<< SwitchNames::sOUTPUT;
			throw CInvalidCommandLineSwitchException(msgStream.str());
		}
	}

	/* TODO: Do USR descriptors generation. */
	if (commandLineArguments.existSwitch(SwitchNames::sUSR_DESCRIPTOR))
	{
		// If necessary command line switches exist:
		if (!commandLineArguments.isEmptySwitch(SwitchNames::sDATABASE)
			&& !commandLineArguments.isEmptySwitch(SwitchNames::sOUTPUT))
		{
			// start ID limit for database molecule
			int nDbMoleculeStartIdLimit = 0;
			// end ID limit for database molecule
			int nDbMoleculeEndIdLimit = std::numeric_limits<unsigned int>::max();

			/* Handle DB_RANGE switch. */
			// If specified switch (database molecule ID range limit) exists:
			if (commandLineArguments.existSwitch(SwitchNames::sDB_RANGE))
			{
				// If valid switch value number:
				if (commandLineArguments.getArgumentsCount(SwitchNames::sDB_RANGE) >= 2)
				{
					const vector<string> dbMoleculeIdLimits = commandLineArguments.getArguments(SwitchNames::sDB_RANGE);
					const int nConversionError_0 = CUtility::parseString(dbMoleculeIdLimits[0], nDbMoleculeStartIdLimit);
					const int nConversionError_1 = CUtility::parseString(dbMoleculeIdLimits[1], nDbMoleculeEndIdLimit);
					// If conversion success:
					if (nConversionError_0 == CUtility::ErrorCodes::nNORMAL && nConversionError_1 == CUtility::ErrorCodes::nNORMAL)
					{
						// If switch values are logical:
						if (nDbMoleculeEndIdLimit >= nDbMoleculeStartIdLimit && nDbMoleculeStartIdLimit >= 0)
						{
						}
						// If switch values are illogical:
						else
						{
							std::stringstream msgStream;
							msgStream 
								<< LOCATION_STREAM_INSERTION
								<< MessageTexts::sINVALID_COMMAND_LINE_SWITCH_VALUE
								<< SwitchNames::sDB_RANGE << " "
								<< dbMoleculeIdLimits[0] << " "
								<< dbMoleculeIdLimits[1];
							throw CInvalidCommandLineSwitchException(msgStream.str());
						}
					}
					// If conversion failure:
					else
					{
						std::stringstream msgStream;
						msgStream 
							<< LOCATION_STREAM_INSERTION
							<< MessageTexts::sINVALID_COMMAND_LINE_SWITCH_VALUE
							<< SwitchNames::sDB_RANGE << " "
							<< dbMoleculeIdLimits[0] << " "
							<< dbMoleculeIdLimits[1];
						throw CInvalidCommandLineSwitchException(msgStream.str());
					}
				}
				// If invalid switch value number:
				else
				{
					std::stringstream msgStream;
					msgStream 
						<< LOCATION_STREAM_INSERTION
						<< MessageTexts::sMISSING_COMMAND_LINE_SWITCH_VALUES
						<< SwitchNames::sDB_RANGE;
					throw CInvalidCommandLineSwitchException(msgStream.str());
				}
			}

			/* Construct output file stream. */
			const string sOutputFileName = commandLineArguments.getArguments(SwitchNames::sOUTPUT)[0];
			std::fstream outputStream(sOutputFileName.c_str(), std::ios_base::out);

			// If output stream OK:
			if (outputStream.good())
			{
				/* Construct reader for database molecule. */
				const string sDbFileName = commandLineArguments.getArguments(SwitchNames::sDATABASE)[0];
				auto_ptr<IMoleculeReader> dbMoleculeReaderPtr = CMoleculeReaderManager::getMoleculeReader(sDbFileName);
				dbMoleculeReaderPtr->setReadHydrogenFlag(false);
				dbMoleculeReaderPtr->locateMolecule(nDbMoleculeStartIdLimit);

				// for storing database molecule
				auto_ptr<IMolecule> dbMoleculePtr = CMoleculeManager::getMolecule();

				// ID of current database molecule
				int nDbMoleculeId = nDbMoleculeStartIdLimit;
				// main service
				CUsrService usrService;
				// For each database molecule:
				while (dbMoleculeReaderPtr->readMolecule(*dbMoleculePtr) == IMoleculeReader::ErrorCodes::nNORMAL)
				{
					// If current database molecule is still in range limit:
					if (nDbMoleculeId <= nDbMoleculeEndIdLimit)
					{
						// USR descriptor for this molecule
						vector<double> usrDescriptor;
						usrService.evaluateUsrMolecularDescriptor(*dbMoleculePtr, usrDescriptor);

						++ nDbMoleculeId;

						/* Output for each database molecule. */
						outputStream 
							<< dbMoleculePtr->getMolecularName();
						for (int iComponent = 0; iComponent < static_cast<int>(usrDescriptor.size()); ++ iComponent)
						{
							outputStream
								<< "; "
								<< usrDescriptor[iComponent];
						}
						outputStream 
							<< endl;
					}
					// If current database molecule is out of range limit:
					else
					{
						break;
					}
				}
			}
			// If output stream fails:
			else
			{
				std::stringstream msgStream;
				msgStream
					<< LOCATION_STREAM_INSERTION
					<< MessageTexts::sCAN_NOT_WRITE_FILE
					<< sOutputFileName;
				throw CFileIoException(msgStream.str());
			}
		}
		// If not enough command line switches
		else
		{
			std::stringstream msgStream;
			msgStream
				<< LOCATION_STREAM_INSERTION
				<< MessageTexts::sEMPTY_COMMAND_LINE_SWITCH
				<< SwitchNames::sDATABASE
				<< "; "
				<< SwitchNames::sOUTPUT;
			throw CInvalidCommandLineSwitchException(msgStream.str());
		}
	}


	/* TODO: Pocket comparison. */
	if (commandLineArguments.existSwitch(SwitchNames::sPOCKET))
	{
		// If necessary command line switches exist:
		if (commandLineArguments.getArgumentsCount(SwitchNames::sREFERENCE) >= 2
			&& commandLineArguments.getArgumentsCount(SwitchNames::sFIT) >= 2
			&& !commandLineArguments.isEmptySwitch(SwitchNames::sOUTPUT))
		{
			/* Construct output stream. */
			const string sOutputFileName = commandLineArguments.getArguments(SwitchNames::sOUTPUT)[0];
			std::fstream outputStream(sOutputFileName.c_str(), std::ios_base::out);

			// If output stream OK:
			if (outputStream.good())
			{
				/* Read reference pocket volume. */
				const string sRefPocketVolumeFileName = commandLineArguments.getArguments(SwitchNames::sREFERENCE)[0];
				auto_ptr<IMoleculeReader> refPocketVolumeReaderPtr = CMoleculeReaderManager::getMoleculeReader(sRefPocketVolumeFileName);
				refPocketVolumeReaderPtr->setReadHydrogenFlag(true);
				auto_ptr<IMolecule> refPocketVolumePtr = CMoleculeManager::getMolecule();
				refPocketVolumeReaderPtr->readMolecule(*refPocketVolumePtr);

				/* Read reference pocket. */
				const string sRefPocketFileName = commandLineArguments.getArguments(SwitchNames::sREFERENCE)[1];
				auto_ptr<IMoleculeReader> refPocketReaderPtr = CMoleculeReaderManager::getMoleculeReader(sRefPocketFileName);
				refPocketReaderPtr->setReadHydrogenFlag(true);
				auto_ptr<IMolecule> refPocketPtr = CMoleculeManager::getMolecule();
				refPocketReaderPtr->readMolecule(*refPocketPtr);

				/* Read fit pocket volume. */
				const string sFitPocketVolumeFileName = commandLineArguments.getArguments(SwitchNames::sFIT)[0];
				auto_ptr<IMoleculeReader> fitPocketVolumeReaderPtr = CMoleculeReaderManager::getMoleculeReader(sFitPocketVolumeFileName);
				fitPocketVolumeReaderPtr->setReadHydrogenFlag(true);
				auto_ptr<IMolecule> fitPocketVolumePtr = CMoleculeManager::getMolecule();
				fitPocketVolumeReaderPtr->readMolecule(*fitPocketVolumePtr);

				/* Read fit pocket. */
				const string sFitPocketFileName = commandLineArguments.getArguments(SwitchNames::sFIT)[1];
				auto_ptr<IMoleculeReader> fitPocketReaderPtr = CMoleculeReaderManager::getMoleculeReader(sFitPocketFileName);
				fitPocketReaderPtr->setReadHydrogenFlag(true);
				auto_ptr<IMolecule> fitPocketPtr = CMoleculeManager::getMolecule();
				fitPocketReaderPtr->readMolecule(*fitPocketPtr);

				/* Do Gaussian alignment. */
				// optimal transformation for fit molecule
				vector<vector<double> > optimalFitTransformations;
				CGaussianService gaussianService(getConfigurationArguments());
				double dSimilarity = gaussianService.evaluatePocketComboSimilarity(
					*refPocketVolumePtr,
					*refPocketPtr,
					*fitPocketVolumePtr,
					*fitPocketPtr,
					&optimalFitTransformations
					);

				/* Output result. */
				outputStream << dSimilarity << std::endl;
				for (int iTransformation = 0; iTransformation < static_cast<int>(optimalFitTransformations.size()); ++ iTransformation)
				{
					for (int iDimension = 0; iDimension < static_cast<int>(optimalFitTransformations[iTransformation].size()); ++ iDimension)
					{
						outputStream << optimalFitTransformations[iTransformation][iDimension] << "; ";
					}
					outputStream << std::endl;
				}
			}
			// If output stream fails:
			else
			{
				std::stringstream msgStream;
				msgStream
					<< LOCATION_STREAM_INSERTION
					<< MessageTexts::sCAN_NOT_WRITE_FILE
					<< sOutputFileName;
				throw CFileIoException(msgStream.str());
			}
		}
		// If not enough command line switches:
		else
		{
			std::stringstream msgStream;
			msgStream
				<< LOCATION_STREAM_INSERTION
				<< "Not enough arguments for command line switch! "
				<< SwitchNames::sREFERENCE
				<< "; "
				<< SwitchNames::sFIT
				<< "; "
				<< SwitchNames::sOUTPUT;
			throw CInvalidCommandLineSwitchException(msgStream.str());
		}
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description: Get configuration arguments.
 * @return: Configuration arguments.
 */
const CConfigurationArguments& CCommandLineService::getConfigurationArguments() const
{
	return _configurationArguments;
}
