#include "RenderQuad.h"
#include"../GraphicsRender.h"

using namespace MathUtils;

RenderQuad::RenderQuad()
{
	name = "Quad 1";

	LoadModel("Models/DefaultQuad/DefaultQuad.fbx");

	GraphicsRender::GetInstance().AddModelAndShader(this, GraphicsRender::GetInstance().defaultShader);
}

void RenderQuad::DrawProperties()
{
	Model::DrawProperties();
}

void RenderQuad::SceneDraw()
{
	Model::SceneDraw();
}

void RenderQuad::Start()
{
}

void RenderQuad::Update(float deltaTime)
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

void RenderQuad::Render()
{
}

void RenderQuad::OnDestroy()
{
}

void RenderQuad::SetTexture(BaseTexture* texture)
{
	meshes[0]->meshMaterial->material()->diffuseTexture = texture;
}

void RenderQuad::SetChangeTextures(const std::vector<BaseTexture*>& textures)
{
	monitorTexures = textures;
}

void RenderQuad::OnTextureChange()
{
	int random = Math::GetRandomIntNumber(0, monitorTexures.size() - 1);
	interval = Math::GetRandomIntNumber(2, 7);

	SetTexture(monitorTexures[random]);
}
