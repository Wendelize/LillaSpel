#include "Header Files\Object.h"

Object::Object(btVector3 pos, int type, Model* model)
{
	m_transform = new Transform;

	m_btTransform = new btTransform();
	m_btTransform->setIdentity();
	m_btTransform->setOrigin(pos);
	m_transform->SetScale(1, 1, 1);
	m_scale = 1.0f;
	btVector3 localInertia(0, 0, 0);
	float mass;

	if (type == 0) {
		mass = 100000.0f;
	}
	else {
		mass = 200.f;
	}
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

	m_color = vec3((rand() % 255)/255.f + 0.1, (rand() % 255) / 255.f + 0.1, (rand() % 255) / 255.f + 0.1);
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

	mat4 temp(tempScalar[0], tempScalar[1], tempScalar[2], tempScalar[3],
		tempScalar[4], tempScalar[5], tempScalar[6], tempScalar[7]
	, tempScalar[8], tempScalar[9], tempScalar[10], tempScalar[11]
	, tempScalar[12], tempScalar[13], tempScalar[14], tempScalar[15]);
	m_info = new ObjectInfo(scale(temp, vec3(m_scale)), m_model, 3, m_color, true);
	return m_info;
}

btRigidBody* Object::GetObject()
{
	return m_body;
}

void Object::Update()
{
	m_body->activate();
	if (m_body->getWorldTransform().getOrigin().y() < -10.f) {
		m_body->setLinearVelocity(btVector3(0, 0, 0));
		m_body->getWorldTransform().setOrigin(btVector3(0, 30, 0));
	}
	btVector3 moveVector = m_body->getWorldTransform().getOrigin() - m_currentPos;
	m_transform->Translate(vec3(moveVector.x(), moveVector.y(), moveVector.z()));
	m_currentPos = m_body->getWorldTransform().getOrigin();
}

void Object::SetScale(float scale)
{
	m_physicsMesh->setLocalScaling(btVector3(scale,scale,scale));
	m_scale = scale;
}

float Object::GetScale()
{
	return m_physicsMesh->getLocalScaling().x();
}

void Object::SetRotation(float degrees)
{
	btQuaternion quat = m_body->getWorldTransform().getRotation();
	btTransform trans = m_body->getWorldTransform();
	quat.setRotation(btVector3(0,1,0), quat.getAngle() + degrees);
	
	trans.setRotation(quat);

	m_body->setCenterOfMassTransform(trans);

}

void Object::Move(vec3 dir)
{
	m_body->setLinearVelocity(btVector3(dir.x,dir.y,dir.z));
}

vec3 Object::GetPos()
{
	return vec3(m_currentPos.x(), m_currentPos.y(), m_currentPos.z());
}

void Object::SetWay(bool way)
{
	m_way = way;
}

bool Object::GetWay()
{
	return m_way;
}

