#pragma once
#include "SinglePassEffect.h"
#include "../Math.h"
class FlickerEffect : public SinglePassEffect
{
public:

	FlickerEffect();
	FlickerEffect(float width, float height);




	// Inherited via SinglePassEffect
	void InitializeEffect(const std::string& vertex, const std::string& fragment) override;

	void SetShaderUniforms() override;

	void DrawProperties() override;
	void SceneDraw() override;

	FrameBuffer* flickerFrameBuffer;

private:

	bool isFlicker = true;

	float timer = 0;
	float timeInterval = 2;
	float uvY = 0.7f;

	int randomSide = 0;
	void OnStateChange();
	void OnTimerUpdate();
};

