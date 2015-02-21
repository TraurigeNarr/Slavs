#ifndef __UICLIP_H__
#define __UICLIP_H__

#include "InformationView.h"

namespace UI
	{
	
	class Clip : public InformationView
		{
		public:
			struct FrameInformation
				{
				std::string m_text;
				std::string m_image;
				long				m_show_time;

				FrameInformation(const std::string& i_text, const std::string& i_image, long i_show_time)
					: m_text(i_text)
					, m_image(i_image)
					, m_show_time(i_show_time)
					{}

				FrameInformation()
					: m_text()
					, m_image()
					, m_show_time(0)
					{}
				};

		private:
			std::vector<FrameInformation> m_frames;

			std::string m_caption;

			int m_current_frame_index;

		public:
			Clip()
				: InformationView(WindowType::MovieClip, -1, "")
				, m_current_frame_index(-1)
				, m_caption()
				{}

			Clip(int i_id, const std::string& i_string_id, const std::string& i_caption)
				: InformationView(WindowType::MovieClip, i_id, i_string_id)
				, m_current_frame_index(0)
				, m_caption(i_caption)
				{}

			virtual ~Clip(){}

			FrameInformation	GetFrame(size_t i_index) const { return m_frames.at(i_index); }
			std::string				GetCaption() const { return m_caption; }
			void AddFrame(const std::string& i_text, const std::string& i_image, float i_show_time);

			FrameInformation	GetCurrentFrame() const;
			void							MoveToNextFrame();
			void							MoveToPreviousFrame();
			void							MoveToLastFrame();
			size_t						GetFramesCount() const { return m_frames.size(); }
			bool							IsLastFrame() const;
			void							Reset();
		};

	} // UI

#endif