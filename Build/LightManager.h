#pragma once
#include "Coreminimal.h"

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

};

