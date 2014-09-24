/**
 *	@file	StatisticHelper.h
 *
 *	@brief	Header file simple statictic helper classes.
 *
 *	$Log: $
 */

#if !defined (ACTIV_STATISTIC_HELPER_H)
#define ACTIV_STATISTIC_HELPER_H

namespace Activ
{

/**
 *	@brief	Statistic value.
 */
template<typename T, T INITIAL = 0>
class StatisticValue
{
public:
	/**
	 *	@brief	Constructor. Initialize value to INITIAL.
	 */
	StatisticValue();

	/**
	 *	@brief	Increment stored value.
	 *
	 *	@return	*this.
	 */
	StatisticValue &operator++();

	/**
	 *	@brief	operator+=.
	 *
	 *	@param	value value to increment stored value by.
	 *
	 *	@return	*this.
	 */
	StatisticValue &operator+=(const T &value);

	/**
	 *	@brief	Deccrement stored value.
	 *
	 *	@return	*this.
	 */
	StatisticValue &operator--();

	/**
	 *	@brief	operator-=.
	 *
	 *	@param	value value to decrement stored value by.
	 *
	 *	@return	*this.
	 */
	StatisticValue &operator-=(const T &value);

	/**
	 *	@brief	Get stored value.
	 *
	 *	@return	Stored value.
	 */
	const T &GetValue() const;

	/**
	 *	@brief	Reset value.
	 */
	void ResetValue();
	
protected:
	T	m_value;
};

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T, T INITIAL>
inline StatisticValue<T, INITIAL>::StatisticValue()
{
	ResetValue();
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T, T INITIAL>
inline StatisticValue<T, INITIAL> &StatisticValue<T, INITIAL>::operator++()
{
	++m_value;
	return *this;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T, T INITIAL>
inline StatisticValue<T, INITIAL> &StatisticValue<T, INITIAL>::operator+=(const T &value)
{
	m_value += value;
	return *this;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T, T INITIAL>
inline StatisticValue<T, INITIAL> &StatisticValue<T, INITIAL>::operator--()
{
	--m_value;
	return *this;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T, T INITIAL>
inline StatisticValue<T, INITIAL> &StatisticValue<T, INITIAL>::operator-=(const T &value)
{
	m_value -= value;
	return *this;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T, T INITIAL>
inline const T &StatisticValue<T, INITIAL>::GetValue() const
{
	return m_value;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T, T INITIAL>
inline void StatisticValue<T, INITIAL>::ResetValue()
{
	m_value = INITIAL;
}

// ---------------------------------------------------------------------------------------------------------------------------------

/**
 *	@brief	Statistic value with high count.
 */
template<typename T, T INITIAL = 0>
class StatisticHighValue : public StatisticValue<T, INITIAL>
{
public:
	/**
	 *	@brief	Constructor. Initialize value to INITIAL.
	 */
	StatisticHighValue();

	/**
	 *	@brief	Increment stored value and adjust high value as necessary.
	 *
	 *	@return	*this.
	 */
	StatisticHighValue &operator++();

	/**
	 *	@brief	operator+= and adjust high value as necessary.
	 *
	 *	@param	value value to increment stored value by.
	 *
	 *	@return	*this.
	 */
	StatisticHighValue &operator+=(const T &value);

	/**
	 *	@brief	Get high value.
	 *
	 *	@return	High value.
	 */
	const T &GetHighValue() const;

	/**
	 *	@brief	Reset high value.
	 */
	void ResetHighValue();

protected:
	T	m_highValue;
};

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T, T INITIAL>
inline StatisticHighValue<T, INITIAL>::StatisticHighValue()
{
	ResetHighValue();
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T, T INITIAL>
inline StatisticHighValue<T, INITIAL> &StatisticHighValue<T, INITIAL>::operator++()
{
	(void) StatisticValue<T, INITIAL>::operator++();
	
	if (StatisticValue<T, INITIAL>::m_value > m_highValue)
		m_highValue = StatisticValue<T, INITIAL>::m_value;

	return *this;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T, T INITIAL>
inline StatisticHighValue<T, INITIAL> &StatisticHighValue<T, INITIAL>::operator+=(const T &value)
{
	(void) StatisticValue<T, INITIAL>::operator+=(value);

	if (StatisticValue<T, INITIAL>::m_value > m_highValue)
		m_highValue = StatisticValue<T, INITIAL>::m_value;

	return *this;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T, T INITIAL>
inline const T &StatisticHighValue<T, INITIAL>::GetHighValue() const
{
	return m_highValue;
}

// ---------------------------------------------------------------------------------------------------------------------------------

template<typename T, T INITIAL>
inline void StatisticHighValue<T, INITIAL>::ResetHighValue()
{
	m_highValue = INITIAL;
}

} // namespace Activ

#endif // !defined (ACTIV_STATISTIC_HELPER_H)
