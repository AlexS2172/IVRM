/**
 *	@file	DatabaseUiOptions.h
 *
 *	@brief	Header file for the database user interface options.
 *
 *	$Log: $
 */

#if !defined (ACTIV_DATABASE_UI_DATABASE_UI_OPTIONS_H)
#define ACTIV_DATABASE_UI_DATABASE_UI_OPTIONS_H

#include "ActivMiddleware/ActivDatabaseUi/External.h"

#include "ActivMiddleware/ActivDatabase/Database.h"

#include "ActivMiddleware/ActivFramework/UiMenu.h"
#include "ActivMiddleware/ActivFramework/UiIo.h"

#include <string>

namespace Activ
{

namespace Database
{

/**
 *	@brief	Database user interface options class.
 */
class DatabaseUiOptions
{
public:
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	database the database.
	 */
	ACTIV_DATABASE_UI_API DatabaseUiOptions(Database &database);

	/**
	 *	@brief	Add all options.
	 *
	 *	@param	uiMenu the user interface menu to add the options to.
	 */
	ACTIV_DATABASE_UI_API void AddAllOptions(UiMenu &uiMenu);

	/**
	 *	@brief	Display table statistics.
	 *
	 *	@param	uiIo the user interface i/o object.
	 */
	ACTIV_DATABASE_UI_API void DisplayTableStatistics(const UiIo &uiIo);

	/**
	 *	@brief	Display table menu.
	 *
	 *	@param	uiIo the user interface i/o object.
	 */
	ACTIV_DATABASE_UI_API void DisplayTableMenu(const UiIo &uiIo);

	/**
	 *	@brief	Display table statistics.
	 *
	 *	@param	uiIo the user interface i/o object.
	 *	@param	database the database.
	 */
	ACTIV_DATABASE_UI_API static void DisplayTableStatistics(const UiIo &uiIo, const Database &database);

	/**
	 *	@brief	Display table menu.
	 *
	 *	@param	uiIo the user interface i/o object.
	 *	@param	database the database.
	 */
	ACTIV_DATABASE_UI_API static void DisplayTableMenu(const UiIo &uiIo, Database &database);

	/**
	 *	@brief	Get table id.
	 *
	 *	@param	uiIo the user interface i/o object.
	 *	@param	database the database.
	 *	@param	tableId set equal to the table id.
	 */
	ACTIV_DATABASE_UI_API static StatusCode GetTableId(const UiIo &uiIo, const Database &database, DatabaseSpecification::TableId &tableId);

private:
	Database &	m_database;	///< A reference to the database.
};

} // namespace Database

} // namespace Activ

#endif // !defined(ACTIV_DATABASE_UI_DATABASE_UI_OPTIONS_H)
