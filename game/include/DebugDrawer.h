#pragma once
#include <vector>
#include <glad/glad.h>
#include <LinearMath/btIDebugDraw.h>
#include "Resources/Shader.h"
#include "Mat4/Mat4.h"

class DebugDrawer : public btIDebugDraw
{
	struct LineVertex
	{
		LineVertex() = default;

		LineVertex(btVector3 const& pos, btVector3 const& color) :
			Position{ (float)pos.x(), (float)pos.y(), (float)pos.z() },
			Color{ (float)color.x(),(float)color.y(),(float)color.z() }
		{}

		float	Position[3]{ 0 };
		float	Color[3]{ 0 };
	};

	struct Buffer
	{
		GLuint	BufferID = 0;
		size_t	Size = 0;
		size_t	Capacity = 0;
	};

public:

	DebugDrawer(int debugDrawModes);
	~DebugDrawer();

	void	drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;

	void	drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
	void	reportErrorWarning(const char* warningString) override;
	void	draw3dText(const btVector3& location, const char* textString) override;
	void	setDebugMode(int debugMode) override;
	int		getDebugMode() const override;

	void	clearLines() override;
	void	flushLines() override;

	void	SetViewProjection(const lm::mat4& mat)
	{
		ViewProjectionMatrix = mat;
	}

private:

	int	DebugFlags{ 0 };
	Buffer LinesBuffer;
	GLuint LinePointVAO = 0;

	std::vector<LineVertex>	LineVertices;

	Resources::Shader	DebugDrawShader;

	lm::mat4	ViewProjectionMatrix;
};
