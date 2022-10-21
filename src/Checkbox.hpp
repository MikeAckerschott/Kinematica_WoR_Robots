#ifndef CHECKBOX_HPP_
#define CHECKBOX_HPP_

#include "Config.hpp"

#include "Widgets.hpp"

#include <functional>
#include <string>

namespace Application
{
	/**
	 * Utility function that creates a Checkbox with aLabel that binds the given ActionFunction with the
	 * wxEVT_COMMAND_BUTTON_CLICKED of the button.
	 *
	 * @param aParent
	 * @param aLabel
	 * @param anActionFunction
	 * @return
	 */
	template< typename ActionFunction >
	Checkbox* makeCheckbox(	Window* aParent,
							const std::string& aLabel,
							const ActionFunction& anActionFunction)
	{
		Checkbox* checkbox = new Checkbox( aParent, DEFAULT_ID, WXSTRING(aLabel));
		checkbox->Bind(wxEVT_CHECKBOX, anActionFunction);
		return checkbox ;
	}
} // namespace Application

#endif /* CHECKBOX_HPP_ */
