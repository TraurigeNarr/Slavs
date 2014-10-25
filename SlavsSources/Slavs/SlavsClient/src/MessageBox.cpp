#include "MessageBox.h"
#include "ClientEnums.h"
#include "ScreenManager.h"

#include <Patterns/Singleton.h>

#include <CEGUI\WindowManager.h>

//////////////////////////////////////////////////////////////////////////

ClientMessageBox::ClientMessageBox(const std::string& i_text, int i_state)
{
  BuildGUI(i_text, i_state);
}

ClientMessageBox::~ClientMessageBox()
{
  ScreenBase::ClearWindow(m_root_window);
  Singleton<ScreenManager>::GetInstance().GetCurrentScreen()->GetRootWindow()->removeChild(m_root_window);
}

void ClientMessageBox::BuildGUI(const std::string& i_text, int i_state)
{
  CEGUI::WindowManager* wmgr = CEGUI::WindowManager::getSingletonPtr();

  m_root_window = wmgr->createWindow("DefaultWindow");
  m_root_window->setSize(CEGUI::USize(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.5f, 0)));
  m_root_window->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.25f, 0)));

  CEGUI::Editbox* editbox = static_cast<CEGUI::Editbox*>(wmgr->createWindow("TaharezLook/Editbox"));
  editbox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0.0f), CEGUI::UDim(0.0f, 0.0f))); // Start below the image
  editbox->setWidth(CEGUI::UDim(1.0f, 0.0f)); 
  editbox->setHeight(CEGUI::UDim(0.55f, 0.0f));
  editbox->setMaxTextLength(i_text.size());
  editbox->setText(i_text);

  m_root_window->addChild(editbox);

  Singleton<ScreenManager>::GetInstance().GetCurrentScreen()->GetRootWindow()->addChild(m_root_window);

  CEGUI::Window* button = nullptr;
  //YES
  if(0 != (i_state & MBS_OK))
  {
    button = wmgr->createWindow("TaharezLook/Button");
    button->setText("Yes");
    button->setID(Btn_UI_OK);

    button->setSize(CEGUI::USize(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.3f, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0), CEGUI::UDim(0.6f, 0)));

    button->subscribeEvent(CEGUI::PushButton::EventClicked,
      CEGUI::Event::Subscriber(&ClientMessageBox::ButtonHandler, this));

    m_root_window->addChild(button);
  }
  //IGNORE
  if(0 != (i_state & MBS_Ignore))
  {
    button = wmgr->createWindow("TaharezLook/Button");
    button->setText("Ignore");
    button->setID(Btn_UI_Cancel);

    button->setSize(CEGUI::USize(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.3f, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.4f, 0)));

    button->subscribeEvent(CEGUI::PushButton::EventClicked,
      CEGUI::Event::Subscriber(&ClientMessageBox::ButtonHandler, this));

    m_root_window->addChild(button);
  }
  //CANCEL
  if(0 != (i_state & MBS_Cancel))
  {
    button = wmgr->createWindow("TaharezLook/Button");
    button->setText("Cancel");
    button->setID(Btn_UI_Cancel);

    button->setSize(CEGUI::USize(CEGUI::UDim(0.2f, 0), CEGUI::UDim(0.3f, 0)));
    button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.6f, 0), CEGUI::UDim(0.6f, 0)));

    button->subscribeEvent(CEGUI::PushButton::EventClicked,
      CEGUI::Event::Subscriber(&ClientMessageBox::ButtonHandler, this));

    m_root_window->addChild(button);
  }
}

bool ClientMessageBox::ButtonHandler(const CEGUI::EventArgs &e)
{
  Singleton<ScreenManager>::GetInstance().GetCurrentScreen()->ButtonHandler(e);
  Singleton<ScreenManager>::GetInstance().GetCurrentScreen()->HideMessageBox();
  return true;
}