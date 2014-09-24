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
#include "ActivMiddleware/ActivIndexer/Typedefs.h"

#include "ActivMiddleware/ActivBase/StatusCodeException.h"

#include <algorithm>
#include <string>

namespace Activ
{

namespace Indexer
{

// Forward references
class IQueryNode;
class QueryOperator;
class Stemmer;

/**
 *	@brief	Container for queries
 */
class Query
{
public:
	/**
	 *	@brief	Default constructor
	 */
	ACTIV_INDEXER_API Query();

	/**
	 *	@brief	Default destructor.  If the query was constructed using ConstructQueryFromString()
	 *			then the m_rootNode will be deleted. Otherwise it is up to the user to to
	 *			delete the root node.
	 */
	ACTIV_INDEXER_API ~Query();

	/**
	 *	@brief	Construct a query tree from a string e.g. "(bob OR fred) AND NOT jill"
	 *	The query is automatically Prepare()'d
	 *
	 *	@brief	queryString				The string to convert to a query object
	 *	@param	loc						Locale that the query terms are in
	 *	@brief	stemTerms				Whether to stem the terms of the query
	 *	@brief	pStemValues				Which values to perform stemming (or zero to stem none)
	 *	@param	pStemmer				Stemmer to use (or zero for no stemming)
	 *	@param	caseSensitive			Whether the terms are case sensitive
	 *	@param	pCaseSensitiveValues	Which values are case sensitive (or zero if none are case sensitive)
	 */
	ACTIV_INDEXER_API StatusCode ConstructQueryFromString(const std::string &queryString, const std::locale &loc,
		bool stemTerms, const StringSet *pStemValues = 0, const Stemmer *pStemmer = 0,
		bool caseSensitive = false, const StringSet *pCaseSensitiveValues = 0);

	/**
	 *	@brief	Prepare the query for processing
	 *
	 *	@param	loc						Locale that the query terms are in (or zero to use Indexer's default locale)
	 *	@brief	stemTerms				Whether to stem the terms of the query
	 *	@brief	pStemValues				Which values to perform stemming (or zero to stem none)
	 *	@param	pStemmer				Stemmer to use (or zero for no stemming)
	 *	@param	caseSensitive			Whether the terms are case sensitive
	 *	@param	pcaseSensitiveValues	Which values are case sensitive (or zero if none are case sensitive)
	 */
	StatusCode Prepare(const std::locale &loc, bool stemTerms, const StringSet *pStemValues = 0, const Stemmer *pStemmer = 0,
		bool caseSensitive = false, const StringSet *pCaseSensitiveValues = 0);

	/**
	 *	@brief	Convert the query to a string
	 */
	std::string ToString(void) const;

	IQueryNode *	m_rootNode;		///< Root node of the query

private:
	bool	m_constructedFromString;
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
	 *	@brief	Prepare node for processing
	 */
	ACTIV_INDEXER_API virtual void Prepare(const std::locale &loc, bool stemTerms, const StringSet *pStemValues, const Stemmer *pStemmer,
		bool caseSensitive, const StringSet *pCaseSensitiveValues) = 0;

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
	 *	@brief	constructor
	 */
	QueryLeaf(const std::string &term);

	/**
	 *	@brief	constructor
	 */
	template <typename Iterator>
	QueryLeaf(Iterator start, Iterator end);

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

protected:
	/**
	 *	@brief	Prepare node for processing
	 */
	ACTIV_INDEXER_API virtual void Prepare(const std::locale &loc, bool stemTerms, const StringSet *pStemValues, const Stemmer *pStemmer,
		bool caseSensitive, const StringSet *pCaseSensitiveValues);

private:
	std::string			m_term;		///< The term associated with this leaf node
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
	 *	@brief	constructor
	 */
	template <typename Iterator>
	QueryValueLeaf(const std::string &valueName, Iterator valueDataStart, Iterator valueDataEnd);

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

protected:
	/**
	 *	@brief	Prepare node for processing
	 */
	ACTIV_INDEXER_API virtual void Prepare(const std::locale &loc, bool stemTerms, const StringSet *pStemValues, const Stemmer *pStemmer,
		bool caseSensitive, const StringSet *pCaseSensitiveValues);

private:
	std::string	m_valueName;	///< The name of the value associated with this node
	std::string	m_valueData;	///< The data of the value associated with this node
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

protected:
	/**
	 *	@brief	Prepare node for processing
	 */
	ACTIV_INDEXER_API virtual void Prepare(const std::locale &loc, bool stemTerms, const StringSet *pStemValues, const Stemmer *pStemmer,
		bool caseSensitive, const StringSet *pCaseSensitiveValues);

private:
	IQueryNode *	m_left;			///< Pointer to the node on the left hand side of the operator
	IQueryNode *	m_right;		///< Pointer to the node on the right hand side of the operator
	Operator		m_operator;		///< The operator
};

// ---------------------------------------------------------------------------------------------------------------------------------
// Query
// ---------------------------------------------------------------------------------------------------------------------------------

inline StatusCode Query::Prepare(const std::locale &loc, bool stemTerms, const StringSet *pStemValues, const Stemmer *pStemmer,
								 bool caseSensitive, const StringSet *pCaseSensitiveValues)
{
	if (0 == m_rootNode)
		return STATUS_CODE_NOT_INITIALIZED;

	m_rootNode->Prepare(loc, stemTerms, pStemValues, pStemmer, caseSensitive, pCaseSensitiveValues);

	return STATUS_CODE_SUCCESS;
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline std::string Query::ToString(void) const
{
	if (0 == m_rootNode)
		return "";

	return m_rootNode->ToString();
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

template <typename Iterator>
inline QueryLeaf::QueryLeaf(Iterator start, Iterator end) :
	IQueryNode(),
	m_term(start, end)
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

template <typename Iterator>
inline QueryValueLeaf::QueryValueLeaf(const std::string &valueName, Iterator valueDataStart, Iterator valueDataEnd) :
	IQueryNode(),
	m_valueName(valueName),
	m_valueData(valueDataStart, valueDataEnd)
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
	if ((0 == m_left) || (0 == m_right))
		ACTIV_THROW(StatusCodeException, STATUS_CODE_INVALID_PARAMETER);

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
