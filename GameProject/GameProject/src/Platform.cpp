#include "Header Files/Platform.h"


Platform::Platform(Model* model, int platformIndex)
{
	m_transform = new Transform();
	m_transform->SetScale(1.f, 1.f, 1.f);
	m_transform->Translate(vec3(0.f, -0.f, 0.f));
	m_info = nullptr;
	m_modelId = 0;
	m_skyboxId = 0;

	vector<btVector3> points;

	for (int i = 0; i < model->GetMeshes().size(); i++)
	{
		for (int j = 0; j < model->GetMeshes()[i].m_vertices.size(); j++)
		{
			points.push_back(btVector3(model->GetMeshes()[i].m_vertices[j].pos.x, model->GetMeshes()[i].m_vertices[j].pos.y, model->GetMeshes()[i].m_vertices[j].pos.z));
		}
	}
	
	m_platformShape = new btConvexHullShape(&points[0].getX(), points.size(), sizeof(btVector3)); // vertexdata, numberofvertices, stride
	m_btTransform = new btTransform;
	m_btTransform->setIdentity();
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
	m_info = new ObjectInfo(m_transform->GetMatrix(), m_modelId, 1, vec3(1, 1, 1));
	return m_info;
}