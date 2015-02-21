#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "InformationView.h"

namespace UI
	{

	class Dialog : public InformationView
		{
		private:
			std::string m_caption;
			std::string m_text;
			std::string m_image;

		public:
			Dialog(WindowType i_type, int i_id, const std::string& i_string_id)
				: InformationView(i_type, i_id, i_string_id)
				, m_caption()
				, m_text()
				, m_image()
				{}

			Dialog(WindowType i_type, int i_id, const std::string& i_string_id, const std::string& i_text, const std::string& i_caption, const std::string& i_image)
				: InformationView(i_type, i_id, i_string_id)
				, m_caption(i_caption)
				, m_text(i_text)
				, m_image(i_image)
				{}

			virtual ~Dialog(){}

			void SetCaption(const std::string& i_caption) { m_caption = i_caption; }
			std::string GetCaption() const { return m_caption; }

			void  SetText(const std::string& i_text) { m_text = i_text; }
			std::string GetText() const { return m_text; }

			std::string GetImage() const { return m_image; }
		};

	} // UI

#endif