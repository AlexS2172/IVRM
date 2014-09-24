/**
 *	@file	TableUiOptions.h
 *
 *	@brief	Header file for the table user interface options.
 *
 *	$Log: $
 */

#if !defined (ACTIV_DATABASE_UI_TABLE_UI_OPTIONS_H)
#define ACTIV_DATABASE_UI_TABLE_UI_OPTIONS_H

#include "ActivMiddleware/ActivDatabaseUi/External.h"
#include "ActivMiddleware/ActivDatabase/ITable.h"
#include "ActivMiddleware/ActivDatabase/Key.h"
#include "ActivMiddleware/ActivDatabase/Record.h"
#include "ActivMiddleware/ActivFramework/UiMenu.h"
#include "ActivMiddleware/ActivFramework/UiIo.h"

namespace Activ
{

namespace Database
{

/**
 *	@brief	Table user interface options class.
 */
class TableUiOptions
{
public:
	static const size_t TABLE_NAME_DISPLAY_WIDTH	= 30;	///< The table name display width.
	static const size_t INDEX_NAME_DISPLAY_WIDTH	= 50;	///< The index name display width.
	static const size_t FIELD_NAME_DISPLAY_WIDTH	= 30;	///< The field name display width.
	static const size_t RECORD_COUNT_DISPLAY_WIDTH	= 10;	///< The record count display width.
	static const size_t MAX_DISPLAY_WIDTH			= 79;	///< The maximum display width.
	static const size_t DISPLAY_BUFFER_SIZE			= 1024;	///< The display buffer size width.

	static const std::string UNDEFINED_FIELD;

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	pTable a pointer to the table.
	 */
	ACTIV_DATABASE_UI_API TableUiOptions(ITable *pTable);

	/**
	 *	@brief	Add all options.
	 *
	 *	@param	uiMenu the user interface menu to add the options to.
	 */
	ACTIV_DATABASE_UI_API void AddAllOptions(UiMenu &uiMenu);

	/**
	 *	@brief	Display statistics.
	 *
	 *	@param	uiIo the user interface i/o object.
	 */
	ACTIV_DATABASE_UI_API void DisplayStatistics(const UiIo &uiIo);

	/**
	 *	@brief	Display index.
	 *
	 *	@param	uiIo the user interface i/o object.
	 */
	ACTIV_DATABASE_UI_API void DisplayIndex(const UiIo &uiIo);

	/**
	 *	@brief	Display record.
	 *
	 *	@param	uiIo the user interface i/o object.
	 */
	ACTIV_DATABASE_UI_API void DisplayRecord(const UiIo &uiIo);

	/**
	 *	@brief	Add record.
	 *
	 *	@param	uiIo the user interface i/o object.
	 */
	ACTIV_DATABASE_UI_API void AddRecord(const UiIo &uiIo);

	/**
	 *	@brief	Modify record.
	 *
	 *	@param	uiIo the user interface i/o object.
	 */
	ACTIV_DATABASE_UI_API void ModifyRecord(const UiIo &uiIo);

	/**
	 *	@brief	Delete record.
	 *
	 *	@param	uiIo the user interface i/o object.
	 */
	ACTIV_DATABASE_UI_API void DeleteRecord(const UiIo &uiIo);

	/**
	 *	@brief	Save as a text file.
	 *
	 *	@param	uiIo the user interface i/o object.
	 */
	ACTIV_DATABASE_UI_API void SaveAsTextFile(const UiIo &uiIo);

	/**
	 *	@brief	Clear.
	 *
	 *	@param	uiIo the user interface i/o object.
	 */
	ACTIV_DATABASE_UI_API void Clear(const UiIo &uiIo);

	/**
	 *	@brief	Save.
	 *
	 *	@param	uiIo the user interface i/o object.
	 */
	ACTIV_DATABASE_UI_API void Save(const UiIo &uiIo);

	/**
	 *	@brief	Read.
	 *
	 *	@param	uiIo the user interface i/o object.
	 */
	ACTIV_DATABASE_UI_API void Read(const UiIo &uiIo);

	/**
	 *	@brief	Check table integrity.
	 *
	 *	@param	uiIo the user interface i/o object.
	 */
	ACTIV_DATABASE_UI_API void CheckIntegrity(const UiIo &uiIo);

	/**
	 *	@brief	Display statistics.
	 *
	 *	@param	uiIo the user interface i/o object.
	 *	@param	pTable a pointer to the table.
	 */
	ACTIV_DATABASE_UI_API static void DisplayStatistics(const UiIo &uiIo, ITable *pTable);

