/**
 *	@file	UiMenu.h
 *
 *	@brief	Header file for the ui menu class..
 *
 *	$Log: $
 */

#if !defined (ACTIV_UI_MENU_H)
#define ACTIV_UI_MENU_H

#include "ActivMiddleware/ActivFramework/External.h"
#include "ActivMiddleware/ActivFramework/UiPermissions.h"

namespace Activ
{

class UiUser;
class UiIo;

/**
 *	@brief	Ui menu class.
 */
class UiMenu
{
public:
	/**
	 *	@brief	Option attributes.
	 */
	class OptionAttributes
	{
	public:
		/**
		 *	@brief	Default constructor.
		 *
		 *	UiPermissions::READ, no section name.
		 */
		ACTIV_FRAMEWORK_API OptionAttributes();

		/**
		 *	@brief	Constructor.
		 *
		 *	@param	uiPermissions user permissions for the option.
		 */
		ACTIV_FRAMEWORK_API OptionAttributes(const UiPermissions &uiPermissions);

		/**
		 *	@brief	Constructor.
		 *
		 *	@param	sectionName start a new section in the menu with this name.
		 */
		ACTIV_FRAMEWORK_API OptionAttributes(const std::string &sectionName);

		/**
		 *	@brief	Constructor.
		 *
		 *	@param	uiPermissions user permissions for the option.
		 *	@param	sectionName start a new section in the menu with this name.
		 */
		ACTIV_FRAMEWORK_API OptionAttributes(const UiPermissions &uiPermissions, const std::string &sectionName);

		const UiPermissions	m_uiPermissions;					///< the ui permissions required to use this option
		const std::string	m_sectionName;
	};

	/**
	 *	@brief	Global menu option function pointer type.
	 */
	typedef void (*GlobalOptionFunction)(const UiIo &uiIo);

	/**
	 *	@brief	Constructor.
	 *
	 *	Use this constructor for a top level menu.
	 *
	 *	@param	name name of the menu.
	 *	@param	uiUser user using this menu.
	 *	@param	showQuitOption show a quit option to move back to a parent menu.
	 */
	ACTIV_FRAMEWORK_API UiMenu(const std::string &name, UiUser &uiUser, const bool showQuitOption = true);

	/**
	 *	@brief	Constructor.
	 *
	 *	Use this constructor for a sub-menu. Note a sub-menu always has a quit option.
	 *
	 *	@param	name name of the sub-menu.
	 *	@param	uiIo as passed to the menu option that is creating the sub-menu.
	 */
	ACTIV_FRAMEWORK_API UiMenu(const std::string &name, const UiIo &uiIo);
	
	/**
	 *	@brief	Destructor.
	 */
	ACTIV_FRAMEWORK_API virtual ~UiMenu();

	/**
	 *	@brief	Get the name of the menu.
	 *
	 *	@return	The name of the menu.
	 */
	ACTIV_FRAMEWORK_API std::string GetName() const;

	/**
	 *	@brief	Add a member function menu option.
	 *
	 *	@param	name name of the menu option.
	 *	@param	object reference to the object the function is to be run on.
	 *	@param	pFunction member function in class T.
	 *	@param	optionAttributes optional option attributes.
	 */
	template<typename T>
	void AddOption(const std::string &name, T &object, void (T::*pFunction)(const UiIo &), const OptionAttributes &optionAttributes = OptionAttributes())
	{
		// NB VC++ 7.0 limitation, have to have this inline here
		Option *pOption = new (std::nothrow) OptionTemplate<T>(object, pFunction, name, optionAttributes);

		if (0 == pOption)
			return;

		m_optionVector.push_back(pOption);
	}

	/**
	 *	@brief	Add a global function menu option.
	 *
	 *	@param	name name of the menu option.
	 *	@param	pFunction pointer to a global function to run.
	 *	@param	optionAttributes optional option attributes.
	 */
	ACTIV_FRAMEWORK_API void AddOption(const std::string &name, GlobalOptionFunction pFunction, const OptionAttributes &optionAttributes = OptionAttributes());

	/**
	 *	@brief	Run the menu.
	 */
	ACTIV_FRAMEWORK_API void Run();

protected:
	/**
	 *	@brief	Display a menu title title.
	 *
	 *	Default is "<name> options:". Override to change.
	 *
	 *	@param	uiIo to read / write to / from the user.
	 */
	ACTIV_FRAMEWORK_API virtual void DisplayTitle(const UiIo &uiIo) const;

	/**
	 *	@brief	Display an option.
	 *
	 *	Override this to change the format of menu option displays.
	 *
	 *	@param	uiIo to read / write to / from the user.
	 *	@param	optionIndex the index of the option.
	 *	@param	name the name of the option.
	 *	@param	optionAttributes attributes for this option.
	 */
	ACTIV_FRAMEWORK_API virtual void DisplayOption(const UiIo &uiIo, const size_t optionIndex, const std::string &name, const OptionAttributes &optionAttributes) const;

	/**
	 *	@brief	Display message after options.
	 *
	 *	@param	uiIo to read / write to / from the user.
	 */
	ACTIV_FRAMEWORK_API virtual void DisplayFooter(const UiIo &uiIo) const;

	/**
	 *	@brief	Display the prompt that is used when selecting a menu option.
	 *
	 *	Default is ">> ". Override to change.
	 *
	 *	@param	uiIo to read / write to / from the user.
	 */
	ACTIV_FRAMEWORK_API virtual void DisplayOptionPrompt(const UiIo &uiIo) const;

