#include "Profession.h"
#include "SHumanComponent.h"

Profession::Profession(SHumanComponent* human)
	: m_pHuman(human), m_fSkill(0),
  m_growing_nums(0), m_current_tick_for_growth(0),
  m_current_growing_skill(0.0f), m_has_changes(true)
{}

Profession::~Profession()
{
  m_callback.clear();
}

void Profession::SetCallback(boost::function<void(float)> i_function_callback)
{
  m_callback = i_function_callback;
}