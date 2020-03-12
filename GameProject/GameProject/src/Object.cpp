#include "Header Files/Object.h"


// ID 0 = BORING LOG - DYNAMIC
// ID 1 = BALL - DYNAMIC
// ID 2 = NEW SICK LOG
// ID 3 = Rock
// ID 4 = RED MUSHROOM 
// ID 5 = LOW POLY PINE
// ID 6 = LOW POLY TREE
// ID 7 = RAMP
// ID 8 = RAMP OTHER WAY
// ID 9 = STRANGER DANGER, SPINNER DINNER
// ID 10 = ASTRO

Object::Object(btVector3 pos, int type, Model* model, float speed, float scale)
{
	m_transform = new Transform;

	m_btTransform = new btTransform();
	m_btTransform->setIdentity();
	m_btTransform->setOrigin(pos);
	m_transform->SetScale(1, 1, 1);
	m_scale = 1.0f;
	m_speed = speed;
	m_scale = scale;
	btVector3 localInertia(0, 0, 0);
	float mass;

	if (type == 0 || type == 2 || type == 9)
	{
		mass = 100000.0f;
	}
	else if (type == 3 || type == 4 || type == 5 || type == 6 || type == 7 || type == 8 || type == 10)
	{
		mass = 0.f;
	}
	else
	{
		mass = 700.f;
	}
	m_motionState = new btDefaultMotionState(*m_btTransform);
	vector<btVector3> points;
	vector<int> indicies;

	for (int i = 0; i < model->GetMeshes().size(); i++)
	{
		for (int j = 0; j < model->GetMeshes()[i].m_vertices.size(); j++)
		{
			points.push_back(btVector3(model->GetMeshes()[i].m_vertices[j].pos.x,
			                           model->GetMeshes()[i].m_vertices[j].pos.y,
			                           model->GetMeshes()[i].m_vertices[j].pos.z));
		}
	}

	m_physicsMesh = new btConvexHullShape(&points[0].getX(), points.size(), sizeof(btVector3));


	m_physicsMesh->calculateLocalInertia(mass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_motionState, m_physicsMesh, localInertia);
	m_body = new btRigidBody(rbInfo);
	if (type == 1)
	{
		m_body->setRestitution(3);
		m_body->setFriction(0);
	}
	else if (type == 7 || type == 8)
	{
		m_body->setRestitution(0);
	}
	else
	{
		m_body->setRestitution(0.4);
	}
	vec3 btPos = vec3(m_btTransform->getOrigin().x(), m_btTransform->getOrigin().y(), m_btTransform->getOrigin().z());
	m_transform->SetTranslation(btPos);

	//m_color = vec3((rand() % 255)/255.f + 0.1, (rand() % 255) / 255.f + 0.1, (rand() % 255) / 255.f + 0.1);
	m_color = vec3(1);
	m_model = type;
	m_currentPos = m_btTransform->getOrigin();
	m_body->getCollisionShape()->setLocalScaling(btVector3(m_scale, m_scale, m_scale));
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
	m_info = new ObjectInfo(scale(temp, vec3(m_scale)), m_model, 3, m_color, false);
	return m_info;
}

btRigidBody* Object::GetObject()
{
	return m_body;
}

void Object::Update()
{
	m_body->activate();
	if (m_body->getWorldTransform().getOrigin().y() < -10.f)
	{
		m_body->setLinearVelocity(btVector3(0, 0, 0));
		m_body->getWorldTransform().setOrigin(btVector3(0, 30, 0));
	}
	btVector3 moveVector = m_body->getWorldTransform().getOrigin() - m_currentPos;
	m_transform->Translate(vec3(moveVector.x(), moveVector.y(), moveVector.z()));
	m_currentPos = m_body->getWorldTransform().getOrigin();
}

void Object::SetScale(float scale)
{
	m_physicsMesh->setLocalScaling(btVector3(scale, scale, scale));
	m_scale = scale;
}

float Object::GetScale()
{
	return m_physicsMesh->getLocalScaling().x();
}

void Object::SetRotation(float degrees)
{
	btQuaternion quat = m_body->getWorldTransform().getRotation();
	btTransform trans;
	trans.setIdentity();
	btQuaternion quat2;
	quat2.setEuler(quat.getAngle() + degrees, 0, 0);

	trans.setRotation(quat2);

	m_body->setCenterOfMassTransform(trans);
}

void Object::Move(vec3 dir)
{
	m_body->setLinearVelocity(btVector3(dir.x, dir.y, dir.z));
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

void Object::rotateAroundCenter(float dt)
{
	btMatrix3x3 orn = m_body->getWorldTransform().getBasis();
	btQuaternion quat;
	orn.getRotation(quat);
	btVector3 axis = quat.getAxis();

	//Move rigidbody 2 units along its axis to the origin
	m_body->translate(-m_currentPos);
	float temp = -3 * 3.14 / 180 * dt * m_speed;
	//Rotate the rigidbody 	
	//by 1 degree on its center of mass
	orn *= btMatrix3x3(btQuaternion(btVector3(0, 1, 0), btScalar(temp)));
	m_body->getWorldTransform().setBasis(orn);

	//Get rotation matrix
	btTransform invRot(btQuaternion(btVector3(0, 1, 0), btScalar(temp)), btVector3(0, 0, 0));
	//Rotate your first translation vector with the matrix
	btVector3 invTrans = invRot * -m_currentPos;

	//Update axis variable to apply transform on
	orn.getRotation(quat);
	axis = quat.getAxis();

	//Translate back by rotated vector
	m_body->translate(-invTrans);
	m_currentPos = -invTrans;

	rotate(dt);
}

void Object::rotate(float dt)
{
	btMatrix3x3 orn = m_body->getWorldTransform().getBasis();
	btQuaternion quat;
	orn.getRotation(quat);
	btVector3 axis = quat.getAxis();

	//Move rigidbody 2 units along its axis to the origin
	float temp = -3 * 3.14 / 180 * dt * m_speed * 10;
	//Rotate the rigidbody 	
	//by 1 degree on its center of mass
	orn *= btMatrix3x3(btQuaternion(btVector3(1, 0, 1), btScalar(temp)));
	m_body->getWorldTransform().setBasis(orn);

	//Get rotation matrix
	btTransform invRot(btQuaternion(btVector3(1, 0, 1), btScalar(temp)), btVector3(0, 0, 0));
	//Rotate your first translation vector with the matrix

	//Update axis variable to apply transform on
	orn.getRotation(quat);
	axis = quat.getAxis();

	//Translate back by rotated vector
}
