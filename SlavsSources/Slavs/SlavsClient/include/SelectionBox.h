#ifndef SelectionBox_h
#define SelectionBox_h

#include <OgreManualObject.h>
#include <string>

class SelectionBox : public Ogre::ManualObject
{
public :
	SelectionBox();
	~SelectionBox(void);

	void setCorners(float left, float top, float right, float bottom);
	void setCorners(const Ogre::Vector2& topLeft, const Ogre::Vector2& topRight);
};

#endif