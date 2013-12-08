#ifndef SteeringBehaviors_h
#define SteeringBehaviors_h

/***********************************************************************
Class that encapsulates steering behaviors for Dynamic objects.
***********************************************************************/
#include <Math/Vector2D.h>
#include <list>

class SDynamicObjCom;
class SStaticObjCom;

class SteeringBehaviors
{
private:
	enum behavior_type
	{
		none               = 0x00000,
		seek               = 0x00002,
		flee               = 0x00004,
		arrive             = 0x00008,
		wander             = 0x00010,
		cohesion           = 0x00020,
		separation         = 0x00040,
		allignment         = 0x00080,
		obstacle_avoidance = 0x00100,
		wall_avoidance     = 0x00200,
		follow_path        = 0x00400,
		pursuit            = 0x00800,
		evade              = 0x01000,
		interpose          = 0x02000,
		hide               = 0x04000,
		flock              = 0x08000,
		offset_pursuit     = 0x10000,
	};
public:
	SteeringBehaviors(SDynamicObjCom* movingAgent);
	virtual ~SteeringBehaviors();

	//calculates and sums the steering forces from any active behaviors
	Vector2D		Calculate();

	void			SetTarget(const Vector2D t){m_vTarget = t;}

	void FleeOn(){m_iFlags |= flee;}
	void SeekOn(){m_iFlags |= seek;}
	void ArriveOn(){m_iFlags |= arrive;}
	void WanderOn(float jitter = 100.0f){m_iFlags |= wander;}
	//void PursuitOn(Vehicle* v){m_iFlags |= pursuit; m_pTargetAgent1 = v;}
	//void EvadeOn(Vehicle* v){m_iFlags |= evade; m_pTargetAgent1 = v;}
	void CohesionOn(){m_iFlags |= cohesion;}
	void SeparationOn(){m_iFlags |= separation;}
	void AlignmentOn(){m_iFlags |= allignment;}
	void ObstacleAvoidanceOn(){m_iFlags |= obstacle_avoidance;}
	void WallAvoidanceOn(){m_iFlags |= wall_avoidance;}
	void FollowPathOn(){m_iFlags |= follow_path;}
	//void InterposeOn(Vehicle* v1, Vehicle* v2){m_iFlags |= interpose; m_pTargetAgent1 = v1; m_pTargetAgent2 = v2;}
	//void HideOn(Vehicle* v){m_iFlags |= hide; m_pTargetAgent1 = v;}
	//void OffsetPursuitOn(Vehicle* v1, const Vector2D offset){m_iFlags |= offset_pursuit; m_vOffset = offset; m_pTargetAgent1 = v1;}  
	void FlockingOn(){CohesionOn(); AlignmentOn(); SeparationOn(); WanderOn();}

	void FleeOff()  {if(On(flee))   m_iFlags ^=flee;}
	void SeekOff()  {if(On(seek))   m_iFlags ^=seek;}
	void ArriveOff(){if(On(arrive)) m_iFlags ^=arrive;}
	void WanderOff(){if(On(wander)) m_iFlags ^=wander;}
	//void PursuitOff(){if(On(pursuit)) m_iFlags ^=pursuit;}
	//void EvadeOff(){if(On(evade)) m_iFlags ^=evade;}
	void CohesionOff(){if(On(cohesion)) m_iFlags ^=cohesion;}
	void SeparationOff(){if(On(separation)) m_iFlags ^=separation;}
	void AlignmentOff(){if(On(allignment)) m_iFlags ^=allignment;}
	void ObstacleAvoidanceOff(){if(On(obstacle_avoidance)) m_iFlags ^=obstacle_avoidance;}
	void WallAvoidanceOff(){if(On(wall_avoidance)) m_iFlags ^=wall_avoidance;}
	void FollowPathOff(){if(On(follow_path)) m_iFlags ^=follow_path;}
	//void InterposeOff(){if(On(interpose)) m_iFlags ^=interpose;}
	//void HideOff(){if(On(hide)) m_iFlags ^=hide;}
	//void OffsetPursuitOff(){if(On(offset_pursuit)) m_iFlags ^=offset_pursuit;}
	void FlockingOff(){CohesionOff(); AlignmentOff(); SeparationOff(); WanderOff();}

