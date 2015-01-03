#ifndef __BASEPLUGIN_PROFESSION_H__
#define __BASEPLUGIN_PROFESSION_H__

namespace BasePlugin
	{

	class Profession
		{
		protected:
			int m_profession_id;

		public:
			Profession(int i_proffession_id) : m_profession_id(i_proffession_id){}
			virtual ~Profession(){}

			int GetId() const { return m_profession_id; }

			
		};

	} // BasePlugin

#endif