	/**
	 *	@brief	Display index.
	 *
	 *	@param	uiIo the user interface i/o object.
	 *	@param	pTable a pointer to the table.
	 */
	ACTIV_DATABASE_UI_API static void DisplayIndex(const UiIo &uiIo, ITable *pTable);

	/**
	 *	@brief	Display record.
	 *
	 *	@param	uiIo the user interface i/o object.
	 *	@param	pTable a pointer to the table.
	 *	@param	record the displayed record.
	 *
	 *	@return	whether a record was displayed.
	 */
	ACTIV_DATABASE_UI_API static bool DisplayRecord(const UiIo &uiIo, ITable *pTable, Record &record);

	/**
	 *	@brief	Add record.
	 *
	 *	@param	uiIo the user interface i/o object.
	 *	@param	pTable a pointer to the table.
	 *	@param	record the added record.
	 *
	 *	@return	whether a record was added.
	 */
	ACTIV_DATABASE_UI_API static bool AddRecord(const UiIo &uiIo, ITable *pTable, Record &record);

	/**
	 *	@brief	Modify record.
	 *
	 *	@param	uiIo the user interface i/o object.
	 *	@param	pTable a pointer to the table.
	 *	@param	record the modified record.
	 *	@param	originalRecord the original record.
	 *
	 *	@return	whether a record was modified.
	 */
	ACTIV_DATABASE_UI_API static bool ModifyRecord(const UiIo &uiIo, ITable *pTable, Record &record, Record &originalRecord);

	/**
	 *	@brief	Delete record.
	 *
	 *	@param	uiIo the user interface i/o object.
	 *	@param	pTable a pointer to the table.
	 *	@param	key the deleted key.
	 *	@param	record the deleted record.
	 *
	 *	@return	whether a record was deleted.
	 */
	ACTIV_DATABASE_UI_API static bool DeleteRecord(const UiIo &uiIo, ITable *pTable, Key &key, Record &record);

	/**
	 *	@brief	Save as a text file.
	 *
	 *	@param	uiIo the user interface i/o object.
	 *	@param	pTable a pointer to the table.
	 */
	ACTIV_DATABASE_UI_API static void SaveAsTextFile(const UiIo &uiIo, ITable *pTable);

	/**
	 *	@brief	Clear.
	 *
	 *	@param	uiIo the user interface i/o object.
	 *	@param	pTable a pointer to the table.
	 */
	ACTIV_DATABASE_UI_API static void Clear(const UiIo &uiIo, ITable *pTable);

	/**
	 *	@brief	Save.
	 *
	 *	@param	uiIo the user interface i/o object.
	 *	@param	pTable a pointer to the table.
	 *	@param	directory the directory to save the data into.
	 */
	ACTIV_DATABASE_UI_API static void Save(const UiIo &uiIo, ITable *pTable, const std::string &directory = std::string(""));

	/**
	 *	@brief	Read.
	 *
	 *	@param	uiIo the user interface i/o object.
	 *	@param	pTable a pointer to the table.
	 *	@param	directory the directory to read the data from.
	 */
	ACTIV_DATABASE_UI_API static void Read(const UiIo &uiIo, ITable *pTable, const std::string &directory = std::string(""));

	/**
	 *	@brief	Check table integrity.
	 *
	 *	@param	uiIo the user interface i/o object.
	 *	@param	pTable a pointer to the table.
	 */
	ACTIV_DATABASE_UI_API static void CheckIntegrity(const UiIo &uiIo, ITable *pTable);

	/**
	 *	@brief	Display a record.
	 *
	 *	@param	uiIo the user interface i/o object.
	 *	@param	record the record to display.
	 *
	 *	@return	whether a record was displayed.
	 */
	ACTIV_DATABASE_UI_API static bool DisplayRecord(const UiIo &uiIo, Record &record);

	/**
	 *	@brief	Get the index number.
	 *
	 *	@param	uiIo the user interface i/o object.
	 *	@param	pTable a pointer to the table.
	 *	@param	indexNo the index number.
	 *	@param	allowDuplicateIndex whether allowed to get a duplicate index.
	 *	@param	allowHashIndex whether allowed to get a hash index.
	 *
	 *	@return	whether the call was a success.
	 */
	ACTIV_DATABASE_UI_API static bool GetIndexNumber(const UiIo &uiIo, ITable *pTable, IndexSpecification::Number &indexNo, const bool allowDuplicateIndex, const bool allowHashIndex);

