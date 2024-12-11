#include "SceneAsteroid.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

const float SceneAsteroid::ROTATION_SPEED = 2.f;
const float SceneAsteroid::MAX_ROTATION_SPEED = 5.f;
const float SceneAsteroid::GRAVITY_CONSTANT = 3.f;

SceneAsteroid::SceneAsteroid()
{
}

SceneAsteroid::~SceneAsteroid()
{
}

Vector3 RotateVector(const Vector3& vec, float radian)
{
	return Vector3(vec.x * cos(radian) + vec.y * -sin(radian), 
				   vec.x * sin(radian) + vec.y * cos(radian), 0.f);
}

void SceneAsteroid::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	Math::InitRNG();
	for (int i = 0; i <= 999; i++) {
		m_goList.push_back(new GameObject(GameObject::GO_ASTEROID));
	}
	m_ship = new GameObject(GameObject::GO_SHIP);
	m_ship->active = true;
	m_ship->scale.Set(1, 1, 1);
	m_ship->pos.Set(m_worldWidth / 2, m_worldHeight / 2, 1);
	m_ship->direction.Set(0, 1, 0);

	m_ship->momentOfInertia = m_ship->mass * m_ship->scale.x * m_ship->scale.x;
	m_ship->angularVelocity = 0;
	m_torque.SetZero();
}

GameObject* SceneAsteroid::FetchGO()
{
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); it++) {
		GameObject* go = (GameObject*)*it;
		if (!go->active) {
			go->active = true;
			return go;
		}
	}
	int prevSize = m_goList.size();
	for (int i = 0; i < 10; i++) {
		m_goList.push_back(new GameObject(GameObject::GO_ASTEROID));
	}
	m_goList.at(prevSize)->active = true;
	return m_goList.at(prevSize);
}

