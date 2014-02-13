#ifndef Profession_h
#define Profession_h

#include <Game/Enumerations.h>
#include <Patterns/Telegram.h>

#include <boost\function.hpp>

class SHumanComponent;


/*
------------------------------------------------------------
This is abstract class and superclass for all professions. 
------------------------------------------------------------
*/
class Profession
{
public:
						Profession(SHumanComponent* human);
							
	virtual				~Profession();

	virtual void		TickPerformed() = 0;
	virtual bool		HandleMessage(const Telegram& msg) = 0;
	virtual bool		IsPlaceOfWork(void *placeOfWork) const = 0;

	int       			GetProfessionType() const { return m_ProfessionType; }
	//returns cycles passed after latest increasing of skill
	int					    GetCurrentCycles() const { return m_current_tick_for_growth; }
	float				    GetSkill() const { return m_fSkill; }
  //returns number from 0 to 4 (rank of the human in this profession, relative to max skill asymptote)
  virtual size_t  GetRelativeSkill() const = 0;
  bool            HasChanges() const { return m_has_changes; }
  void            ReleaseChanges() { m_has_changes = false; }

  void            SetCallback(boost::function<void(float)>);
protected:
	float				    m_fSkill;
	SHumanComponent*	m_pHuman;
	int         			m_ProfessionType;

	//how many cycles passed after latest increasing of skill
	size_t					m_current_tick_for_growth;
  //number of times of growing
  size_t          m_growing_nums;
  //after initialise decreasing by learning curvature coefficient
  float           m_current_growing_skill;

  bool            m_has_changes;

  boost::function<void(float)> m_callback;
};

#endif