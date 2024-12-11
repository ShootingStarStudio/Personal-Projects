#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CUBE,
		GO_WALL, GO_PILLAR,
		GO_CORE,
		GO_PARTICLE,
		GO_PIVOT, GO_FLIPPER,
		GO_POWERUP,
		GO_BASE, 
		GO_LINES,					//no collisions
		GO_TOTAL, //must be last
	};
	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	float angle;
	bool active;
	float mass;

	bool action;

	//Rotation variables
	Vector3 direction;
	float momentOfInertia;
	float angularVelocity;

	Vector3 color;

	//flipper variables
	Vector3 normal;
	GameObject* ptr_flipper;
	GameObject* ptr_pivot;
	bool clockwise;
	float flipperangle;
	float angularDisplacement = 0.f;
	
	//age based deletion
	float age;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif