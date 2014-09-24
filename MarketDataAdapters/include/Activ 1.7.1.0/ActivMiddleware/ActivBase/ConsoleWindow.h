/**
 *	@file	ConsoleWindow.h
 *
 *	@brief	Header file for console window.
 *
 *	$Log: $
 */

#if (!defined ACTIV_BASE_CONSOLE_WINDOW_H)
#define ACTIV_BASE_CONSOLE_WINDOW_H

#include "ActivMiddleware/ActivBase/External.h"

#include <string>

#if (defined ACTIV_PLATFORM_POSIX)
extern "C" typedef struct _win_st WINDOW;
#endif

namespace Activ
{

class IMutex;

/**
 *	@brief	Console window class.
 */
class ConsoleWindow
{
public:
	/**
	 *	@brief	Cursor coordinate.
	 */
	class Coord
	{
	public:
		/**
		 *	@brief	Default constructor. x & y coordinates set to 0.
		 */
		Coord();

		/**
		 *	@brief	Constructor.
		 *
		 *	@param	x coordinate.
		 *	@param	y coordinate.
		 */
		Coord(const int16_t x, const int16_t y);

		int16_t	m_x;
		int16_t	m_y;
	};

	typedef uint32_t Flags;										///< Flags type

	static const Flags FLAG_NONE = 0x00000000;
	static const Flags FLAG_NO_AUTOSCROLL = 0x00000001;			///< Don't scroll the window when output reaches the bottom
	static const Flags FLAG_IMMEDIATE_REFRESH = 0x00000002;		///< Refresh display immediately after a write

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	flags flags defining console behaviour.
	 */
	ACTIV_BASE_API ConsoleWindow(const Flags = FLAG_NONE);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_BASE_API ~ConsoleWindow();

	/**
	 *	@brief	Wait for input on the console.
	 *
	 *	Note the resolution of the timeout on Win32 is around 10ms, and on Linux is 100ms.
	 *
	 *	@param	timeoutPeriod how long to wait for input, in ms.
	 *
	 *	@retval	STATUS_CODE_SUCCESS there is input queued on the console.
	 *	@retval	STATUS_CODE_TIMEOUT no input queued in the timeout period.
	 *	@retval	STATUS_CODE_FAILURE an error occured.
	 */
	ACTIV_BASE_API StatusCode WaitForInput(const TimeoutPeriod timeoutPeriod = 100) const;

	/**
	 *	@brief	Read a complete line from the console. Note returned buffer will not contain terminating
	 *			cr / lf characters. This is a blocking call.
	 *
	 *	@param	input reference to a string to receive input.
	 *
	 *	@retval	STATUS_CODE_SUCCESS.
	 *	@retval	STATUS_CODE_FAILURE did not read a complete line.
	 */
	ACTIV_BASE_API StatusCode ReadLine(std::string &input) const;

	/**
	 *	@brief	Get the current console display size.
	 *
	 *	@param	size to receive the size.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_BASE_API StatusCode GetSize(Coord &size) const;

	/**
	 *	@brief	Set the console display size.
	 *
	 *	@param	size the new size.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_BASE_API StatusCode SetSize(const Coord &size);

	/**
	 *	@brief	Get the current cursor position.
	 *
	 *	@param	coord to receive the cursor position.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_BASE_API StatusCode GetCursorPosition(Coord &coord) const;

	/**
	 *	@brief	Set the cursor position.
	 *
	 *	@param	coord the position to place the cursor.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_BASE_API StatusCode SetCursorPosition(const Coord &coord);

	/**
	 *	@brief	Set the cursor visibility.
	 *
	 *	@param	isVisible true to make cursor visible, false to make invisible.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_BASE_API StatusCode SetCursorVisibility(const bool isVisible);

	/**
	 *	@brief	Clear the console from a coordinate to the end.
	 *
	 *	Note the current cursor position will not be altered by this call.
	 *
	 *	@param	coord the coordinate to clear from.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_BASE_API StatusCode ClearFrom(const Coord &coord);

	/**
	 *	@brief	Write a string beginning at a specified coordinate.
	 *
	 *	Note the current cursor position will not be altered by this call.
	 *
	 *	@param	coord the coordinate to start writing from.
	 *	@param	pStr the string to write.
	 *	@param	length number of characters in \a string to write, default is all.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	ACTIV_BASE_API StatusCode WriteAt(const Coord &coord, const char * const pStr, const size_t length = 0);

	/**
	 *	@brief	Write formatted output to the console from the current cursor position.
	 *
	 *	@param	pStr the format string.
	 *	@param	... args.
	 */
	ACTIV_BASE_API void Printf(const char * const pStr, ...) const;

	/**
	 *	@brief	Set title of console.
	 *
	 *	@param	title.
	 */
	ACTIV_BASE_API static void SetTitle(const std::string &title);

private:
#if (defined ACTIV_PLATFORM_WINDOWS)

	const HANDLE	m_outputHandle;								///< Console output handle
	const HANDLE	m_inputHandle;								///< Console input handle
	DWORD			m_originalMode;								///< Original console mode settings

#elif (defined ACTIV_PLATFORM_POSIX)

	WINDOW * const 	m_pWindow;
	IMutex * const	m_pMutex;

#else
#error Configure for this platform
#endif
};

// ---------------------------------------------------------------------------------------------------------------------------------

inline ConsoleWindow::Coord::Coord() :
	m_x(0),
	m_y(0)
{
}

// ---------------------------------------------------------------------------------------------------------------------------------

inline ConsoleWindow::Coord::Coord(const int16_t x, const int16_t y) :
	m_x(x),
	m_y(y)
{
}

} // namespace Activ

#endif // (!defined ACTIV_BASE_CONSOLE_WINDOW_H)
