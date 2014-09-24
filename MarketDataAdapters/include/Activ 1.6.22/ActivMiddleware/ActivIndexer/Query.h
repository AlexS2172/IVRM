/**
 *  @file	Query.h
 *
 *	@brief	Header file for Query classes
 *
 *	$Log: $
 */

#if !defined (ACTIV_INDEXER_QUERY_H)
#define ACTIV_INDEXER_QUERY_H

#include "ActivMiddleware/ActivIndexer/External.h"

#include "ActivMiddleware/ActivFieldTypes/Date.h"

#include <algorithm>
#include <string>

namespace Activ
{

namespace Indexer
{

// Forward references
class IQueryNode;
class QueryOperator;

/**
 *	@brief	Container for queries
 */
class Query
{
public:
	/**
	 *	@brief	Default constructor
	 */
	Query();

	/**
	 *	@brief	Construct a query tree from a string e.g. "(bob OR fred) AND NOT jill"
	 *	The query is automatically reduced using ReduceQuery()
	 */
	ACTIV_INDEXER_API static StatusCode ConstructQueryFromString(const std::string &queryString, Query &query);

	/**
	 *	@brief	Destroy a query that was created with ConstructQueryFromString().
	 *
	 *	This *MUST* be called for any queries created using ConstructQueryFromString() after use.
	 *	Manually constructed queries should be destroyed by calling delete on \a m_rootNode.
	 */
	ACTIV_INDEXER_API void DestroyQuery(void);

	/**
	 *	@brief	Reduce a query for more efficient processing
	 */
	StatusCode ReduceQuery(void);

	IQueryNode *	m_rootNode;		///< Root node of the query
	Date			m_startDate;	///< Earliest date for matching documents
	Date			m_endDate;		///< Latest date for matching documents
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Interface to all query nodes
 */
class IQueryNode
{
friend class Query;
friend class QueryOperator;
public:
	enum NodeType
	{
		QUERY_NODE_LEAF,
		QUERY_NODE_VALUE_LEAF,
		QUERY_NODE_OPERATOR,
	};

	/**
	 *	@brief	Default constructor
	 */
	IQueryNode();

	/**
	 *	@brief	Default destructor
	 */
	virtual ~IQueryNode();

	/**
	 *	@brief	Return the parent node
	 */
	IQueryNode *GetParent(void) const;

	/**
	 *	@brief	Make this node negative
	 */
	void SetNegative(void);

	/**
	 *	@brief	Make this node positive
	 */
	void SetPositive(void);

	/**
	 *	@brief	Flip the negative/positive value of this node
	 */
	bool FlipNegation(void);

	/**
	 *	@brief	Get whether the node is negative or not
	 */
	bool IsNegative(void) const;

	/**
	 *	@brief	Return the type of node (QUERY_NODE_LEAF, QUERY_NODE_VALUE_LEAF or QUERY_NODE_OPERATOR)
	 */
	virtual NodeType GetType(void) const = 0;

	/**
	 *	@brief	Convert the query to a string
	 */
	virtual std::string ToString(void) const;

protected:
	/**
	 *	@brief	Reduce a query for more efficient processing
	 */
	ACTIV_INDEXER_API void ReduceQuery(void);

	IQueryNode		*m_parent;		///< Pointer back to the parent node
	bool			m_negative;		///< Whether the node is negative or not
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Leaf node of a query
 */
class QueryLeaf : public IQueryNode
{
public:
	/**
	 *	@brief	Default constructor
	 */
	QueryLeaf(const std::string &term);

	/**
	 *	@brief	Get the term to search for
	 */
	const std::string &GetTerm(void) const;

	/**
	 *	@brief	Return the type of node (QUERY_NODE_LEAF)
	 */
	virtual NodeType GetType(void) const;

	/**
	 *	@brief	Convert the query to a string
	 */
	virtual std::string ToString(void) const;

private:
	const std::string	m_term;		///< The term associated with this leaf node
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Value leaf node of a query
 */
class QueryValueLeaf : public IQueryNode
{
public:
	/**
	 *	@brief	Default constructor
	 */
	QueryValueLeaf(const std::string &valueName, const std::string &valueData);

	/**
	 *	@brief	Get the name of the value to search for
	 */
	const std::string &GetValueName(void) const;

	/**
	 *	@brief	Get the data of the value to search for
	 */
	const std::string &GetValueData(void) const;

	/**
	 *	@brief	Return the type of node (QUERY_NODE_VALUE_LEAF)
	 */
	virtual NodeType GetType(void) const;

	/**
	 *	@brief	Convert the query to a string
	 */
	virtual std::string ToString(void) const;

private:
	const std::string	m_valueName;	///< The name of the value associated with this node
	const std::string	m_valueData;	///< The data of the value associated with this node
};

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Operator node of a query
 */
class QueryOperator : public IQueryNode
{
public:
	enum Operator
	{
		OPERATOR_AND,
		OPERATOR_OR
	};

	/**
	 *	@brief	Default constructor
	 */
	QueryOperator(IQueryNode * const left, IQueryNode * const right, Operator op);

