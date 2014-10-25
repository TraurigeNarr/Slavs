#include "SteeringBehaviors.h"
//components
#include "SDynamicObjCom.h"
#include "SStaticObjCom.h"
//server
#include "IController.h"
#include "SGameContext.h"
//math
#include "2D/Transformations.h"
//standard
#include <limits>

SteeringBehaviors::SteeringBehaviors(SDynamicObjCom* movingAgent)
	: m_pVehicle(movingAgent)
{
	m_vSteeringForce.Zero();
	m_vTarget.Zero();
	m_fDBoxLength = m_pVehicle->GetBRadius();
}

SteeringBehaviors::~SteeringBehaviors()
{}


//----------------------- Calculate --------------------------------------
//
//  calculates the accumulated steering force according to the method set
//  in m_SummingMethod
//------------------------------------------------------------------------
Vector2D SteeringBehaviors::Calculate()
{
	m_vSteeringForce.Zero();

	//calculate neighbors in cell-space if any of the following 3 group
	//behaviors are switched on
	if (On(separation) || On(allignment) || On(cohesion))
	{
		m_pVehicle->GetOwner()->GetContext()->TagDynamicObjectsWithinViewRange(m_pVehicle, m_pVehicle->GetBRadius());
	}

	m_vSteeringForce = CalculatePrioritized();

	return m_vSteeringForce;
}

