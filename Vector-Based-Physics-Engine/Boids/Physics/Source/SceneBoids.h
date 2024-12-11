#ifndef SCENE_BOIDS_H
#define SCENE_BOIDS_H

#include "AIHerds.h"
#include <vector>
#include "SceneHerdBase.h"

class SceneBoids : public SceneHerdBase
{

public:
	SceneBoids();
	~SceneBoids();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(AIHERDS*go);

	AIHERDS* FetchGO();

	void Wrap(float& val, float bound) {
		if (val < 0)val = bound;
		if (val > bound)val = 0;
	}

	Vector3 RotateVector(const Vector3& vec, float radian)
	{
		return Vector3(vec.x * cos(radian) + vec.y * -sin(radian),
			vec.x * sin(radian) + vec.y * cos(radian), 0.f);
	}

protected:

	//Physics
	std::vector<AIHERDS*> m_goList;
	float m_speed = 1.0f;
	float m_worldWidth;
	float m_worldHeight;

	AIHERDS *leader;

	//boids have 
	//position in x,y
	//velocity in x,y,z
	//orientation, ie direction in degrees 0-360

	//use of raycasting to move away from obstacles

	//seperation
	//steer away from other agents
	//use of ray casting to see where 
	
	//alignment
	//steer towards average heading of local flock mates
	//use or radial check to find which agents are close to it, 
	//then finding average orientation of all agents in proximity
	//steering itself towards that orientation
	
	//cohesion
	//weighted to steer towards center of flock
	//use or radial check to find which agents are close to it, 
	//then finding average position of all agents in proximity
	//steering itself towards that position


};

#endif