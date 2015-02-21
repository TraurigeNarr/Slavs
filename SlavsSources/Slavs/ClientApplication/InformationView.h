#ifndef __INFORMATIONVIEW_H__
#define __INFORMATIONVIEW_H__

#include "WindowInformation.h"

namespace UI
	{

	class InformationView
		{
		protected:
			WindowInformation m_information;

		public:
			InformationView(WindowType i_type, int i_id, const std::string& i_string_id)
				: m_information(i_type, i_id, i_string_id)
				{}

			virtual ~InformationView(){}

			WindowInformation GetInformation() const { return m_information; }
		};

	} // UI

#endif