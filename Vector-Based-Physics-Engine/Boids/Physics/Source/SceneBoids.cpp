#include "SceneBoids.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneBoids::SceneBoids()
{
}

SceneBoids::~SceneBoids()
{
}

void SceneBoids::Init()
{
	SceneHerdBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	Math::InitRNG();

	leader = new AIHERDS(AIHERDS::GO_LEADER);
	leader->active = true;
	leader->pos.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), 0);
	leader->direction.Set(Math::RandFloatMinMax(-1, 1), Math::RandFloatMinMax(-1, 1), 0);
	leader->direction.Normalized();
	leader->vel = leader->direction * 5.f;
	leader->scale.Set(1, 1, 1);
	m_goList.push_back(leader);

	for (int i = 0; i < 100; i++) {
		m_goList.push_back(new AIHERDS(AIHERDS::GO_BALL));
	}

	//for (int i = 0; i < 200; i++) {		//spawn agents when init
	//	AIHERDS* go = FetchGO();
	//	go->type = AIHERDS::GO_CELL;
	//	go->pos.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), go->pos.z);
	//	go->direction.Set(Math::RandFloatMinMax(-1, 1), Math::RandFloatMinMax(-1, 1), 0);
	//	go->direction.Normalized();
	//	go->vel = go->direction * 5.f;
	//	go->scale.Set(go->size, go->size, go->size);
	//}
}

AIHERDS* SceneBoids::FetchGO()
{
	for (std::vector<AIHERDS*>::iterator it = m_goList.begin(); it != m_goList.end(); it++) {
		AIHERDS* go = (AIHERDS*)*it;
		if (!go->active) {
			go->active = true;
			return go;
		}
	}
	int prevSize = m_goList.size();
	for (int i = 0; i < 10; i++) {
		m_goList.push_back(new AIHERDS(AIHERDS::GO_BALL));
	}
	m_goList.at(prevSize)->active = true;
	return m_goList.at(prevSize);
}

