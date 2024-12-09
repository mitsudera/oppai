#pragma once
#include "component.h"

class AssetsManager;
class MeshData;

class AnimationNode
{
public:
	AnimationNode();
	~AnimationNode();

private:

	AnimationNode* transferNode;

};

//つけるのはメッシュコンポーネントのルート
class AnimationControlerComponent : public Component
{
public:

	enum class ANIM_MODE:int
	{
		NO_ANIM,
		DATA_ANIM,
		BLEND_ANIM,
		CROSSFADE_ANIM, // クロスフェード補間を行うモード
	};
	enum class ANIM_STATE:int
	{
		STOP,
		TRANSFER,
		ANIMATION,
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

	void CreateAnimationNode(string name);

protected:
	
	AssetsManager* pAssetsManager;

	vector<int> AnimDataIndexArray;

	int animindex;
	int lastanimindex;
	float framecnt;
	int framenum;
	float blendcnt;
	int blendcntmax;
	ANIM_MODE animMode;
	BOOL motionblend;
	BOOL animation;

	float animSpeed;

	BOOL frameBlendMode;

	int defaultAnimIndex;



	vector<AnimationNode*>  animNodeArray;





};

