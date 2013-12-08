#ifndef Goal_Evaluator_h
#define Goal_Evaluator_h

class SHumanComponent;

/*
interface to classes that evaluates desirability of some action
*/

class Goal_Evaluator
{
public:
	Goal_Evaluator(float fHumanBias)
		: m_fHumanBias(fHumanBias)
	{}
	virtual ~Goal_Evaluator(){}

	//returns value between 0 and 1
	virtual float CalculateDesirability(SHumanComponent* pHuman) = 0;

	//adds goal to Human`s brain
	virtual void SetGoal(SHumanComponent* pHuman) = 0;
protected:
	float m_fHumanBias;
};

#endif