#include "SceneCollision.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneCollision::SceneCollision() {

}

SceneCollision::~SceneCollision() {

}

void SceneCollision::Init() {
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 1.f;
	m_objectCount = 0;

	bLightEnabled = true;

	Math::InitRNG();
	for (int i = 0; i <= 30; i++) {
		m_goList.push_back(new GameObject(GameObject::GO_BALL));
	}

	//wrecking ball at 10 units away from player
	//nodes in between
	m_player = new GameObject(GameObject::GO_CORE); {
		m_player->active = true;
		m_player->pos.Set(25, 50, 0);
		m_player->color.Set(.2f, .2f, 1.f);
		m_player->scale.Set(1, 1, 1);
		m_player->mass = m_player->scale.x * m_player->scale.y * m_player->scale.z;
	}
	m_playerFlail = new GameObject(GameObject::GO_BALL); {
		m_playerFlail->active = true;
		m_playerFlail->pos.Set(15, 50, 0);
		m_playerFlail->color = allyColor;
		m_playerFlail->scale.Set(3, 3, 3);
		m_playerFlail->mass = m_playerFlail->scale.x * m_playerFlail->scale.y * m_playerFlail->scale.z;
	}

	//rope here
	int numNodes = 5;
	float chainLengthX = m_playerFlail->pos.x - m_player->pos.x;
	float chainLengthY = m_playerFlail->pos.y - m_player->pos.y;
	for (int i = 0; i < numNodes; i++) {
		GameObject* tempNode = new GameObject(GameObject::GO_BALL);
		tempNode->active = true;
		tempNode->pos.Set(m_player->pos.x + ((chainLengthX / numNodes) * (i + 1)), m_player->pos.y + ((chainLengthY / numNodes) * (i + 1)), 0);;
		tempNode->color.Set(1.f, 1.f, 1.f);
		tempNode->scale.Set(.1f, .1f, .1f);
		tempNode->mass = tempNode->scale.x * tempNode->scale.y * tempNode->scale.z;

		m_NodeList.push_back(tempNode);
	}

	m_enemy = new GameObject(GameObject::GO_WALL); {		//enemy paddle 
		m_enemy->active = true;
		m_enemy->pos.Set(m_worldWidth - 7, 50, 0);
		m_enemy->color = enemyColor;
		m_enemy->scale.Set(1, 10, 1);
		m_enemy->normal.Set(1, 0, 0);
	}
	Vector3 White(1.f, 1.f, 1.f);
	Vector3 TopLeft(0, m_worldHeight, 0);
	Vector3 TopRight(m_worldWidth, m_worldHeight, 0);
	Vector3 BottomLeft(0, 20, 0);
	Vector3 BottomRight(m_worldWidth, 20, 0);
	{
		makeWall(TopLeft, TopRight, White);		//top
		makeWall(BottomLeft, BottomRight, White);		//down
		makeWall(TopLeft, BottomLeft, White);		//left
		makeWall(TopRight, BottomRight, White);		//right
	}

	
	AllyBase = new GameObject(GameObject::GO_BASE); {	//dark blue
		AllyBase->active = false;
		AllyBase->vel.SetZero();
		AllyBase->pos.Set(2, m_worldHeight / 2 + 10, 0);
		AllyBase->normal.Set(1, 0, 0);
		AllyBase->color.Set(0.1f, 0.1f, 0.3f);
		AllyBase->scale.Set(5, 30, 1);
		AllyBase->mass = 1;
	}
	EnemyBase = new GameObject(GameObject::GO_BASE); {	//dark blue
		EnemyBase->active = false;
		EnemyBase->vel.SetZero();
		EnemyBase->pos.Set(m_worldWidth - 2, m_worldHeight / 2 + 10, 0);
		EnemyBase->normal.Set(-1, 0, 0);
		EnemyBase->color.Set(0.3f, 0.1f, 0.1f);
		EnemyBase->scale.Set(5, 30, 1);
		EnemyBase->mass = 1;
	}

	{
		makeFlipper(Vector3(30, m_worldHeight, 0), 15, Vector3(-1, -1, 0), 1.5f, false);
		makeFlipper(Vector3(30, 20, 0), 15, Vector3(-1, 1, 0), 1.5f, true);

		makeFlipper(Vector3(70, m_worldHeight, 0), 10, Vector3(-1, -1, 0), 1.5f, false);
		makeFlipper(Vector3(70, 20, 0), 10, Vector3(-1, 1, 0), 1.5f, true);

		makeFlipper(Vector3(m_worldWidth - 30, m_worldHeight, 0), 10, Vector3(1, -1, 0), 1.5f, true);
		makeFlipper(Vector3(m_worldWidth - 30, 20, 0), 15, Vector3(1, 1, 0), 1.5f, false);

		makeFlipper(Vector3(m_worldWidth - 70, m_worldHeight, 0), 10, Vector3(1, -1, 0), 1.5f, true);
		makeFlipper(Vector3(m_worldWidth - 70, 20, 0), 10, Vector3(1, 1, 0), 1.5f, false);
	}

	m_ghost = new GameObject(GameObject::GO_WALL); {	//dark blue
		m_ghost->active = false;
		m_ghost->vel.SetZero();
		m_ghost->pos.Set(0, 0, 0);
		m_ghost->color = allyColor;
		m_ghost->scale.Set(1, 1, 1);
		m_ghost->mass = 1;
	}
}

