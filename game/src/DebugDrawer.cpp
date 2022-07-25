#include "DebugDrawer.h"

void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance,
	int lifeTime, const btVector3& color)
{
}

void DebugDrawer::reportErrorWarning(const char* warningString)
{
}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
}

void DebugDrawer::setDebugMode(int debugMode)
{
	DebugFlags = debugMode;
}

int DebugDrawer::getDebugMode() const
{
	return DebugFlags;
}

void DebugDrawer::clearLines()
{
	LineVertices.resize(0);
}

void DebugDrawer::flushLines()
{
	glDisable(GL_DEPTH_TEST);
	DebugDrawShader.use();
	DebugDrawShader.setMat4f(ViewProjectionMatrix, "viewProjectionMatrix");

	glBindVertexArray(LinePointVAO);

	glBindBuffer(GL_ARRAY_BUFFER, LinesBuffer.BufferID);
	glBufferData(GL_ARRAY_BUFFER, LineVertices.size() * sizeof(LineVertex), LineVertices.data(), GL_DYNAMIC_DRAW);

	glDrawArrays(GL_LINES, 0, LineVertices.size());

	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
}

DebugDrawer::DebugDrawer(int debugDrawModes) :
	DebugFlags(debugDrawModes)
{
	// Reserve the size for all the lines of one box. (*2 because each line will have 2 vertices)
	LineVertices.reserve(6 * 4 * 2);

	static const auto EXPECTED_VERTEX_SIZE = 6 * sizeof(float);
	static_assert(EXPECTED_VERTEX_SIZE == sizeof(LineVertex));

	// Create the VAO for our points : 3D position and color
	glGenBuffers(1, &LinesBuffer.BufferID);
	glGenVertexArrays(1, &LinePointVAO);
	glBindVertexArray(LinePointVAO);

	glBindBuffer(GL_ARRAY_BUFFER, LinesBuffer.BufferID);
	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, EXPECTED_VERTEX_SIZE, (void*)0);

	// color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, EXPECTED_VERTEX_SIZE, (void*)(3 * sizeof(float)));

	glBindVertexArray(0);

	DebugDrawShader.init("game/shader/DebugDrawerVertexShader.glsl", "game/shader/DebugDrawerFragShader.glsl");
}

DebugDrawer::~DebugDrawer()
{
	glDeleteVertexArrays(1, &LinePointVAO);
	glDeleteBuffers(1, &LinesBuffer.BufferID);
}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	LineVertices.emplace_back(from, color);
	LineVertices.emplace_back(to, color);
}