#ifndef AI_HERDS_H
#define AI_HERDS_H

#include "Vector3.h"

struct AIHERDS
{
	enum AIHERDS_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CUBE,
		GO_CELL,		//triangular boid
		GO_LEADER,		//debug
		GO_FOOD,		//eat if hungry, else follow herd
		GO_WALL,		//avoid obstacles
		GO_TOTAL, //must be last
	};
	enum AISTATE_TYPE
	{
		STATE_IDLE,
		STATE_FLEE,
		STATE_HUNGRY,
		STATE_MIGRATE,
		STATE_MATE,
		STATE_TOTAL,
	};
	AIHERDS_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 direction;
	Vector3 torque;
	bool active = false;
	float mass;			//heavy require more food to move, lighter is faster
	double angle = 0;
	int cohesionrange = 10;
	int seperationrange = 5;
	//float age = 0;		//time
	int FOV = 90; 
	float size = 0.2f;			//older is larger
	float acceleration; float max_speed = 20.f;
	float agility; float max_rotation_speed;
	//float strength; float max_energy;
	//float hunger = 100;	//hunger decreases faster with movement

	AIHERDS(AIHERDS_TYPE typeValue = GO_BALL);
	~AIHERDS();
};

#endif