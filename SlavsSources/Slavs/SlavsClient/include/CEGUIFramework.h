#ifndef CEGUIFramework_h
#define CEGUIFramework_h
#include "OgreFramework.h"

#include <CEGUI\CEGUI.h>
#include <CEGUI\RendererModules/Ogre/Renderer.h>
#include <CEGUI\ResourceProvider.h>

#include <OgreRoot.h>

class CEGUIFramework
{
public:
	CEGUIFramework(void);
	~CEGUIFramework(void);

	bool initCEGUI();

	CEGUI::OgreRenderer *m_cRenderer;
private:
	bool initialiseResourceGroupDirectories();
	void initialiseDefaultResourceGroups();
	void destroy();
};

#endif