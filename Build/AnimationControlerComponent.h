#pragma once
#include "component.h"

class AssetsManager;
class MeshData;

//つけるのはメッシュコンポーネントのルート
class AnimationControlerComponent : public Component
{
public:

	enum class ANIM_MODE:int
	{
		NO_ANIM,
		DATA_ANIM,
		BLEND_ANIM,
		CROSSFADE_ANIM, // クロスフェード補間を行うステート
	};


	AnimationControlerComponent(GameObject* gameObject);
	~AnimationControlerComponent();

	virtual void Init(void) override;
	virtual void Update(void) override;
	virtual void Uninit(void) override;


	void LoadAnimationData(string fileName);

	int GetDefaultAnimIndex(void);
	void SetDefaultAnimIndex(int n);


	void SetAnimationSpeed(float speed);
	float GetAnimationSpeed(void);


protected:
	
	AssetsManager* pAssetsManager;

	vector<int> AnimDataIndexArray;

	int animindex;
	int lastanimindex;
	float framecnt;
	int framenum;
	float blendcnt;
	int blendcntmax;
	ANIM_MODE animstate;
	BOOL motionblend;
	BOOL animation;

	float animSpeed;

	BOOL frameBlendMode;

	int defaultAnimIndex;







};

