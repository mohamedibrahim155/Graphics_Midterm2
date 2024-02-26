#pragma once
#include"SinglePassEffect.h"
#include "../Textures.h"
class StaticEffect : public SinglePassEffect
{
public:
	StaticEffect();
	StaticEffect(float width, float height);


	// Inherited via SinglePassEffect
	void InitializeEffect(const std::string& vertex, const std::string& fragment) override;
	void SetShaderUniforms() override;

	void DrawProperties() override;
	void SceneDraw() override;

	FrameBuffer* staticEffectBuffer;

	BaseTexture* staticTexture;
};