	/**
	 *	@brief	Display the prompt used when requesting input other than menu option selection.
	 *
	 *	Default is "prompt <value>: " if \a initializedWithDefault, else "prompt: ". Override to change.
	 *
	 *	@param	uiIo to read / write to / from the user.
	 *	@param	prompt the prompt to display.
	 *	@param	value the default value, if \a initializedWithDefault.
	 *	@param	initializedWithDefault whether \a value contains a default value.
	 */
	ACTIV_FRAMEWORK_API virtual void DisplayInputPrompt(const UiIo &uiIo, const std::string &prompt, const std::string &value, const bool initializedWithDefault) const;

	/**
	 *	@brief	User has entered an option.
	 *
	 *	Default processing checks for quit (if the menu was constructed with a quit option),
	 *	then for an unsigned integer in the range 1 ... number of options in menu, running the option if in range.
	 *
	 *	@param	uiIo to read / write to / from the user.
	 *	@param	optionString the entered option string.
	 *
	 *	@retval	STATUS_CODE_SUCCESS option has been run.
	 *	@retval	STATUS_CODE_EXIT to leave a menu.
	 *	@retval	...
	 */
	ACTIV_FRAMEWORK_API virtual StatusCode OnOption(const UiIo &uiIo, const std::string &optionString);

private:
	/**
	 *	@brief	Option class.
	 */
	class Option
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	name name of the menu option.
		 *	@param	optionAttributes attributes for the menu option.
		 */
		ACTIV_FRAMEWORK_API Option(const std::string &name, const OptionAttributes &optionAttributes);

		/**
		 *	@brief	Destructor.
		 */
		ACTIV_FRAMEWORK_API virtual ~Option();

		/**
		 *	@brief	Run the option.
		 *
		 *	@param	uiIo to read / write to / from the user.
		 */
		virtual void Run(const UiIo &uiIo) const = 0;

		std::string			m_name;
		OptionAttributes	m_attributes;
	};

	/**
	 *	@brief	Template for member function options.
	 */
	template <typename T>
	class OptionTemplate : public Option
	{
	public:
		typedef void (T::*OptionFunction)(const UiIo &uiIo);

		// NB VC++ 7.0 - inlines

		/**
		 *	@brief	Constructor.
		 *
		 *	@param	object reference to an object on which the member function is to be run.
		 *	@param	pFunction the member function in class T.
		 *	@param	name name of the menu option.
		 *	@param	optionAttributes attributes for the menu option.
		 */
		OptionTemplate(T &object, OptionFunction pFunction, const std::string &name, const OptionAttributes &optionAttributes) :
			Option(name, optionAttributes),
			m_object(object),
			m_pFunction(pFunction)
		{
		}

	private:
		virtual void Run(const UiIo &uiIo) const
		{
			(m_object.*m_pFunction)(uiIo);
		}

		T				&m_object;								///< reference to object to run member function on
		OptionFunction	m_pFunction;							///< function pointer to member function in T
	};

	/**
	 *	@brief	Global function option class.
	 */
	class GlobalOption : public Option
	{
	public:
		/**
		 *	@brief	Constructor.
		 *
		 *	@param	pFunction global function to the option is to run.
		 *	@param	name name of the menu option.
		 *	@param	optionAttributes attributes for the menu option.
		 */
		GlobalOption(GlobalOptionFunction pFunction, const std::string &name, const OptionAttributes &optionAttributes);

	private:
		virtual void Run(const UiIo &uiIo) const;

		GlobalOptionFunction	m_pFunction;					///< global function pointer
	};

	/**
	 *	@brief	Display the menu.
	 *
	 *	@param	uiIo to read / write to / from the user.
	 */
	void Display(const UiIo &uiIo) const;

	/**
	 *	@brief	Display the list of options.
	 *
	 *	@param	uiIo to read / write to / from the user.
	 */
	void DisplayOptions(const UiIo &uiIo) const;

	/**
	 *	@brief	Get an option from the user and run it.
	 *
	 *	@param	uiIo to read / write to / from the user.
	 *
	 *	@retval	STATUS_CODE_SUCCESS option has been run.
	 *	@retval	STATUS_CODE_EXIT user wants to leave menu.
	 *	@retval	STATUS_CODE_FAILURE no option run, menu is redisplayed.
	 */
	StatusCode GetOption(const UiIo &uiIo);

	/**
	 *	@brief	Run an option.
	 *
	 *	@param	uiIo to read / write to / from the user.
	 *	@param	optionIndex the index of the option (0 .. number of options - 1).
	 */
	void RunOption(const UiIo &uiIo, const size_t optionIndex);

	/**
	 *	@brief	Check if the user is sufficiently permissioned to use an option.
	 *
	 *	@return	true if the user meets the permissioning requirements to use the option.
	 */
	ACTIV_FRAMEWORK_API bool IsUserPermissionedForOption(const UiPermissions &optionUiPermissions) const;

	typedef std::vector<Option *> OptionVector;					///< option vector type

	static const std::string	m_quitSet[];					///< quit options

	const std::string			m_name;							///< the name of the menu
	UiUser &					m_uiUser;						///< the user of this menu
	UiMenu * const				m_pParentMenu;					///< parent menu, or 0
	const bool					m_showQuitOption;				///< show quit menu option
	OptionVector				m_optionVector;					///< vector of menu options

	friend class UiIo;
};

} // namespace Activ

#endif // !defined (ACTIV_UI_MENU_H)
