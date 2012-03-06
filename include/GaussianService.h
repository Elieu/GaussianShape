/**
 * Gaussian Service Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file Service.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-02-21
 */

#ifndef SERVICE_INCLUDE_H
#define SERVICE_INCLUDE_H
//


#include <map>
#include <string>
#include <vector>


class CConfigurationArguments;
class IMolecule;


/**
 * Description:
 */
class CGaussianService
{
	/* data: */
public:
	/* Error codes. */
	struct ErrorCodes
	{
		static const int nINVALID_CONFIGURATION_ARGUMENT;
		static const int nNORMAL;

	private:
		ErrorCodes() {};
	};


private:
	/* Default values. */
	struct DefaultValues
	{
		// for parameter "dSimplexContractionFactor"
		static const double dSIMPLEX_CONTRACTION_FACTOR;
		// for parameter "dSimplexExtensionFactor"
		static const double dSIMPLEX_EXTENSION_FACTOR;
		// for parameter "dSimplexReflectionFactor"
		static const double dSIMPLEX_REFLECTION_FACTOR;
		// for parameter "nSimplexInitialSolutionGroupsNumber"
		static const int nSIMPLEX_INITIAL_SOLUTION_GROUPS_NUMBER;
		// for parameter "nSimplexMaxIterations"
		static const int nSIMPLEX_MAX_ITERATIONS;

	private:
		DefaultValues() {};
	};


	/* Message texts. */
	struct MessageTexts
	{
		static const std::string sBAD_CAST;
		static const std::string sEMPTY_MOLECULE;
		static const std::string sINVALID_ARGUMENT;
		static const std::string sNOT_IMOLECULE_INTERFACE;

	private:
		MessageTexts() {};
	};


	/**
	 * Description: Aggregation of all parameters.
	 */
	struct ParametersAggregation
	{
		// contraction factor for simplex optimization
		double dSimplexContractionFactor;
		// extension factor for simplex optimization
		double dSimplexExtensionFactor;
		// reflection factor for simplex optimization
		double dSimplexReflectionFactor;
		// number of initial solution group
		int nSimplexInitialSolutionGroupsNumber;
		// max iteration for simplex optimization
		int nSimplexMaxIterations;
	};


	/**
	 * Description: Collection of parameter names to look up for specified parameter in configuration file.
	 */
	struct ParameterNames
	{
		// for parameter "dSimplexContractionFactor"
		static const std::string sSIMPLEX_CONTRACTION_FACTOR;
		// for parameter "dSimplexExtensionFactor"
		static const std::string sSIMPLEX_EXTENSION_FACTOR;
		// for parameter "nSimplexInitialSolutionGroupsNumber"
		static const std::string sSIMPLEX_INITIAL_SOLUTION_GROUPS_NUMBER;
		// for parameter "nSimplexMaxIterations"
		static const std::string sSIMPLEX_MAX_ITERATIONS;
		// for parameter "dSimplexReflectionFactor"
		static const std::string sSIMPLEX_REFLECTION_FACTOR;

	private:
		ParameterNames() {};
	};


	// dimension for alignment problem (degree of freedom)
	static const int _nDIMENSIONS;

	// aggregation of parameters to be used in this service
	ParametersAggregation _parameterAggregation;
	
	/* method: */
public:
	CGaussianService();
	CGaussianService(const CConfigurationArguments& configArguments);
	~CGaussianService();

	int configure(const CConfigurationArguments& configurationArguments);
	double evaluateGaussianVolume(const IMolecule& molecule) const;
	double evaluateMaxGaussianVolumeOverlap(const IMolecule& refMol, const IMolecule& fitMol, std::vector<std::vector<double> >* pFitTransformations = NULL) const;
	double evaluatePocketComboSimilarity(const IMolecule& refPocketVolume, const IMolecule& refPocket, const IMolecule& fitPocketVolume, const IMolecule& fitPocket, std::vector<std::vector<double> >* pFitTransformations = NULL) const;
	std::map<std::string, std::string> getParametersMap() const;
	double getSimplexContractionFactor() const;
	double getSimplexExtensionFactor() const;
	int getSimplexInitialSolutionGroupsNumber() const;
	int getSimplexMaxIterations() const;
	double getSimplexReflectionFactor() const;
	void setSimplexContractionFactor(double dContractionFactor);
	void setSimplexExtensionFactor(double dExtensionFactor);
	void setSimplexInitialSolutionGroupsNumber(int nGroupsNumber);
	void setSimplexMaxIterations(int nMaxIterations);
	void setSimplexReflectionFactor(double dReflectionFactor);
private:
	int generateInitialSolutionGroups(int nGroups, int nSolutionsPerGroup, std::vector<std::vector<std::vector<double> > >& initialSolutionGroups) const;
	int initialize();
	int initParameters();
	int initParameters(const CConfigurationArguments& configArguments);
};


//
#endif