	/**
	 *	@brief	Get the ordered field specification list.
	 *
	 *	@param	fieldListObject the field list object.
	 *	@param	orderedFieldSpecificationList the ordered field specification list.
	 *
	 *	@return	whether the call was a success.
	 */
	ACTIV_DATABASE_UI_API static bool GetOrderedFieldSpecificationList(FieldListObject &fieldListObject, FieldSpecificationList &orderedFieldSpecificationList);

	/**
	 *	@brief	Get the field specification list.
	 *
	 *	@param	uiIo the user interface i/o object.
	 *	@param	prompt the prompt.
	 *	@param	fieldListObject the field list object.
	 *	@param	fieldSpecificationList the field specification list.
	 *	@param	displayFieldList whether the field list should be displayed.
	 *
	 *	@return	whether the call was a success.
	 */
	ACTIV_DATABASE_UI_API static bool GetFieldSpecificationList(const UiIo &uiIo, const std::string &prompt, FieldListObject &fieldListObject, FieldSpecificationList &fieldSpecificationList, const bool displayFieldList);

	/**
	 *	@brief	Get the field specification.
	 *
	 *	@param	uiIo the user interface i/o object.
	 *	@param	prompt the prompt.
	 *	@param	fieldListObject the field list object.
	 *	@param	fieldSpecification the field specification.
	 *	@param	displayFieldList whether the field list should be displayed.
	 *
	 *	@return	whether the call was a success.
	 */
	ACTIV_DATABASE_UI_API static bool GetFieldSpecification(const UiIo &uiIo, const std::string &prompt, FieldListObject &fieldListObject, FieldSpecification &fieldSpecification, const bool displayFieldList);

	/**
	 *	@brief	Get key fields.
	 *
	 *	@param	uiIo the user interface i/o object.
	 *	@param	key the key.
	 *	@param	shouldGetMatchingKey whether should attempt to get a matching key.
	 *	@param	pAllKeyFields whether all the key fields were entered.
	 *
	 *	@return	whether the call was a success.
	 */
	ACTIV_DATABASE_UI_API static bool GetKeyFields(const UiIo &uiIo, Key &key, const bool shouldGetMatchingKey, bool *pAllKeyFields = 0);

	/**
	 *	@brief	Get fields.
	 *
	 *	@param	uiIo the user interface i/o object.
	 *	@param	fieldListObject the field list object.
	 *
	 *	@return	whether the call was a success.
	 */
	ACTIV_DATABASE_UI_API static bool GetFields(const UiIo &uiIo, FieldListObject &fieldListObject);

	/**
	 *	@brief	Get a field.
	 *
	 *	@param	uiIo the user interface i/o object.
	 *	@param	fieldSpecification the field specification.
	 *	@param	fieldListObject the field list object.
	 *
	 *	@return	whether the call was a success.
	 */
	ACTIV_DATABASE_UI_API static StatusCode GetField(const UiIo &uiIo, const FieldSpecification &fieldSpecification, FieldListObject &fieldListObject);

	/**
	 *	@brief	Display the field.
	 *
	 *	@param	fieldListObject the field list object.
	 *	@param	fieldSpecification the field specification.
	 *	@param	maxFieldDisplayWidth the maximum field display width.
	 *
	 *	@return	the maximum field display width.
	 */
	ACTIV_DATABASE_UI_API static std::string DisplayField(const FieldListObject &fieldListObject, const FieldSpecification &fieldSpecification, const size_t maxFieldDisplayWidth = ~0);

	/**
	 *	@brief	Get the maximum field display width.
	 *
	 *	@param	fieldSpecification the field specification.
	 *	@param	shouldIncludeFieldName whether the field name should be included in the calculation.
	 *
	 *	@return	the maximum field display width.
	 */
	ACTIV_DATABASE_UI_API static size_t GetMaxFieldDisplayWidth(const FieldSpecification &fieldSpecification, const bool shouldIncludeFieldName = true);

	/**
	 *	@brief	Field display sort function.
	 *
	 *	@return	whether lhs is less than rhs.
	 */
	ACTIV_DATABASE_UI_API static bool FieldDisplaySort(const std::string &lhs, const std::string &rhs);

private:
	ITable	*m_pTable;	///< A pointer to the table.
};

} // namespace Database

} // namespace Activ

#endif // !defined(ACTIV_DATABASE_UI_TABLE_UI_OPTIONS_H)
