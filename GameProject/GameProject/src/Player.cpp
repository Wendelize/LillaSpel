#include "Header Files/Player.h"

Player::Player(Model* model, int modelId, vec3 pos)
{
	m_controller = new Controller;
	m_transform = new Transform;

	float radius = 1.;
	float scale = 0.69;
	m_restitution = 1.6699;
	m_transform->SetScale(scale, scale, scale);

	m_lives = 3;
	m_name = "";
	m_health = 0;
	m_controllerID = 0;
	m_modelId = modelId;
	m_weight = 0.f;
	m_speed = 0.f;
	m_powerMultiplier = 1.0f;
	m_powerActive = false;
	m_powerType = 0;
	m_inverted = false;
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
		m_powerMultiplier = 1.0f;

		break;
	case 1:
		//truck
		mass = 1200.f;
		m_powerMultiplier = 1.2f;

		break;
	case 2:
		mass = 1100.f;
		m_powerMultiplier = 1.1f;

		break;
	case 3:
		mass = 950.f;
		m_powerMultiplier = 0.95f;

		break;
	default:
		mass = 1000.f;
		m_powerMultiplier = 1.0f;

		break;
	}
	m_btTransform->setOrigin(btVector3(pos.x,pos.y,pos.z));
	vec3 testPos = vec3(m_btTransform->getOrigin().x(), m_btTransform->getOrigin().y()- radius * scale, m_btTransform->getOrigin().z());
	m_transform->SetTranslation(testPos);

	//float dotRotation = dot(m_transform->GetForward(), m_transform->GetPos());
	vec2 forward = vec2(m_transform->GetForward().x, m_transform->GetForward().z);
	vec2 posVec2 = vec2(m_transform->GetPos().x, m_transform->GetPos().z);
	float forwardLenght = sqrt(pow(forward.x, 2) + pow(forward.y, 2));
	float posLenght = sqrt(pow(posVec2.x, 2) + pow(posVec2.y, 2));

	float theta = acos(dot(normalize(forward), normalize(posVec2)));

	if (posVec2.x < 0) {
		m_transform->Rotate(vec3(0, -theta, 0));
	}
	else {
		m_transform->Rotate(vec3(0, theta, 0));
	}


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
		m_body->setDamping(0.20, 50.0);
		break;
	case 1:
		m_body->setDamping(0.15, 50.0);
		break;
	case 2:
		m_body->setDamping(0.15, 50.0);
		break;
	case 3:
		m_body->setDamping(0.2, 50.0);
		break;
	default:
		m_body->setDamping(0.15, 15.0);
		break;
	}

	m_soundEngine = createIrrKlangDevice();
	m_honkEngine = createIrrKlangDevice();

	if (m_soundEngine && m_honkEngine)
	{
		m_soundEngine->setListenerPosition(vec3df(0, 18, 33), vec3df(0, -4, 3)); // Listener position, view direction
		m_soundEngine->setDefault3DSoundMinDistance(1.0f);
		m_fallen = false;

		m_carSounds.push_back(m_soundEngine->addSoundSourceFromFile("src/Audio/Player - Engine2.0.mp3"));
		m_carSounds.push_back(m_soundEngine->addSoundSourceFromFile("src/Audio/Player - Backing.mp3"));
		m_carSounds.push_back(m_soundEngine->addSoundSourceFromFile("src/Audio/Player - HonkDELUXE.mp3"));

		m_soundEngine->setSoundVolume(0.3f);

		m_sound = m_soundEngine->play2D(m_carSounds[0], true, true);
		m_sound->setPlaybackSpeed(1.0f);

		m_honkEngine->setSoundVolume(0.2f);
		m_honk = m_honkEngine->play2D(m_carSounds[2], false, true);
	}

	m_hookActive = false;

}

Player::~Player()
{
	delete m_controller;
	delete m_transform;
	delete m_btTransform;
	delete m_carShape;
	if (m_soundEngine) 
	{
		for (uint i = 0; i < m_carSounds.size(); i++)
		{
			m_carSounds[i]->drop();
		}
		m_carSounds.clear();

		m_sound->drop();
		m_honk->drop();
		m_honkEngine->drop();

	}

}

