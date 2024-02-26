#pragma once
#include"../model.h"
#include "../Math.h"

class ComputerScreen : public Model
{
public:
    ComputerScreen();
    ComputerScreen(int index = 0);

   void DrawProperties() override;
   void SceneDraw()override ;

   void Start() override;
   void Update(float deltaTime) override;
   void Render() override;
   void OnDestroy() override;

   void SetTexture(BaseTexture* texture);
   void SetRenderTextures(const std::vector<BaseTexture*>& textures);

   void OnTextureChange();
   void RunTimer(float deltatime);
private:
    int index = 0;
   
    std::vector<std::string> modelPath = { "SM_Env_Consoles_01_screen_1_xyz_n_rgba_uv.ply",
        "SM_Env_Consoles_Corner_01_screen_1_xyz_n_rgba_uv" };

    std::vector<BaseTexture*> listOfCameraRenderTexure;

    float interval = 2;
    float timer = 0;
    
};

