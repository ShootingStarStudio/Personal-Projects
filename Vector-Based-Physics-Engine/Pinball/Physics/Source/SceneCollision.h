#ifndef SCENE_COLLISION_H
#define SCENE_COLLISION_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

class SceneCollision : public SceneBase
{
public:
	SceneCollision();
	~SceneCollision();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();
	void ReturnGO(GameObject *go);
	bool CheckCollision(GameObject* go1, GameObject* go2);
	void CollisionResponse(GameObject* go1, GameObject* go2);

	void spawnPong(Vector3& color);

	void makeWall(Vector3& startPos, Vector3& endPos, Vector3& color);
	void makePillar(Vector3& startPos, Vector3& endPos, Vector3& color);
	void makeFlipper(Vector3& PivotPos, float FlipperLength, Vector3& flipperdirection, float flipperRad, bool clockwise);

	void ghostPreview(GameObject::GAMEOBJECT_TYPE m_type, Vector3& startPos, Vector3& endPos);

	void spawnParticles(Vector3& positionP, Vector3& directionP, Vector3& colorP);
	void spawnPowerup(Vector3& positionP, Vector3& directionP);
protected:

	//Physics
	std::vector<GameObject*> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	Vector3 mousePos;
	GameObject *m_ghost;
	
	std::vector<GameObject*> m_NodeList;
	std::vector<GameObject*> m_MapList;		//no collisions
	GameObject *m_player;
	GameObject *m_playerFlail;
	int playerhealth = 5;
	int enemyhealth = 5;
	float material = 0;

	GameObject* AllyBase;
	GameObject* EnemyBase;
	GameObject* Line_Center;

	Vector3 StartPos, EndPos;
	
	GameObject *m_enemy;		//paddle
		GameObject *m_target;			//use to find target of enemy, then move y to target

	int m_objectCount;

	float m1, m2;
	Vector3 u1, u2, v1, v2;

	Vector3 color;

	float ballspawn_cd = 5.f;		//cooldown and delta time for sapwning balls
	float ballspawn_dt = 0;

	float player_material;
	float enemy_material;

	Vector3 white = Vector3(1.f, 1.f, 1.f);
	Vector3 allyColor = Vector3(.1f, .1f, .8f);
	Vector3 enemyColor = Vector3(.8f, .1f, .1f);
	Vector3 neutralColor = Vector3(.7f, .7f, .7f);

	float minwallsize = 5.f;
	float maxwallsize = 20.f;
	float minpillarsize = 1.f;
	float maxpillarsize = 3.f;


	//0 = game generated
	//1 = neutral
	//2 = player
	//3 = enemy

	bool flaildetach = false;
	Vector3 poweruplist[3] = { Vector3(0.2, 1.0, 0.2), Vector3(0.6, 0.4, 0.0), Vector3(0.5, 0.2, 0.5) };
};

#endif