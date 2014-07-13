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

#include "scene.h"

#include <GL/glew.h>
#include <GL/glfx.h>
#include <glm/glm.hpp>

#include <string>
#include <iostream>

#include "glp/effect.h"
#include "glp/buffer.h"
#include "glp/texture.h"
#include "glp/vertexarray.h"

#include "camera.h"

static Camera	camera;

static unsigned short idxs[12 * 3] = {	0, 1, 3, 0, 3, 2,
										1, 5, 7, 1, 7, 3,
										3, 7, 6, 3, 6, 2,
										2, 6, 4, 2, 4, 0,
										0, 4, 5, 0, 5, 1,
										5, 4, 7, 4, 6, 7 };

glp::Effect effect;
glp::Program program;
glp::Buffer indexBuffer;
glp::VertexArray vertexArray;
glp::Texture cubeTexture;

glp::Uniform uCubeTex;
glp::Uniform uWVP;
glp::Uniform uCameraPos;

void Scene::init(int w, int h) {

	camera.setAspect(float(w)/h);
	cubeTexture.loadCube("data/Earth_map.dds");

	if(!effect.load("data/planet.glsl")) {
		std::cout << "Error parsing effect: " << effect.getLog() << std::endl;
		return;
	} 

	program = effect.compileProgram("raycasting");
	if(!program) {
		std::cout << "Error compile program: " << effect.getLog() << std::endl;
		return;
	} 

	uCubeTex = program.getUniform("cubeTex");
	uWVP = program.getUniform("WVP");
	uCameraPos = program.getUniform("cameraPos");

	indexBuffer.data(GL_ELEMENT_ARRAY_BUFFER, sizeof(idxs), idxs, GL_STATIC_DRAW);

	vertexArray.bind();
	indexBuffer.bind();
	vertexArray.unbind();

}


void Scene::release() {
}

void Scene::draw() {

	glClear(GL_COLOR_BUFFER_BIT);
		
	program.use();
	vertexArray.bind();

	cubeTexture.bind(0);
	uCubeTex.set(0);

	uWVP.set(camera.getViewProj());
	uCameraPos.set(camera.getPos());

	glDrawElements(GL_TRIANGLES, 12*3, GL_UNSIGNED_SHORT, 0);

}

void Scene::move(float dx, float dy, float dz) {
	camera.move(glm::vec3(dx, dy, dz));
}

void Scene::rotate(float dx, float dy) {
	camera.rotate(glm::vec3(dx, dy, 0));
}
