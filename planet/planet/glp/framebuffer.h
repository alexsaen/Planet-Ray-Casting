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

#ifndef GLP_FRAMEBUFFER_H
#define GLP_FRAMEBUFFER_H

#include "viewport.h"
#include "texture.h"

namespace glp {

	class Framebuffer {
	public:

		operator bool() const {
			return mID > 0;
		}

		void	bind() {
			if (!mID) {
				glGenFramebuffers(1, &mID.getID());
				if (!mID)
					return;
			}
			glBindFramebuffer(GL_FRAMEBUFFER, mID);
		}

		bool	attach(Texture &t, int level=0, int idx=0) {
		
			GLint tw=0, th=0;
			t.bind();
			glGetTexLevelParameteriv(t.type, level, GL_TEXTURE_WIDTH, &tw);
			glGetTexLevelParameteriv(t.type, level, GL_TEXTURE_HEIGHT, &th);
			t.unbind();
			if (tw < 1 || th < 1)
				return false;
			if (mDims.x != 0 && mDims.y != 0 && mDims.x != tw && mDims.y != th)
				return false;
			
			mDims.x = tw; mDims.y = th;

			bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, t.type, t.mID.getID(), level);
			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			unbind();
			return status == GL_FRAMEBUFFER_COMPLETE;
		}

		static void	unbind() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void	push()	{
			bind();
			getStack().push_back(this);
			Viewport(0, 0, mDims.x, mDims.y).push();
		}

		static void	pop()	{
			std::vector<Framebuffer*>& stack = getStack();
			if(stack.empty())
				return;

			stack.pop_back();
			if(stack.empty())
				unbind();
			else
				stack.back()->bind();
			Viewport::pop();
		}

		void	release() {
			free(mID);
			mID = 0;
		}

		const glm::ivec2&	getDims() const {
			return	mDims;
		}

	private:

		static void	free(GLuint aid) {
			glDeleteFramebuffers(1, &aid);
		}

		ID<GLuint, free> mID;
		glm::ivec2	mDims;

		static std::vector<Framebuffer*>&	getStack() {
			static std::vector<Framebuffer*>	stack;
			return stack;
		}

	};

	class UseFramebuffer {
	public: 
		UseFramebuffer(Framebuffer &fb) {
			fb.push();
		}
	
		~UseFramebuffer() {
			Framebuffer::pop();
		}
	};


}

#endif
