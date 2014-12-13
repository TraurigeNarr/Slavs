#include "stdafx.h"

#include "SelectionBox.h"

namespace UI
	{

	SelectionBox::SelectionBox(const std::string& i_name, Ogre::SceneManager& i_manager)
		: ManualObject(i_name)
		, m_scene_manager(i_manager)
		{
		setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY); // when using this, ensure Depth Check is Off in the material
		setUseIdentityProjection(true);
		setUseIdentityView(true);
		setQueryFlags(0);

		m_scene_manager.getRootSceneNode()->attachObject(this);
		}

	SelectionBox::~SelectionBox()
		{
		m_scene_manager.destroyManualObject(getName());
		}

	void SelectionBox::setCorners(float left, float top, float right, float bottom)
		{
		left = left * 2 - 1;
		right = right * 2 - 1;
		top = 1 - top * 2;
		bottom = 1 - bottom * 2;

		clear();
		estimateIndexCount(5);
		begin("", Ogre::RenderOperation::OT_LINE_STRIP);
		colour(Ogre::ColourValue::White);
			position(left, top, -1);
			position(right, top, -1);
			position(right, bottom, -1);
			position(left, bottom, -1);
			position(left, top, -1);
		end();

		setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
		}

	void SelectionBox::setCorners(const Ogre::Vector2& topLeft, const Ogre::Vector2& bottomRight)
		{
		setCorners(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
		}

	} // UI