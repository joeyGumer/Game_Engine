#include "GameObject.h"



GameObject::GameObject()
{
}

GameObject::GameObject(GameObject * parent)
{
}


GameObject::~GameObject()
{
}

void GameObject::Update()
{
	for (int i = 0; i < components.size(); i++)
	{
		Component* item = components[i];
		if(item->isEnable())
			item->Update();
	}
}

void GameObject::GuiUpdate()
{
	for (int i = 0; i < components.size(); i++)
	{
		Component* item = components[i];
			item->GuiDraw();
	}
}

void GameObject::SetParent(GameObject * set_parent)
{
	if (set_parent == nullptr)
	{
		LOG("ERROR parent null")
		return;
	}
	parent = set_parent;
	set_parent->SetChild(this);
}

void GameObject::SetChild(GameObject * child)
{
	if (child == nullptr)
	{
		LOG("ERROR child null");
		return;
	}
	childs.push_back(child);
}

void GameObject::AddComponent(Component * component_to_add)
{
	components.push_back(component_to_add);
}

Component * GameObject::CreateComponent(ComponentType type)
{
	Component* item = nullptr;
	switch (type)
	{
	case TRANSFORM:
		item = new ComponentTransform(this);
		break;
	case MESH:
		item = new ComponentMesh(this);

		break;
	case MESH_RENDER:
		item = new ComponentMeshRenderer(this);
		break;
	default:
		break;
	}
	return item;
}

Component* GameObject::FindComponent(ComponentType type, Component * component_to_find)
{
	Component* ret = nullptr;
	for (int i = 0; i < components.size(); i++)
	{
		Component* item = components[i];

		if (item->type == type)
		{
			if (item == component_to_find)
			{
				ret = item;
				return item;
			}
		}
	}
	return ret;
}

Component* GameObject::FindComponent(ComponentType type)
{
	Component* ret = nullptr;
	for (int i = 0; i < components.size(); i++)
	{
		Component* item = components[i];

		if (item->type == type)
		{
			ret = item;
			return item;
		}
	}
	return ret;
}