GameObject* SceneCollision::FetchGO() {
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); it++) {
		GameObject* go = (GameObject*)*it;
		if (go->active) {
			continue;
		}

		go->active = true;
		m_objectCount++;
		return go;
	}
	int prevSize = m_goList.size();
	for (int i = 0; i < 10; i++) {
		m_goList.push_back(new GameObject(GameObject::GO_BALL));
	}
	m_goList.at(prevSize)->active = true;
	m_objectCount++;
	return m_goList[prevSize];
}
void SceneCollision::ReturnGO(GameObject* go) {
	go->active = false;
	m_objectCount--;
}

void SceneCollision::Update(double dt)
{
	if (playerhealth > 0 && enemyhealth > 0) {
		SceneBase::Update(dt);

		//Calculating aspect ratio
		m_worldHeight = 100.f;
		m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

		if (Application::IsKeyPressed('9')) { m_speed = Math::Max(0.f, m_speed - 0.1f); }
		if (Application::IsKeyPressed('0')) { m_speed += 0.1f; }
		static bool SpaceDown = false;
		if (Application::IsKeyPressed(VK_SPACE) && !SpaceDown) {
			SpaceDown = true;
			if (!flaildetach) {
				flaildetach = true;
			}
			else if (flaildetach) {
				flaildetach = false;
			}
		}
		else if (!Application::IsKeyPressed(VK_SPACE) && SpaceDown) {
			SpaceDown = false;
		}

		//Mouse Section
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		mousePos.Set((x / w) * m_worldWidth, (h - y) / h * m_worldHeight, 0);

		static bool bLButtonState = false;
		if (!bLButtonState && Application::IsMousePressed(0))
		{
			bLButtonState = true;
			//std::cout << "LBUTTON DOWN" << std::endl;
			StartPos = mousePos;
		}
		else if (bLButtonState && !Application::IsMousePressed(0))
		{
			bLButtonState = false;
			//std::cout << "LBUTTON UP" << std::endl;
			m_ghost->active = false;
			EndPos = mousePos;
			if (StartPos != EndPos) {
				if ((EndPos - StartPos).Length() < minpillarsize) {
					EndPos = StartPos + (EndPos - StartPos).Normalize() * minpillarsize;
				}
				else if ((StartPos - EndPos).Length() > maxpillarsize) {
					EndPos = StartPos + (EndPos - StartPos).Normalize() * maxpillarsize;
				}
				makePillar(StartPos, EndPos, allyColor);
			}
		}
		if (bLButtonState) {
			//std::cout << "LBUTTON held" << std::endl;
			EndPos = mousePos;
			if (StartPos != EndPos) {
				if ((EndPos - StartPos).Length() < minpillarsize) {
					EndPos = StartPos + (EndPos - StartPos).Normalize() * minpillarsize;
				}
				else if ((EndPos - StartPos).Length() > maxpillarsize) {
					EndPos = StartPos + (EndPos - StartPos).Normalize() * maxpillarsize;
				}
				ghostPreview(GameObject::GO_PILLAR, StartPos, EndPos);
			}
		}
		static bool bRButtonState = false;
		if (!bRButtonState && Application::IsMousePressed(1))
		{
			bRButtonState = true;
			//std::cout << "RBUTTON DOWN" << std::endl;
			StartPos = mousePos;
		}
		else if (bRButtonState && !Application::IsMousePressed(1))
		{
			bRButtonState = false;
			//std::cout << "RBUTTON UP" << std::endl;
			m_ghost->active = false;
			EndPos = mousePos;
			if (StartPos != EndPos) {
				if ((StartPos - EndPos).Length() < minwallsize) {
					EndPos = StartPos + (EndPos  - StartPos).Normalize() * minwallsize;
				}
				else if ((StartPos - EndPos).Length() > maxwallsize) {
					EndPos = StartPos + (EndPos - StartPos).Normalize() * maxwallsize;
				}
				makeWall(StartPos, EndPos, allyColor);
			}
		}
		if (bRButtonState) {
			//std::cout << "RBUTTON held" << std::endl;

			EndPos = mousePos;
			if (StartPos != EndPos) {
				if ((EndPos - StartPos).Length() < minwallsize) {
					EndPos = StartPos + (EndPos - StartPos).Normalize() * minwallsize;
				}
				else if ((EndPos - StartPos).Length() > maxwallsize) {
					EndPos = StartPos + (EndPos - StartPos).Normalize() * maxwallsize;
				}
				ghostPreview(GameObject::GO_WALL, StartPos, EndPos);
			}
		}

		if (Application::IsKeyPressed('W')) {
			m_player->vel += Vector3(0, 1.f, 0);
		}
		if (Application::IsKeyPressed('S')) {
			m_player->vel -= Vector3(0, 1.f, 0);
		}
		if (Application::IsKeyPressed('A')) {
			m_player->vel -= Vector3(1.f, 0, 0);
		}
		if (Application::IsKeyPressed('D')) {
			m_player->vel += Vector3(1.f, 0, 0);
		}
		m_player->vel *= 0.99f;
		//Physics Simulation Section
		unsigned size = m_goList.size();

		//spring joint physics
		{
			float strength = 1.f, damp = .1f, distance = 2.0f;
			m_NodeList[0]->vel = m_player->vel;
			m_NodeList[0]->pos = m_player->pos;
			if (!flaildetach) {
				m_NodeList.back()->vel = m_playerFlail->vel;
				m_NodeList.back()->pos = m_playerFlail->pos;
			}

			for (int i = 1; i < m_NodeList.size(); i++) {
				if (m_NodeList[i]->pos != m_NodeList[i - 1]->pos) {
					//apply force constraint and damper		F = -kx - bv	hooke's law with spring
					Vector3 Force = -strength * ((m_NodeList[i]->pos - m_NodeList[i - 1]->pos).Length() - distance) *
						(m_NodeList[i]->pos - m_NodeList[i - 1]->pos).Normalize() -
						(m_NodeList[i]->vel - m_NodeList[i - 1]->vel) * damp;
					m_NodeList[i]->vel += Force;
				}
				//update pos
				m_NodeList[i]->pos += m_NodeList[i]->vel * dt * m_speed;
				//rope border lock
				if (m_NodeList[i]->pos.x < 0) {
					m_NodeList[i]->pos.x = 0;
				}
				if (m_NodeList[i]->pos.x > m_worldWidth) {
					m_NodeList[i]->pos.x = m_worldWidth;
				}
				if (m_NodeList[i]->pos.y < 0) {
					m_NodeList[i]->pos.y = 0;
				}
				if (m_NodeList[i]->pos.y > m_worldHeight) {
					m_NodeList[i]->pos.y = m_worldHeight;
				}
			}
		}

		//player physics
		{
			m_player->vel = m_NodeList[0]->vel;
			m_player->pos += m_player->vel * dt * m_speed;

			if ((m_player->pos.x - m_player->scale.x < 0 && m_player->vel.x < 0) ||
				(m_player->pos.x + m_player->scale.x > m_worldWidth && m_player->vel.x > 0))
				m_player->vel.x = -m_player->vel.x;
			if ((m_player->pos.y - m_player->scale.y < 0 && m_player->vel.y < 0) ||
				(m_player->pos.y + m_player->scale.y > m_worldHeight && m_player->vel.y > 0))
				m_player->vel.y = -m_player->vel.y;

			for (unsigned i = 0; i < size; i++) {
				GameObject* go = m_goList[i];
				if (go->active) {
					if (CheckCollision(m_player, go)) {
						CollisionResponse(m_player, go);
						if (go->type == GameObject::GO_BALL && go->color == m_enemy->color) {
							playerhealth--;
						}
					}
				}
			}
		}

		//flail
		{
			if (!flaildetach) {
				m_playerFlail->vel = m_NodeList.back()->vel;
			}
			m_playerFlail->pos += m_playerFlail->vel * dt * m_speed;
			if ((m_playerFlail->pos.x - m_playerFlail->scale.x < 0 && m_playerFlail->vel.x < 0) ||
				(m_playerFlail->pos.x + m_playerFlail->scale.x > m_worldWidth && m_playerFlail->vel.x > 0))
				m_playerFlail->vel.x = -m_playerFlail->vel.x;
			if ((m_playerFlail->pos.y - m_playerFlail->scale.y < 0 && m_playerFlail->vel.y < 0) ||
				(m_playerFlail->pos.y + m_playerFlail->scale.y > m_worldHeight && m_playerFlail->vel.y > 0))
				m_playerFlail->vel.y = -m_playerFlail->vel.y;

			for (unsigned i = 0; i < size; i++) {
				GameObject* go = m_goList[i];
				if (go->active) {			//flail physics
					if (CheckCollision(m_playerFlail, go)) {
						CollisionResponse(m_playerFlail, go);
						flaildetach = false;
						if (go->type == GameObject::GO_BALL) {		//boundaries
							go->color = m_playerFlail->color;
						}
					}
				}
			}
			//particles
			if (m_playerFlail->vel.Length() > 50) {
				spawnParticles(m_playerFlail->pos, m_playerFlail->vel.Normalized(), allyColor);
			}
		}
		//enemy AI
		{
			m_target = m_player;
			for (unsigned i = 0; i < size; i++) {
				GameObject* go = m_goList[i];
				if (go->active && go->type == GameObject::GO_BALL) {
					if (go->pos.x > m_target->pos.x && go->vel.x > 0) {
						m_target = go;
					}
				}
			}
			if (m_enemy->pos.y > (m_target->pos.y - m_enemy->scale.y/4) && m_enemy->pos.y < (m_target->pos.y + m_enemy->scale.y / 4)) {
				m_enemy->vel.y *= 0.75;
			}
			else if (m_enemy->vel.y < 5 || m_enemy->vel.y > -5) {
				if (m_enemy->pos.y > m_target->pos.y) {
					m_enemy->vel.y -= 1;
				}
				if (m_enemy->pos.y < m_target->pos.y) {
					m_enemy->vel.y += 1;
				}
			}
			else {
				m_enemy->vel.y *= 0.9;
			}
			m_enemy->pos += m_enemy->vel * dt * m_speed;
			for (unsigned i = 0; i < size; i++) {
				GameObject* go = m_goList[i];
				if (go->active) {			//enemy physics
					if (CheckCollision(go, m_enemy)) {
						CollisionResponse(go, m_enemy);
						if (go->color != Vector3(1.f, 1.f, 1.f)) {		//boundaries
							go->color = m_enemy->color;
						}
					}
				}
			}
		}
		
		//base collision
		{
			for (unsigned i = 0; i < size; i++) {
				GameObject* go = m_goList[i];
				if (go->active) {
					if (CheckCollision(go, AllyBase)) {
						if (go->type == GameObject::GO_BALL && go->color == enemyColor) {
							playerhealth--;
							ReturnGO(go);
						}
					}
					if (CheckCollision(go, EnemyBase)) {
						if (go->type == GameObject::GO_BALL && go->color == allyColor) {
							enemyhealth--;
							ReturnGO(go);
						}
					}
				}
			}
		}
		
		//event manager
		{	
			//spawn powerup
			if (rand() % 200 > 198) {
				spawnPowerup(Vector3(Math::RandFloatMinMax(50, m_worldWidth-50), Math::RandFloatMinMax(20, m_worldHeight), 0), Vector3(Math::RandFloatMinMax(-1, 1), Math::RandFloatMinMax(-1, 1), 0));
			}
			//add new items here
		}

		//entity physics
		size = m_goList.size();
		int ballcount = 0;
		for (unsigned i = 0; i < size; i++) {
			GameObject* go = m_goList[i];
			if (go->active) {
				go->pos += go->vel * dt * m_speed;
				{		//boundary collision
					if ((go->pos.x - go->scale.x < 0 && go->vel.x < 0) ||
						(go->pos.x + go->scale.x > m_worldWidth && go->vel.x > 0))
						go->vel.x = -go->vel.x;

					if ((go->pos.y - go->scale.y < 0 && go->vel.y < 0) ||
						(go->pos.y + go->scale.y > m_worldHeight && go->vel.y > 0))
						go->vel.y = -go->vel.y;

					if (go->pos.x < -go->scale.x || go->pos.x > m_worldWidth + go->scale.x ||
						go->pos.y < -go->scale.y + 20 || go->pos.y > m_worldHeight + go->scale.y) {
						ReturnGO(go);
						continue;
					}
				}
				if (go->type == GameObject::GO_POWERUP) {
					go->age--;
					if (go->age <= 0) {
						ReturnGO(go);
					}
				}
				if (go->type == GameObject::GO_PARTICLE) {
					go->age--;
					if (go->age <= 0) {
						ReturnGO(go);
					}
					else {
						if (go->scale.x > 0.01) {
							go->scale *= 0.95;
						}
					}
					continue;
				}
				if (go->type == GameObject::GO_BALL) {
					ballcount++;
				}
				if (go->type == GameObject::GO_PIVOT) {
					if (Application::IsKeyPressed('E')) {
						//flip
						if (go->angularDisplacement < go->flipperangle) {
							go->angularVelocity = 0.2f;
						}
						else {				//if angle reached, stop
							go->angularVelocity = 0.f;
						}
					}
					else if (!Application::IsKeyPressed('E')) {		//go back default angle
						if (go->angularDisplacement > 0) {					//if angle is greater than default direction, go back
							go->angularVelocity = -0.2f;
						}
						else {
							go->angularVelocity = 0.f;
						}
					}
					go->angularDisplacement += go->angularVelocity;
					if (go->clockwise) {
						go->direction.Set(go->direction.x * cos(-go->angularVelocity) - go->direction.y * sin(-go->angularVelocity),
							go->direction.x * sin(-go->angularVelocity) + go->direction.y * cos(-go->angularVelocity), 0);
					}
					else if (!go->clockwise) {
						go->direction.Set(go->direction.x * cos(go->angularVelocity) - go->direction.y * sin(go->angularVelocity),
							go->direction.x * sin(go->angularVelocity) + go->direction.y * cos(go->angularVelocity), 0);
					}
					
					go->ptr_flipper->pos = go->pos + (go->direction.Normalize() * (go->ptr_flipper->pos - go->pos).Length());
					go->ptr_flipper->normal = go->direction.Normalize().Cross(Vector3(0, 0, 1));
				}

				GameObject* go2 = nullptr;
				for (int j = i + 1; j < size; j++) {
					go2 = m_goList[j];
					if (go2->active) {
						GameObject* actor(go);
						GameObject* actee(go2);
						if (go2->type == GameObject::GO_BALL) {
							actor = go2; actee = go;
						}
						if (CheckCollision(actor, actee)) {
							if (go->color == neutralColor ||		//neutral/boundary
								go->color == white ||
								go2->color == neutralColor ||
								go2->color == white) {
								CollisionResponse(actor, actee);
							}
							//ally and enemy collision delete
							else if ((go->color == allyColor && go2->color == enemyColor) ||
									 (go->color == enemyColor && go2->color == allyColor)) {
								ReturnGO(go);
								ReturnGO(go2);
							}
							else {
								//particle collision
								if (go->type == GameObject::GO_PARTICLE && go2->type == GameObject::GO_BALL && go2->color == allyColor) {
									if (go->color == poweruplist[0]) {
										playerhealth += 3;
									}
									else if (go->color == poweruplist[1]) {
										player_material += 50;
									}
									else if (go->color == poweruplist[2]) {
										spawnPong(allyColor);
									}
									ReturnGO(go);
								}
								else if (go->type == GameObject::GO_PARTICLE && go2->type == GameObject::GO_BALL && go2->color == enemyColor) {
									if (go->color == poweruplist[0]) {
										enemyhealth += 3;
									}
									else if (go->color == poweruplist[1]) {
										enemy_material += 50;
									}
									else if (go->color == poweruplist[2]) {
										spawnPong(enemyColor);
									}
									ReturnGO(go);
								}
								else if (go2->type == GameObject::GO_PARTICLE && go->type == GameObject::GO_BALL && go->color == allyColor) {
									if (go2->color == poweruplist[0]) {
										playerhealth += 3;
									}
									else if (go2->color == poweruplist[1]) {
										player_material += 50;
									}
									else if (go2->color == poweruplist[2]) {
										spawnPong(allyColor);
									}
									ReturnGO(go2);
								}
								else if (go2->type == GameObject::GO_PARTICLE && go->type == GameObject::GO_BALL && go->color == enemyColor) {
									if (go2->color == poweruplist[0]) {
										enemyhealth += 3;
									}
									else if (go2->color == poweruplist[1]) {
										enemy_material += 50;
									}
									else if (go2->color == poweruplist[2]) {
										spawnPong(enemyColor);
									}
									ReturnGO(go2);
								}
							}
						}
					}
				}
			}
		}
		ballspawn_dt += dt;
		if (ballcount < 5 && ballspawn_dt >= ballspawn_cd) {
			spawnPong(neutralColor);
			ballspawn_dt = 0;
		}
	}
	else {
		//reset game field
		if (Application::IsKeyPressed(VK_SPACE)) {
			unsigned size = m_goList.size();
			for (int i = 1; i < size; i++) {
				GameObject* go = m_goList[i];
				if (go->active && go->color != white) {
					ReturnGO(go);

				}

			}
			playerhealth = 10;
			enemyhealth = 10;
		}
	}
}


