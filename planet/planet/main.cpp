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

#include <GL/glew.h>  
#include <GLFW/glfw3.h>  

#include <stdio.h>  
#include <stdlib.h>  

#include "scene.h"

static Scene scene;
static float dx = 0, dy = 0, dz = 0;

//Define the key input callback  
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {  
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)  
		glfwSetWindowShouldClose(window, GL_TRUE);  

}  

int main() {  
	//Initialize GLFW  
	glewExperimental = true; 
	if (!glfwInit())  
		exit(EXIT_FAILURE);  

	//Set the GLFW window creation hints - these are optional  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version  
	glfwWindowHint(GLFW_SAMPLES, 4);   
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  

	GLFWwindow* window = glfwCreateWindow(1024, 640, "Planet Raycasting", NULL, NULL);  
	if (!window) {  
		fprintf( stderr, "Failed to open GLFW window.\n" );  
		glfwTerminate();  
		exit(EXIT_FAILURE);  
	}  

	glfwMakeContextCurrent(window);  
	glfwSetKeyCallback(window, key_callback);  

	GLenum err = glewInit();  
	if (err != GLEW_OK) {  
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));  
		return -1;  
	}  

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	scene.init(width, height);

	glClearColor(0.45f, 0.6f, 0.73f, 1.0f);  
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Main Loop  
	while(!glfwWindowShouldClose(window)) {
		static double lastTime = glfwGetTime();
		double time = glfwGetTime();
		double dt = time - lastTime;
		lastTime = time;

		float speed = (float)dt;
		dz = (float)(glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS) - (glfwGetKey(window, GLFW_KEY_S)==GLFW_PRESS);
		dx = (float)(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) - (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);
		if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)==GLFW_PRESS)
			speed*=10.0f;
		if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)==GLFW_PRESS)
			speed*=0.1f;
		scene.move(dx*speed, dy*speed, dz*speed);

		static bool mouseLocked = false;
		static double prevX, prevY;
		if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS) {
			if(!mouseLocked) {
				mouseLocked = true;
				glfwGetCursorPos(window, &prevX, &prevY);
			} else {
				double mX, mY;
				glfwGetCursorPos(window, &mX, &mY);
				scene.rotate(float(mX - prevX)/width, float(mY - prevY)/height);
				prevX = mX;
				prevY = mY;
			}
		} else {
			mouseLocked = false;
		}

		scene.draw();

		glfwSwapBuffers(window);  
		glfwPollEvents();  

	}    

	glfwDestroyWindow(window);  
	glfwTerminate();  

	exit(EXIT_SUCCESS);  
} 

