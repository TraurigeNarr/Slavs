#ifndef __WINDOWINFORMATION_H__
#define __WINDOWINFORMATION_H__

namespace UI
	{
	enum class WindowType
		{
		None,
		Information,
		Dialog,
		MovieClip,
		};

	struct WindowInformation
		{
		WindowType	m_type;
		int					m_id;
		std::string m_string_id;

		int					m_flags;
		
		WindowInformation()
			: m_type(WindowType::None)
			, m_id(-1)
			{}

		WindowInformation(WindowType i_type, int i_id, const std::string& i_string_id)
			: m_type(i_type)
			, m_id(i_id)
			, m_string_id(i_string_id)
			{}
		};

	} // UI

#endif