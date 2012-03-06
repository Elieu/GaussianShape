/**
 * Assignment Solver Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file AssignmentSolver.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-06-27
 */


#include "AssignmentSolver.h"

#include "Exception.h"
#include "Utility.h"

#include <algorithm>
#include <iterator>
#include <limits>
#include <string>
#include <sstream>


using std::map;
using std::set;
using std::string;
using std::vector;


/* Static members: */
/* Error codes: */
const int CAssignmentSolver::ErrorCodes::nNORMAL = 0;
const int CAssignmentSolver::ErrorCodes::nNO_MORE_ELEMENT = 1;


/**
 * Description: Constructor.
 * @param rawCostMatrix: (IN) Cost matrix. It is the client's responsibility to maintain this matrix accessible during whole optimization process.
 * @exception:
 * 	CInvalidArgumentException:
 */
CAssignmentSolver::CAssignmentSolver(const std::vector<std::vector<double> >& rawCostMatrix)
{
	try
	{
		setCostMatrix(rawCostMatrix);
	}
	catch(CInvalidArgumentException& exception)
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "Invalid arguments! "
			<< "Caused by: \n"
			<< exception.getErrorMessage();
		throw CInvalidArgumentException(msgStream.str());
	}
}


/**
 * Description: Destructor.
 */
CAssignmentSolver::~CAssignmentSolver()
{
}


/**
 * Description:
 * @return:
 */
int CAssignmentSolver::assignOneArbitrarily()
{
	// For each row containing zero cost:
	for (map<int, set<int> >::const_iterator iterZeroRowMap = _zeroRowIdsMap.begin();
		iterZeroRowMap != _zeroRowIdsMap.end();
		++ iterZeroRowMap)
	{
		// row ID of zero cost
		const int& iZeroRowId = iterZeroRowMap->first;

		// If this row has not been assigned:
		if (!EXIST(iZeroRowId, _assignmentRowIdsSet))
		{
			// column IDs set of zero cost
			const set<int>& zeroColumnIdsSet = iterZeroRowMap->second;

			// For each column containing zero in this row:
			FOREACH(iterZeroColumnIds, zeroColumnIdsSet, set<int>::const_iterator)
			{
				// column ID of zero cost
				const int& iZeroColumnId = *iterZeroColumnIds;

				// If this column has not been assigned:
				if (!EXIST(iZeroColumnId, _assignmentColumnIdsSet))
				{
					recordAssignmentPosition(iZeroRowId, iZeroColumnId);

					return _assignmentRowIdsSet.size();
				}
			}
		}
	}

	// For each column containing zero cost:
	for (map<int, set<int> >::const_iterator iterZeroColumnMap = _zeroColumnIdsMap.begin();
		iterZeroColumnMap != _zeroColumnIdsMap.end();
		++ iterZeroColumnMap)
	{
		// column ID of zero cost
		const int& iZeroColumnId = iterZeroColumnMap->first;

		// If this column has not been assigned:
		if (!EXIST(iZeroColumnId, _assignmentColumnIdsSet))
		{
			// row IDs set of zero cost
			const set<int>& zeroRowIdsSet = iterZeroColumnMap->second;

			// For each row containing zero in this column:
			FOREACH(iterZeroRowIds, zeroRowIdsSet, set<int>::const_iterator)
			{
				const int& iZeroRwoId = *iterZeroRowIds;

				// If this row has not been assigned:
				if (!EXIST(iZeroRwoId, _assignmentRowIdsSet))
				{
					recordAssignmentPosition(iZeroRwoId, iZeroColumnId);

					return _assignmentRowIdsSet.size();
				}
			}
		}
	}

	return _assignmentRowIdsSet.size();
}


/**
 * Description:
 */
int CAssignmentSolver::assignUniquely()
{
	// indicating new unique assignment by row may still be made:
	bool bNewAssignmentByRow = true;
	// indicating new unique assignment by column may still be made:
	bool bNewAssignmentByColumn = true;

	// While new unique assignment may still be made:
	while (bNewAssignmentByRow || bNewAssignmentByColumn)
	{
		// While new unique assignment by row may still be made:
		while (assignUniquelyByRow())
		{
			bNewAssignmentByColumn = true;
		}
		bNewAssignmentByRow = false;

		// While new unique assignment by column may still be made:
		while (assignUniquelyByColumn())
		{
			bNewAssignmentByRow = true;
		}
		bNewAssignmentByColumn = false;
	}

	return _assignmentRowIdsSet.size();
}


