#include "Component.h"



Component::Component(GameObject* my_go):enable(true)
{
	SetMyGO(my_go);
}


Component::~Component()
{
}

void Component::Enable()
{
	enable = true;

}
void Component::CleanUp()
{
}

void Component::Update()
{
}

void Component::GuiUpdate()
{
}

void Component::InspectorUpdate()
{
}



bool Component::SaveComponent(JSONConfig& data)const
{
	return true;
}


void Component::Load()
{
}

void Component::Disable()
{
	enable = false;
}

bool Component::isEnable()
{
	return enable==true;
}

void Component::SetMyGO(GameObject * get)
{
	my_go = get;
}