//---------------------- CalculatePrioritized ----------------------------
//
//  this method calls each active steering behavior in order of priority
//  and accumulates their forces until the max steering force magnitude
//  is reached, at which time the function returns the steering force 
//  accumulated to that  point
//------------------------------------------------------------------------
Vector2D SteeringBehaviors::CalculatePrioritized()
{
	Vector2D force;
	if(On(obstacle_avoidance))
	{
		force = ObstacleAvoidance(m_pVehicle->GetOwner()->GetContext()->GetObstacles());

		if(!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if(On(separation))
	{
		force = Separation(m_pVehicle->GetOwner()->GetContext()->GetDynamicObjects());

		if(!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(seek))
	{
		force = Seek(m_vTarget);

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	return m_vSteeringForce;
}

//--------------------- AccumulateForce ----------------------------------
//
//  This function calculates how much of its max steering force the 
//  vehicle has left to apply and then applies that amount of the
//  force to add.
//------------------------------------------------------------------------
bool SteeringBehaviors::AccumulateForce(Vector2D &RunningTot, Vector2D ForceToAdd)
{

	//calculate how much steering force the vehicle has used so far
	float MagnitudeSoFar = RunningTot.Length();

	//calculate how much steering force remains to be used by this vehicle
	float MagnitudeRemaining = m_pVehicle->GetMaxForce() - MagnitudeSoFar;

	//return false if there is no more force left to use
	if (MagnitudeRemaining <= 0.0) return false;

	//calculate the magnitude of the force we want to add
	float MagnitudeToAdd = ForceToAdd.Length();

	//if the magnitude of the sum of ForceToAdd and the running total
	//does not exceed the maximum force available to this vehicle, just
	//add together. Otherwise add as much of the ForceToAdd vector is
	//possible without going over the max.
	if (MagnitudeToAdd < MagnitudeRemaining)
	{
		RunningTot += ForceToAdd;
	}

	else
	{
		//add it to the steering force
		RunningTot += (Vec2DNormalize(ForceToAdd) * MagnitudeRemaining); 
	}

	return true;
}


/************************************************************************/
/*                   START OF BEHAVIORS                                 */
/************************************************************************/

//------------------------------- Seek -----------------------------------
//
//  Given a target, this behavior returns a steering force which will
//  direct the agent towards the target
//------------------------------------------------------------------------
Vector2D SteeringBehaviors::Seek(Vector2D TargetPos)
{
	//only flee if the target is within 'panic distance'. Work in distance
  //squared space.
 /* const float PanicDistanceSq = 100.0f * 100.0;
  if (Vec2DDistanceSq(m_pVehicle->GetPosition(), target) > PanicDistanceSq)
  {
    return Vector2D(0,0);
  }
  */
	Vector2D DesiredVelocity = Vec2DNormalize(TargetPos - m_pVehicle->GetPosition())
		* m_pVehicle->GetMaxSpeed();

	return (DesiredVelocity - m_pVehicle->GetVelocity());
}

//----------------------------- Flee -------------------------------------
//
//  Does the opposite of Seek
//------------------------------------------------------------------------
Vector2D SteeringBehaviors::Flee(Vector2D TargetPos)
{
	Vector2D DesiredVelocity = Vec2DNormalize(m_pVehicle->GetPosition() - TargetPos) 
		* m_pVehicle->GetMaxSpeed();

	return (DesiredVelocity - m_pVehicle->GetVelocity());
}


//---------------------- ObstacleAvoidance -------------------------------
//
//  Given a vector of CObstacles, this method returns a steering force
//  that will prevent the agent colliding with the closest obstacle
//------------------------------------------------------------------------
Vector2D SteeringBehaviors::ObstacleAvoidance(const std::list<SStaticObjCom* const>& obstacles)
{
	//the detection box length is proportional to the agent's velocity
	m_fDBoxLength = m_pVehicle->GetBRadius() + m_pVehicle->GetSpeed() / m_pVehicle->GetMaxSpeed();

	//tag all obstacles within range of the box for processing
	m_pVehicle->GetOwner()->GetOwner()->GetContext()->TagObstaclesWithinViewRange(m_pVehicle, m_fDBoxLength);

	//this will keep track of the closest intersecting obstacle (CIB)
	SStaticObjCom* ClosestIntersectingObstacle = NULL;

	//this will be used to track the distance to the CIB
	float DistToClosestIP = 1e7;

	//this will record the transformed local coordinates of the CIB
	Vector2D LocalPosOfClosestObstacle;

	Obstacles::const_iterator curOb = obstacles.begin();
	Obstacles::const_iterator endIter = obstacles.end();

	while(curOb != endIter)
	{
		//if the obstacle has been tagged within range proceed
		if ((*curOb)->bTag)
		{
			//calculate this obstacle's position in local space
			Vector2D LocalPos = PointToLocalSpace((*curOb)->GetPosition(),
				m_pVehicle->GetHeading(),
				m_pVehicle->GetSide(),
				m_pVehicle->GetPosition());

			//if the local position has a negative x value then it must lay
			//behind the agent. (in which case it can be ignored)
			if (LocalPos.x >= 0)
			{
				//if the distance from the x axis to the object's position is less
				//than its radius + half the width of the detection box then there
				//is a potential intersection.
				float ExpandedRadius = (*curOb)->GetBRadius() + m_pVehicle->GetBRadius();

				if (fabs(LocalPos.y) < ExpandedRadius)
				{
					//now to do a line/circle intersection test. The center of the 
					//circle is represented by (cX, cY). The intersection points are 
					//given by the formula x = cX +/-sqrt(r^2-cY^2) for y=0. 
					//We only need to look at the smallest positive value of x because
					//that will be the closest point of intersection.
					float cX = LocalPos.x;
					float cY = LocalPos.y;

					//we only need to calculate the sqrt part of the above equation once
					float SqrtPart = sqrt(ExpandedRadius*ExpandedRadius - cY*cY);

					float ip = cX - SqrtPart;

					if (ip <= 0.0)
					{
						ip = cX + SqrtPart;
					}

					//test to see if this is the closest so far. If it is keep a
					//record of the obstacle and its local coordinates
					if (ip < DistToClosestIP)
					{
						DistToClosestIP = ip;

						ClosestIntersectingObstacle = *curOb;

						LocalPosOfClosestObstacle = LocalPos;
					}         
				}
			}
		}

		++curOb;
	}

	//if we have found an intersecting obstacle, calculate a steering 
	//force away from it
	Vector2D SteeringForce;

	if (ClosestIntersectingObstacle)
	{
		//the closer the agent is to an object, the stronger the 
		//steering force should be
		float multiplier = 1.0f + (m_fDBoxLength - LocalPosOfClosestObstacle.x) /
			m_fDBoxLength;

		//calculate the lateral force
		SteeringForce.y = (ClosestIntersectingObstacle->GetBRadius()-
			LocalPosOfClosestObstacle.y)  * multiplier;   

		//apply a braking force proportional to the obstacles distance from
		//the vehicle. 
		const float BrakingWeight = 0.2f;

		SteeringForce.x = (ClosestIntersectingObstacle->GetBRadius() - 
			LocalPosOfClosestObstacle.x) * 
			BrakingWeight;
	}

	//finally, convert the steering vector from local to world space
	return VectorToWorldSpace(SteeringForce,
		m_pVehicle->GetHeading(),
		m_pVehicle->GetSide());
}

//---------------------------- Separation --------------------------------
//
// this calculates a force repelling from the other neighbors
//------------------------------------------------------------------------
Vector2D SteeringBehaviors::Separation(const std::list<SDynamicObjCom* const> &agents)
{
	Vector2D SteeringForce;

	DynamicObjects::const_iterator it = agents.begin();
	DynamicObjects::const_iterator endIt = agents.end();
	Vector2D ToAgent;
	for (it; it != endIt; ++it)
	{
		//make sure this agent isn't included in the calculations and that
		//the agent being examined is close enough. ***also make sure it doesn't
		//include the evade target ***
		if((*it != m_pVehicle) && (*it)->bTag)
		{
			ToAgent = m_pVehicle->GetPosition() - (*it)->GetPosition();

			//scale the force inversely proportional to the agents distance  
			//from its neighbor.
			SteeringForce += Vec2DNormalize(ToAgent)*(m_pVehicle->GetBRadius()*ToAgent.Length());
		}
	}
	return SteeringForce;
}