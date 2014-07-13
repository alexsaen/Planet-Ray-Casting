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

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

class Camera {
public:
	Camera(): aspect(1.6f), fov(45.0f), near(0.1f), far(1000.0f)	{
		proj = glm::perspective(fov, aspect, near, far);
		view = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,-3.0));
	};

	const	glm::mat4&	getView() const			{	return view;	}
	const	glm::mat4&	getProj() const			{	return proj;	}
	const	glm::mat4	getViewProj() const		{	return proj*view;	}

	const	glm::vec3	getPos() const			{	
		glm::mat4 iview = glm::inverse(view);
		return glm::vec3(iview[3][0], iview[3][1], iview[3][2]);
	}

	void	move(const glm::vec3& v)			{
		view = glm::translate(glm::mat4(1.0f), v) * view;
	}

	void	rotate(const glm::vec3& v)			{
		view = glm::rotate(glm::mat4(1.0f), v.x, glm::vec3(0,1,0)) * glm::rotate(glm::mat4(1.0f), v.y, glm::vec3(1,0,0)) * view;
	}

	void	setAspect(float a) {
		aspect = a;
		proj = glm::perspective(fov, aspect, near, far);
	}

private:
	glm::mat4	view, proj;
	float	aspect, fov, near, far;

};



#endif
