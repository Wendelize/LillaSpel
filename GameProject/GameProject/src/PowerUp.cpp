#include "Header Files/PowerUp.h"

PowerUp::PowerUp(int spawn, btVector3 pos, int type,float duration)
{
	m_pos = pos;
	m_type = type;
	m_duration = duration;
	m_transform = new Transform;
	m_model = 0;
	float scale = 1.f;
	switch (type) {
		case 0: //Player sizeUp
			m_model = 6;
			m_color = vec3(0, 1, 0);
			scale = 0.1;
			break;

		case 1: //Enemy get inverted Controller
			m_model = 2;
			m_color = vec3(0, 1, 0);
			scale = 0.2f;
			break;

		case 2: //Player PowerMutiplier
			m_model = 4; //Exclamation mark
			m_color = vec3(0, 0, 3);
			scale = 0.12f;
			break;

		case 3: //player sizeDown
			m_model = 5;
			m_color = vec3(1, 0.5, 0);
			scale = 0.1f;
			break;

		case 4: //Enemy sizeUp
			m_model = 6;
			m_color = vec3(1, 0, 0);
			scale = 0.1f;
			break;

		case 5: //Enemy sizeDown
			m_model = 5;
			m_color = vec3(0, 1, 0);
			scale = 0.1f;
			break;

		case 6:
			m_model = 0; //Bomb - Tempor�rt kollision koeficient �kad. 
			m_color = vec3(0, 0.1, 0.1);
			scale = 0.3f;
			break;

		case 7: 
			m_model = 7; //LightBulb - INGEN FUNKTION KOPPLAD
			m_color = vec3(2, 2, 0);
			scale = 0.15f;
			break;
		case 8:
			m_model = 8; //Heart
			m_color = vec3(5.000, 0.719, 0.738);
			scale = 0.01f;
			break;
		case 9:
			m_model = 3; //Question mark - INGEN FUNKTION KOPPLAD
			m_color = vec3(0.741, 0.520, 3.000);
			scale = 0.09f;
			break;
	}
	m_spawn = spawn;

		m_btTransform = new btTransform();
		m_btTransform->setIdentity();
		m_btTransform->setOrigin(m_pos);
		m_transform->SetScale(scale, scale, scale);
	
	btVector3 localInertia(0, 0, 0);
	float mass = 1.f;
	m_shape = new btBoxShape(btVector3(0.75f, 0.75f, 0.75f));
	m_motionState = new btDefaultMotionState(*m_btTransform);

	m_shape->calculateLocalInertia(mass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_motionState, m_shape, localInertia);

	m_body = new btRigidBody(rbInfo);
	m_body->setCollisionShape(m_shape);
	m_body->setWorldTransform(*m_btTransform);
	//m_body->setLinearVelocity(btVector3(0.0f, -1.0f, 0.0f));
	//m_body->setDamping(1, 0);
	m_body->setRestitution(0);
	m_body->setFriction(100);
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
		m_transform->Rotate(0.f, 0.15f, 0.f);
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

