#include "ScreenBase.h"
#include "InputManager.h"
#include "MessageBox.h"

#include <vector>
#include <algorithm>
#include <cassert>

//////////////////////////////////////////////////////////////////////////

ScreenBase::ScreenBase()
	: m_pRootCEGUIWindow(NULL), m_message_box(nullptr)
{}

ScreenBase::~ScreenBase()
{
  if(nullptr != m_message_box)
    delete m_message_box;
  CEGUI::WindowManager::getSingletonPtr()->destroyWindow(m_pRootCEGUIWindow);
	CEGUI::WindowManager::getSingletonPtr()->destroyAllWindows();
}

bool ScreenBase::ButtonHandler(const CEGUI::EventArgs &e)
{
	const std::vector<InputSubscriber*> &subs = Singleton<InputManager>::GetInstancePtr()->GetSubscribers();
	const CEGUI::WindowEventArgs* args = static_cast<const CEGUI::WindowEventArgs*>(&e);
	std::for_each(subs.begin(), subs.end(), [&args](InputSubscriber* subscriber)
	{
		subscriber->ButtonPressed((ButtonID)args->window->getID());
	});
	return true;
}

void ScreenBase::ClearWindow(CEGUI::Window* wnd)
{
	std::vector<CEGUI::Window*> children;//list of children of the wnd
	CEGUI::WindowManager* wmgr = CEGUI::WindowManager::getSingletonPtr();
	for(unsigned int i = 0; i < wnd->getChildCount(); ++i)
		children.push_back(wnd->getChildAtIdx(i));
	for(unsigned int i = 0; i < children.size(); ++i)
		wmgr->destroyWindow(children.at(i));
}

void ScreenBase::ShowMessageBox(const std::string& i_text, int i_state)
{
  m_message_box = new ClientMessageBox(i_text, i_state);
}

void ScreenBase::HideMessageBox()
{
  if(nullptr != m_message_box)
    delete m_message_box;
  m_message_box = nullptr;
}