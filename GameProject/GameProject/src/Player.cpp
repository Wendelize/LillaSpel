#include "Header Files/Player.h"

Player::Player(Model* model, int modelId, vec3 pos)
{
	m_controller = new Controller;
	m_transform = new Transform;

	float radius = 1.;
	float scale = 0.69;
	m_restitution = 1.6699;
	m_transform->SetScale(scale, scale, scale);

	m_name = "";
	m_health = 0;
	m_controllerID = 0;
	m_modelId = modelId;
	m_weight = 0.f;
	m_speed = 0.f;
	m_powerMultiplier = 1.0f;
	m_powerActive = false;
	m_powerType = 0;
	m_scale = vec3(scale, scale, scale);
	m_transform->SetScale(scale, scale, scale);
	m_carShape = new btSphereShape(radius);
	m_btTransform = new btTransform();
	m_btTransform->setIdentity();

	// ConvexHullShape for car. Very precise but expensive since it creates A LOT of lines.
	/*vector<btVector3> points;

	for (int i = 0; i < model->GetMeshes().size(); i++)
	{
		for (int j = 0; j < model->GetMeshes()[i].m_vertices.size(); j++)
		{
			points.push_back(btVector3(model->GetMeshes()[i].m_vertices[j].pos.x, model->GetMeshes()[i].m_vertices[j].pos.y, model->GetMeshes()[i].m_vertices[j].pos.z));
		}
	}
	
	m_carShape = new btConvexHullShape(&points[0].getX(), points.size(), sizeof(btVector3));
	*/

	// MASSA FYSIK
	btScalar mass;
	switch (modelId)
	{
	case 0:
		//deafult bil
		mass = 1000.f;
		break;
	case 1:
		//truck
		mass = 1100.f;
		break;
	case 2:
		mass = 1050.f;
		break;
	case 3:
		mass = 950.f;
		break;
	default:
		mass = 1000.f;
		break;
	}
	m_btTransform->setOrigin(btVector3(pos.x,pos.y,pos.z));
	vec3 testPos = vec3(m_btTransform->getOrigin().x(), m_btTransform->getOrigin().y()- radius * scale, m_btTransform->getOrigin().z());
	m_transform->SetTranslation(testPos);

	btVector3 localInertia(0, 0, 0);
	m_carShape->calculateLocalInertia(mass, localInertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	m_motionState = new btDefaultMotionState(*m_btTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_motionState, m_carShape, localInertia);
	rbInfo.m_additionalDamping = true;
	m_body = new btRigidBody(rbInfo);
	m_body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
	m_body->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
	m_body->getCollisionShape()->setLocalScaling(btVector3(scale, scale, scale));
	m_body->clearForces();
	m_body->setRestitution(m_restitution);
	m_currentPos = m_btTransform->getOrigin();

	switch (modelId)
	{
	case 0:
		m_body->setDamping(0.3, 1.5);
		break;
	case 1:
		m_body->setDamping(0.1, 4.);
		break;
	case 2:
		m_body->setDamping(0.01, 2.0);
		break;
	case 3:
		m_body->setDamping(0.6, 6.8);
		break;
	default:
		m_body->setDamping(0.1, 1);
		break;
	}

	m_soundEngine = createIrrKlangDevice();

	m_soundEngine->setListenerPosition(vec3df(0, 18, 33), vec3df(0, -4, 3)); // Listener position, view direction
	m_soundEngine->setDefault3DSoundMinDistance(1.0f);
	m_fallen = false;

	m_carSounds.push_back(m_soundEngine->addSoundSourceFromFile("src/Audio/Player - Engine.mp3"));
	m_carSounds.push_back(m_soundEngine->addSoundSourceFromFile("src/Audio/Player - Backing.mp3"));

	m_soundEngine->setSoundVolume(0.0f);

	m_sound = m_soundEngine->play3D(m_carSounds[0], vec3df(GetCurrentPos().x(), GetCurrentPos().y(), GetCurrentPos().z()), true, false, true, true);
	m_sound->setPlaybackSpeed(1.0f);

	m_sfx = m_sound->getSoundEffectControl();
	m_sfx->enableDistortionSoundEffect();
}

Player::~Player()
{
	delete m_controller;
	delete m_transform;
	delete m_btTransform;
	delete m_carShape;

	for (uint i = 0; i < m_carSounds.size(); i++)
	{
		m_carSounds[i]->drop();
	}
	m_carSounds.clear();

	m_sound->drop();
	// m_sfx->drop();
}

void Player::Update(float dt)
{
	m_body->activate(true);

	vec3 rotate(0, 0, 0);
	vec3 direction(0, 0, 0);
	float rotationSpeed = 2.f;
	float jump = 0;
	bool pressed = false;

	if (glfwJoystickPresent(m_controllerID) == 1 && m_body->getWorldTransform().getOrigin().y() < 4.0f && m_body->getWorldTransform().getOrigin().y() > -1.0f)
	{
		if (m_controller->ButtonOptionsIsPressed(m_controllerID))
		{
			//Temporary
			m_body->setLinearVelocity(btVector3(0, 0, 0));
		}

		if (m_controller->ButtonAIsPressed(m_controllerID))
		{
			//Acceleration
			m_speed = 600000.f * m_powerMultiplier;
			pressed = true;
		}

		if (m_controller->ButtonXIsPressed(m_controllerID))
		{
			m_speed = -400000.f * m_powerMultiplier;
			pressed = true;
		}

		//Triggers
		if (m_controller->GetRightTrigger(m_controllerID) != -1)
		{
			//Left trigger pressed
			//Power-Up
			m_speed = 1000000.f * m_powerMultiplier;
			pressed = true;

		}
		if (m_controller->GetLefTrigger(m_controllerID) != -1)
		{
			//Left trigger pressed
			//Power-Up
			m_speed = -800000.f * m_powerMultiplier;
			pressed = true;

		}

		// Left stick horisontal input
		if (m_controller->GetLeftStickHorisontal(m_controllerID) > 0.2f || m_controller->GetLeftStickHorisontal(m_controllerID) < -0.2f)
		{
			if (m_speed < 0)
				rotate.y -= m_controller->GetLeftStickHorisontal(m_controllerID) * -1.0;
			else
				rotate.y -= m_controller->GetLeftStickHorisontal(m_controllerID);
		}
		
		//Set rotationSpeed depending on your speed, less speed--> Can turn less. 
		if (m_body->getLinearVelocity().length() < 3.f)
		{
			rotationSpeed = 2 * (m_body->getLinearVelocity().length()/3);
		}
		direction = m_transform->TranslateDirection(rotate * dt * rotationSpeed);
	}

	//apply force
	btVector3 directionBt = { direction.x,0,direction.z };
	if(pressed){
		m_body->applyForce(directionBt * -m_speed * dt , m_body->getWorldTransform().getOrigin());
	}

	if (m_body->getLinearVelocity().y() < 0.3f && m_body->getLinearVelocity().y() > -0.3f)
	{
		m_soundEngine->setSoundVolume(m_body->getLinearVelocity().length() / 6 + 0.4f);
		m_sound->setPlaybackSpeed(m_body->getLinearVelocity().length() / 25 + 1.0f);
	}
	m_body->applyDamping(dt);

	btVector3 moveVector = m_body->getWorldTransform().getOrigin() - m_currentPos;

	m_transform->Translate(vec3(moveVector.x(), moveVector.y(), moveVector.z())); 
	m_currentPos = m_body->getWorldTransform().getOrigin();
}

string Player::GetName()
{
	return m_name;
}

void Player::SetName(string name)
{
	m_name = name;
}

int Player::GetHealth()
{
	return m_health;
}

void Player::SetHealth(int health)
{
	m_health = health;
}

int Player::GetModelId()
{
	return m_modelId;
}

void Player::SetModelId(int id)
{
	m_modelId = id;
}

float Player::GetSpeed()
{
	return m_speed;
}

void Player::SetSpeed(float speed)
{
	m_speed = speed;
}

vec3 Player::GetColor()
{
	return m_color;
}

void Player::SetColor(vec3 color)
{
	m_color = color;
}

float Player::GetWeight()
{
	return m_weight;
}

void Player::SetWeight(float weight)
{
	m_weight = weight;
}

vec3 Player::GetScale()
{
	return m_scale;
}

void Player::SetScale(vec3 scale)
{
//	m_scale = scale;
//	m_transform->SetScale(scale.x, scale.y, scale.z);
}

int Player::GetControllerID()
{
	return m_controllerID;
}

void Player::SetControllerID(int id)
{
	m_controllerID = id;
}

vec3 Player::GetDirection()
{
	return m_transform->GetForward();
}

ObjectInfo* Player::GetObjectInfo()
{
	m_info = new ObjectInfo(m_transform->GetMatrix(), m_modelId, 0, m_color, true);
	return m_info;
}

btRigidBody* Player::GetBody()
{
	return m_body;
}

btVector3 Player::GetCurrentPos()
{
	return m_body->getWorldTransform().getOrigin();
}

void Player::SetPos(vec3 pos)
{
	m_body->getWorldTransform().setOrigin(btVector3(pos.x, pos.y, pos.z));
	m_body->setLinearVelocity(btVector3(0,0,0));
}

bool Player::GetFallen()
{
	return m_fallen;
}

void Player::GivePower(int type)
{

	if (m_powerActive) {
		removePower(m_powerType);
	}
	cout << "Activating power type " << type  << endl;

	m_powerType = type;
	m_powerDuration = 300.f;
	m_powerActive = true;

	float mass;
	btVector3 localInertia(0, 0, 0);
	switch (type) {
		case 0:
			mass = m_body->getMass() * 1.5f;
			m_body->getCollisionShape()->calculateLocalInertia(mass, localInertia);
			m_body->setMassProps(mass, localInertia);
			m_transform->SetScale(m_scale.x * (1.5f), m_scale.y * (1.5f), m_scale.z * (1.5f));
			m_powerMultiplier = 1.0f;
			break;

		case 1:
			m_body->setRestitution(m_restitution * 1.2);
			break;

		case 2:
			m_powerMultiplier = 0.5f;
			break;

		case 3:
			mass = m_body->getMass() / 1.5f;
			m_body->getCollisionShape()->calculateLocalInertia(mass, localInertia);
			m_body->setMassProps(mass, localInertia);
			m_transform->SetScale(m_scale.x * (0.75f), m_scale.y * (0.75f), m_scale.z * (0.75f));
			m_powerMultiplier = 1.0f;
			break;
		case 4:
			mass = m_body->getMass() * 1.5f;
			m_body->getCollisionShape()->calculateLocalInertia(mass, localInertia);
			m_body->setMassProps(mass, localInertia);
			m_transform->SetScale(m_scale.x * (1.5f), m_scale.y * (1.5f), m_scale.z * (1.5f));
			m_powerMultiplier = 1.0f;
			break;	

		case 5:
			mass = m_body->getMass() / 1.5f;
			m_body->getCollisionShape()->calculateLocalInertia(mass, localInertia);
			m_body->setMassProps(mass, localInertia);
			m_transform->SetScale(m_scale.x * (0.75f), m_scale.y * (0.75f), m_scale.z * (0.75f));
			m_powerMultiplier = 1.0f;
			break;
	}
}

void Player::removePower(int type)
{
	float mass;
	btVector3 localInertia(0, 0, 0);

	m_powerActive = false;
	cout << "Deactivating power type " << type << " for player " << m_controllerID << endl;
	switch (type) {
	case 0:
		mass = m_body->getMass() / 1.5f;
		m_body->getCollisionShape()->calculateLocalInertia(mass, localInertia);
		m_body->setMassProps(mass, localInertia);
		m_powerMultiplier = 1.f;
		m_transform->SetScale(m_scale.x, m_scale.y, m_scale.z);
		break;

	case 1:
		m_body->setRestitution(m_restitution);
		break;

	case 2:
		m_powerMultiplier = 1.f;
		break;

	case 3:
		mass = m_body->getMass() * 1.5f;
		cout << mass << endl;
		m_body->getCollisionShape()->calculateLocalInertia(mass, localInertia);
		m_body->setMassProps(mass, localInertia);
		m_powerMultiplier = 1.f;
		m_transform->SetScale(m_scale.x, m_scale.y, m_scale.z);
		break;

	case 4:
		mass = m_body->getMass() / 1.5f;
		cout << mass << endl;
		m_body->getCollisionShape()->calculateLocalInertia(mass, localInertia);
		m_body->setMassProps(mass, localInertia);
		m_powerMultiplier = 1.f;
		m_transform->SetScale(m_scale.x, m_scale.y, m_scale.z);
		break;

	case 5:
		mass = m_body->getMass() * 1.5f;
		cout << mass << endl;
		m_body->getCollisionShape()->calculateLocalInertia(mass, localInertia);
		m_body->setMassProps(mass, localInertia);
		m_powerMultiplier = 1.f;
		m_transform->SetScale(m_scale.x, m_scale.y, m_scale.z);
		break;
	}
}

bool Player::updatePower(float dt)
{
	bool destroy = false;
	m_powerDuration = m_powerDuration - dt;
	if (m_powerDuration <= 0.f && m_powerActive == true) {
		m_powerActive = false;
		destroy = true;
	}

	return destroy;
}

int Player::GetActivePower()
{
	return m_powerType;
}

void Player::SetFallen()
{
	m_fallen = true;
}

void Player::SetNotFallen()
{
	m_fallen = false;
}

float Player::GetLinearVelocity()
{
	return m_body->getLinearVelocity().length();
}

void Player::StartEngineSounds()
{
	m_sound->setIsPaused(false);
	m_soundEngine->setSoundVolume(0.1f);
	m_sound->setPlaybackSpeed(1.0f);
}

void Player::StopEngineSounds()
{
	m_sound->setIsPaused(true);
}
