#pragma once
#include "../model.h"
class RenderQuad : public Model
{
public:
	RenderQuad();


     void DrawProperties()override;
     void SceneDraw()override;

     void Start() override;
     void Update(float deltaTime) override;
     void Render()override;
     void OnDestroy()override;

     void SetTexture(BaseTexture* texture);
     void SetChangeTextures(const std::vector<BaseTexture*>& textures);
private:

    std::vector<BaseTexture*> monitorTexures;

    float interval = 2;
    float timer = 0;

    void OnTextureChange();
};

