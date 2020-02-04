#include "Header Files/Platform.h"


Platform::Platform()
{
	m_transform = new Transform();
	m_transform->SetScale(0.4f, 0.4f, 0.4f);
	m_transform->Translate(vec3(0.f, -0.5f, 0));
	m_info = nullptr;
	m_modelId = 0;
	m_skyboxId = 0;
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

void Platform::SetSkyboxId(int id)
{
	m_skyboxId = id;
}

ObjectInfo* Platform::GetObjectInfo()
{
	m_info = new ObjectInfo(m_transform->GetMatrix(), m_modelId, 1, vec3(1, 0, 0));
	return m_info;
}