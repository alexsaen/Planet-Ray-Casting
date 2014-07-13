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

#ifndef GLP_PROGRAM_H
#define GLP_PROGRAM_H

#include "base.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> 

namespace glp {

	class Program;

	class Uniform {
	public:
		Uniform() : mID(0), program(0)							{}
		Uniform(GLuint aid, Program *p) : mID(aid), program(p)	{}

		operator bool() const {
			return mID >= 0;
		}

		void	set(int v)				{	glUniform1i(mID, v); }
		void	set(const glm::mat4 &v)	{	glUniformMatrix4fv(mID, 1, GL_FALSE, glm::value_ptr(v)); }
		void	set(const glm::vec3 &v)	{	glUniform3fv(mID, 1, glm::value_ptr(v)); }
		void	set(int *v, int count)	{	glUniform1iv(mID, count, v); }

	private:
		GLint mID;
		Program *program;
	};

	class Program {
	public:
		Program() {}
		Program(GLuint aid): mID(aid) {}

		operator bool() const {
			return mID > 0;
		}

		void	use() {
			glUseProgram(mID.getID());
		}

		Uniform	getUniform(const char* name) {
			return Uniform(glGetUniformLocation(mID.getID(), name), this);
		}

		void	release() {
			free(mID);
			mID=0;
		}

	private:

		static void	free(GLuint p) {
			glDeleteProgram(p);
		}

		ID<GLuint, free> mID;

	};


}

#endif
