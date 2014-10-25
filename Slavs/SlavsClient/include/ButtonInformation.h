#ifndef ButtonInformation_h
#define ButtonInformation_h

#include <string>
#include "ClientEnums.h"

struct ButtonInformation
{
	std::string BtnText;
	std::string BtnToolTip;
	ButtonID	BtnID;

	int			Position;

	ButtonInformation(ButtonID btnID, int position = 0, const std::string& text = "", const std::string& toolTip = "")
		: BtnID(btnID), Position(position), BtnText(text), BtnToolTip(toolTip)
	{}
};
#endif