	/**
	 *	@brief	Default destructor
	 */
	~QueryOperator();

	/**
	 *	@brief	Return the node on the left hand side of the operator
	 */
	IQueryNode *GetLeft(void) const;

	/**
	 *	@brief	Return the node on the right hand side of the operator
	 */
	IQueryNode *GetRight(void) const;
	
	/**
	 *	@brief	Return the operator between the two nodes
	 */
	Operator GetOperator(void) const;

	/**
	 *	@brief	Return the operator between the two nodes
	 */
	void SetOperator(const Operator &op);

	/**
	 *	@brief	Flip the operator between AND / OR
	 */
	void FlipOperator(void);

	/**
	 *	@brief	Swap the left and right nodes around
	 */
	void SwapNodes(void);

	/**
	 *	@brief	Return the type of node (QUERY_NODE_OPERATOR)
	 */
	virtual NodeType GetType(void) const;

	/**
	 *	@brief	Convert the query to a string
	 */
	virtual std::string ToString(void) const;

private:
	IQueryNode *	m_left;			///< Pointer to the node on the left hand side of the operator
	IQueryNode *	m_right;		///< Pointer to the node on the right hand side of the operator
	Operator		m_operator;		///< The operator
};

// ---------------------------------------------------------------------------------------------------------------------------------
// Query
// ---------------------------------------------------------------------------------------------------------------------------------

inline Query::Query() :
	m_rootNode(0)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Query::ReduceQuery(void)
{
	if (0 == m_rootNode)
		return STATUS_CODE_NOT_INITIALIZED;

	m_rootNode->ReduceQuery();

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// IQueryNode
// ---------------------------------------------------------------------------------------------------------------------------------

inline IQueryNode::IQueryNode() :
	m_parent(0),
	m_negative(false)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline IQueryNode::~IQueryNode()
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline IQueryNode *IQueryNode::GetParent(void) const
{
	return m_parent;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void IQueryNode::SetNegative(void)
{
	m_negative = true;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void IQueryNode::SetPositive(void)
{
	m_negative = false;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool IQueryNode::FlipNegation(void)
{
	return ((m_negative = !m_negative));
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline bool IQueryNode::IsNegative(void) const
{
	return m_negative;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline std::string IQueryNode::ToString(void) const
{
	return (m_negative ? "NOT " : "");
}

// ---------------------------------------------------------------------------------------------------------------------------------
// QueryLeaf
// ---------------------------------------------------------------------------------------------------------------------------------

inline QueryLeaf::QueryLeaf(const std::string &term) :
	IQueryNode(),
	m_term(term)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const std::string &QueryLeaf::GetTerm(void) const
{
	return m_term;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline IQueryNode::NodeType QueryLeaf::GetType(void) const
{
	return QUERY_NODE_LEAF;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline std::string QueryLeaf::ToString(void) const
{
	return IQueryNode::ToString() + m_term;
}


// ---------------------------------------------------------------------------------------------------------------------------------
// QueryValueLeaf
// ---------------------------------------------------------------------------------------------------------------------------------

inline QueryValueLeaf::QueryValueLeaf(const std::string &valueName, const std::string &valueData) :
	IQueryNode(),
	m_valueName(valueName),
	m_valueData(valueData)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const std::string &QueryValueLeaf::GetValueName(void) const
{
	return m_valueName;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline const std::string &QueryValueLeaf::GetValueData(void) const
{
	return m_valueData;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline IQueryNode::NodeType QueryValueLeaf::GetType(void) const
{
	return QUERY_NODE_VALUE_LEAF;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline std::string QueryValueLeaf::ToString(void) const
{
	return IQueryNode::ToString() + m_valueName + "=" + m_valueData;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// QueryOperator
// ---------------------------------------------------------------------------------------------------------------------------------

inline QueryOperator::QueryOperator(IQueryNode * const left, IQueryNode * const right, Operator op) :
	IQueryNode(),
	m_left(left), m_right(right), m_operator(op)
{
	m_left->m_parent = this;
	m_right->m_parent = this;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline QueryOperator::~QueryOperator()
{
	delete m_left;
	delete m_right;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline IQueryNode *QueryOperator::GetLeft(void) const
{
	return m_left;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline IQueryNode *QueryOperator::GetRight(void) const
{
	return m_right;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline QueryOperator::Operator QueryOperator::GetOperator(void) const
{
	return m_operator;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void QueryOperator::SetOperator(const Operator &op)
{
	m_operator = op;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void QueryOperator::FlipOperator(void)
{
	if (OPERATOR_AND == m_operator)
		m_operator = OPERATOR_OR;
	else
		m_operator = OPERATOR_AND;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline void QueryOperator::SwapNodes(void)
{
	std::swap(m_left, m_right);
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline IQueryNode::NodeType QueryOperator::GetType(void) const
{
	return QUERY_NODE_OPERATOR;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline std::string QueryOperator::ToString(void) const
{
	return IQueryNode::ToString() + "(" + m_left->ToString() + (m_operator == OPERATOR_AND ? " AND " : " OR ") + m_right->ToString() + ")";
};

} // namespace Activ

} //namespace Indexer

#endif
