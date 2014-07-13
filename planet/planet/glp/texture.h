/*
Copyright (c) 2014, Alexey Saenko
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef GLP_TEXTURE_H
#define GLP_TEXTURE_H

#include "base.h"
#include <GL/glew.h>
#include <gli/gli.hpp>

namespace glp {

	class Texture {
		friend class Framebuffer;
	public:
		Texture() : type(0), lastBindUnit(-1) {}

		operator bool() const {
			return mID > 0;
		}

		void	init2D(int width, int height, int levels = 1, GLenum format = GL_RGBA8) {
			if (!mID) {
				glGenTextures(1, &mID.getID());
				if (!mID)
					return;
			}
			type = GL_TEXTURE_2D;
			glBindTexture(type, mID.getID());

			glTexParameteri(type, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(type, GL_TEXTURE_MAX_LEVEL, levels - 1);
			glTexParameteri(type, GL_TEXTURE_SWIZZLE_R, GL_RED);
			glTexParameteri(type, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
			glTexParameteri(type, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
			glTexParameteri(type, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);

			glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexStorage2D(type, levels, format, width, height);
		}

		void	load(const char* filename) {
			if (!mID) {
				glGenTextures(1, &mID.getID());
				if (!mID)
					return;
			}
			type = GL_TEXTURE_2D;

			gli::texture2D Texture(gli::load_dds(filename));
			//	assert(!Texture.empty());
			glBindTexture(type, mID.getID());

			glTexParameteri(type, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(type, GL_TEXTURE_MAX_LEVEL, GLint(Texture.levels() - 1));
//			glTexParameteri(type, GL_TEXTURE_SWIZZLE_R, GL_RED);
//			glTexParameteri(type, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
//			glTexParameteri(type, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
//			glTexParameteri(type, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
			glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexStorage2D(type,
				GLint(Texture.levels()),
				GLenum(gli::internal_format(Texture.format())),
				GLsizei(Texture.dimensions().x),
				GLsizei(Texture.dimensions().y));

			if (gli::is_compressed(Texture.format())) {
				for (gli::texture2D::size_type Level = 0; Level < Texture.levels(); ++Level) {
					glCompressedTexSubImage2D(type,
						GLint(Level),
						0, 0,
						GLsizei(Texture[Level].dimensions().x),
						GLsizei(Texture[Level].dimensions().y),
						GLenum(gli::internal_format(Texture.format())),
						GLsizei(Texture[Level].size()),
						Texture[Level].data());
				}
			} else {
				for (gli::texture2D::size_type Level = 0; Level < Texture.levels(); ++Level) {
					glTexSubImage2D(type,
						GLint(Level),
						0, 0,
						GLsizei(Texture[Level].dimensions().x),
						GLsizei(Texture[Level].dimensions().y),
						GLenum(gli::external_format(Texture.format())),
						GLenum(gli::type_format(Texture.format())),
						Texture[Level].data());
				}
			}

		}

		void	loadCube(const char* filename) {
			if (!mID) {
				glGenTextures(1, &mID.getID());
				if (!mID)
					return;
			}
			type = GL_TEXTURE_CUBE_MAP;

			gli::textureCube Texture(gli::load_dds(filename));
			//	assert(!Texture.empty());
			glBindTexture(type, mID.getID());

			glTexParameteri(type, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(type, GL_TEXTURE_MAX_LEVEL, GLint(Texture.levels() - 1));
			glTexParameteri(type, GL_TEXTURE_SWIZZLE_R, GL_RED);
			glTexParameteri(type, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
			glTexParameteri(type, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
			glTexParameteri(type, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);

			glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			for (std::size_t face = 0; face < 6; ++face)
				if (gli::is_compressed(Texture.format())) {
					for (gli::texture2D::size_type Level = 0; Level < Texture.levels(); ++Level) {
						glCompressedTexImage2D(
							GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
							GLint(Level),
							GLenum(gli::internal_format(Texture[face].format())),
							GLsizei(Texture[face][Level].dimensions().x),
							GLsizei(Texture[face][Level].dimensions().y),
							0,
							GLsizei(Texture[face][Level].size()),
							Texture[face][Level].data());
					}
				} else {
					for (gli::texture2D::size_type Level = 0; Level < Texture.levels(); ++Level) {
						glTexImage2D(
							GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
							GLint(Level),
							GLenum(gli::internal_format(Texture[face].format())),
							GLsizei(Texture[face][Level].dimensions().x),
							GLsizei(Texture[face][Level].dimensions().y),
							0,
							GLenum(gli::external_format(Texture[face].format())),
							GLenum(gli::type_format(Texture[face].format())),
							Texture[face][Level].data());
					}
				}

		}


		void	bind(unsigned int unit=0) {
			if (!mID)
				return;
			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(type, mID);
			lastBindUnit = unit;
		}

		void	unbind() {
			glActiveTexture(GL_TEXTURE0 + lastBindUnit);
			glBindTexture(type, 0);
			lastBindUnit = -1;
		}

		void	release() {
			free(mID);
			mID = 0;
		}

	private:

		static void	free(GLuint aid) {
			glDeleteTextures(1, &aid);
		}

		ID<GLuint, free> mID;
		GLenum type;
		int	lastBindUnit;
	};

}

#endif
