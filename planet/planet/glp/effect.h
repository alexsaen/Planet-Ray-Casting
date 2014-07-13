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

#ifndef GLP_EFFECT_H
#define GLP_EFFECT_H

#include "program.h"
#include <GL/glfx.h>

namespace glp {

	class Effect {
	public:

		Effect(): mID(-1) {}

		bool	load(const char* filename) {
			mID = glfxGenEffect(); 	
			if(mID<0)
				return  false;
			return glfxParseEffectFromFile(mID, filename);
		}

		Program	compileProgram(const char *name) {
			if(mID<0)
				return Program(0u);
			int p = glfxCompileProgram(mID, name);
			return Program(GLuint(p<=0 ? 0 : p));
		}

		const char* getLog() const {
			if(mID<0)
				return "Invalid Effect";
			return glfxGetEffectLog(mID);
		}

		void	release() {
			glfxDeleteEffect(mID);
			mID=-1;
		}
	private:
		ID<int, glfxDeleteEffect> mID;

	};

}

#endif