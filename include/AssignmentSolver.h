/**
 * Assignment Solver Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file AssignmentSolver.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-06-27
 */


#include <map>
#include <set>
#include <vector>


class CAssignmentSolver
{
	/* data: */
public:
	/* Error Codes. */
	struct ErrorCodes
	{
		static const int nNORMAL;

		friend class CAssignmentSolver;

	private:
		static const int nNO_MORE_ELEMENT;

	private:
		ErrorCodes() {};
	};


private:
	// IDs of all columns have been assigned
	std::set<int> _assignmentColumnIdsSet;
	// Assignment. Key: column index of assignment; Value: row index of assignment.
	std::map<int, int> _assignmentColumnIdsMap;
	// IDs of all rows have been assigned
	std::set<int> _assignmentRowIdsSet;
	// a flag indicating multiple runs of optimization (the cost matrix is already optimal now)
	bool _bMultiRuns;
	// a flag indicating whether the raw cost matrix has been transposed
	bool _bTransposedCostMatrix;
	// cost matrix (number of rows is always less than or equal to number of columns)
	std::vector<std::vector<double> > _costMatrix;
	// set contains full column indices
	std::set<int> _fullColumnIndicesSet;
	// set contains full row indices
	std::set<int> _fullRowIndicesSet;
	// IDs of all marked columns
	std::set<int> _markedColumnIdsSet;
	// IDs of all marked rows
	std::set<int> _markedRowIdsSet;
	// number of columns of raw cost matrix
	int _nColumns;
	// number of dimensions of assignment problem
	int _nDimensions;
	// number of columns of raw cost matrix
	int _nRows;
	// original cost matrix (may not be squared)
	const std::vector<std::vector<double> >* _pRawCostMatrix;
	// Position of the zero cost. Key: row index of zero cost; Value: set of column indices of zero cost.
	std::map<int, std::set<int> > _zeroColumnIdsMap;
	// Position of the zero cost. Key: column index of zero cost; Value: set of row indices of zero cost.
	std::map<int, std::set<int> > _zeroRowIdsMap;

	/* method: */
public:
	CAssignmentSolver(const std::vector<std::vector<double> >& rawCostMatrix);
	~CAssignmentSolver();

	double evaluateOptimalAssignments(std::map<int, int>& assignmentsMap);
	int setCostMatrix(const std::vector<std::vector<double> >& rawCostMatrix);
private:
	int assignOneArbitrarily();
	int assignUniquely();
	bool assignUniquelyByColumn();
	bool assignUniquelyByRow();
	int adjustCostMatrix();
	std::map<int, int> getOptimalAssignments();
	int markColumns();
	int markRows();
	int markRowsAndColumns();
	int minimumLineCover();
	int preProcessCostMatrix();
	int recordAssignmentPosition(const int iRowId, const int iColumnId);
	int recordZeroPosition(const int iRowId, const int iColumnId);
	int removeZeroPosition(const int iRowId, const int iColumnId);
	int subtractRowAndColumnMinimum();
	int tryAssign();
};
