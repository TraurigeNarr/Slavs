#include "CEGUIFramework.h"

#include <Utilities/FileUtilities.h>

#include <OgreRenderTarget.h>

#include <Patterns/Singleton.h>

const int PATH_MAX = 255;

CEGUIFramework::CEGUIFramework(void)
: m_cRenderer(NULL)
{
}


CEGUIFramework::~CEGUIFramework(void)
{
	/*if(m_cRenderer)
		CEGUI::OgreRenderer::destroy(*m_cRenderer);
	*/
}

bool CEGUIFramework::initCEGUI()
{
	try
	{
    new CEGUI::DefaultLogger();
		Ogre::RenderTarget *rendTarget = &Singleton<OgreFramework>::GetInstancePtr()->GetAppRenderTarget();
		m_cRenderer = &CEGUI::OgreRenderer::bootstrapSystem(*rendTarget);
    Ogre::String logFileName = FileUtilities::JoinPath(FileUtilities::GetApplicationDirectory(), "cegui.log");
    CEGUI::Logger::getSingletonPtr()->setLogFilename(logFileName);
    initialiseDefaultResourceGroups();
	}
	catch (std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "Raw initialization!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}

	try
	{
    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
		
		//initialize tooltips
    CEGUI::GUIContext& default_context = CEGUI::System::getSingleton().getDefaultGUIContext();
    default_context.setDefaultTooltipType("TaharezLook/Tooltip"); // Set the name of the default tooltip
		CEGUI::Tooltip* tooltip = default_context.getDefaultTooltipObject();
		if(0 != tooltip)
		{
			tooltip->setHoverTime(0.02f); // Display the tooltip after the mouse has been hovering over the widget
			tooltip->setDisplayTime(15.0f); // Display for 15 seconds then disappear
		}
	}
	catch (CEGUI::Exception& e)
	{
		MessageBoxA(NULL, e.what(), "An exception has occurred while initializing CEGUI!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		// something went wrong - probably did not load the imageset first!
		//TODO: Handle error,
	}
	return true;
}

bool CEGUIFramework::initialiseResourceGroupDirectories()
{
	CEGUI::DefaultResourceProvider* rp =
		static_cast<CEGUI::DefaultResourceProvider*>
		(CEGUI::System::getSingleton().getResourceProvider());

	const char* dataPathPrefix = FileUtilities::GetApplicationDirectory().c_str();

	try
	{
    std::string path = FileUtilities::JoinPath(FileUtilities::GetApplicationDirectory(), "Client\\Media\\CEGUI\\layouts");
		rp->setResourceGroupDirectory("Layouts", path.c_str());
	}
	catch (...)
	{
		return false;
	}
	return true;
}


void CEGUIFramework::initialiseDefaultResourceGroups()
{
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
}