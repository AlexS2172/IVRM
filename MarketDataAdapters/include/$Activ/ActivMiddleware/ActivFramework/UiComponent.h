/**
 *	@file	UiComponent.h
 *
 *	@brief	Header file for the ui component class.
 *
 *	$Log: $
 */

#if !defined (ACTIV_UI_COMPONENT_H)
#define ACTIV_UI_COMPONENT_H

#include "ActivMiddleware/ActivFramework/External.h"
#include "ActivMiddleware/ActivFramework/Component.h"
#include "ActivMiddleware/ActivFramework/Application.h"

namespace Activ
{

class UiUser;

/**
 *	@brief	Ui component class.
 *
 *	Note a ui component cannot be attached to the ui service endpoint.
 */
class UiComponent : public Component
{
public:
	/**
	 *	@brief	Ui component types.
	 */
	enum Type
	{
		TYPE_DEFAULT,											///< normal user uis
		TYPE_MAIN,												///< for main menu component
		TYPE_BASE_SERVICE,										///< for base service / middleware provided uis
	};

	/**
	 *	@brief	Constructor.
	 *
	 *	@param	application application to attach to.
	 *	@param	managedEndPoint endpoint to attach to - must be singly threaded.
	 *	@param	componentName name of the ui component. This name is used to register the component with the main menu.
	 *	@param	type the type to register as.
	 *	@param	uiServiceName name of the ui service.
	 *	@param	pHeap heap to use for this component, or 0 for the service's heap
	 *
	 *	@throw	Exception.
	 */
	ACTIV_FRAMEWORK_API UiComponent(Application &application, ManagedEndPoint &managedEndPoint, const std::string &componentName,
									const Type type = TYPE_DEFAULT, const std::string &uiServiceName = Application::GetUiServiceName(),
									IHeap * const pHeap = 0);

	/**
	 *	@brief	Destructor.
	 */
	ACTIV_FRAMEWORK_API ~UiComponent();

	/**
	 *	@brief	Run the component.
	 *
	 *	An override of this function must construct a menu using uiUser, and call Run() on the menu object.
	 *
	 *	@param	uiUser the user wishing to communicate with this component.
	 */
	virtual void Run(UiUser &uiUser) = 0;

	/**
	 *	@brief	Get the name of the ui service this component is attached to.
	 *
	 *	@return	Ui service name.
	 */
	ACTIV_FRAMEWORK_API const std::string &GetUiServiceName() const;

private:
	/**
	 *	@brief	Register the ui component with the main menu.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Register() const;

	/**
	 *	@brief	Deregister the ui component.
	 *
	 *	@retval	STATUS_CODE_SUCCESS
	 *	@retval	...
	 */
	StatusCode Deregister() const;

	ACTIV_FRAMEWORK_API virtual StatusCode OnRequest(HeapMessage &request);

	/**
	 *	@brief	Process a new user request.
	 *
	 *	@param	request the request HeapMessage.
	 */
	void ProcessNewUserRequest(HeapMessage &request);

	const Type			m_type;									///< ui component type
	const std::string	m_uiServiceName;						///< name of ui service
};

} // namespace Activ

#endif // !defined (ACTIV_UI_COMPONENT_H)
