#pragma once
#include "Coreminimal.h"
#include "renderer.h"

class LightComponent;
class GameEngine;



class LightManager
{

public:
    LightManager(GameEngine* gameEngine);
    ~LightManager();

    void Init();
    void Update();
    void Draw();
    void Uninit();

private:
    GameEngine* gameEngine;

    LightComponent** activeLight;

    vector<LightComponent*> lightList;

};

