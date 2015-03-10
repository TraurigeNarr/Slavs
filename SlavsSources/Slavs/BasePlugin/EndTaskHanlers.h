#ifndef __ENDTASKHANDLERS_H__
#define __ENDTASKHANDLERS_H__

#include <boost/any.hpp>

namespace BasePlugin
	{

	class GameController;

	void ShowWorldHistoryHandler(const boost::any& i_value, GameController& i_game_controller);
	void ShowVelesHandler(const boost::any& i_value, BasePlugin::GameController& i_game_controller);

	} // BasePluin

#endif