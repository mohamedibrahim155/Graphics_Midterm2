#include "StaticEffect.h"

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

	//staticTexture->SetTextureSlot(1);
	//shader->setInt("staticTexture", 1);
	//staticTexture->Bind();

	
	//GLCALL(glBindTexture(GL_TEXTURE_2D, frameBuffer->GetColorAttachmentID()));
	

	//SetShaderUniforms();

	
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

		//ImGui::Checkbox("IsFlickering", &isFlicker);

		ImGui::TreePop();
	}
}

void StaticEffect::SceneDraw()
{
}