/**
 * Description:
 * @return:
 */
bool CAssignmentSolver::assignUniquelyByColumn()
{
	// a flag indicating whether new assignment has been made
	bool bNewUniqueAssignment = false;

	// For each zero column:
	for (map<int, set<int> >::const_iterator iterZeroColumnIdMap = _zeroColumnIdsMap.begin();
		iterZeroColumnIdMap != _zeroColumnIdsMap.end();
		++ iterZeroColumnIdMap)
	{
		const int& iZeroColumnId = iterZeroColumnIdMap->first;

		// If this column has not been assigned:
		if (!EXIST(iZeroColumnId, _assignmentColumnIdsSet))
		{
			// corresponding zero row IDs set (may contain columns which has already been assigned)
			const set<int>& zeroRowIdsSet = iterZeroColumnIdMap->second;

			// corresponding unassigned zero row IDs set
			set<int> zeroRowIdsSetUnassigned;
			std::set_difference(
				zeroRowIdsSet.begin(),
				zeroRowIdsSet.end(),
				_assignmentRowIdsSet.begin(),
				_assignmentRowIdsSet.end(),
				std::inserter(zeroRowIdsSetUnassigned, zeroRowIdsSetUnassigned.begin())
				);

			// If unique assignment can be made:
			if (zeroRowIdsSetUnassigned.size() == 1)
			{
				const int& iZeroRowIdUnique = *zeroRowIdsSetUnassigned.begin();
				recordAssignmentPosition(iZeroRowIdUnique, iZeroColumnId);

				bNewUniqueAssignment = true;
			}
		}
	}

	return bNewUniqueAssignment;
}


/**
 * Description:
 * @return:
 */
bool CAssignmentSolver::assignUniquelyByRow()
{
	// a flag indicating whether new assignment has been made
	bool bNewUniqueAssignment = false;

	// For each zero cost row:
	for (map<int, set<int> >::const_iterator iterZeroRowIdMap = _zeroRowIdsMap.begin();
		iterZeroRowIdMap != _zeroRowIdsMap.end();
		++ iterZeroRowIdMap)
	{
		const int& iZeroRowId = iterZeroRowIdMap->first;

		// If this row has not been assigned:
		if (!EXIST(iZeroRowId, _assignmentRowIdsSet))
		{
			// corresponding zero column IDs set (may contain columns which has already been assigned)
			const set<int>& zeroColumnIdsSet = iterZeroRowIdMap->second;

			// corresponding unassigned zero column IDs set
			set<int> zeroColumnIdsSetUnassigned;
			std::set_difference(
				zeroColumnIdsSet.begin(),
				zeroColumnIdsSet.end(),
				_assignmentColumnIdsSet.begin(),
				_assignmentColumnIdsSet.end(),
				std::inserter(zeroColumnIdsSetUnassigned, zeroColumnIdsSetUnassigned.begin())
				);

			// If unique assignment can be made:
			if (zeroColumnIdsSetUnassigned.size() == 1)
			{
				const int& iZeroColumnIdUnique = *zeroColumnIdsSetUnassigned.begin();
				recordAssignmentPosition(iZeroRowId, iZeroColumnIdUnique);

				bNewUniqueAssignment = true;
			}
		}
	}

	return bNewUniqueAssignment;
}


/**
 * Description: Adjust cost matrix according to line cover.
 * @note: Minimum line cover must be drawn before calling this function.
 */
