#include "stdafx.h"

#include "SendSelectionTask.h"

#include "Application.h"
#include "ClientGameContext.h"

#include <Common/Utilities/TemplateFunctions.h>

#include <Common/Math/Vector2D.h>

#include <Network/include/Connection.h>
#include <Network/PacketType.h>

namespace
	{
	Ogre::Vector3 GetWorldPoint(float mouseX, float mouseY)
		{
		auto& framework = ClientGame::appInstance.GetOgreFramework();

		const int w = framework.GetViewport()->getActualWidth();
		const int h = framework.GetViewport()->getActualHeight();
		float x = mouseX * w;
		float y = mouseY * h;
		Ogre::Ray cameraRay = framework.GetCamera()->getCameraToViewportRay(mouseX, mouseY);
		Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
		std::pair<bool, Ogre::Real> t = cameraRay.intersects(plane);
		if (t.first)
			return cameraRay.getPoint(t.second);
		return Ogre::Vector3();
		}
	}

namespace Tasks
	{

	SendSelcetionTask::SendSelcetionTask(int i_task_id, net::Connection& i_connection, ClientGameContext& i_context)
		: Task(i_task_id)
		, m_connection(i_connection)
		, m_context(i_context)
		, m_last_selected_number(0)
		{}

	SendSelcetionTask::~SendSelcetionTask(){}

	void SendSelcetionTask::CompleteImpl(const boost::any& i_value)
		{
		typedef std::pair<Ogre::Vector2, Ogre::Vector2> StartStopPair;
		auto pair = boost::any_cast<StartStopPair>(i_value);

		auto start = GetWorldPoint(pair.first.x, pair.first.y);
		auto stop = GetWorldPoint(pair.second.x, pair.second.y);

		if (start.x > stop.x)
			std::swap(start.x, stop.x);
		if (start.y > stop.y)
			std::swap(start.y, stop.y);

		auto obj_ids = m_context.GetObjcetsInBox(Vector2D(start.x, start.z), Vector2D(stop.x, stop.z));

		if (!obj_ids.empty())
			{
			const size_t packet_size = sizeof(Network::PacketType) + sizeof(long)*obj_ids.size();
			std::unique_ptr<char[]> p_buffer(new char[packet_size]);
			size_t offset = 0;
			ToChar(Network::PacketType::PT_Selection, &p_buffer[0], sizeof(Network::PacketType));
			offset += sizeof(Network::PacketType);
			
			for (long id : obj_ids)
				{
				ToChar(id, &p_buffer[offset], sizeof(long));
				offset += sizeof(long);
				}

			m_connection.SendPacket(&p_buffer[0], packet_size);
			m_last_selected_number = obj_ids.size();
			}
		else if (m_last_selected_number != 0)
			{
			const size_t packet_size = sizeof(Network::PacketType);
			std::unique_ptr<char[]> p_buffer(new char[packet_size]);
			size_t offset = 0;
			ToChar(Network::PacketType::PT_Selection, &p_buffer[0], sizeof(Network::PacketType));
			offset += sizeof(Network::PacketType);
			m_connection.SendPacket(&p_buffer[0], packet_size);
			}

		}

	void SendSelcetionTask::DiscardImpl(const boost::any& i_value)
		{
		// we should do nothing here
		}

	} // Tasks