void Player::Update(float dt)
{
	m_body->activate(true);

	vec3 rotate(0, 0, 0);
	vec3 direction(0, 0, 0);
	float rotationSpeed = 2.f;
	float jump = 0;
	bool pressed = false;

	m_timeOut += dt;
	if (m_timeOut >= 2 && !m_lightsOn)
	{
		m_lightsOn = true;
	}


	if (glfwJoystickPresent(m_controllerID) == 1 && m_body->getWorldTransform().getOrigin().y() > -1.0f)
	{
		if (m_honkEngine && m_soundEngine && m_controller->ButtonRightJoystickIsPressed(m_controllerID))
		{
			m_honkEngine->play2D(m_honk->getSoundSource(), false, false, true);
		}
		if (m_controller->ButtonAIsPressed(m_controllerID))
		{
			//Acceleration
			
			if (m_inverted == false)
			{
				m_speed = 900000.f * m_powerMultiplier / (dt * 60);
			}
			else if (m_inverted == true)
			{
				m_speed = -900000.f * m_powerMultiplier / (dt * 60);
			}
			pressed = true;
		}

		if (m_controller->ButtonXIsPressed(m_controllerID))
		{
			if (m_inverted == false)
			{
				m_speed = -800000.f * m_powerMultiplier / (dt * 60);
			}
			else if (m_inverted == true)
			{
				m_speed = 800000.f * m_powerMultiplier / (dt * 60);
			}
				pressed = true;
		}

		//Triggers
		if (m_controller->GetRightTrigger(m_controllerID) != -1)
		{
			//Left trigger pressed
			//Speed-Up
			if (m_inverted == false)
			{
				m_speed = 1100000.f * m_powerMultiplier / (dt * 60);
			}
			else if (m_inverted == true)
			{
				m_speed = -1100000.f * m_powerMultiplier / (dt * 60);
			}
			pressed = true;
		}
		if (m_controller->GetLeftTrigger(m_controllerID) != -1)
		{
			//Left trigger pressed
			//Speed-Up
			
			if (m_inverted == false)
			{
				m_speed = -900000.f * m_powerMultiplier / (dt * 60);
			}
			else if (m_inverted == true)
			{
				m_speed = 900000.f * m_powerMultiplier / (dt * 60);
			}
			pressed = true;
		}

		// Left stick horisontal input
		if ((m_controller->GetLeftStickHorisontal(m_controllerID) > 0.2f || m_controller->GetLeftStickHorisontal(m_controllerID) < -0.2f) && m_inverted == false)
		{
			if (m_speed < 0)
				rotate.y -= m_controller->GetLeftStickHorisontal(m_controllerID) * -1.0;
			else
				rotate.y -= m_controller->GetLeftStickHorisontal(m_controllerID);
		}
		else if ((m_controller->GetLeftStickHorisontal(m_controllerID) > 0.2f || m_controller->GetLeftStickHorisontal(m_controllerID) < -0.2f) && m_inverted == true)
		{
			if (m_speed < 0)
				rotate.y -= m_controller->GetLeftStickHorisontal(m_controllerID);
			else
				rotate.y -= m_controller->GetLeftStickHorisontal(m_controllerID)* -1.0;
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

	if (m_soundEngine && m_body->getLinearVelocity().y() < 0.3f && m_body->getLinearVelocity().y() > -0.3f)
	{
		m_soundEngine->setSoundVolume(m_body->getLinearVelocity().length() / 25 + 0.3f);
		m_sound->setPlaybackSpeed(m_body->getLinearVelocity().length() / 15 + 1.0f);
	}
	m_body->applyDamping(dt);

	btVector3 moveVector = m_body->getWorldTransform().getOrigin() - m_currentPos;

	m_transform->Translate(vec3(moveVector.x(), moveVector.y(), moveVector.z())); 
	m_currentPos = m_body->getWorldTransform().getOrigin();
}

int Player::GetLives()
{
	return m_lives;
}

void Player::SetLives(int num)
{
	m_lives = num;
}

void Player::ReduceLife()
{
	m_lives--;
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
	return m_body->getLinearVelocity().length();
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
	m_scale = scale;
	m_transform->SetScale(scale.x, scale.y, scale.z);
}

int Player::GetControllerID()
{
	return m_controllerID;
}

void Player::SetControllerID(int id)
{
	m_controllerID = id;
}

void Player::FinishRotation()
{
	m_transform->SetRotation(0, 3.14*1.2, 0);
}

vec3 Player::GetDirection()
{
	return m_transform->GetForward();
}

mat4 Player::GetMatrix()
{
	return m_transform->GetMatrix();
}

void Player::Rotate(float speed)
{
	m_transform->Rotate(vec3(0, speed, 0));
}

ObjectInfo* Player::GetObjectInfo()
{
	m_info = new ObjectInfo(m_transform->GetMatrix(), m_modelId, 0, m_color, false);
	return m_info;
}

Light* Player::GetLight(int index)
{
	vec3 pos = vec3(m_transform->GetMatrix()[3][0], m_transform->GetMatrix()[3][1], m_transform->GetMatrix()[3][2]);
	vec3 t = cross(m_transform->GetForward(), { 0,1,0 });
	switch (index)
	{
	case 0:
		m_lights[0] = new Light(2, -m_transform->GetForward() - vec3(0, 0.05, 0), pos /*+ m_transform->GetForward() * 1.f*/ + t * 0.5f + vec3(0, 0.5, 0), vec3(m_color.x * 5, m_color.y * 5, m_color.z * 5), 12.5);
		break;
	case 1:
		m_lights[1] = new Light(2, -m_transform->GetForward() - vec3(0, 0.05, 0), pos /*+ m_transform->GetForward() * 1.f*/ - t * 0.5f + vec3(0, 0.5, 0), vec3(m_color.x * 5, m_color.y * 5, m_color.z * 5), 12.5);
		break;
	case 2:
		m_lights[2] = new Light(2, m_transform->GetForward() - vec3(0, 0.05, 0), pos + vec3(-0.5, 0.5, 0), vec3(m_color.x * 5, m_color.y * 5, m_color.z * 5), 12.5);
		break;
	case 3:
		m_lights[3] = new Light(2, m_transform->GetForward() - vec3(0, 0.05, 0), pos + vec3(0.5, 0.5, 0), vec3(m_color.x * 5, m_color.y * 5, m_color.z * 5), 12.5);
		break;
	default:
		break;
	}

	return m_lights[index];
}

btRigidBody* Player::GetBody()
{
	return m_body;
}

btVector3 Player::GetCurrentPos()
{
	return m_body->getWorldTransform().getOrigin();
}

void Player::SetFinishPos(vec3 pos)
{
	m_body->getWorldTransform().setOrigin(btVector3(pos.x, pos.y, pos.z));
	m_body->setLinearVelocity(btVector3(0, 0, 0));
	m_transform->SetTranslation(pos);
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
	if (m_powerActive) 
	{
		removePower(m_powerType);
	}
	m_powerType = type;
	m_powerDuration = 10.f;
	m_powerActive = true;

	float mass;
	btVector3 localInertia(0, 0, 0);
	switch (type) 
	{
		case 0:
			mass = m_body->getMass() * 1.5f;
			m_body->getCollisionShape()->calculateLocalInertia(mass, localInertia);
			m_body->setMassProps(mass, localInertia);
			m_transform->SetScale(m_scale.x * (1.5f), m_scale.y * (1.5f), m_scale.z * (1.5f));
			m_powerMultiplier = 1.0f;
			break;

		case 1:
			m_inverted = true;
			break;

		case 2:
			m_powerDuration = 10000.f;
			//m_powerMultiplier = 0.5f;
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

		case 6: //Bomb
			break;

		case 7: //Lightbulb
			break;

		case 8:
			m_lives++;
			break;

		case 9: //Invisible terrain
			//m_body->setRestitution(m_restitution * 1.2);
			break;
	}
}

void Player::removePower(int type)
{
	float mass;
	btVector3 localInertia(0, 0, 0);
	m_powerActive = false;

	switch (type) 
	{
	case 0:
		mass = m_body->getMass() / 1.5f;
		m_body->getCollisionShape()->calculateLocalInertia(mass, localInertia);
		m_body->setMassProps(mass, localInertia);
		m_powerMultiplier = 1.f;
		m_transform->SetScale(m_scale.x, m_scale.y, m_scale.z);
		break;

	case 1:
		m_inverted = false;
		break;

	case 2:
		//m_powerMultiplier = 1.f;
		m_hookActive = false;
		break;

	case 3:
		mass = m_body->getMass() * 1.5f;
		m_body->getCollisionShape()->calculateLocalInertia(mass, localInertia);
		m_body->setMassProps(mass, localInertia);
		m_powerMultiplier = 1.f;
		m_transform->SetScale(m_scale.x, m_scale.y, m_scale.z);
		break;

	case 4:
		mass = m_body->getMass() / 1.5f;
		m_body->getCollisionShape()->calculateLocalInertia(mass, localInertia);
		m_body->setMassProps(mass, localInertia);
		m_powerMultiplier = 1.f;
		m_transform->SetScale(m_scale.x, m_scale.y, m_scale.z);
		break;

	case 5:
		mass = m_body->getMass() * 1.5f;
		m_body->getCollisionShape()->calculateLocalInertia(mass, localInertia);
		m_body->setMassProps(mass, localInertia);
		m_powerMultiplier = 1.f;
		m_transform->SetScale(m_scale.x, m_scale.y, m_scale.z);
		break;
	case 6:
		
		break;
	case 7:
		break;
	case 8:
		//No functionality needed
		break;
	case 9:
		//m_body->setRestitution(m_restitution);
		break;
	}

	m_powerType = NULL;
}

bool Player::updatePower(float dt)
{
	bool destroy = false;
	m_powerDuration = m_powerDuration - dt;

	if (m_powerDuration <= 0.f && m_powerActive == true) 
	{
		m_powerActive = false;
		destroy = true;
	}

	return destroy;
}

int Player::GetActivePower()
{
	return m_powerType;
}

Controller* Player::GetController()
{
	return m_controller;
}

bool Player::GetBoolLights()
{
	return m_lightsOn;
}

void Player::SetBoolLights(bool state)
{
	m_lightsOn = state;
	if (!m_lightsOn)
	{
		m_timeOut = 0;
	}
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
	if (m_soundEngine)
	{
		m_sound->setIsPaused(false);
		m_soundEngine->setSoundVolume(0.3f);
		m_sound->setPlaybackSpeed(1.0f);
	}
}

void Player::StopEngineSounds()
{
	if (m_soundEngine)
		m_sound->setIsPaused(true);
}

vec3 Player::GetLastPos()
{
	return m_lastPos;
}

void Player::SetLastPos(vec3 pos)
{
	m_lastPos = pos;

}

bool Player::GetHook()
{
	return m_hookActive;
}

void Player::SetHook(bool state)
{
	m_hookActive = state;
}
