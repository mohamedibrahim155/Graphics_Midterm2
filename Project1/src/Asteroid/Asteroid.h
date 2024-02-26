#pragma once
#include"../model.h"
class Asteroid  : public Model
{
public:
    Asteroid(int modelIndex);
    Asteroid();
    ~Asteroid();

 
    void InitialiseAsteroid(int index);

    void DrawProperties() override;
    void SceneDraw() override;

    void Start() override;
    void Update(float deltaTime) override;
    void Render() override;
    void OnDestroy()override;
private:
    int modelIndex = 0;
    std::vector< std::string> modelPath = { "asteroid_industrial_xyz_n_rgba_uv.ply","asteroid_main_xyz_n_rgba_uv.ply",
        "asteroid_other_xyz_n_rgba_uv.ply", "stuff_main_asteroid_xyz_n_rgba_uv.ply" };
};

