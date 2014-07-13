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

#ifndef GLP_VIEWPORT_H
#define GLP_VIEWPORT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

namespace glp {

	class Viewport {
	public:
		Viewport() {}
		Viewport(int x, int y, int w, int h): rect(x, y, w, h) {}

		void	push() {
			glViewport(rect[0], rect[1], rect[2], rect[3]);
			getStack().push_back(rect);
		}

		static void	pop() {
			std::vector<glm::ivec4> &stack = getStack();
			if (stack.empty())
				return;
			stack.pop_back();
			if (stack.empty())
				return;
			glm::ivec4 &vp = stack.back();
			glViewport(vp[0], vp[1], vp[2], vp[3]);
		}

	private:
		glm::ivec4 rect;
		static std::vector<glm::ivec4>&	getStack() {
			static std::vector<glm::ivec4>	stack;
			return stack;	
		}

	};

	class UseViewport {
	public:
		UseViewport(Viewport &vp) {
			vp.push();
		}
		~UseViewport() {
			Viewport::pop();
		}

	private:
		Viewport viewport;
	};

}

#endif