	bool isFleeOn(){return On(flee);}
	bool isSeekOn(){return On(seek);}
	bool isArriveOn(){return On(arrive);}
	bool isWanderOn(){return On(wander);}
	bool isPursuitOn(){return On(pursuit);}
	bool isEvadeOn(){return On(evade);}
	bool isCohesionOn(){return On(cohesion);}
	bool isSeparationOn(){return On(separation);}
	bool isAlignmentOn(){return On(allignment);}
	bool isObstacleAvoidanceOn(){return On(obstacle_avoidance);}
	bool isWallAvoidanceOn(){return On(wall_avoidance);}
	bool isFollowPathOn(){return On(follow_path);}
	bool isInterposeOn(){return On(interpose);}
	bool isHideOn(){return On(hide);}
	bool isOffsetPursuitOn(){return On(offset_pursuit);}
private:
	//this function tests if a specific bit of m_iFlags is set
	bool			On(behavior_type bt){return (m_iFlags & bt) == bt;}

	bool			AccumulateForce(Vector2D &sf, Vector2D ForceToAdd);

	Vector2D		CalculatePrioritized();

	/************************************************************************/
	/*                  BEGIN BEHAVIOR DECLARATIONS                         */
	/************************************************************************/

	//this behavior moves the agent towards a target position
	Vector2D		Seek(Vector2D TargetPos);

	//this behavior returns a vector that moves the agent away
	//from a target position
	Vector2D		Flee(Vector2D TargetPos);
	//this returns a steering force which will attempt to keep the agent 
	//away from any obstacles it may encounter
	//now realize as avoidance from static objects - avoidance from dynamic objects does not calculated
	Vector2D ObstacleAvoidance(const std::list<SStaticObjCom* const>& obstacles);

	Vector2D Separation(const std::list<SDynamicObjCom* const> &agents);
/*
	//this behavior is similar to seek but it attempts to arrive 
	//at the target position with a zero velocity
	Vector2D Arrive(Vector2D     TargetPos,
		Deceleration deceleration);

	//this behavior predicts where an agent will be in time T and seeks
	//towards that point to intercept it.
	Vector2D Pursuit(const Vehicle* agent);

	//this behavior maintains a position, in the direction of offset
	//from the target vehicle
	Vector2D OffsetPursuit(const Vehicle* agent, const Vector2D offset);

	//this behavior attempts to evade a pursuer
	Vector2D Evade(const Vehicle* agent);

	//this behavior makes the agent wander about randomly
	Vector2D Wander();

	//this returns a steering force which will attempt to keep the agent 
	//away from any obstacles it may encounter
	Vector2D ObstacleAvoidance(const std::vector<BaseGameEntity*>& obstacles);

	//this returns a steering force which will keep the agent away from any
	//walls it may encounter
	Vector2D WallAvoidance(const std::vector<Wall2D> &walls);


	//given a series of Vector2Ds, this method produces a force that will
	//move the agent along the waypoints in order
	Vector2D FollowPath();

	//this results in a steering force that attempts to steer the vehicle
	//to the center of the vector connecting two moving agents.
	Vector2D Interpose(const Vehicle* VehicleA, const Vehicle* VehicleB);

	//given another agent position to hide from and a list of BaseGameEntitys this
	//method attempts to put an obstacle between itself and its opponent
	Vector2D Hide(const Vehicle* hunter, const std::vector<BaseGameEntity*>& obstacles);
	*/

	//a pointer to the owner of this instance
	SDynamicObjCom*	m_pVehicle;

	//the steering force created by the combined effect of all
	//the selected behaviors
	Vector2D		m_vSteeringForce;

	//the current target
	Vector2D		m_vTarget;

	//length of the 'detection box' utilized in obstacle avoidance
	float			m_fDBoxLength;

	//binary flags to indicate whether or not a behavior should be active
	int				m_iFlags;
	//This is the distance the wander circle is projected in front of the agent
	float			m_fWanderJitter;
};

#endif