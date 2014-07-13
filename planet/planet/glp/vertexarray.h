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

#ifndef GLP_VERTEXARRAY_H
#define GLP_VERTEXARRAY_H

#include "base.h"
#include <GL/glew.h>

namespace glp {

	class VertexArray {
	public:

		operator bool() const {
			return mID > 0;
		}

		void	bind() {
			if(!mID) {
				glGenVertexArrays(1, &mID.getID());
				if(!mID)
					return;
			}
			glBindVertexArray(mID);			
		}

		static void	unbind() {
			glBindVertexArray(0);			
		}

		void	pointerf(GLuint index, GLint size, GLenum type,	GLsizei stride=0) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, size, type, GL_FALSE, stride, 0);
		}

		void	release() {
			free(mID);
			mID=0;
		}

	private:

		static void	free(GLuint aid) {
			glDeleteVertexArrays(1, &aid);
		}

		ID<GLuint, free> mID;
	};

}

#endif


