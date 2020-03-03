#include "Header Files\Object.h"

Object::Object(btVector3 pos, int type, Model* model)
{
	m_transform = new Transform;

	m_btTransform = new btTransform();
	m_btTransform->setIdentity();
	m_btTransform->setOrigin(pos);
	m_transform->SetScale(1, 1, 1);

	btVector3 localInertia(0, 0, 0);
	float mass = 1000.f;
	//m_shape = new btBoxShape(btVector3(1.f, 1.f, 1.f));
	m_motionState = new btDefaultMotionState(*m_btTransform);
	vector<btVector3> points;
	vector<int> indicies;

	for (int i = 0; i < model->GetMeshes().size(); i++)
	{
		for (int j = 0; j < model->GetMeshes()[i].m_vertices.size(); j++)
		{
			points.push_back(btVector3(model->GetMeshes()[i].m_vertices[j].pos.x, model->GetMeshes()[i].m_vertices[j].pos.y, model->GetMeshes()[i].m_vertices[j].pos.z));
		}
	}

	m_physicsMesh = new btConvexHullShape(&points[0].getX(), points.size(), sizeof(btVector3));


	m_physicsMesh->calculateLocalInertia(mass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_motionState, m_physicsMesh, localInertia);
	m_body = new btRigidBody(rbInfo);

	vec3 btPos = vec3(m_btTransform->getOrigin().x(), m_btTransform->getOrigin().y(), m_btTransform->getOrigin().z());
	m_transform->SetTranslation(btPos);
	m_color = vec3(1);
	m_model = type;
	m_currentPos = m_btTransform->getOrigin();

}

Object::~Object()
{
	delete m_btTransform;
	delete m_transform;
	delete m_physicsMesh;
}

ObjectInfo* Object::GetObjectInfo()
{
	btScalar tempScalar[16];
	m_body->getWorldTransform().getOpenGLMatrix(tempScalar);
	//m_btTransform->getOpenGLMatrix(tempScalar);

	mat4 temp(tempScalar[0], tempScalar[1], tempScalar[2], tempScalar[3],
		tempScalar[4], tempScalar[5], tempScalar[6], tempScalar[7]
	, tempScalar[8], tempScalar[9], tempScalar[10], tempScalar[11]
	, tempScalar[12], tempScalar[13], tempScalar[14], tempScalar[15]);
	m_info = new ObjectInfo(temp, m_model, 3, m_color, true);
	return m_info;
}

btRigidBody* Object::getObject()
{
	return m_body;
}

void Object::update()
{
	btVector3 moveVector = m_body->getWorldTransform().getOrigin() - m_currentPos;
	m_transform->Translate(vec3(moveVector.x(), moveVector.y(), moveVector.z()));
	m_currentPos = m_body->getWorldTransform().getOrigin();

}
