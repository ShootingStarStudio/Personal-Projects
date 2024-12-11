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
		GO_SHIP,
		GO_TURRET,
		GO_ASTEROID,
		GO_ORE, GO_RESOURCE,
		GO_BULLET,		//shotgun reuse bullet math
		GO_RAILGUN, GO_BARRIER,
		//railgun passes through everything, shield bounces enemies away
		GO_PARTICLES,		//thruster
		GO_PARTICLESA,		//afterburn
		GO_ENEMYBULLET,
		GO_SCOUT, GO_BOMBER,
		GO_QUEEN,GO_HIVE,
		GO_BLACKHOLE, GO_WHITEHOLE,
		GO_TOTAL, //must be last
	};
	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	bool active = false;
	float mass = 1;
	float angle = 0;
	int age;		//frames
	int size;		//scale for asteroids
	int health = 50;		//only for queen and hive
	bool bounced = false;
	int b_frames = 10;
	
	//rotation variable
	Vector3 direction = Vector3(0, 1, 0);
	float momentOfInertia;
	float angularVelocity;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif