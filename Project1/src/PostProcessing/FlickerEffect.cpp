#include "FlickerEffect.h"
#include "../Time.h"
using namespace MathUtils;

FlickerEffect::FlickerEffect()
{
	name = "Flicker";

	InitializeEffect("Shaders/PostProcessing/Flicker.vert", "Shaders/PostProcessing/Flicker.frag");

}

FlickerEffect::FlickerEffect(float width, float height)
{

	name = "Flicker";


	framebufferWidthAndHeight.x = width;
	framebufferWidthAndHeight.y = height;


	InitializeEffect("Shaders/PostProcessing/Flicker.vert", "Shaders/PostProcessing/Flicker.frag");

}

void FlickerEffect::InitializeEffect(const std::string& vertex, const std::string& fragment)
{
	shader = new Shader(vertex.c_str(), fragment.c_str());

	FrameBufferSpecification specification;

	specification.width = framebufferWidthAndHeight.x;
	specification.height = framebufferWidthAndHeight.y;

	specification.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::DEPTH };

	singlepassFramebuffer = new FrameBuffer(specification);
}

void FlickerEffect::SetShaderUniforms()
{

	shader->setInt("sceneTexture", 0);  // Scene texture from framebuffer
	shader->setFloat("time", time);
	shader->setBool("isFlicker", isFlicker);
	shader->setInt("randomNumber", randomSide);

	OnTimerUpdate();
}

void FlickerEffect::DrawProperties()
{
	SinglePassEffect::DrawProperties();

	if (isEnabled)
	{
		if (!ImGui::TreeNodeEx("properties", ImGuiTreeNodeFlags_DefaultOpen))
		{
			return;
		}

		ImGui::Checkbox("IsFlickering", &isFlicker);

		ImGui::TreePop();
	}
}

void FlickerEffect::SceneDraw()
{
}

void FlickerEffect::OnStateChange()
{
	isFlicker = !isFlicker;

	timeInterval = Math::GetRandomFloatNumber(0.1f, 1);
	randomSide = Math::GetRandomIntNumber(-1, 1);

	
}

void FlickerEffect::OnTimerUpdate()
{
	if (timer >= timeInterval)
	{
		OnStateChange();
		timer = 0;

		uvY = 0.7f;
	}
	else
	{
		timer += Time::GetInstance().deltaTime;
		
	}
}
