#pragma once
#include "level.h"

class Player;
class Title :public Level
{
public:
	Title(Main* main);
	~Title();

	// Level ����Čp������܂���
	virtual void Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	virtual void DrawGameObject(void) override;

	virtual void DrawShadowObject(void) override;
private:
	


};