void SceneCollision::RenderGO(GameObject* go)
{
	modelStack.PushMatrix();
	modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
	switch (go->type) {
	case GameObject::GO_CORE:
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		meshList[GEO_BALL]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
		RenderMesh(meshList[GEO_BALL], true);
		break;
	case GameObject::GO_BALL:
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		meshList[GEO_BALL]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
		RenderMesh(meshList[GEO_BALL], true);
		break;
	case GameObject::GO_CUBE:
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		meshList[GEO_CUBE]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
		RenderMesh(meshList[GEO_CUBE], true);
		break; 
	case GameObject::GO_WALL:
		modelStack.Rotate(Math::RadianToDegree(atan2f(go->normal.y, go->normal.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		meshList[GEO_CUBE]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
		RenderMesh(meshList[GEO_CUBE], true);
		break;
	case GameObject::GO_PILLAR:
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		meshList[GEO_BALL]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
		RenderMesh(meshList[GEO_BALL], true);
		break;
	case GameObject::GO_PARTICLE:
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		meshList[GEO_BALL]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
		RenderMesh(meshList[GEO_BALL], true);
		break;
	case GameObject::GO_PIVOT:
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		meshList[GEO_BALL]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
		RenderMesh(meshList[GEO_BALL], true);
		break;
	case GameObject::GO_FLIPPER:
		modelStack.Rotate(Math::RadianToDegree(atan2f(go->normal.y, go->normal.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		meshList[GEO_CUBE]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
		RenderMesh(meshList[GEO_CUBE], true);
		break;
	case GameObject::GO_POWERUP:
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		meshList[GEO_BALL]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
		RenderMesh(meshList[GEO_BALL], true);
		break;
	case GameObject::GO_BASE:
		modelStack.Rotate(Math::RadianToDegree(atan2f(go->normal.y, go->normal.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		meshList[GEO_CUBE]->material.kAmbient.Set(go->color.x, go->color.y, go->color.z);
		RenderMesh(meshList[GEO_CUBE], true);
		break;
	}
	modelStack.PopMatrix();
}

void SceneCollision::Render() {
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Projection matrix : Orthographic Projection
		Mtx44 projection;
		projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
		projectionStack.LoadMatrix(projection);

		// Camera matrix
		viewStack.LoadIdentity();
		viewStack.LookAt(
			camera.position.x, camera.position.y, camera.position.z,
			camera.target.x, camera.target.y, camera.target.z,
			camera.up.x, camera.up.y, camera.up.z
		);
		// Model matrix : an identity matrix (model will be at the origin)
		modelStack.LoadIdentity();
		RenderGO(AllyBase);
		RenderGO(EnemyBase);

		RenderMesh(meshList[GEO_AXES], false);
		RenderGO(m_player);
		RenderGO(m_playerFlail);

		for (int i = 0; i < m_NodeList.size(); i++)		//render the rope
		{
			//draw lines 
			//render rope via lines between nodes
			//draw lines between player and first node, last node and flail
			RenderGO(m_NodeList[i]);
		}

		RenderGO(m_enemy);
		
		if (m_ghost->active) {
			RenderGO(m_ghost);
		}

		for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it) {
			GameObject* go = (GameObject*)*it;
			if (go->active) {
				RenderGO(go);
			}
		}
	}
	//On screen text
	std::ostringstream ss;
	ss << mousePos;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, 6);
	ss.str("");
	ss << "Active:" << m_objectCount << "/" << m_goList.size();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, 4);

	ss.str("");
	ss.precision(3); ss << "Speed:" << m_speed;
	ss.precision(5); ss << "FPS:" << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, 2);

	RenderTextOnScreen(meshList[GEO_TEXT], "Wrecker's Mayhem", Color(0, 1, 0), 2, 0, 0);

	ss.str("");
	ss << "Health:" << playerhealth;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, 55);

	ss.str("");
	ss << "Health:" << enemyhealth;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 60, 55);
	if (playerhealth <= 0) {
		ss.str("");
		ss << "You lost. Play Again? (Space)";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 10, 50);
	}
	else if (enemyhealth <= 0) {
		ss.str("");
		ss << "You won. Play Again? (Space)";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 10, 50);
	}
}

