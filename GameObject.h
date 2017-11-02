#pragma once
#include <vector>
#include <string>
#include "MathGeoLib-1.5\src\Math\float3.h"
#include "MathGeoLib-1.5\src\Math\Quat.h"
#include "MathGeoLib-1.5\src\Math\float4x4.h"
#include "MathGeoLib-1.5\src\Geometry\OBB.h"

class Component;
enum ComponentType;
class GameObject
{
public:
	GameObject(float3 = float3::one, Quat = Quat::identity, float3 = float3::zero);
	GameObject(GameObject* parent, float3 = float3::one,Quat = Quat::identity, float3 = float3::zero);

	~GameObject();
	void CleanUp();
	void Update();
	void GuiUpdate();
	void InspectorUpdate();


	void OpenStaticQuestion();


	void AddComponent(Component* component_to_add);
	void LoadGameObject(const JSONConfig& data);

	void SaveGameObject(JSONConfig& data)const;
	Component* CreateComponent(ComponentType type);
	Component* FindComponent(ComponentType type,Component* component_to_find)const;
	Component* FindComponent(ComponentType type)const;
	uint GetUID()const;
	uint GetParentUID()const;

	
	const char* GetName();
	//Setters
	void SetScale(float3 scale);
	void SetRotation(float3 rotation);
	void SetPosition(float3 Position);

	void SetStatic(bool set);
	void SetParent(GameObject * parent);
	void SetChild(GameObject * child);
	void SetName(const char* name);
	void SetTransform(math::float3 scale, math::Quat rotation, math::float3 position);
	
	//Update Matrix
	void UpdateMatrix();
	float4x4 GetTransposedMatrix() const;



public:
	std::vector<Component*> components;
	std::vector<GameObject*> childs;

	std::string name;
	std::string input_name;



	float3 scale=float3::one;
	Quat rotation=Quat::identity;
	float3 gui_rotation= float3::zero;
	float3 position = float3::zero;

	OBB global_bounding_box;

private:
	bool enable = true;
	bool static_go = false;
	bool gui_static = false;
	uint UID = 0;
	uint parent_UID = 0;

	float4x4 transform_matrix = float4x4::identity;
	
	//globals transforms
	float4x4 global_transform_matrix = float4x4::identity;
	float4x4 global_transform_matrix_transposed = float4x4::identity;

	


	GameObject* parent=nullptr;

};

