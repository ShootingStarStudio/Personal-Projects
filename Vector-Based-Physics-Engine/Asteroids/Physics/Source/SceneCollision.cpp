#include "SceneCollision.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneCollision::SceneCollision()
{
}

SceneCollision::~SceneCollision()
{
}

void SceneCollision::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 1.f;
	m_objectCount = 0;
	
	Math::InitRNG();
	for (int i = 0; i <= 100; i++) {
		m_goList.push_back(new GameObject(GameObject::GO_BALL));
	}
	m_ghost = new GameObject(GameObject::GO_BALL);
}

GameObject* SceneCollision::FetchGO()
{
	m_objectCount++;
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); it++) {
		GameObject* go = (GameObject*)*it;
		if (!go->active) {
			go->active = true;
			return go;
		}
	}
	int prevSize = m_goList.size();
	for (int i = 0; i < 10; i++) {
		m_goList.push_back(new GameObject(GameObject::GO_BALL));
	}
	m_goList.at(prevSize)->active = true;
	return m_goList.at(prevSize);
}

void SceneCollision::ReturnGO(GameObject *go)
{
	go->active = false;
	m_objectCount--;
}

void SceneCollision::Update(double dt)
{
	SceneBase::Update(dt);

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	
	if(Application::IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if(Application::IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}

	//Mouse Section
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	mousePos.Set((x / w) * m_worldWidth, (h - y) / h * m_worldHeight, 0);

	static bool bLButtonState = false;
	if(!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;

		GameObject* go = FetchGO();
		go->type = GameObject::GO_BALL;
		go->pos = mousePos;
		go->vel.Set(0, 0, 0);
		go->scale.Set(1, 1, 1);

	}
	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if(bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;

		//Exercise 10: spawn large GO_BALL
		//GameObject* go = FetchGO();
		//go->type = GameObject::GO_BALL;
		////go->pos.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), go->pos.z);
		////mouse position
		//go->vel.Set(0, 0, 0);
		//go->scale.Set(3, 3, 1);
	}

	//Physics Simulation Section

	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			go->pos += go->vel * dt * m_speed;
			//Exercise 7a: implement movement for game objects
			
			//Exercise 7b: handle out of bound game objects
			
			//Exercise 8a: handle collision between GO_BALL and GO_BALL using velocity swap

			//Exercise 8b: store values in auditing variables

			//Exercise 10: handle collision using momentum swap instead

			//Exercise 12: improve inner loop to prevent double collision

			//Exercise 13: improve collision detection algorithm 
		}
	}
}


void SceneCollision::RenderGO(GameObject *go)
{
	modelStack.PushMatrix();
	modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
	modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
	switch(go->type)
	{
	case GameObject::GO_BALL:
		//Exercise 4: render a sphere using scale and pos
		RenderMesh(meshList[GEO_BALL], false);
		//Exercise 11: think of a way to give balls different colors

		break;
	}
	modelStack.PopMatrix();
}

void SceneCollision::Render()
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

		RenderMesh(meshList[GEO_AXES], false);

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
	ss << mousePos;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, 6);
	ss.str("");
	//Exercise 5: Render m_objectCount
	ss << "Entities:" << m_objectCount << "/" << m_goList.size();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, 4);
	//Exercise 8c: Render initial and final momentum

	ss.str("");
	ss.precision(3); ss << "Speed:" << m_speed;
	ss.precision(5); ss << "FPS:" << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, 2);
	
	RenderTextOnScreen(meshList[GEO_TEXT], "Collision", Color(0, 1, 0), 2, 0, 0);
}

void SceneCollision::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if(m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}
}
