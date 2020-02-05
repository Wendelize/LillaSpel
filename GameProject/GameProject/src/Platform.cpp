#include "Header Files/Platform.h"


Platform::Platform()
{
	m_transform = new Transform();
	m_transform->SetScale(0.4f, 0.4f, 0.4f);
	m_transform->Translate(vec3(0.f, -0.f, 0.f));
	m_info = nullptr;
	m_modelId = 0;
	m_skyboxId = 0;



	m_platformShape = new btBoxShape(btVector3(btScalar(10.), btScalar(2.), btScalar(10.)));
	m_btTransform = new btTransform;
	m_btTransform->setIdentity();
	m_btTransform->getBasis();
	btMatrix3x3 temp;

	temp.scaled(btVector3(0.4f,0.4f,0.4f));
	m_btTransform->setOrigin(btVector3(0.f, -0.f, 0.f));
	btScalar mass(0.);
	btVector3 localInertia(0, 0, 0);

	m_motionState = new btDefaultMotionState(*m_btTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_motionState, m_platformShape, localInertia);
	m_body = new btRigidBody(rbInfo);

	m_body->setFriction(3);
}

Platform::~Platform()
{
	delete m_transform;
	delete m_btTransform;
	delete m_platformShape;
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