#pragma once

#include "Include.h"

class DebugDrawer : public btIDebugDraw
{
public:
	// debug mode functions
	void setDebugMode(int debugMode) override { m_debugMode = debugMode; }
	int getDebugMode() const override { return m_debugMode; }

	// drawing functions
	void drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime,
	                      const btVector3& color) override;
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;

	// unused
	void reportErrorWarning(const char* warningString) override
	{
	}

	void draw3dText(const btVector3& location, const char* textString) override
	{
	}

	void ToggleDebugFlag(int flag);

protected:
	int m_debugMode;
};
