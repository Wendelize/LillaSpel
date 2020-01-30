#include "Header Files/Platform.h"


Platform::Platform()
{
	m_transform = nullptr;
}

Platform::~Platform()
{
	delete m_transform;
}

ObjectInfo* Platform::GetObjectInfo()
{
	return new ObjectInfo(m_transform->GetMatrix(), m_modelId, 1);
}