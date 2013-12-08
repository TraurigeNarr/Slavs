#include "HelpBox.h"
#include "ClientEnums.h"

#include <Utilities\FileUtilities.h>

#include <string>
#include <fstream>
#include <Windows.h>


const std::string HELP_FILE = "help.txt";

HelpBox::HelpBox(CEGUI::Window* i_root_window)
  : m_root_window(i_root_window), m_edit_box(nullptr), m_ok_btn(nullptr), 
  m_modal_window(nullptr)
{
}

HelpBox::~HelpBox()
{

}


void HelpBox::Init()
{
  CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();

  /* ScrollablePane */
  m_modal_window = wmgr.createWindow("DefaultWindow");
  m_modal_window->setSize(CEGUI::USize(CEGUI::UDim(0.7f, 0), CEGUI::UDim(0.5f, 0)));
  m_modal_window->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.25f, 0)));

  m_root_window->addChild(m_modal_window);

  m_ok_btn = wmgr.createWindow("TaharezLook/Button");
  m_ok_btn->setText("OK");
  m_ok_btn->setID(Btn_UI_Menu);
  m_ok_btn->subscribeEvent(CEGUI::PushButton::EventClicked,
    CEGUI::Event::Subscriber(&HelpBox::ButtonHandler, this));
  m_ok_btn->setSize(CEGUI::USize(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.1f, 0.0f)));
  m_ok_btn->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0), CEGUI::UDim(0.45f, 0.0f)));

  m_edit_box = static_cast<CEGUI::MultiLineEditbox*>(wmgr.createWindow("TaharezLook/MultiLineEditbox"));
  m_edit_box->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15f, 0.0f), CEGUI::UDim(0.0f, 0.0f))); // Start below the image
  m_edit_box->setWidth(CEGUI::UDim(0.85f, 0.0f)); 
  m_edit_box->setHeight(CEGUI::UDim(1.0f, 0.0f));
  
  m_modal_window->addChild(m_ok_btn);
  m_modal_window->addChild(m_edit_box);

  m_edit_box->setReadOnly(true); 

  std::string file_name = FileUtilities::GetApplicationDirectory() + HELP_FILE;

  std::ifstream ins(file_name.c_str(), std::ios::in | std::ios::binary);

  if(ins.bad() || ins.fail() || ERROR_ACCESS_DENIED == GetLastError())
  {
    ins.close();
    throw std::exception("There is no help file.\n");
  }

  std::string help_text;
  ins.seekg(0, std::ios::end);
  help_text.resize(ins.tellg());
  ins.seekg(0, std::ios::beg);
  ins.read(&help_text[0], help_text.size());
  ins.close();

  m_edit_box->appendText(help_text);
  Deactivate();
}

void HelpBox::Activate()
{
  m_modal_window->setVisible(true);
  m_modal_window->activate();
  m_modal_window->setModalState(true);

  m_edit_box->setVisible(true);
  m_edit_box->activate();

  m_ok_btn->setVisible(true);
  m_ok_btn->activate();
}

void HelpBox::Deactivate()
{
  m_modal_window->setVisible(false);
  m_modal_window->deactivate();
  m_modal_window->setModalState(false);

  m_edit_box->setVisible(false);
  m_edit_box->deactivate();

  m_ok_btn->setVisible(false);
  m_ok_btn->deactivate();
}

bool HelpBox::ButtonHandler(const CEGUI::EventArgs &e)
{
  const CEGUI::WindowEventArgs* args = static_cast<const CEGUI::WindowEventArgs*>(&e);

  switch(args->window->getID())
  {
  case Btn_UI_Menu:
    Deactivate();
    break;
  case Btn_UI_Help:
    Activate();
    break;
  }
  return true;
}