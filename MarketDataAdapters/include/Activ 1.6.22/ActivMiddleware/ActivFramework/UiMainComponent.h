/**
 *	@file	UiMainComponent.h
 *
 *	@brief	Header file for the main ui component class.
 *
 *	$Log: $
 */

#if !defined (UI_MAIN_COMPONENT_H)
#define UI_MAIN_COMPONENT_H

#include "ActivMiddleware/ActivFramework/External.h"
#include "ActivMiddleware/ActivFramework/UiComponent.h"

#include "boost/scoped_ptr.hpp"

namespace Activ
{

class UiMenu;
class UiIo;

/**
 *	@brief	Main ui component.
 *
 *	The main ui component acts as the main, or master, menu of a ui service (the first menu clients will see).
 *	It also maintains a list of other ui components in the system that can be communicated with.
 */
class UiMainComponent : public UiComponent
{
public:
	/**
	 *	@brief	Constructor.
	 *
	 *	@param	application application to attach to.
	 *	@param	managedEndPoint endpoint to attach to - must be singly threaded.
 	 *	@param	componentName name of the ui component. This name is used to register the component with the main menu.
	 *	@param	uiServiceName name of the ui service.
	 *
	 *	@throw	Exception.
	 */
	ACTIV_FRAMEWORK_API UiMainComponent(Application &application, ManagedEndPoint &managedEndPoint, const std::string &uiComponentName, const std::string &uiServiceName);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_FRAMEWORK_API ~UiMainComponent();

	/**
	 *	@brief	Menu option to display list of available ui components.
	 *
	 *	@param	uiIo to read / write to / from the user.
	 *	@param	type type of ui component to display.
	 */
	ACTIV_FRAMEWORK_API void UiDisplayAvailableUiComponentsByType(const UiIo &uiIo, const Type type) const;

	/**
	 *	@brief	User has entered an option.
	 *
	 *	@param	uiIo to read / write to / from the user.
	 *	@param	optionString the option string.
	 */
	ACTIV_FRAMEWORK_API StatusCode OnOption(const UiIo &uiIo, const std::string &optionString);

private:
	ACTIV_FRAMEWORK_API virtual StatusCode OnMessage(HeapMessage &request);

	/**
	 *	@brief	Process a register new component message.
	 *
	 *	The component is added to the list of available components and can be communicated with.
	 */
	void ProcessRegisterComponentMessage(HeapMessage &message);

	/**
	 *	@brief	Process a deregister component message.
	 *
	 *	The component is removed from the list of available components and can no longer be communicated with.
	 */
	void ProcessDeregisterComponentMessage(HeapMessage &message);

	/**
	 *	@brief	Process endpoint shutdown / die message.
	 */
	void ProcessSystemEndPointIdSetMessage(HeapMessage &message, const char * const type);

	class UiComponentMap;

	boost::scoped_ptr<UiComponentMap>	m_pUiComponentMap;		///< map of available ui components
};

} // namespace Activ

#endif // !defined (UI_MAIN_COMPONENT_H)
