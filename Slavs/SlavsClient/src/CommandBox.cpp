#include "CommandBox.h"

#include "HUD.h"
#include "VisualInformation.h"

#include <CEGUI\Size.h>

CommandBox::CommandBox(HUD* owner, CEGUI::Window* rootWindow)
	: m_pOwner(owner), m_pRootWindow(rootWindow)
{
}

CommandBox::~CommandBox()
{}

void CommandBox::Show(ObjectInformation* objInfo)
{
	m_pRootWindow->activate();
	DrawCommandBox(objInfo);
}

void CommandBox::Hide()
{
	m_pRootWindow->deactivate();
	ScreenBase::ClearWindow(m_pRootWindow);
}

bool CommandBox::ButtonHandler(const CEGUI::EventArgs &e)
{
	return m_pOwner->ButtonHandler(e);
}

void CommandBox::DrawCommandBox(ObjectInformation* objInfo)
{
	const std::vector<ButtonID>& commands = objInfo->GetCommands();
	CEGUI::WindowManager* wmgr = CEGUI::WindowManager::getSingletonPtr();
	
	unsigned int k = 0;//for commands
	for(unsigned int i = 0; i < 3; ++i)
	{
		for(unsigned int j = 0; j < 3; ++j)
		{
			//create button
			CEGUI::Window *button = wmgr->createWindow("TaharezLook/Button");
			button->setSize(CEGUI::USize(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.3f, 0)));
			button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3f*j, 0), CEGUI::UDim(0.3f*i, 0)));
			m_pRootWindow->addChild(button);

			//get button information if there is some in VisualInformation for such command ID
			ButtonInformation *btnInfo = NULL;
			if(commands.size() > k)
				btnInfo = Singleton<VisualInformation>::GetInstancePtr()->GetBtnInformation(commands.at(k));


			if(NULL != btnInfo)
			{
				button->setText(btnInfo->BtnText);
				button->setTooltipText(btnInfo->BtnToolTip);
				button->setID(commands.at(k));

				button->subscribeEvent(CEGUI::PushButton::EventClicked,
					CEGUI::Event::Subscriber(&CommandBox::ButtonHandler, this));
			}
			else
			{
				button->setText("");
				button->setTooltipText("");
				button->setID(Btn_None);
			}
			//increase counter for commands
			++k;
		}
	}
}