int CAssignmentSolver::adjustCostMatrix()
{
	/* Get line covered rows (unmarked rows) and columns (marked columns). */
	// IDs of line covered rows
	set<int> rowIdsSetLineCovered;
	std::set_difference(
		_fullRowIndicesSet.begin(),
		_fullRowIndicesSet.end(),
		_markedRowIdsSet.begin(),
		_markedRowIdsSet.end(),
		std::insert_iterator<set<int> >(rowIdsSetLineCovered, rowIdsSetLineCovered.begin())
		);
	// IDs of line covered columns
	const set<int>& columnIdsSetLineCovered = _markedColumnIdsSet;

	/* Get uncovered rows (marked rows) and columns (unmarked columns). */
	// IDs of uncovered columns
	set<int> columnIdsSetUncovered;
	std::set_difference(
		_fullColumnIndicesSet.begin(),
		_fullColumnIndicesSet.end(),
		_markedColumnIdsSet.begin(),
		_markedColumnIdsSet.end(),
		std::insert_iterator<set<int> >(columnIdsSetUncovered, columnIdsSetUncovered.begin())
		);
	// IDs of uncovered rows
	const set<int>& rowIdsSetUncovered = _markedRowIdsSet;

	/* Get UNMARKED rows and UNMARKED columns. */
	// IDs of unmarked rows
	//set<int>& rowIdsSetUnmarked = rowIdsSetLineCovered;
	// IDS of unmarked columns
	set<int> columnIdsSetUnmarked;
	std::set_difference(
		_fullColumnIndicesSet.begin(),
		_fullColumnIndicesSet.end(),
		_markedColumnIdsSet.begin(),
		_markedColumnIdsSet.end(),
		std::insert_iterator<set<int> >(columnIdsSetUnmarked, columnIdsSetUnmarked.begin())
		);

	/* Subtract minimum cost from UNMARKED elements. Note: It is "UNMARKED" instead of "uncovered"! */
	/* Note: This operation will produce a zero cost element! */
	// minimum value of uncovered element
	double dMinCostUncoverd = std::numeric_limits<double>::max();
	int iRow_minCostUncovered = -1;
	int iColumn_minCostUncovered = -1;
	/* Find minimum cost. */
	// For each uncovered element:
	FOREACH(iterRowId, rowIdsSetUncovered, set<int>::const_iterator)
	{
		const int& iRow = *iterRowId;

		FOREACH(iterColumnId, columnIdsSetUncovered, set<int>::const_iterator)
		{
			const int& iColumn = *iterColumnId;

			// Get minimum.
			if (_costMatrix[iRow][iColumn] < dMinCostUncoverd)
			{
				dMinCostUncoverd = _costMatrix[iRow][iColumn];
				iRow_minCostUncovered = iRow;
				iColumn_minCostUncovered = iColumn;
			}
		}
	}
	/* Subtract minimum cost. */
	// For each uncovered element:
	FOREACH(iterRowId, rowIdsSetUncovered, set<int>::const_iterator)
	{
		const int& iRow = *iterRowId;

		FOREACH(iterColumnId, columnIdsSetUncovered, set<int>::const_iterator)
		{
			const int& iColumn = *iterColumnId;

			_costMatrix[iRow][iColumn] -= dMinCostUncoverd;
		}
	}
	/* Keep track of zero cost. */
	_costMatrix[iRow_minCostUncovered][iColumn_minCostUncovered] = 0;
	recordZeroPosition(iRow_minCostUncovered, iColumn_minCostUncovered);

	/* Add uncovered minimum cost to elements covered by two lines. */
	FOREACH(iterRowId, rowIdsSetLineCovered, set<int>::const_iterator)
	{
		const int& iRow = *iterRowId;

		FOREACH(iterColumnId, columnIdsSetLineCovered, set<int>::const_iterator)
		{
			const int& iColumn = * iterColumnId;

			/* Keep track of zero cost. */
			if (_costMatrix[iRow][iColumn] == 0)
			{
				removeZeroPosition(iRow, iColumn);
			}

			_costMatrix[iRow][iColumn] += dMinCostUncoverd;
		}
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description: Calculate optimal assignments according to current cost matrix.
 * @note: In order to get correct assignments, the number of lines in minimum line cover must equal to the problem dimension before calling this function.
 * @return:
 */
std::map<int, int> CAssignmentSolver::getOptimalAssignments()
{
	_assignmentColumnIdsMap.clear();
	_assignmentColumnIdsSet.clear();
	_assignmentRowIdsSet.clear();

	int nAssignments = tryAssign();
	// If all assignments have been made:
	if (nAssignments == _nDimensions)
	{
		// optimal assignments (key: row ID of raw cost matrix; value: column ID of raw cost matrix)
		map<int, int> assignmentsMap;
		// For each assignment:
		for (map<int, int>::const_iterator iterAssignment = _assignmentColumnIdsMap.begin();
			iterAssignment != _assignmentColumnIdsMap.end();
			++ iterAssignment)
		{
			const int& iColumnIdAssignment = iterAssignment->first;
			const int& iRowIdAssignment = iterAssignment->second;

			// If cost matrix has been transposed:
			if (_bTransposedCostMatrix)
			{
				assignmentsMap[iColumnIdAssignment] = iRowIdAssignment;
			}
			// If cost matrix has not been transposed:
			else
			{
				assignmentsMap[iRowIdAssignment] = iColumnIdAssignment;
			}
		}

		return assignmentsMap;
	}
	// If there are assignments can not be made:
	/* Note: This exception should never happened, which indicates a defect of implementation. */
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "Logic error! "
			<< "Unable to  get enough optimal assignments! ";
		throw CLogicException(msgStream.str());
	}
}


/**
 * Description: Calculate the optimal assignments based on given cost matrix.
 * @param assignmentsMap: (OUT) Resulted optimal assignments (key: row ID; Value: column ID).
 * @return: Total cost.
 */
double CAssignmentSolver::evaluateOptimalAssignments(std::map<int, int>& assignmentsMap)
{
	// If first run:
	if (!_bMultiRuns)
	{
		preProcessCostMatrix();

		subtractRowAndColumnMinimum();
		while (minimumLineCover() < _nDimensions)
		{
			adjustCostMatrix();
			//subtractRowAndColumnMinimum();
		}

		_bMultiRuns = true;
	}

	assignmentsMap = getOptimalAssignments();

	/* Get total cost under these assignments. */
	double dCostTotal = 0;
	for (map<int, int>::const_iterator iterAssignment = assignmentsMap.begin();
		iterAssignment != assignmentsMap.end();
		++ iterAssignment)
	{
		const int& iRowIdAssignment = iterAssignment->first;
		const int& iColumnIdAssignment = iterAssignment->second;

		dCostTotal += (*_pRawCostMatrix)[iRowIdAssignment][iColumnIdAssignment];
	}

	return dCostTotal;
}


/**
 * Description: Mark all columns having zero in marked rows.
 * @return:
 * 	ErrorCodes::nNORMAL: New columns marked.
 * 	ErrorCodes::nNO_MORE_ELEMENT: No more new columns marked.
 */
int CAssignmentSolver::markColumns()
{
	// a flag indicating no more columns have been marked
	bool bNoMoreColumns = true;

	// For each marked row:
	FOREACH(iterMarkedRowId, _markedRowIdsSet, set<int>::const_iterator)
	{
		const int& iMarkedRowId = *iterMarkedRowId;

		// If we have zero in this row:
		if (EXIST(iMarkedRowId, _zeroRowIdsMap))
		{
			// zero columns for this row
			set<int>& zeroColumnIdsSet = _zeroRowIdsMap[iMarkedRowId];
			FOREACH(iterZeroColumnId, zeroColumnIdsSet, set<int>::const_iterator)
			{
				const int& iZeroColumnId = *iterZeroColumnId;

				// If new column marked:
				if (!EXIST(iZeroColumnId, _markedColumnIdsSet))
				{
					_markedColumnIdsSet.insert(iZeroColumnId);
					bNoMoreColumns = false;
				}
			}
		}
	}

	return bNoMoreColumns ? ErrorCodes::nNO_MORE_ELEMENT : ErrorCodes::nNORMAL;
}


/**
 * Description: mark all rows having assignment in marked columns.
 * @return:
 * 	ErrorCodes::nNORMAL: New rows marked.
 * 	ErrorCodes::nNO_MORE_ELEMENT: No more new rows marked.
 */
int CAssignmentSolver::markRows()
{
	// a flag indicating no more rows have been marked
	bool bNoMoreRows = true;

	// For each marked column:
	FOREACH(iterMarkedColumnId, _markedColumnIdsSet, set<int>::const_iterator)
	{
		const int& iMarkedColumnId = *iterMarkedColumnId;

		// If we have assignment in this column:
		if (EXIST(iMarkedColumnId, _assignmentColumnIdsMap))
		{
			// assignment row for this column
			const int& iAssignmentRowId = _assignmentColumnIdsMap[iMarkedColumnId];

			// If new row marked:
			if (!EXIST(iAssignmentRowId, _markedRowIdsSet))
			{
				_markedRowIdsSet.insert(iAssignmentRowId);
				bNoMoreRows = false;
			}
		}
	}

	return bNoMoreRows ? ErrorCodes::nNO_MORE_ELEMENT : ErrorCodes::nNORMAL;
}


/**
 * Description: Mark rows and columns for performing minimum line cover.
 * @return: Total number of marked columns and unmarked rows, which corresponds to the minimum line cover.
 */
int CAssignmentSolver::markRowsAndColumns()
{
	_markedRowIdsSet.clear();
	_markedColumnIdsSet.clear();

	/* Mark all rows having no assignments. */
	std::set_difference(
		_fullRowIndicesSet.begin(),
		_fullRowIndicesSet.end(),
		_assignmentRowIdsSet.begin(),
		_assignmentRowIdsSet.end(),
		std::insert_iterator<set<int> >(_markedRowIdsSet, _markedRowIdsSet.begin())
		);

	/* Mark more columns and rows alternately. */
	while (true)
	{
		int nErrorCode = -1;
		nErrorCode = markColumns();
		if (nErrorCode == ErrorCodes::nNO_MORE_ELEMENT)
		{
			break;
		}

		nErrorCode = markRows();
		if (nErrorCode == ErrorCodes::nNO_MORE_ELEMENT)
		{
			break;
		}
	}

	// number of lines (marked columns and unmarked rows) to cover zeros
	const int nMinLines = _markedColumnIdsSet.size() + _nDimensions - _markedRowIdsSet.size();

	return nMinLines;
}


/**
 * Description: Cover all zeros in cost matrix with minimum number of lines.
 * @return: Number of lines.
 */
int CAssignmentSolver::minimumLineCover()
{
	tryAssign();
	const int nMinLines = markRowsAndColumns();

	return nMinLines;
}


/**
 * Description: Pre-process the cost matrix.
 */
int CAssignmentSolver::preProcessCostMatrix()
{
//	/* Add dummy cost if not a squared cost matrix. */
//	if (_nRows != _nColumns)
//	{
//		/* Get maximum individual cost value. */
//		double dMaxIndividualCost = - std::numeric_limits<double>::max();
//		for (unsigned int iRow = 0; iRow < _nRows; ++ iRow)
//		{
//			for (unsigned int iColumn = 0; iColumn < _nColumns; ++ iColumn)
//			{
//				if (_costMatrix[iRow][iColumn] > dMaxIndividualCost)
//				{
//					dMaxIndividualCost = _costMatrix[iRow][iColumn];
//				}
//			}
//		}
//
//		/* Add dummy cost. */
//		const double dDummyCost = 2 * dMaxIndividualCost;
//		// If there are more columns than rows:
//		if (_nRows < _nColumns)
//		{
//			/* Add dummy rows. */
//			const unsigned int nDeltaDimensions = _nColumns - _nRows;
//			vector<double> dummyRow(_nDimensions, dDummyCost);
//			for (unsigned int iDeltaDimension = 0; iDeltaDimension < nDeltaDimensions; ++ iDeltaDimension)
//			{
//				_costMatrix.push_back(dummyRow);
//			}
//		}
//		// If there are more rows than columns:
//		else
//		{
//			/* Add dummy columns. */
//			const unsigned int nDeltaDimensions = _nRows - _nColumns;
//			for (unsigned int iRow = 0; iRow < _nRows; ++ iRow)
//			{
//				for (unsigned int iDeltaDimension = 0; iDeltaDimension < nDeltaDimensions; ++ iDeltaDimension)
//				{
//					_costMatrix[iRow].push_back(dDummyCost);
//				}
//			}
//		}
//	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description: Record the position of an assignment in cost matrix.
 * @param iRowId: (IN) Row ID of an assignment.
 * @param iColumnId: (IN) Column ID of an assignment.
 */
int CAssignmentSolver::recordAssignmentPosition(const int iRowId, const int iColumnId)
{
	/* Record row and column ID. */
	_assignmentColumnIdsSet.insert(iColumnId);
	_assignmentRowIdsSet.insert(iRowId);

	/* Record in column IDs map. */
	_assignmentColumnIdsMap[iColumnId] = iRowId;

	/* Check integrity. */
	/* Note: This should never happened if the implementation is correct. */
	if (_assignmentColumnIdsMap.size() != _assignmentRowIdsSet.size() ||
		_assignmentColumnIdsMap.size() != _assignmentColumnIdsSet.size())
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "Logic error! "
			<< "Internal representations of assignments are not identical! May be a defect of the implementation! ";
		throw CLogicException(msgStream.str());
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description: Record the position of an zero element in cost matrix.
 * @param iRowId: (IN) Row ID of a zero element.
 * @param iColumnId: (IN) Column ID of a zero element.
 */
int CAssignmentSolver::recordZeroPosition(const int iRowId, const int iColumnId)
{
	/* Record in row ID map. */
	if (EXIST(iRowId, _zeroRowIdsMap))
	{
		_zeroRowIdsMap[iRowId].insert(iColumnId);
	}
	else
	{
		_zeroRowIdsMap[iRowId] = set<int>();
		_zeroRowIdsMap[iRowId].insert(iColumnId);
	}

	/* Record in column ID map. */
	if (EXIST(iColumnId, _zeroColumnIdsMap))
	{
		_zeroColumnIdsMap[iColumnId].insert(iRowId);
	}
	else
	{
		_zeroColumnIdsMap[iColumnId] = set<int>();
		_zeroColumnIdsMap[iColumnId].insert(iRowId);
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description: Remove the record of a zero element in cost matrix.
 * @param iRowId: (IN) Row ID of a zero element.
 * @param iColumnId: (IN) Column ID of a zero element.
 */
int CAssignmentSolver::removeZeroPosition(const int iRowId, const int iColumnId)
{
	/* Remove from row ID map. */
	if (EXIST(iRowId, _zeroRowIdsMap))
	{
		set<int>& columnIdsSet = _zeroRowIdsMap[iRowId];

		columnIdsSet.erase(iColumnId);
		if (columnIdsSet.empty())
		{
			_zeroRowIdsMap.erase(iRowId);
		}
	}

	/* Remove from column ID map. */
	if (EXIST(iColumnId, _zeroColumnIdsMap))
	{
		set<int>& rowIdsSet = _zeroColumnIdsMap[iColumnId];

		rowIdsSet.erase(iRowId);
		if (rowIdsSet.empty())
		{
			_zeroColumnIdsMap.erase(iColumnId);
		}
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description: Set raw cost matrix.
 * @param rawCostMatrix: (IN) Cost matrix. It is the client's responsibility to maintain this matrix accessible during whole optimization process.
 * @exception:
 * 	CInvalidArgumentException:
 */
int CAssignmentSolver::setCostMatrix(const std::vector<vector<double> >& rawCostMatrix)
{
	// If not empty cost matrix:
	if (rawCostMatrix.size() > 0 && rawCostMatrix[0].size() > 0)
	{
		/* Check the integrity of cost matrix. */
		// number of rows of the raw cost matrix
		const int nRowsRawCostMatrix = rawCostMatrix.size();
		// number of columns of the raw cost matrix
		const int nColumnsRawCostMatrix = rawCostMatrix[0].size();
		for (int iRow = 0; iRow < nRowsRawCostMatrix; ++ iRow)
		{
			if (static_cast<int>(rawCostMatrix[iRow].size()) != nColumnsRawCostMatrix)
			{
				std::stringstream msgStream;
				msgStream
					<< LOCATION_STREAM_INSERTION
					<< "Invalid arguments (rawCostMatrix)! "
					<< "Vector dimensions not match! ";
				throw CInvalidArgumentException(msgStream.str());
			}
		}

		_bMultiRuns = false;
		_pRawCostMatrix = &rawCostMatrix;

		// If there are less or equal rows than columns:
		if (rawCostMatrix.size() <= rawCostMatrix[0].size())
		{
			/* Raw cost matrix can be directly used as cost matrix. */
			_bTransposedCostMatrix = false;
			_nRows = nRowsRawCostMatrix;
			_nColumns = nColumnsRawCostMatrix;
			_costMatrix = rawCostMatrix;
		}
		// If there are more rows than columns:
		else
		{
			/* Use the transposed raw cost matrix as cost matrix. */
			_bTransposedCostMatrix = true;
			_nRows = nColumnsRawCostMatrix;
			_nColumns = nRowsRawCostMatrix;

			/* Construct a transposed cost matrix. */
			_costMatrix = vector<vector<double> >(_nRows);
			for (int iRow = 0; iRow < nRowsRawCostMatrix; ++ iRow)
			{
				for (int iColumn = 0; iColumn < nColumnsRawCostMatrix; ++ iColumn)
				{
					const double& dCostValue = rawCostMatrix[iRow][iColumn];
					_costMatrix[iColumn].push_back(dCostValue);
				}
			}
		}

		_nDimensions = _nRows <= _nColumns ? _nRows : _nColumns;

		// full row indices: [0, number of rows)
		_fullRowIndicesSet.clear();
		for (int iId = 0; iId < _nRows; ++ iId)
		{
			_fullRowIndicesSet.insert(iId);
		}

		// full column indices: [0, number of columns)
		_fullColumnIndicesSet.clear();
		for (int iId = 0; iId < _nColumns; ++ iId)
		{
			_fullColumnIndicesSet.insert(iId);
		}
	}
	// If empty cost matrix:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< "Invalid arguments! "
			<< "Empty vector: rawCostMatrix. ";
		throw CInvalidArgumentException(msgStream.str());
	}

	return ErrorCodes::nNORMAL;
}



/**
 * Description:
 */
int CAssignmentSolver::subtractRowAndColumnMinimum()
{
	/* Subtract row minimum.  */
	for (int iRow = 0; iRow < _nRows; ++ iRow)
	{
		// minimum value of this row
		double dRowMinValue = std::numeric_limits<double>::max();
		// column ID corresponds to the row minimum
		int iColumnIdMin = -1;
		/* Get row minimum. */
		for (int iColumn = 0; iColumn < _nColumns; ++ iColumn)
		{
			if (_costMatrix[iRow][iColumn] < dRowMinValue)
			{
				dRowMinValue = _costMatrix[iRow][iColumn];
				iColumnIdMin = iColumn;
			}
		}
		/* Do subtraction. */
		for (int iColumn = 0; iColumn < _nColumns; ++ iColumn)
		{
			_costMatrix[iRow][iColumn] -= dRowMinValue;
		}

		// zero cost
		_costMatrix[iRow][iColumnIdMin] = 0;
		// Record the position of zero cost.
		recordZeroPosition(iRow, iColumnIdMin);
	}

	/* Subtract column minimum. */
	for (int iColumn = 0; iColumn < _nColumns; ++ iColumn)
	{
		// minimum value of this column
		double dColumnMinValue = std::numeric_limits<double>::max();
		int iRowIdMin = -1;
		/* Get column minimum. */
		for (int iRow = 0; iRow < _nRows; ++ iRow)
		{
			if (_costMatrix[iRow][iColumn] < dColumnMinValue)
			{
				dColumnMinValue = _costMatrix[iRow][iColumn];
				iRowIdMin = iRow;
			}
		}
		/* Do subtraction. */
		for (int iRow = 0; iRow < _nRows; ++ iRow)
		{
			_costMatrix[iRow][iColumn] -= dColumnMinValue;
		}

		// zero cost
		_costMatrix[iRowIdMin][iColumn] = 0;
		// Record the position of zero cost.
		recordZeroPosition(iRowIdMin, iColumn);
	}

	return 0;
}


/**
 * Description: Try doing assignments as many as possible.
 * @note: Resulted assignments may not be optimal unless some conditions are satisfied (number of lines in minimum line cover equals to the problem dimension).
 */
int CAssignmentSolver::tryAssign()
{
	_assignmentColumnIdsSet.clear();
	_assignmentRowIdsSet.clear();
	_assignmentColumnIdsMap.clear();

	// number of assignments been made so far
	int nAssignments = 0;
	int nNewAssignments = 0;
	bool bNewAssignment = true;
	// While there are still assignments left:
	while (bNewAssignment)
	{
		nNewAssignments = assignUniquely();
		if (nNewAssignments > nAssignments )
		{
			nAssignments = nNewAssignments;
			bNewAssignment = true;
		}
		else
		{
			bNewAssignment = false;
		}

		nNewAssignments = assignOneArbitrarily();
		if (nNewAssignments > nAssignments )
		{
			nAssignments = nNewAssignments;
			bNewAssignment = true;
		}
		else
		{
			bNewAssignment = false;
		}
	}

	return nAssignments;
}