void SceneBoids::Update(double dt)
{
	SceneHerdBase::Update(dt);
	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();

	if (Application::IsKeyPressed('1')) {
		//spawn boids
		AIHERDS* go = FetchGO();
		go->type = AIHERDS::GO_CELL;
		go->pos.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), go->pos.z);
		go->direction.Set(Math::RandFloatMinMax(-1, 1), Math::RandFloatMinMax(-1, 1), 0);
		go->direction.Normalized();
		go->vel = go->direction * 5.f;
		go->scale.Set(go->size, go->size, go->size);
	}

	if (Application::IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if (Application::IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}

	//Mouse Section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		//attract agents
		//Vector3((x / w) * m_worldWidth, (h - y) / h * m_worldHeight, 0);
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
		//place walls
		//Vector3((x / w) * m_worldWidth, (h - y) / h * m_worldHeight, 0);
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}

	//Physics Simulation Section

	for (std::vector<AIHERDS*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		AIHERDS* go = (AIHERDS*)*it;
		if (go->active) {
			go->pos += go->vel * dt * m_speed;
			//get rid of wrap after ray cast
			Wrap(go->pos.x, m_worldWidth);
			Wrap(go->pos.y, m_worldHeight);
			//ray cast for line of sight and avoidance
			//if () {		//cast ray directly infront

			//}
			////if center ray doesnt hit
			//else {
			//	for (int i = 0; i < ((go->FOV / 2) / 5); i++) {
			//		//cast left and right at the same time
			//		//break when hit
			//	}
			//}
			//return ray that hit, rotate self towards that angle

			//if dont have collision, pass alignment and cohesion
			//scan for walls and boundary

			//seperation
			//find distance of other boids, if there is smth beside itself
			//turn away

			//radius check for alignment and cohesion
			Vector3 targetPos = go->pos, avoidPos = go->pos;
			int targetSize = 1; double targetAngle = 0;
			int avoidSize = 1; double avoidAngle = 0;
			if (bLButtonState) {
				targetPos += Vector3((x / w) * m_worldWidth, (h - y) / h * m_worldHeight, 0);
				targetSize++;
			}
			if (bRButtonState) {
				avoidPos += Vector3((x / w) * m_worldWidth, (h - y) / h * m_worldHeight, 0);
				avoidSize++;
			}
			else {
				for (int i = 0; i < m_goList.size(); ++i) {
					if (m_goList[i]->type == AIHERDS::GO_CELL) {
						float distSqr = go->pos.DistanceSquared(m_goList[i]->pos);
						float cohesiondist = go->cohesionrange * go->cohesionrange;
						float seperationdist = go->seperationrange * go->seperationrange;
						if (distSqr < seperationdist) {
							avoidPos += m_goList[i]->pos;
							avoidSize++;
						}
						else if (distSqr < cohesiondist) {
							targetPos += m_goList[i]->pos;
							targetSize++;
						}
						
					}
				}
				targetPos.Set(targetPos.x / targetSize, targetPos.y / targetSize, 0);
				avoidPos.Set(avoidPos.x / avoidSize, avoidPos.y / avoidSize, 0);
			}

			targetAngle = (Math::RadianToDegree(std::atan2(targetPos.y - go->pos.y, targetPos.x - go->pos.x)) + 
							Math::RadianToDegree(std::atan2(avoidPos.y - go->pos.y, avoidPos.x - go->pos.x))) / 2;
			
			//issue: overall direction is towards right
			//observation, atan2 returns -180 to 0, then 0 to 180 with respect to x-axis
			//this causes the -180/180 mark, with -x, to go to a feedback loop
			//observation cannot be fixed currently
			go->angle = Math::RadianToDegree(std::atan2(go->direction.y, go->direction.x));
			if ((targetAngle - go->angle) > 0) {
				//std::cout << targetAngle - go->angle << std::endl;
				go->direction = RotateVector(go->direction, 0.05f);
				//std::cout << "turn left" << std::endl;
			}
			else if ((targetAngle - go->angle) < 0) {
				//std::cout << targetAngle - go->angle << std::endl;
				go->direction = RotateVector(go->direction, -0.05f);
				//std::cout << "turn right" << std::endl;
			}
			//update velocity
			go->vel += go->direction.Normalized() * 10.f;
			if (go->vel.LengthSquared() > (go->max_speed * go->max_speed)) {
				go->vel = go->direction.Normalized() * go->max_speed;
			}
			if (go->type == AIHERDS::GO_LEADER) {
				//std::cout << "angle:		" << leader->angle << "/" << targetAngle << "/" << targetAngle - go->angle << std::endl;
			}
		}
	}
}

void SceneBoids::RenderGO(AIHERDS* go)
{
	modelStack.PushMatrix();
	modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
	modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
	switch (go->type)
	{
	case AIHERDS::GO_LEADER:
		modelStack.Rotate(go->angle, 0, 0, 1);
		RenderMesh(meshList[GEO_LEADER], false);
		break;
	case AIHERDS::GO_CELL:
		modelStack.Rotate(go->angle, 0, 0, 1);
		RenderMesh(meshList[GEO_CELL], false);
		break;
	case AIHERDS::GO_FOOD:
		RenderMesh(meshList[GEO_FOOD], false);
		break;
	}
	modelStack.PopMatrix();
}

void SceneBoids::Render()
{
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

		for (std::vector<AIHERDS*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			AIHERDS* go = (AIHERDS*)*it;
			if (go->active) RenderGO(go);
		}
	}
	RenderGO(leader);
	//On screen text

	std::ostringstream ss;
	ss.precision(3);
	ss << "SceneBoids";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 0);
	ss.str("");
	int entityactive = 0;
	for (int i = 0; i < m_goList.size();i++) {
		if (m_goList.at(i)->active) {
			entityactive++;
		}
	}
	ss << fps << "    " << entityactive << "/" << m_goList.size();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);
}

void SceneBoids::Exit()
{
	SceneHerdBase::Exit();
	//Cleanup AIHERDSs
	while(m_goList.size() > 0)
	{
		AIHERDS *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
}