#pragma once

#include "Coreminimal.h"
#include "component.h"
#include "ShaderSet.h"

typedef enum
{
	TagNone,
	TagPlayer,
	TagEnemy,
	TagField,
	TagPlayerBullet,
	TagPlayerMissile,
	TagEnemyMissile,

	ObjectTagMax,
}ObjectTag;

typedef enum
{	
	Default,
	Sky,
	Field,
	Water,
	UI,
	Player,

	LayerMax,
}
Layer;

class ColliderComponent;
class Scene;
class TransformComponent;
class Component;
class MeshData;

class GameObject
{
public:
	GameObject();
	GameObject(Scene* scene);
	GameObject(GameObject* parent);
	~GameObject();

	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void UpdateMatrix(void);
	virtual void Draw(ShaderSet::ShaderIndex index);
	virtual void ShadowMapping(void);
	
	void InitAllComponentAndChild(void);

	Scene* GetScene(void);
	TransformComponent* GetTransFormComponent(void);
	ColliderComponent* GetCollider(void);

	ObjectTag GetTag(void);
	Layer GetLayer(void);

	BOOL GetActive(void);
	void SetActive(BOOL isActive);

	GameObject* GetParent(void);
	GameObject* GetRootObject(void);

	GameObject* GetChild(int index);
	GameObject* GetChild(string name);
	vector<GameObject*>& GetChild();

	vector<Component*>& GetComponentList(void);

	//指定したアトリビュートを持つコンポーネントの中でn番目のコンポーネントのポインタを取得
	Component* GetComponentAttrbute(Component::Attribute attr, int n);

	template<class T>
	T* GetComponent(void);

	template<class T>
	T* AddComponent(void);


	void SetHasShadowAll(BOOL b);

	GameObject* AddChild(string name);


	string GetName(void);
	void SetName(string name);

	void LoadFbxFileMesh(string fName);

	void LoadMeshNode(MeshData* node);

protected:
	Scene* pScene;
	TransformComponent* transformComponent;
	ColliderComponent* collider;


	vector<Component*> componentList;

	string name;
	ObjectTag tag;
	Layer layer;

	

	BOOL isActive;

	GameObject* parent;
	vector <GameObject*> childList;

};



template<class T>
T* GameObject::GetComponent(void)
{
	for (Component* com : componentList) {
		T* buff = dynamic_cast<T*>(com);
		if (buff != nullptr)
			return buff;
	}
	return nullptr;

}

template<class T>
T* GameObject::AddComponent(void)
{
	T* com = new T(this);

	this->componentList.push_back(com);

	Component* c = dynamic_cast<Component*>(com);
	c->Init();
	return com;
}
