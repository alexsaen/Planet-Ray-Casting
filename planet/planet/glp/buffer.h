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

#ifndef GLP_BUFFER_H
#define GLP_BUFFER_H

#include "base.h"
#include <GL/glew.h>

namespace glp {

	class Buffer {
	public:
		Buffer(): type(0) {}

		operator bool() const {
			return mID > 0;
		}

		void	data(GLenum targ, GLsizeiptr size, const GLvoid *data, GLenum usage) {
			if(!mID) {
				glGenBuffers(1, &mID.getID());
				if(!mID)
					return;
			}
			type = targ;
			glBindBuffer(type, mID);
			glBufferData(type, size, data, usage);
		}

		void	bind() {
			if(!mID)
				return;
			glBindBuffer(type, mID);
		}

		void	unbind() {
			glBindBuffer(type, 0);
		}

		void	release() {
			free(mID);
			mID=0;
			type=0;
		}

	private:

		static void	free(GLuint aid) {
			glDeleteBuffers(1, &aid);
		}

		ID<GLuint, free> mID;
		GLenum type;
	};

}

#endif