void SceneAsteroid::Update(double dt)
{
	if (m_lives > 0) {
		gameplay = true;
		afterburn = false;

		SceneBase::Update(dt);

		elapsedFire += dt;
		elapsedwave += dt;

		//Calculating aspect ratio
		m_worldHeight = 100.f;
		m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

		if (Application::IsKeyPressed('9'))
		{
			m_speed = Math::Max(0.f, m_speed - 0.1f);
		}
		if (Application::IsKeyPressed('0'))
		{
			m_speed += 0.1f;
		}

		//ship movement
		{
			m_force.SetZero();
			m_torque.SetZero();
			if (Application::IsKeyPressed('W'))
			{
				thrust = 20.f;
				MAX_SPEED = 20;
				if (Application::IsKeyPressed(VK_SHIFT) && afterburn_fuel > 0.f) {		//afterburn, aka dash with fuel
					afterburn = true;
					afterburn_fuel -= 0.1f;
					MAX_SPEED = 50;
					thrust = 50.f;
					//afterburn particles
					{
						GameObject* go = FetchGO();
						go->type = GameObject::GO_PARTICLESA;
						go->pos = m_ship->pos;
						go->scale.Set(0.1, 0.1, 1);
						go->vel = -Vector3(cos(Math::DegreeToRadian(m_ship->angle)), sin(Math::DegreeToRadian(m_ship->angle)), 0) * 20.f;
						float oldx = go->vel.x;
						float oldy = go->vel.y;
						float thruster_offset = Math::RandFloatMinMax(-0.5, 0.5);
						float newx = oldx * cos(thruster_offset) - oldy * sin(thruster_offset);
						float newy = oldx * sin(thruster_offset) + oldy * cos(thruster_offset);
						go->age = rand() % 20 + 30;
						go->vel = Vector3(newx, newy, 1);
					}
				}
				//thruster particles
				{
					GameObject* go = FetchGO();
					go->type = GameObject::GO_PARTICLES;
					go->pos = m_ship->pos;
					go->scale.Set(0.1, 0.1, 1);
					go->vel = -m_ship->direction * 10.f;
					float oldx = go->vel.x;
					float oldy = go->vel.y;
					float thruster_offset = Math::RandFloatMinMax(-0.5, 0.5);
					float newx = oldx * cos(thruster_offset) - oldy * sin(thruster_offset);
					float newy = oldx * sin(thruster_offset) + oldy * cos(thruster_offset);
					go->age = rand() % 20 + 30;
					go->vel = Vector3(newx, newy, 1);
				}

				m_force += m_ship->direction * thrust;
			}
			if (Application::IsKeyPressed('S') && !afterburn)
			{
				m_force -= m_ship->direction * 5.0f;
			}
			//velocity
			Vector3 acceleration = m_force * (1.f / m_ship->mass);
			m_ship->vel += acceleration * dt * m_speed;
			if (m_ship->vel.LengthSquared() > MAX_SPEED * MAX_SPEED) m_ship->vel.Normalize() *= MAX_SPEED;
			m_ship->pos += m_ship->vel * dt * m_speed;
			if (Application::IsKeyPressed('A') && !afterburn)
			{
				m_force += m_ship->direction * ROTATION_SPEED;
				m_torque += Vector3(-m_ship->scale.x, -m_ship->scale.y, 0).Cross(Vector3(ROTATION_SPEED, 0, 0));
			}
			if (Application::IsKeyPressed('D') && !afterburn)
			{
				m_force += m_ship->direction * ROTATION_SPEED;
				m_torque += Vector3(-m_ship->scale.x, m_ship->scale.y, 0).Cross(Vector3(ROTATION_SPEED, 0, 0));
			}
			//Anguluar Rotation
			float anguluarAcceration = m_torque.z / m_ship->momentOfInertia;
			m_ship->angularVelocity += anguluarAcceration * dt * m_speed;
			m_ship->angularVelocity = Math::Clamp(m_ship->angularVelocity, (float)-MAX_ROTATION_SPEED, (float)MAX_ROTATION_SPEED);
			m_ship->direction = RotateVector(m_ship->direction, m_ship->angularVelocity * dt * m_speed);
			m_ship->angle = Math::RadianToDegree(atan2(m_ship->direction.y, m_ship->direction.x));

			Wrap(m_ship->pos.x, m_worldWidth);
			Wrap(m_ship->pos.y, m_worldHeight);
		}
		if (Application::IsKeyPressed(VK_SPACE) && ammo > 0 && !afterburn)			//bullets
		{
			float diff = elapsedFire - prevFire;
			if (diff >= fire_rate && ammo > 0) {
				GameObject* go = FetchGO();
				go->type = GameObject::GO_BULLET;
				go->pos.Set(m_ship->pos.x, m_ship->pos.y, 0);
				float accuracy = Math::RandFloatMinMax(-0.15, 0.15);    //in radians
				go->vel = Vector3(cos(Math::DegreeToRadian(m_ship->angle)), sin(Math::DegreeToRadian(m_ship->angle)), 0).Normalize() * BULLET_SPEED;
				float oldx = go->vel.x;
				float oldy = go->vel.y;
				float newx = oldx * cos(accuracy) - oldy * sin(accuracy);
				float newy = oldx * sin(accuracy) + oldy * cos(accuracy);
				go->vel = Vector3(newx, newy, 1);
				go->scale.Set(.2f, .2f, 1);
				go->age = 200;
				prevFire = elapsedFire;
				ammo--;
			}
		}
		if ((Application::IsKeyPressed('R') || ammo == 0) && ammo != max_ammo) {		//reload
			//set ammo to 0, wait, then refill ammo
			ammo = 0;
			float diff = elapsedFire - prevFire;
			if (diff >= reload_time) ammo = max_ammo;
		}
		//special cycle
		if ((!Application::IsKeyPressed('Q') && !Application::IsKeyPressed('E')) && !special_cycle) special_cycle = true;
		if (Application::IsKeyPressed('Q') && special_cycle)
		{
			special_choice--;
			special_cycle = false;
			if (special_choice < 1) special_choice = 3;
		}
		if (Application::IsKeyPressed('E') && special_cycle)
		{
			special_choice++;
			special_cycle = false;
			if (special_choice > 3) special_choice = 1;
		}
		//special fire
		if (!Application::IsKeyPressed(VK_TAB) && !special_loaded) special_loaded = true;
		if (Application::IsKeyPressed(VK_TAB) && special_ammo > 0 && !afterburn && special_loaded) {
			if (special_choice == 1) {		//shotgun
				special_ammo--; special_loaded = false;
				for (int i = 0; i < 25; i++) {
					GameObject* go = FetchGO();
					go->type = GameObject::GO_BULLET;
					go->pos.Set(m_ship->pos.x, m_ship->pos.y, 0);
					float accuracy = Math::RandFloatMinMax(-0.4, 0.4);    //in radians
					go->vel = m_ship->direction.Normalize() * (rand() % 50 + 100);
					float oldx = go->vel.x;
					float oldy = go->vel.y;
					float newx = oldx * cos(accuracy) - oldy * sin(accuracy);
					float newy = oldx * sin(accuracy) + oldy * cos(accuracy);
					go->vel = Vector3(newx, newy, 1);
					go->scale.Set(.3f, .3f, 1);
					go->age = 15;
				}
			}
			else if (special_choice == 2) {		//railgun
				special_ammo--; special_loaded = false;
				for (int i = 0; i < 25; i++) {
					GameObject* go = FetchGO();
					go->type = GameObject::GO_RAILGUN;
					go->pos.Set(m_ship->pos.x, m_ship->pos.y, 0);
					go->vel = m_ship->direction.Normalize() * 250.f;
					go->scale.Set(.75f, .75f, 1);
					go->age = 999;
				}
			}
			else if (special_choice == 3 && !barrier) {		//barrier bounce
				special_ammo--; special_loaded = false;
				barrier = true;
				for (int i = -10; i < 10; i++) {
					GameObject* go = FetchGO();
					go->type = GameObject::GO_BARRIER;
					go->pos.Set(m_ship->pos.x, m_ship->pos.y, 0);
					float accuracy = Math::DegreeToRadian(i * 5);    //in radians
					go->vel = m_ship->direction.Normalize() * 100.f;
					float oldx = go->vel.x; float oldy = go->vel.y;
					float newx = oldx * cos(accuracy) - oldy * sin(accuracy);
					float newy = oldx * sin(accuracy) + oldy * cos(accuracy);
					go->vel = Vector3(newx, newy, 1);
					go->scale.Set(.5f, .5f, 1);
					go->age = 10;		//to despawn
				}
			}
		}

		//Mouse Section
		static bool bLButtonState = false;
		if (!bLButtonState && Application::IsMousePressed(0))
		{
			bLButtonState = true;
			std::cout << "LBUTTON DOWN" << std::endl;
		}
		else if (bLButtonState && !Application::IsMousePressed(0))
		{
			bLButtonState = false;
			std::cout << "LBUTTON UP" << std::endl;
		}
		static bool bRButtonState = false;
		if (!bRButtonState && Application::IsMousePressed(1))
		{
			bRButtonState = true;
			std::cout << "RBUTTON DOWN" << std::endl;
		}
		else if (bRButtonState && !Application::IsMousePressed(1))
		{
			bRButtonState = false;
			std::cout << "RBUTTON UP" << std::endl;
		}
		//wave spawner
		if (elapsedwave >= wavebetween) {
			elapsedwave = 0;
			waveint++;
			for (int i = 0; i < waveint * 5; i++) {
				GameObject* go = FetchGO();
				go->type = GameObject::GO_ASTEROID;
				go->pos.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), go->pos.z);
				go->vel.Set(Math::RandFloatMinMax(-20, 20), Math::RandFloatMinMax(-20, 20), 0);
				go->size = rand() % 3 + 1;
				go->scale.Set(go->size, go->size, 1);
			}

			if (waveint > 2) {		//spawn enemy ships
				for (int i = 0; i < waveint * 3; i++) {		//scout
					GameObject* go = FetchGO();
					go->type = GameObject::GO_SCOUT;
					go->pos.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), go->pos.z);
					go->direction.Set(Math::RandFloatMinMax(-1, 1), Math::RandFloatMinMax(-1, 1), 0);
					go->size = 1;
					go->scale.Set(go->size, go->size, 1);
				}
				for (int i = 0; i < waveint; i++) {		//bomber
					GameObject* go = FetchGO();
					go->type = GameObject::GO_BOMBER;
					go->pos.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), go->pos.z);
					go->direction.Set(Math::RandFloatMinMax(-1, 1), Math::RandFloatMinMax(-1, 1), 0);
					go->size = 2;
					go->scale.Set(go->size, go->size, 1);
				}
			}
			if (waveint >= 4) {		//hive spawner
				for (int i = 0; i < waveint - 4; i++) {
					GameObject* go = FetchGO();
					go->type = GameObject::GO_HIVE;
					go->pos.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), go->pos.z);
					go->size = 4;
					go->scale.Set(go->size, go->size, 1);
				}
			}
		}
		//event spawner
		{
			//Spawn Black Hole
			if ((rand() % 3000 + 1) == 1) {
				GameObject* go = FetchGO();
				go->type = GameObject::GO_BLACKHOLE;
				go->scale.Set(5, 5, 1);
				go->mass = 10000;
				go->pos.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), go->pos.z);
				go->vel.SetZero();
				go->age = 1800;
			}
			//Spawn White Hole
			if ((rand() % 3000 + 1) == 1) {
				GameObject* go = FetchGO();
				go->type = GameObject::GO_WHITEHOLE;
				go->scale.Set(5, 5, 1);
				go->mass = 10000;
				go->pos.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), go->pos.z);
				go->vel.SetZero();
				go->age = 1800;
			}
			//ore spawner
			if ((rand() % 500 + 1) == 1) {
				GameObject* go = FetchGO();
				go->type = GameObject::GO_ORE;
				go->pos.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), go->pos.z);
				go->vel.Set(Math::RandFloatMinMax(-20, 20), Math::RandFloatMinMax(-20, 20), 0);
				go->age = 500;
				go->scale.Set(3, 3, 1);
			}
		}
		//Physics Simulation Section
		int parse = m_goList.size();
		for (int i = 0; i<parse; i++)
		{
			GameObject* go = m_goList[i];
			if (go->active)
			{
				go->pos += go->vel * dt * m_speed;
				if (go->bounced == true) {
					go->b_frames--;
				}
				if (go->b_frames <= 0) {
					go->bounced = false;
				}
				if (go->type == GameObject::GO_ASTEROID) {
					if (go->size <= 1) go->active = false;
					Wrap(go->pos.x, m_worldWidth);
					Wrap(go->pos.y, m_worldHeight);

					float dist = go->pos.DistanceSquared(m_ship->pos);
					float rad = (m_ship->scale.x + go->scale.x) * (m_ship->scale.x + go->scale.x);
					if (dist < rad) {
						go->active = false;
						m_lives -= go->size;
					}
				}
				else if (go->type == GameObject::GO_ENEMYBULLET) {
					go->age--;
					if (go->pos.x > m_worldWidth || go->pos.x < 0 ||
						go->pos.y > m_worldHeight || go->pos.y < 0
						|| go->age <= 0) {
						go->active = false;
						continue;
					}

					float dist = go->pos.DistanceSquared(m_ship->pos);
					float rad = (m_ship->scale.x + go->scale.x) * (m_ship->scale.x + go->scale.x);
					if (dist < rad) {
						go->active = false;
						m_lives--;
					}
				}
				else if (go->type == GameObject::GO_BULLET || go->type == GameObject::GO_RAILGUN) {
					go->age--;
					if (go->pos.x > m_worldWidth || go->pos.x < 0 ||
						go->pos.y > m_worldHeight || go->pos.y < 0
						|| go->age <= 0) {
						go->active = false;
						continue;
					}
					for (int j = 0; j < m_goList.size(); ++j) {
						if (m_goList[j]->active && m_goList[j]->type == GameObject::GO_ASTEROID)
						{
							float dist = Vector3::DistanceSquared(go->pos, m_goList[j]->pos);
							float rad = (go->scale.x + m_goList[j]->scale.x) * (go->scale.x + m_goList[j]->scale.x);
							if (go->type == GameObject::GO_BULLET) {
								if (dist < rad) {
									go->active = false;
									m_goList[j]->active = false;
									m_score++;
									//split the asteroids
									if (m_goList[j]->size > 1) {
										for (int i = 0; i < 5; i++) {
											GameObject* childA = FetchGO();
											childA->type = GameObject::GO_ASTEROID;
											childA->pos = m_goList[j]->pos;
											childA->vel.Set(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), 0);
											childA->size = m_goList[j]->size - 1;
											childA->scale.Set(childA->size, childA->size, 1);
										}
									}
								}
							}
							else {		//railgun doesnt despawn, doesnt split asteroids
								if (dist < rad) {
									m_goList[j]->active = false;
									m_score++;
								}
							}
						}
						else if (m_goList[j]->active && m_goList[j]->type == GameObject::GO_ORE) {
							float dist = Vector3::DistanceSquared(go->pos, m_goList[j]->pos);
							float rad = (go->scale.x + m_goList[j]->scale.x) * (go->scale.x + m_goList[j]->scale.x);
							if (go->type == GameObject::GO_BULLET) {
								if (dist < rad) {
									go->active = false;
									m_goList[j]->type = GameObject::GO_RESOURCE;
									m_goList[j]->vel.Set(0, 0, 0);
									m_goList[j]->scale.Set(2, 2, 2);
									m_goList[j]->age = 300;
									continue;
								}
							}
							else {
								if (dist < rad) {
									m_goList[j]->type = GameObject::GO_RESOURCE;
									m_goList[j]->vel.Set(0, 0, 0);
									m_goList[j]->scale.Set(2, 2, 2);
									m_goList[j]->age = 300;
									continue;
								}
							}
						}
						else if (m_goList[j]->active && m_goList[j]->type == GameObject::GO_SCOUT) {
							float dist = Vector3::DistanceSquared(go->pos, m_goList[j]->pos);
							float rad = (go->scale.x + m_goList[j]->scale.x) * (go->scale.x + m_goList[j]->scale.x);
							if (go->type == GameObject::GO_BULLET) {
								if (dist < rad) {
									go->active = false;
									m_goList[j]->active = false;
									m_score++;
									continue;
								}
							}
							else {
								if (dist < rad) {
									m_goList[j]->active = false;
									m_score++;
									continue;
								}
							}
						}
						else if (m_goList[j]->active && m_goList[j]->type == GameObject::GO_BOMBER) {
							float dist = Vector3::DistanceSquared(go->pos, m_goList[j]->pos);
							float rad = (go->scale.x + m_goList[j]->scale.x) * (go->scale.x + m_goList[j]->scale.x);
							if (go->type == GameObject::GO_BULLET) {
								if (dist < rad) {
									go->active = false;
									m_goList[j]->active = false;
									m_score++;
									continue;
								}
							}
							else {
								if (dist < rad) {
									m_goList[j]->active = false;
									m_score++;
									continue;
								}
							}
						}
						else if (m_goList[j]->active && m_goList[j]->type == GameObject::GO_HIVE) {
							float dist = Vector3::DistanceSquared(go->pos, m_goList[j]->pos);
							float rad = (go->scale.x + m_goList[j]->scale.x) * (go->scale.x + m_goList[j]->scale.x);
							if (go->type == GameObject::GO_BULLET) {
								if (dist < rad) {
									go->active = false;
									m_goList[j]->health--;
									continue;
								}
							}
							else {
								if (dist < rad) {
									m_goList[j]->health -= 10;
									continue;
								}
							}
						}
						else if (m_goList[j]->active && m_goList[j]->type == GameObject::GO_QUEEN) {
							float dist = Vector3::DistanceSquared(go->pos, m_goList[j]->pos);
							float rad = (go->scale.x + m_goList[j]->scale.x) * (go->scale.x + m_goList[j]->scale.x);
							if (go->type == GameObject::GO_BULLET) {
								if (dist < rad) {
									go->active = false;
									m_goList[j]->health--;
									continue;
								}
							}
							else {
								if (dist < rad) {
									m_goList[j]->health -= 10;
									continue;
								}
							}

						}
					}
				}
				else if (go->type == GameObject::GO_BARRIER) {
					go->age--;
					if (go->age <= 0) {
						go->active = false;
						barrier = false;
						continue;
					}
					//collision with all entities will bounce them
					for (int j = 0; j < m_goList.size(); ++j) {
						if (m_goList[j]->active == true &&
							m_goList[j]->type != GameObject::GO_BARRIER && 
							m_goList[j]->type != GameObject::GO_BULLET && 
							m_goList[j]->type != GameObject::GO_RAILGUN &&
							m_goList[j]->bounced == false) {
							float dist = Vector3::DistanceSquared(go->pos, m_goList[j]->pos);
							float rad = (go->scale.x + m_goList[j]->scale.x) * (go->scale.x + m_goList[j]->scale.x);
							if (dist < rad) {
								m_goList[j]->bounced = true;
								Vector3 A = m_goList[j]->vel; Vector3 B = go->vel;
								float entity_speed = A.Length();
								A.Normalized(); B.Normalized();
								Vector3 projection = A.Dot(B) * B;
								m_goList[j]->vel = A - (2 * projection);
								m_goList[j]->vel = m_goList[j]->vel.Normalize() * entity_speed;
								continue;
							}
						}
					}
				}
				else if (go->type == GameObject::GO_PARTICLES || go->type == GameObject::GO_PARTICLESA) {
					go->age--;
					if (go->pos.x > m_worldWidth || go->pos.x < 0 ||
						go->pos.y > m_worldHeight || go->pos.y < 0
						|| go->age <= 0) {
						go->active = false;
						continue;
					}
				}
				else if (go->type == GameObject::GO_ORE) {		//despawn after a while
					Wrap(go->pos.x, m_worldWidth);
					Wrap(go->pos.y, m_worldHeight);
					go->age--;
					if (go->age <= 0) {
						go->active = false;
					}
				}
				else if (go->type == GameObject::GO_RESOURCE) {		//despawn after a while
					go->age--;
					if (go->age <= 0) {
						go->active = false;
						continue;
					}
					float dist = go->pos.DistanceSquared(m_ship->pos);
					float rad = (m_ship->scale.x + go->scale.x) * (m_ship->scale.x + go->scale.x);
					if (dist < rad) {
						go->active = false;
						int powerup = rand() % 3 + 1;
						if (powerup == 1) {
							m_lives += 10;
						}
						else if (powerup == 2) {
							special_ammo += 5;
						}
						else if (powerup == 3) {
							afterburn_fuel += 10;
						}
					}
				}
				else if (go->type == GameObject::GO_BLACKHOLE || go->type == GameObject::GO_WHITEHOLE) {
					go->age--;		//despawns after a while
					if (go->age <= 0) {
						go->active = false;
						go->mass = 1;
						continue;
					}
					for (int j = 0; j < m_goList.size(); ++j) {
						if (m_goList[j]->active && m_goList[j]->type != GameObject::GO_BLACKHOLE &&
							m_goList[j]->type != GameObject::GO_WHITEHOLE) {
							float distance = m_goList[j]->pos.DistanceSquared(go->pos);
							if (distance < 3600.f) {
								//If it is too near, destroy and add mass
								if (distance < 4.0f) {
									go->mass += m_goList[j]->mass;
									m_goList[j]->active = false;
								}
								else {
									float sign = (go->type == GameObject::GO_WHITEHOLE) ? -1.0f : 1.0f;
									Vector3 dir = sign * (go->pos - m_goList[j]->pos).Normalized();

									float force = CalculateAdditionalForce(m_goList[j], go);
									m_goList[j]->vel += 1.f / m_goList[j]->mass * dir * force * dt * m_speed;
								}
							}
						}
					}
				}
				else if (go->type == GameObject::GO_SCOUT) {	//scout ai
					Wrap(go->pos.x, m_worldWidth);
					Wrap(go->pos.y, m_worldHeight);
					//movement
					float targetangle = Math::RadianToDegree(std::atan2(m_ship->pos.y - go->pos.y, m_ship->pos.x - go->pos.x));
					if ((targetangle - go->angle) >= 0) go->direction = RotateVector(go->direction, 0.2f);
					else if ((targetangle - go->angle) <= 0) go->direction = RotateVector(go->direction, -0.2f);
					go->angle = Math::RadianToDegree(std::atan2(go->direction.y, go->direction.x));
					go->vel += go->direction.Normalized() * 5.f;
					if (go->vel.Length() > 10.f) go->vel = go->vel.Normalized() * 10.f;
					//fire
					if ((rand() % 200 + 1) > 199) {
						GameObject* go1 = FetchGO();
						go1->type = GameObject::GO_ENEMYBULLET;
						go1->pos.Set(go->pos.x, go->pos.y, 0);
						go1->vel = go->direction.Normalize() * BULLET_SPEED;
						go1->scale.Set(.2f, .2f, 1);
						go1->age = 100;
					}
					//keep distance
					float dist = go->pos.DistanceSquared(m_ship->pos);
					if (dist < 500.f) go->vel -= go->direction.Normalized() * 10.f;
				}
				else if (go->type == GameObject::GO_BOMBER) {	//bomber ai
					Wrap(go->pos.x, m_worldWidth);
					Wrap(go->pos.y, m_worldHeight);
					//movement
					float targetangle = Math::RadianToDegree(std::atan2(m_ship->pos.y - go->pos.y, m_ship->pos.x - go->pos.x));
					if ((targetangle - go->angle) >= 0) go->direction = RotateVector(go->direction, 0.05f);
					else if ((targetangle - go->angle) <= 0) go->direction = RotateVector(go->direction, -0.05f);
					go->angle = Math::RadianToDegree(std::atan2(go->direction.y, go->direction.x));
					go->vel += go->direction.Normalized() * 10.f;
					if (go->vel.Length() > 10.f) go->vel = go->vel.Normalized() * 10.f;
					//collision
					float dist = go->pos.DistanceSquared(m_ship->pos);
					float rad = (m_ship->scale.x + go->scale.x) * (m_ship->scale.x + go->scale.x);
					if (dist < rad) {
						go->active = false;
						m_lives--;
					}
				}
				else if (go->type == GameObject::GO_HIVE) {		//hive ai
					//does not move
					Wrap(go->pos.x, m_worldWidth);
					Wrap(go->pos.y, m_worldHeight);
					//summon
					if ((rand() % 500 + 1) > 495) {
						GameObject* go1 = FetchGO();
						go1->type = GameObject::GO_SCOUT;
						go1->pos.Set(go->pos.x, go->pos.y, 0);
						go1->scale.Set(1, 1, 1);
					}
					if ((rand() % 500 + 1) > 499) {
						GameObject* go1 = FetchGO();
						go1->type = GameObject::GO_BOMBER;
						go1->pos.Set(go->pos.x, go->pos.y, 0);
						go1->scale.Set(2, 2, 1);
					}
					//death
					if (go->health <= 0) {
						go->type = GameObject::GO_QUEEN;
						go->health = 50;
						go->vel.Set(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), 0);
						go->angle = Math::RadianToDegree(std::atan2(go->vel.y, go->vel.x));
						go->scale.Set(3, 3, 1);
						for (int i = 0; i < 10; ++i) {		//explode with scouts in random direction
							GameObject* go1 = FetchGO();
							go1->type = GameObject::GO_SCOUT;
							go1->pos.Set(go->pos.x, go->pos.y, 0);
							go1->scale.Set(1, 1, 1);
							go1->vel.Set(Math::RandFloatMinMax(-30, 30), Math::RandFloatMinMax(-30, 30), 0);
						}
					}
				}
				else if (go->type == GameObject::GO_QUEEN) {	//queen ai
					Wrap(go->pos.x, m_worldWidth);
					Wrap(go->pos.y, m_worldHeight);
					//evade player
					if ((rand() % 500 + 1) == 1) {
						go->vel.Set(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), 0);
						go->angle = Math::RadianToDegree(std::atan2(go->vel.y, go->vel.x));
					}
					//summon
					if ((rand() % 500 + 1) > 495) {
						GameObject* go1 = FetchGO();
						go1->type = GameObject::GO_SCOUT;
						go1->pos.Set(go->pos.x, go->pos.y, 0);
						go1->scale.Set(1, 1, 1);
					}
					if ((rand() % 500 + 1) > 499) {
						GameObject* go1 = FetchGO();
						go1->type = GameObject::GO_BOMBER;
						go1->pos.Set(go->pos.x, go->pos.y, 0);
						go1->scale.Set(2, 2, 1);
					}
					//death
					if (go->health <= 0) {
						go->active = false;
					}
				}
			}
		}
	}
	else {
		gameplay = false;
		if (Application::IsKeyPressed('C')) {
			m_lives = 25;
			m_score = 0;
			ammo = max_ammo; special_ammo = 10;
			m_ship->pos.Set(m_worldWidth / 2, m_worldHeight / 2, 1);
			m_ship->vel.SetZero();
			m_ship->direction.Set(0, 1, 0);
			m_force.SetZero(); m_torque.SetZero();
			gameplay = true;
			while (m_goList.size() > 0) {
				GameObject* go = m_goList.back();
				delete go;
				m_goList.pop_back();
			}
			for (int i = 0; i <= 250; i++) {
				m_goList.push_back(new GameObject(GameObject::GO_ASTEROID));
			}
			waveint = 0;
		}
	}
}

