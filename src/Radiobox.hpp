#ifndef RADIOBOX_HPP_
#define RADIOBOX_HPP_

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
	Radiobox* makeRadiobox(	Window* aParent,
							const std::string& aLabel,
							const ActionFunction& anActionFunction)
	{
		Radiobox* radiobox= new Radiobox( aParent, DEFAULT_ID, WXSTRING(aLabel));
		radiobox->Bind(wxEVT_RADIOBOX, anActionFunction);
		return radiobox;
	}

	template< typename ActionFunction >
	Radiobox* makeRadiobox(	Window* aParent,
							int aNumberOfChoices,
							const wxString aListOfChoices[],
							const ActionFunction& anActionFunction,
							const std::string& aLabel = "",
							long aStyle = wxRA_SPECIFY_ROWS,
							int aNumberOfRowsOrColumns = 0)
	{
		Radiobox* radiobox = new Radiobox(	aParent,
											DEFAULT_ID,
											WXSTRING(aLabel),
											DefaultPosition,
											DefaultSize,
											aNumberOfChoices,
											aListOfChoices,
											aNumberOfRowsOrColumns,
											aStyle);

		radiobox->Bind(wxEVT_RADIOBOX, anActionFunction);

		return radiobox;
	}

} // namespace Application




#endif /* RADIOBOX_HPP_ */
