#include "Header Files/Platform.h"


Platform::Platform()
{
	m_transform = new Transform();
	m_transform->SetScale(0.2, 0.2, 0.2);
	m_transform->Translate(vec3(0, -1, 0));
}

Platform::~Platform()
{
	delete m_transform;
}

int Platform::GetModelId()
{
	return m_modelId;
}

void Platform::SetModelId(int id)
{
	m_modelId = id;
}

ObjectInfo* Platform::GetObjectInfo()
{
	m_info = new ObjectInfo(m_transform->GetMatrix(), m_modelId, 1, vec3(1, 0, 0));
	return m_info;
}