void SceneAsteroid::RenderGO(GameObject* go)
{
	modelStack.PushMatrix();
	modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
	modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
	switch (go->type)
	{
	case GameObject::GO_SHIP:
		modelStack.Rotate(go->angle, 0, 0, 1);
		RenderMesh(meshList[GEO_SHIP], false);
		break;
	case GameObject::GO_TURRET:
		modelStack.Rotate(go->angle, 0, 0, 1);
		RenderMesh(meshList[GEO_TURRET], false);
		break;
	case GameObject::GO_ASTEROID:
		RenderMesh(meshList[GEO_ASTEROID], false);
		break;
	case GameObject::GO_ORE:
		RenderMesh(meshList[GEO_ORE], false);
		break;
	case GameObject::GO_RESOURCE:
		RenderMesh(meshList[GEO_RESOURCE], false);
		break;
	case GameObject::GO_BULLET:
		RenderMesh(meshList[GEO_BALL], false);
		break;
	case GameObject::GO_RAILGUN:
		RenderMesh(meshList[GEO_BALL], false);
		break;
	case GameObject::GO_BARRIER:
		RenderMesh(meshList[GEO_BALL], false);
		break;
	case GameObject::GO_ENEMYBULLET:
		RenderMesh(meshList[GEO_BALL], false);
		break;
	case GameObject::GO_PARTICLES:
		RenderMesh(meshList[GEO_BALLYELLOW], false);
		break;
	case GameObject::GO_PARTICLESA:
		RenderMesh(meshList[GEO_BALLRED], false);
		break;
	case GameObject::GO_SCOUT:
		modelStack.Rotate(go->angle, 0, 0, 1);
		RenderMesh(meshList[GEO_SCOUT], false);
		break;
	case GameObject::GO_BOMBER:
		modelStack.Rotate(go->angle, 0, 0, 1);
		RenderMesh(meshList[GEO_BOMBER], false);
		break;
	case GameObject::GO_HIVE:
		RenderMesh(meshList[GEO_HIVE], false);
		break;

	case GameObject::GO_QUEEN:
		modelStack.Rotate(go->angle, 0, 0, 1);
		RenderMesh(meshList[GEO_QUEEN], false);
		break;
	case GameObject::GO_BLACKHOLE:
		RenderMesh(meshList[GEO_BLACKHOLE], false);
		break;
	case GameObject::GO_WHITEHOLE:
		RenderMesh(meshList[GEO_WHITEHOLE], false);
		break;
	}
	modelStack.PopMatrix();
}

