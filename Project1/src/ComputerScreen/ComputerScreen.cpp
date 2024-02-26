#include "ComputerScreen.h"
#include"../GraphicsRender.h"

using namespace MathUtils;

ComputerScreen::ComputerScreen()
{
}
ComputerScreen::ComputerScreen(int index)
{
	name = "ComputerScreen ";

	LoadModel("Models/Spacestation/" + modelPath[index]);

	GraphicsRender::GetInstance().AddModelAndShader(this, GraphicsRender::GetInstance().renderTextureShader);
}

void ComputerScreen::DrawProperties()
{
	Model::DrawProperties();
}

void ComputerScreen::SceneDraw()
{
	Model::SceneDraw();
}

void ComputerScreen::Start()
{
}

void ComputerScreen::Update(float deltaTime)
{
	RunTimer(deltaTime);
}

void ComputerScreen::Render()
{
}

void ComputerScreen::OnDestroy()
{
}

void ComputerScreen::SetTexture(BaseTexture* texture)
{
	meshes[0]->meshMaterial->material()->diffuseTexture = texture;
}

void ComputerScreen::SetRenderTextures(const std::vector<BaseTexture*>& textures)
{
	listOfCameraRenderTexure = textures;
}

void ComputerScreen::OnTextureChange()
{
	int random = Math::GetRandomIntNumber(0, listOfCameraRenderTexure.size() - 1);

	SetTexture(listOfCameraRenderTexure[random]);
}

void ComputerScreen::RunTimer(float deltaTime)
{
	if (timer >= interval)
	{
		timer = 0;
		OnTextureChange();
	}
	else
	{
		timer += deltaTime;
	}
}

