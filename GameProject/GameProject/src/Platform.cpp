#include "Header Files/Platform.h"


Platform::Platform()
{
	m_object = new Object(1000, vec3(0.0f, 0.0f, -1.0f));

	m_object->CreatePlane();
	m_object->InitObject();
}

Platform::~Platform()
{
	delete m_object;
}

Object* Platform::GetObject()
{
	return m_object;
}

void Platform::SetObject(Object* theObject)
{
	m_object = theObject;
	// Potential danger if object is deleted. Might need a deep copy.
}