void SceneAsteroid::Render()
{
	//default renderer
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

		RenderGO(m_ship);
		for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject* go = (GameObject*)*it;
			if (go->active)
			{
				RenderGO(go);
			}
		}
	}
	//On screen text
	std::ostringstream ss;
	ss.precision(3);
	
	if (ammo == 0) ss << "Reloading!"; 
	else ss << "Ammo:" << ammo << "/"<<max_ammo;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

	ss.str("");
	ss << "FUEL:";
	if (afterburn_fuel <= 0) ss << "0.0";
	else ss << afterburn_fuel;

	//text change when scrolling through list
	if (special_choice == 1) {
		ss << "  <Shotgun>:";
	}
	else if (special_choice == 2) {
		ss << "  <Railgun>:";
	}
	else if (special_choice == 3) {
		ss << "  <Barrier>:";
	}
	ss << special_ammo;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);
	
	ss.str("");
	ss << "Lives:" << m_lives << "Score:" << m_score << "Wave:" << waveint;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 57);

	ss.str("");
	int entityactive = 0;
	for (int i = 0; i < m_goList.size();i++) {
		if (m_goList.at(i)->active) {
			entityactive++;
		}
	}
	ss << "Active:"<<entityactive<<"/"<<m_goList.size();
	if (Application::IsKeyPressed('T')) {
		for (int i = 0; i < m_goList.size(); i++) {
			if (m_goList.at(i)->active && m_goList.at(i)->type == 5) {
				std::cout << m_goList.at(i)->size << std::endl;
			}
		}
	}
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 0);

	if (gameplay == false) {
		ss.str("");
		ss << "You lose!";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 45);
		ss.str("");
		ss << "Press 'C' to play again.";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 42);

	}
}

void SceneAsteroid::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if(m_ship)
	{
		delete m_ship;
		m_ship = NULL;
	}
}

float SceneAsteroid::CalculateAdditionalForce(GameObject* go1, GameObject* go2)
{
	float radiusSquared = go1->pos.DistanceSquared(go2->pos);
	return (GRAVITY_CONSTANT * (go1->mass * go2->mass)) / radiusSquared;
}