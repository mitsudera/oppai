#pragma once
#include "Coreminimal.h"
#include "renderer.h"

class LightComponent;
class GameEngine;



class LightManager
{

public:


    LightManager(GameEngine* pGameEngine);
    ~LightManager();

    void Init();
    void Update();
    void Draw();
    void Uninit();

private:
    GameEngine* pGameEngine;

    LightComponent** activeLight;

    vector<LightComponent*> lightList;

};

