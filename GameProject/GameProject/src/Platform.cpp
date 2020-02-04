#include "Header Files/Platform.h"


Platform::Platform()
{
	m_transform = new Transform();
	m_transform->SetScale(0.4f, 0.4f, 0.4f);
	m_transform->Translate(vec3(0.f, -2.f, 3.7));
	m_info = nullptr;
	m_modelId = 0;
	m_skyboxId = 0;



	m_platformShape = new btBoxShape(btVector3(btScalar(20.), btScalar(2.), btScalar(20.)));
	m_btTransform = new btTransform;
	m_btTransform->setIdentity();
	m_btTransform->getBasis();
	btMatrix3x3 temp;

	temp.scaled(btVector3(0.4f,0.4f,0.4f));
	m_btTransform->setOrigin(btVector3(0.f, -2.f, 3.7f));
	btScalar mass(0.);
	btVector3 localInertia(0, 0, 0);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(*m_btTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, m_platformShape, localInertia);
	m_body = new btRigidBody(rbInfo);
}

Platform::~Platform()
{
	delete m_transform;
}

int Platform::GetModelId()
{
	return m_modelId;
}

btRigidBody* Platform::getBody()
{
	return m_body;
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