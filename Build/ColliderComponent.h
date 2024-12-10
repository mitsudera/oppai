#pragma once
#include "transformcomponent.h"
#include "Coreminimal.h"
#include "gameobject.h"



struct HitResult
{
	BOOL isHit[ObjectTag::ObjectTagMax];
	vector<GameObject*> hitObject;

};




class ColliderComponent :public Component
{
public:

	enum class Shape: int
	{

		Point,
		Line,
		Sphere,
		Capsule,
		Box,

		//ñ¢é¿ëï
		AABB,
		CBB,

	};



	ColliderComponent();
	ColliderComponent(GameObject* gameObject);
	~ColliderComponent();


	virtual void Init(void) override;

	virtual void Uninit(void) override;

	virtual void Update(void) override;

	BOOL GetHitTag(ObjectTag tag);

	void SetHitTag(ObjectTag tag, BOOL isHit);

	Shape GetShape(void);
	void SetShape(Shape shape);

	ObjectTag GetTag(void);


	void SetHitObject(GameObject* gameObject);
	BOOL GetHitObject(GameObject* gameObject);



	void onCollider(void);
	void offCollider(void);
	void Clear(void);

	XMFLOAT3 GetCenter(void);
	float GetCheckRadius(void);


protected:
	HitResult result;
	Shape shape;

	XMFLOAT3 center;//íÜêS
	float checkRadius;//ëÂÇ‹Ç©Ç»ìñÇΩÇËîªíËÇçsÇ§ÇΩÇﬂÇÃîºåa

};

