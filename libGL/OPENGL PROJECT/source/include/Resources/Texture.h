#pragma once
#include <../../libGL/GLAD/include/glad/glad.h>

#include <../../external/STB/stb_image.h>
#include "../Resources/Resourcesmanager.h"

namespace Resources
{
	class Texture : public IResource
	{
	public:
		GLuint index;

		Texture() {
			index = GL_FALSE;
		};
		Texture(char const* const& filename);

		void loadTexture(char const* const& filename);

		void Bind();
	};
}
