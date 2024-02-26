#include "Asteroid.h"
#include"../GraphicsRender.h"
Asteroid::Asteroid()
{
	name = "Asteroid";
}

Asteroid::Asteroid(int modelIndex) :modelIndex(modelIndex)
{
	name = "Asteroid";
	LoadModel("Models/Asteroids/" + modelPath[modelIndex]);
	transform.SetScale(glm::vec3(0.05f));
	GraphicsRender::GetInstance().AddModelAndShader(this, GraphicsRender::GetInstance().defaultShader);
}

Asteroid::~Asteroid()
{
}

void Asteroid::InitialiseAsteroid(int index)
{
	LoadModel("Models/Asteroids/" + modelPath[index]);
	transform.SetScale(glm::vec3(0.05f));
	GraphicsRender::GetInstance().AddModelAndShader(this, GraphicsRender::GetInstance().defaultShader);
}

void Asteroid::DrawProperties()
{
	Model::DrawProperties();
}

void Asteroid::SceneDraw()
{
	Model::SceneDraw();
}

void Asteroid::Start()
{
}

void Asteroid::Update(float deltaTime)
{
}

void Asteroid::Render()
{
}

void Asteroid::OnDestroy()
{
}
