#include "PowerUp.h"

PowerUp::PowerUp(int spawn, btVector3 pos, int type,float duration)
{
	m_pos = pos;
	m_type = type;
	m_duration = duration;
	m_transform = new Transform;
	switch (type) {
		case 0: 
			m_color = vec3(1, 0, 0);
			break;
		case 1: 
			m_color = vec3(0, 1, 0);
			break;
		case 2: 
			m_color = vec3(0, 0, 1);
			break;
		case 3: 
			m_color = vec3(1, 1, 0);
			break;
		case 4:
			m_color = vec3(0, 1, 1);
			break;
		case 5: 
			m_color = vec3(1, 0, 1);
			break;
	}
	m_spawn = spawn;
	float scale = 1.;
	m_btTransform = new btTransform();
	m_btTransform->setIdentity();
	m_btTransform->setOrigin(m_pos);
	m_transform->SetScale(scale, scale, scale);

	btVector3 localInertia(0, 0, 0);

	m_body = new btGhostObject();
	m_shape = new btBoxShape(btVector3(0.75f * scale, 0.75f * scale, 0.75f * scale));
	m_body->setCollisionShape(m_shape);
	m_body->setWorldTransform(*m_btTransform);
	vec3 testPos = vec3(m_btTransform->getOrigin().x(), m_btTransform->getOrigin().y(), m_btTransform->getOrigin().z());
	m_transform->SetTranslation(testPos);
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
	return m_pos;
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

btGhostObject* PowerUp::getObject()
{
	return m_body;
}

ObjectInfo* PowerUp::GetObjectInfo()
{
	m_info = new ObjectInfo(m_transform->GetMatrix(), 0, 2, m_color);
	return m_info;
}
bool PowerUp::update(float dt)
{
	bool destroy = true;
	if (GetDuration() - dt > 0) {
		SetDuration(GetDuration() - dt);
		destroy = false;
	}
	return destroy;
}

int PowerUp::GetSpawn()
{
	return m_spawn;
}