void SceneCollision::Exit() {
	SceneBase::Exit();
	while (m_goList.size() > 0) {
		GameObject* go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if (m_player) {
		delete m_player;
		m_player = NULL;
	}
	if (m_playerFlail) {
		delete m_playerFlail;
		m_playerFlail = NULL;
	}
	while (m_NodeList.size() > 0) {
		GameObject* go = m_NodeList.back();
		delete go;
		m_NodeList.pop_back();
	}
	if (m_enemy) {
		delete m_enemy;
		m_enemy = NULL;
	}
	if (AllyBase) {
		delete AllyBase;
		AllyBase = NULL;
	}
	if (EnemyBase) {
		delete EnemyBase;
		EnemyBase = NULL;
	}
}

bool SceneCollision::CheckCollision(GameObject* go1, GameObject* go2) {
	if (go1->type != GameObject::GO_BALL && go1->type != GameObject::GO_CORE) return false;		//prevent immobile collision
	
	switch (go2->type) {
		case GameObject::GO_CORE:
		{
			Vector3 relativeVel = go1->vel - go2->vel;
			Vector3 disDiff = go2->pos - go1->pos;

			if (relativeVel.Dot(disDiff) <= 0) {
				return false;
			}
			return disDiff.LengthSquared() <= (go1->scale.x + go2->scale.x) * (go1->scale.x + go2->scale.x);
		}
		case GameObject::GO_BALL:
		{
			Vector3 relativeVel = go1->vel - go2->vel;
			Vector3 disDiff = go2->pos - go1->pos;

			if (relativeVel.Dot(disDiff) <= 0) {
				return false;
			}
			return disDiff.LengthSquared() <= (go1->scale.x + go2->scale.x) * (go1->scale.x + go2->scale.x);
		}
		case GameObject::GO_PILLAR:
		{
			Vector3 relativeVel = go1->vel - go2->vel;
			Vector3 disDiff = go2->pos - go1->pos;

			if (relativeVel.Dot(disDiff) <= 0) {
				return false;
			}
			return disDiff.LengthSquared() <= (go1->scale.x + go2->scale.x) * (go1->scale.x + go2->scale.x);
		}
		case GameObject::GO_WALL:
		{
			Vector3 diff = go1->pos - go2->pos;
			Vector3 AxisX = go2->normal;
			Vector3 AxisY = Vector3(-AxisX.y, AxisX.x, 0);

			float projectedDistX = diff.Dot(AxisX);
			if (projectedDistX > 0) AxisX = -AxisX;

			float projectedDistY = diff.Dot(AxisY);
			if (projectedDistY > 0) AxisY = -AxisY;

			return ( go1->vel.Dot(AxisX) >= 0 &&
				(go2->scale.x * .5f + go1->scale.x) > -diff.Dot(AxisX) &&
				go2->scale.y * .5f > fabs(diff.Dot(AxisY)) )
				||
				( go1->vel.Dot(AxisY) >= 0 &&
				(go2->scale.y * .5f + go1->scale.y) > -diff.Dot(AxisY) &&
				go2->scale.x * .5f > fabs(diff.Dot(AxisX)) );
		}
		case GameObject::GO_PIVOT:
		{
			Vector3 relativeVel = go1->vel - go2->vel;
			Vector3 disDiff = go2->pos - go1->pos;

			if (relativeVel.Dot(disDiff) <= 0) {
				return false;
			}
			return disDiff.LengthSquared() <= (go1->scale.x + go2->scale.x) * (go1->scale.x + go2->scale.x);
		}
		case GameObject::GO_FLIPPER:
		{
			Vector3 diff = go1->pos - go2->pos;
			Vector3 AxisX = go2->normal;
			Vector3 AxisY = Vector3(-AxisX.y, AxisX.x, 0);

			float projectedDistX = diff.Dot(AxisX);
			if (projectedDistX > 0) AxisX = -AxisX;

			return (go1->vel.Dot(AxisX) >= 0 &&
				(go2->scale.x * .5f + go1->scale.x) > -diff.Dot(AxisX) &&
				go2->scale.y * .5f > fabs(diff.Dot(AxisY)));
		}
		case GameObject::GO_POWERUP:
		{
			Vector3 relativeVel = go1->vel - go2->vel;
			Vector3 disDiff = go2->pos - go1->pos;

			if (relativeVel.Dot(disDiff) <= 0) {
				return false;
			}
			return disDiff.LengthSquared() <= (go1->scale.x + go2->scale.x) * (go1->scale.x + go2->scale.x);
		}
		case GameObject::GO_BASE:
		{
			Vector3 diff = go1->pos - go2->pos;
			Vector3 AxisX = go2->normal;
			Vector3 AxisY = Vector3(-AxisX.y, AxisX.x, 0);

			float projectedDistX = diff.Dot(AxisX);
			if (projectedDistX > 0) AxisX = -AxisX;

			float projectedDistY = diff.Dot(AxisY);
			if (projectedDistY > 0) AxisY = -AxisY;

			return (go1->vel.Dot(AxisX) >= 0 &&
				(go2->scale.x * .5f + go1->scale.x) > -diff.Dot(AxisX) &&
				go2->scale.y * .5f > fabs(diff.Dot(AxisY)))
				||
				(go1->vel.Dot(AxisY) >= 0 &&
					(go2->scale.y * .5f + go1->scale.y) > -diff.Dot(AxisY) &&
					go2->scale.x * .5f > fabs(diff.Dot(AxisX)));
		}
	}
	return false;
}

void SceneCollision::CollisionResponse(GameObject* go1, GameObject* go2) {
	u1 = go1->vel;
	m1 = go1->mass;
	u2 = go2->vel;
	m2 = go2->mass;
	switch (go2->type) {
		case GameObject::GO_BALL:
		{
		Vector3 n = go1->pos - go2->pos;
		Vector3 vec = ((u1 - u2).Dot(n) / n.LengthSquared()) * n;
		go1->vel = u1 - (2 * m2 / (m1 + m2)) * vec;
		go2->vel = u2 - (2 * m1 / (m1 + m2)) * -vec;
		break;
	}
		case GameObject::GO_WALL:
		{
		Vector3 diff = go1->pos - go2->pos;
		Vector3 AxisX = go2->normal;
		Vector3 AxisY = Vector3(-AxisX.y, AxisX.x, 0);

		float projectedDistX = diff.Dot(AxisX);
		if (projectedDistX > 0) AxisX = -AxisX;
		float projectedDistY = diff.Dot(AxisY);
		if (projectedDistY > 0) AxisY = -AxisY;

		if (go1->vel.Dot(AxisX) >= 0 &&
			(go2->scale.x * .5f + go1->scale.x) > -diff.Dot(AxisX) &&
			go2->scale.y * .5f > fabs(diff.Dot(AxisY))) {
			go1->vel = u1 - (2.0 * u1.Dot(AxisX)) * AxisX * 0.9;
			break;
		}

		if (go1->vel.Dot(AxisY) >= 0 &&			//side wall collision
			(go2->scale.y * .5f + go1->scale.y) > -diff.Dot(AxisY) &&
			go2->scale.x * .5f > fabs(diff.Dot(AxisX))) {
			go1->vel = u1 - (2.0 * u1.Dot(AxisY)) * AxisY * 0.9;
			break;
		}
	}
		case GameObject::GO_CORE:
		{
			Vector3 n = (go2->pos - go1->pos).Normalize();
			go1->vel = u1 - (2.f * u1.Dot(n)) * n;
			break;
		}
		case GameObject::GO_PILLAR:
		{
			Vector3 n = (go2->pos - go1->pos).Normalize();
			go1->vel = u1 - (2.f * u1.Dot(n)) * n;
			break;
		}
		case GameObject::GO_PIVOT:
		{
			Vector3 n = (go2->pos - go1->pos).Normalize();
			go1->vel = u1 - (2.f * u1.Dot(n)) * n;
			break;
		}
		case GameObject::GO_FLIPPER:
		{
			Vector3 diff = go1->pos - go2->pos;
			Vector3 AxisX = go2->normal;
			Vector3 AxisY = Vector3(-AxisX.y, AxisX.x, 0);

			float projectedDistX = diff.Dot(AxisX);
			if (projectedDistX > 0) AxisX = -AxisX;
			Vector3 torque = (go1->pos - go2->ptr_pivot->pos).Length() * go2->ptr_pivot->angularVelocity * AxisX;

			go1->vel = u1 - (2.0 * u1.Dot(AxisX)) * AxisX;
			go1->vel -= torque * 10;
			
			for (int i = 0; i < 5; i++) {
				spawnParticles(go1->pos, AxisY, go1->color);
			}
			for (int i = 0; i < 5; i++) {
				spawnParticles(go1->pos, -AxisY, go1->color);
			}


			break;
		}
	}
}

void SceneCollision::spawnPong(Vector3& color) {
	GameObject* go = FetchGO();
	go->type = GameObject::GO_BALL;				//light grey until hit by wrecking ball
	go->active = true;
	go->pos.Set(m_worldWidth / 2, m_worldHeight / 2, 0.f);
	go->vel.Set(Math::RandFloatMinMax(-20, 20), Math::RandFloatMinMax(-20, 20), 0);
	//direction is random
	go->color = color;
	go->scale.Set(2, 2, 2);
	go->mass = go->scale.x * go->scale.y * go->scale.z;
}
void SceneCollision::makeWall(Vector3& startPos, Vector3& endPos, Vector3& color) {
	if (startPos != endPos) {
		GameObject* go = FetchGO();
		go->type = GameObject::GO_WALL;
		go->vel.SetZero();
		go->pos.Set((startPos.x + endPos.x) * 0.5f, (startPos.y + endPos.y) * 0.5f, 0.f);
		Vector3 direction = (endPos - startPos).Normalize();
		go->normal = direction.Cross(Vector3(0, 0, 1));
		float length = (endPos - startPos).Length();
		go->scale.Set(2.f, length, 1.f);
		go->color = color;
	}
}
void SceneCollision::makePillar(Vector3& startPos, Vector3& endPos, Vector3& color) {
	if (startPos != endPos) {
		GameObject* go = FetchGO();
		go->type = GameObject::GO_PILLAR;
		go->vel.SetZero();
		go->pos = startPos;
		float radius = (startPos - endPos).Length();
		go->scale.Set(radius, radius, 1.f);
		go->color = color;
	}
}
void SceneCollision::makeFlipper(Vector3& PivotPos, float FlipperLength, Vector3& flipperdirection, float flipperRad, bool clockwise) {
	//pivot
	GameObject* gopivot = FetchGO();
	gopivot->type = GameObject::GO_PIVOT;
	gopivot->vel.SetZero();
	gopivot->pos = PivotPos;
	gopivot->direction = flipperdirection;
	gopivot->scale.Set(1.f, 1.f, 1.f);
	gopivot->color = white;
	gopivot->angularVelocity = 0;

	//flipper
	GameObject* goflipper = FetchGO();
	goflipper->type = GameObject::GO_FLIPPER;
	goflipper->vel.SetZero();
	goflipper->pos = PivotPos + (flipperdirection.Normalize() * (FlipperLength / 2));
	goflipper->normal = flipperdirection.Normalize().Cross(Vector3(0, 0, 1));
	goflipper->scale.Set(1.f, FlipperLength, 1.f);
	goflipper->color = white;

	gopivot->ptr_flipper = goflipper;
	goflipper->ptr_pivot = gopivot;

	gopivot->clockwise = clockwise;
	gopivot->flipperangle = flipperRad;		//radian
	gopivot->action = false;
}

void SceneCollision::ghostPreview(GameObject::GAMEOBJECT_TYPE m_type, Vector3& startPos, Vector3& endPos) {
	if (startPos != endPos) {
		m_ghost->active = true;
		m_ghost->type = m_type;
		m_ghost->vel.SetZero();
		m_ghost->color.Set(0.8f, 0.8f, 0.8f);
		if (m_type == GameObject::GO_WALL) {
			m_ghost->pos.Set((startPos.x + endPos.x) * 0.5f, (startPos.y + endPos.y) * 0.5f, 0.f);
			Vector3 direction = (endPos - startPos).Normalize();
			m_ghost->normal = direction.Cross(Vector3(0, 0, 1));
			float length = (endPos - startPos).Length();
			m_ghost->scale.Set(2.f, length, 1.f);
		}
		else if (m_type == GameObject::GO_PILLAR) {
			m_ghost->pos = startPos;
			float radius = (startPos - endPos).Length();
			m_ghost->scale.Set(radius, radius, 1.f);
		}
	}
}
void SceneCollision::spawnParticles(Vector3& positionP, Vector3& directionP, Vector3& colorP) {
	GameObject* go = FetchGO();
	go->type = GameObject::GO_PARTICLE;
	go->active = true;
	go->color = colorP;
	go->pos = positionP;
		float oldx = directionP.x;
		float oldy = directionP.y;
		float offset = Math::RandFloatMinMax(-0.4, 0.4);
		float newx = oldx * cos(offset) - oldy * sin(offset);
		float newy = oldx * sin(offset) + oldy * cos(offset);
	go->age = Math::RandFloatMinMax(20, 30);
	go->vel = Vector3(newx, newy, 0).Normalize() * go->age;
	go->scale.Set(0.5, 0.5, 0.5);
}

void SceneCollision::spawnPowerup(Vector3& positionP, Vector3& directionP) {
	GameObject* go_powerup = FetchGO();
	go_powerup->active = true;
	go_powerup->type = GameObject::GO_POWERUP;
	go_powerup->pos = positionP;
	go_powerup->vel = directionP.Normalize() * 5.f;
	go_powerup->scale.Set(1, 1, 1);
	//green health, brown material, purple split
	int i = rand() % (sizeof(poweruplist) / sizeof(Vector3));
	go_powerup->color = poweruplist[i];
	go_powerup->age = 600;			//600frames at 60fpx, 10s
}
