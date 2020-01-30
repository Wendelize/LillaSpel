#include "Header Files/Platform.h"


Platform::Platform()
{
	m_transform = nullptr;
}

Platform::~Platform()
{
	delete m_transform;
}

//ObjectInfo* Platform::GetObjectInfo()
//{
//	//ObjectInfo* temp = new ObjectInfo();
//	/*temp->modelId = m_modelId;
//	temp->modelMatrix = m_transform->GetMatrix();
//	return temp;*/
//}