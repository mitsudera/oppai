#include "gameobject.h"
#include "GameEngine.h"
#include "transformcomponent.h"
#include "Scene.h"
#include "ColliderComponent.h"
#include "component.h"
#include "primitivecomponent.h"
#include "AssetsManager.h"
#include "MeshData.h"
#include "MeshComponent.h"
#include "Material.h"

GameObject::GameObject()
{
	pScene =nullptr;
	this->transformComponent = new TransformComponent(this);

	tag = ObjectTag::TagNone;

}


GameObject::GameObject(Scene* scene)
{
	this->parent = nullptr;
	this->pScene = scene;
	tag = ObjectTag::TagNone;

	this->layer = Layer::Default;

}

GameObject::GameObject(GameObject* parent)
{
	this->parent = parent;
	this->pScene = parent->GetScene();
	this->tag = parent->GetTag();

	this->layer = parent->GetLayer();
}

GameObject::~GameObject()
{
	delete this->transformComponent;
}

void GameObject::Init(void)
{
	this->name = "notname";
	this->transformComponent = new TransformComponent(this);
	this->transformComponent->Init();
	componentList.push_back(transformComponent);
	this->collider = nullptr;
	this->isActive = TRUE;

}

void GameObject::Uninit(void)
{
	for (int i = 0; i < this->componentList.size(); i++)
	{
		componentList[i]->Uninit();
	}
	for (int i = 0; i < childList.size(); i++)
	{
		childList[i]->Uninit();
	}

}

void GameObject::Update(void)
{
	if (!isActive)
		return;

	for (Component* com:componentList)
	{
		if (!com->GetActive())
			continue;

		com->Update();
	}
	for (GameObject* gameObject:childList)
	{
		if (!gameObject->GetActive())
			continue;

		gameObject->Update();
	}

}

void GameObject::UpdateMatrix(void)
{
	if (!isActive)
		return;


	for (Component* com : componentList)
	{
		if (!com->GetActive())
			continue;

		TransformComponent* transformComponent = dynamic_cast<TransformComponent*>(com);

		if (transformComponent == nullptr)
			continue;

		transformComponent->UpdateMatrix();

	}
	for (GameObject* gameObject : childList)
	{
		if (!gameObject->GetActive())
			continue;

		gameObject->UpdateMatrix();
	}

}

void GameObject::Draw(ShaderSet::ShaderIndex index)
{
	if (!isActive)
		return;
	this;

	for (Component* component : GetComponentList())
	{
		if (component->GetAttribute() != Component::Attribute::Primitive)
			continue;

		PrimitiveComponent* primitiveComponent = static_cast<PrimitiveComponent*>(component);

		//現在セットしてるシェーダーを使っている場合描画
		if (pScene->GetGameEngine()->GetAssetsManager()->GetMaterial(primitiveComponent->GetMaterialIndex())->GetShaderSet()->GetShaderIndex() != index)
			continue;

		primitiveComponent->Draw();

	}
	for (GameObject* child:childList)
	{
		if (!child->GetActive())
			return;
		child->Draw(index);
	}

}


Scene* GameObject::GetScene(void)
{

	return this->pScene;
}

TransformComponent* GameObject::GetTransFormComponent(void)
{
	return this->transformComponent;
}

ColliderComponent* GameObject::GetCollider(void)
{
	return this->collider;
}

ObjectTag GameObject::GetTag(void)
{
	return tag;
}

Layer GameObject::GetLayer(void)
{
	return this->layer;
}

BOOL GameObject::GetActive(void)
{
	return isActive;
}

void GameObject::SetActive(BOOL isActive)
{
	this->isActive = isActive;
}

GameObject* GameObject::GetParent(void)
{
	return this->parent;
}

GameObject* GameObject::GetRootObject(void)
{
	if (this->parent == nullptr)
	{
		return this;

	}
	else
	{
		return this->parent->GetRootObject();
	}

}

GameObject* GameObject::GetChild(int index)
{
	return this->childList[index];
}

vector<GameObject*>& GameObject::GetChild()
{
	return this->childList;
}

vector<Component*>& GameObject::GetComponentList(void)
{
	return this->componentList;
}

Component* GameObject::GetComponentAttrbute(Component::Attribute attr, int n)
{
	int cnt = 0;

	for (int i = 0; i < componentList.size(); i++)
	{
		if (componentList[i]->GetAttribute()==attr)
		{
			if (cnt==n)
			{
				return componentList[i];

			}
			else
			{
				cnt++;
			}
		}
	}
	return nullptr;
}

string GameObject::GetName(void)
{
	return this->name;
}

void GameObject::SetName(string name)
{
	this->name = name;
}

void GameObject::LoadFbxFileMesh(string fName)
{
	int treeIndex = pScene->GetGameEngine()->GetAssetsManager()->LoadMeshNode(fName);

	MeshData* root = pScene->GetGameEngine()->GetAssetsManager()->GetMeshTree(treeIndex);


	LoadMeshNode(root);

}

void GameObject::LoadMeshNode(MeshData* node)
{
	if (!node->GetIsRoot())
	{
		MeshComponent* mesh = this->AddComponent<MeshComponent>();
		mesh->Init();
		mesh->SetMeshDataIndex(node->GetIndex());

	}


	for (MeshData* childData: node->GetChild())
	{
		GameObject* newObj = new GameObject(this);
		newObj->Init();
		newObj->name = childData->GetName();
		newObj->LoadMeshNode(childData);
		this->childList.push_back(newObj);

	}

}






