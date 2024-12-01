#pragma once
#include "Coreminimal.h"
#include "CBufferManager.h"

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



    void SetMainLight(LightComponent* lightComponent);

private:
    GameEngine* pGameEngine;

    LIGHT_CBUFFER lightCBufferStruct;

    int activeLightIndex[MAX_LIGHT];

    vector<LightComponent*> lightList;

    ID3D11Buffer* lightBuffer;

    void SetLight(LightComponent* lightComponent, int index);

};

