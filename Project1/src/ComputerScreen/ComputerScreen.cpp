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


	blackTexure  =  new Texture(blackTexurePath);

	GraphicsRender::GetInstance().AddModelAndShader(this, GraphicsRender::GetInstance().renderTextureShader);
	SetTexture(blackTexure);
	InputManager::GetInstance().AddObserver(this);
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
	interval = Math::GetRandomIntNumber(2, 7);
	if (keyPressed)
	{
		SetTexture(listOfCameraRenderTexure[random]);

	}
	else
	{
		SetTexture(blackTexure);
	}
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

void ComputerScreen::OnKeyPressed(const int& key)
{
	if (key == GLFW_KEY_2)
	{
		keyPressed = !keyPressed;
		OnTextureChange();
	}
}

void ComputerScreen::OnKeyReleased(const int& key)
{
}

void ComputerScreen::OnKeyHold(const int& key)
{
}

