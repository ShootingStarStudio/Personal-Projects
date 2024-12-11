#ifndef SCENE_ASTEROID_H
#define SCENE_ASTEROID_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

class SceneAsteroid : public SceneBase
{
	static const int BULLET_SPEED = 50;
	static const int MISSILE_SPEED = 20;
	static const int MISSILE_POWER = 1;

	static const float ROTATION_SPEED;
	static const float MAX_ROTATION_SPEED;
	static const float GRAVITY_CONSTANT;

public:
	SceneAsteroid();
	~SceneAsteroid();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();

	void Wrap(float& val, float bound) {
		if (val < 0)val = bound;
		if (val > bound)val = 0;
	}
	float CalculateAdditionalForce(GameObject* go1, GameObject* go2);

protected:

	//Physics
	std::vector<GameObject *> m_goList;
	float m_speed = 1.0f;
	float m_worldWidth;
	float m_worldHeight;
	bool gameplay = true;
	int m_lives = 25; int m_score = 0;
	int waveint = 1;
	double elapsedwave = 10; double wavebetween = 15;		//15s between waves

	GameObject *m_ship;

	double fire_rate = 0.2; double elapsedFire = 0; double prevFire = 0;		//wait time for fire rate
	int ammo = 20; int max_ammo = 20; double reload_time = 2;

	int special_ammo = 10;		bool special_loaded = true;
	int special_choice = 1;		float special_cycle = true;
	bool barrier = false;

	float afterburn_fuel = 20; bool afterburn = false;
	float MAX_SPEED = 20.f;		float thrust = 20.f;

	Vector3 m_force, m_torque;
};

#endif