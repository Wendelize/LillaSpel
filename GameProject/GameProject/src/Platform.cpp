#include "Header Files/Platform.h"


Platform::Platform(Model* model, int platformIndex)
{
	m_transform = new Transform();
	m_transform->SetScale(0.4f, 0.4f, 0.4f);
	m_transform->Translate(vec3(0.f, -0.f, 0.f));
	m_info = nullptr;
	m_modelId = 0;
	m_skyboxId = 0;

	vector<btVector3> points;

	for (int i = 0; i < model->GetMeshes()[platformIndex].m_vertices.size(); i++)
	{
		points.push_back(btVector3(model->GetMeshes()[platformIndex].m_vertices[i].pos.x, model->GetMeshes()[platformIndex].m_vertices[i].pos.y, model->GetMeshes()[platformIndex].m_vertices[i].pos.z));
	}

	m_platformShape = new btConvexHullShape(&points[0].getX(), points.size(), sizeof(btVector3)); // vertexdata, numberofvertices, stride
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