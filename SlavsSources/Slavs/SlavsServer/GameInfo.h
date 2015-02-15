#ifndef __GAMEINFO_H__
#define __GAMEINFO_H__

namespace SDK
	{

	enum class RequestedAction
		{
		None = -1,
		ShowInformation = 0,
		};

	struct GameInfo
		{
		// change to vector of actions
		int m_requested_action;
		// change to map?
		int m_dialog_id;
		};

	} // SDK

#endif