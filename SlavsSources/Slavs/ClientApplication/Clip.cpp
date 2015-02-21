#include "stdafx.h"

#include "Clip.h"

namespace UI
	{

	void Clip::AddFrame(const std::string& i_text, const std::string& i_image, float i_show_time)
		{
		m_frames.push_back(FrameInformation(i_text, i_image, static_cast<long>(i_show_time*1000)));
		}

	Clip::FrameInformation Clip::GetCurrentFrame() const
		{
		if (m_frames.size() <= m_current_frame_index || 0 > m_current_frame_index)
			return FrameInformation();
		return m_frames.at(m_current_frame_index);
		}

	void Clip::MoveToNextFrame()
		{
		if (IsLastFrame())
			return;
		++m_current_frame_index;
		}

	void Clip::MoveToPreviousFrame()
		{
		if (m_current_frame_index == 0)
			return;
		--m_current_frame_index;
		}

	void Clip::MoveToLastFrame()
		{
		m_current_frame_index = static_cast<int>(m_frames.size() - 1);
		}

	bool Clip::IsLastFrame() const
		{
		return m_current_frame_index == m_frames.size() - 1;
		}

	void Clip::Reset()
		{
		m_current_frame_index = 0;
		}

	} // UI