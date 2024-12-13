#pragma once
#include "component.h"

class AssetsManager;
class MeshData;
class MtxNode;

class AnimationNode
{
public:
	AnimationNode(int animIndex,string name);
	~AnimationNode();

	int GetAnimIndex(void);
	string GetName(void);


private:


	string name;
	int animDataIndex;
};

//����̂̓��b�V���R���|�[�l���g�̃��[�g
class AnimationControlerComponent : public Component
{
public:

	enum class ANIM_MODE:int
	{
		NO_ANIM,
		DATA_ANIM,
		BLEND_ANIM,
		CROSSFADE_ANIM, // �N���X�t�F�[�h��Ԃ��s�����[�h
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

	int LoadAnimationData(string fileName, string name);

	void UpdateAnimation(MtxNode* node, GameObject* gameObject);

	void SetAnimation(int index);//�ԍ��ŃZ�b�g
	void SetAnimation(string name);//���O�ŒT���ăZ�b�g�������̕������S

	int GetDefaultAnimIndex(void);
	void SetDefaultAnimIndex(int n);


	void SetAnimationSpeed(float speed);
	float GetAnimationSpeed(void);



protected:
	
	AssetsManager* pAssetsManager;

	vector<int> animIndexArray;
	vector<string> animNameArray;
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








};

