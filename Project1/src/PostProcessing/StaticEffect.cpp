#include "StaticEffect.h"
#include "../Time.h"
#include "../Math.h"

using namespace MathUtils;


StaticEffect::StaticEffect()
{
	name = "Static";

	InitializeEffect("Shaders/PostProcessing/Pixelization.vert", "Shaders/PostProcessing/Pixelization.frag");
}

StaticEffect::StaticEffect(float width, float height)
{
	name = "Static";
	//std::string path = "../../Models/Spacestation/StaticTexture.png";
	//staticTexture = new Texture(path);
	framebufferWidthAndHeight.x = width;
	framebufferWidthAndHeight.y = height;

	InitializeEffect("Shaders/PostProcessing/Static.vert", "Shaders/PostProcessing/Static.frag");
}

void StaticEffect::InitializeEffect(const std::string& vertex, const std::string& fragment)
{
	shader = new Shader(vertex.c_str(), fragment.c_str());

	FrameBufferSpecification specification;

	specification.width = framebufferWidthAndHeight.x;
	specification.height = framebufferWidthAndHeight.y;

	specification.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::DEPTH };

	singlepassFramebuffer = new FrameBuffer(specification);
}

void StaticEffect::SetShaderUniforms()
{

	shader->setInt("sceneTexture", 0);  // Scene texture from framebuffer
	shader->setFloat("time", time);
	shader->setFloat("isEffectActive", isActive);

	RunTimer();
}

void StaticEffect::DrawProperties()
{
	SinglePassEffect::DrawProperties();

	if (isEnabled)
	{
		if (!ImGui::TreeNodeEx("properties", ImGuiTreeNodeFlags_DefaultOpen))
		{
			return;
		}

		ImGui::Checkbox("Is Active", &isActive);

		ImGui::TreePop();
	}
}

void StaticEffect::SceneDraw()
{
}

void StaticEffect::OnActiveState()
{
	isActive = !isActive;
	interval = Math::GetRandomIntNumber(2, 7);


}

void StaticEffect::RunTimer()
{

	if (timer >= interval)
	{
		timer = 0;
		OnActiveState();
	}
	else
	{
		timer += Time::GetInstance().deltaTime;
	}
}
