#include "PowerUp.h"

PowerUp::PowerUp(int spawn, btVector3 pos, int type,float duration)
{
	m_pos = pos;
	m_type = 6;
	m_duration = duration;
	m_transform = new Transform;
	m_model = 0;
	switch (6) {
		case 0: 
			m_model = 1;
			m_color = vec3(0, 1, 0);
			break;
		case 1: 
			m_color = vec3(1, 0, 0);
			break;
		case 2: 
			m_color = vec3(0.5, 0, 1);
			break;
		case 3: 
			m_model = 1;
			m_color = vec3(1, 0.5, 0);
			break;
		case 4:
			m_model = 1;
			m_color = vec3(1, 0, 0);
			break;
		case 5: 
			m_model = 1;
			m_color = vec3(0, 1, 0);
			break;
	}
	m_spawn = spawn;
	float scale = 1.;
	m_btTransform = new btTransform();
	m_btTransform->setIdentity();
	m_btTransform->setOrigin(m_pos);
	m_transform->SetScale(scale, scale, scale);

	btVector3 localInertia(0, 0, 0);
	float mass = 1.f;
	m_shape = new btBoxShape(btVector3(0.75f * scale, 0.75f * scale, 0.75f * scale));
	m_motionState = new btDefaultMotionState(*m_btTransform);

	m_shape->calculateLocalInertia(mass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_motionState, m_shape, localInertia);

	m_body = new btRigidBody(rbInfo);
	m_body->setCollisionShape(m_shape);
	m_body->setWorldTransform(*m_btTransform);
	m_body->setLinearVelocity(btVector3(0.0f, -1.0f, 0.0f));

	vec3 btPos = vec3(m_btTransform->getOrigin().x(), m_btTransform->getOrigin().y(), m_btTransform->getOrigin().z());
	m_transform->SetTranslation(btPos);
	m_currentPos = m_body->getWorldTransform().getOrigin();
	
}

PowerUp::~PowerUp()
{
	delete m_btTransform;
	delete m_transform;
	delete m_shape;
}

void PowerUp::SetPos(btVector3 pos)
{
	m_pos = pos;
}

btVector3 PowerUp::GetPos()
{
	return m_currentPos;
}

void PowerUp::SetType(int type)
{
	m_type = type;
}

int PowerUp::GetType()
{
	return m_type;
}

void PowerUp::SetDuration(float duration)
{
	m_duration = duration;
}

float PowerUp::GetDuration()
{
	return m_duration;
}

btRigidBody* PowerUp::getObject()
{
	return m_body;
}

ObjectInfo* PowerUp::GetObjectInfo()
{
	m_info = new ObjectInfo(m_transform->GetMatrix(), m_model, 2, m_color, true);
	return m_info;
}

bool PowerUp::update(float dt)
{
	bool destroy = true;
	if (GetDuration() - dt > 0) {
		SetDuration(GetDuration() - dt);
		destroy = false;
	}
	//m_body->setLinearVelocity(btVector3(0,-1,0));
	btVector3 moveVector = m_body->getWorldTransform().getOrigin() - m_currentPos;
	m_transform->Translate(vec3(moveVector.x(), moveVector.y(), moveVector.z()));
	m_currentPos = m_body->getWorldTransform().getOrigin();

	return destroy;
}

int PowerUp::GetSpawn()
{
	return m_spawn;
}

