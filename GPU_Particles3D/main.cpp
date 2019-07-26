/*
Title: GPU Simulated Particle System
File Name: main.cpp
Copyright � 2016
Author: David Erbelding
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include "shader.h"
#include "texture.h"
#include "particleSystem.h"
#include "fpsController.h"

glm::vec2 viewportDimensions = glm::vec2(800, 600);
glm::vec2 mousePosition;
ParticleSystem* particleSystem;


// Window resize callback
void resizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    viewportDimensions = glm::vec2(width, height);
}

// This will get called when the mouse moves.
void mouseMoveCallback(GLFWwindow *window, GLdouble mouseX, GLdouble mouseY)
{
    mousePosition = glm::vec2(mouseX, mouseY);
}

// Change settings when keys are pressed
void keyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_R)
    {
        particleSystem->m_acceleration.y += 1.f;
    }
    if (key == GLFW_KEY_F)
    {
        particleSystem->m_acceleration.y -= 1.f;
    }
    if (key == GLFW_KEY_T)
    {
        particleSystem->m_particleSize += 50;
    }
    if (key == GLFW_KEY_G)
    {
        particleSystem->m_particleSize -= 50;
    }
}

int main(int argc, char **argv)
{
	// Initializes the GLFW library
	glfwInit();

	// Initialize window
	GLFWwindow* window = glfwCreateWindow(viewportDimensions.x, viewportDimensions.y, "Particles!", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	//set resize callback and mouse movement callback
	glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwSetCursorPosCallback(window, mouseMoveCallback);
    glfwSetKeyCallback(window, keyPressCallback);

	// Initializes the glew library
	glewInit();



    // Initialize the particle system class with a bunch of parameters:
    particleSystem = new ParticleSystem(new Texture((char*)"../assets/particle.png"));
    particleSystem->m_position = glm::vec3(0, 0, -.5);
    particleSystem->m_lifeTime = 1.0f;
    particleSystem->m_acceleration = glm::vec3(0, 0, 0);
    particleSystem->m_particleSize = glm::vec2(100, 100);


    std::cout << "Controls:" << std::endl;
    std::cout << "Use the mouse to look around, and wasd to move." << std::endl;
    std::cout << "R and F control acceleration." << std::endl;
    std::cout << "T and G control particle size." << std::endl;
    std::cout << "Press escape to exit the demo." << std::endl;

    // Make a first person controller for the camera.
    FPSController controller = FPSController();


    float frames = 0;
    float secCounter = 0;
    float totalTime = 0;
	// Main Loop
	while (!glfwWindowShouldClose(window))
	{
        // Exit when escape is pressed.
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;

        // Calculate delta time and frame rate
        float dt = glfwGetTime();
        totalTime += dt;
        frames++;
        secCounter += dt;
        if (secCounter > 1.f)
        {
            std::string title = "FPS: " + std::to_string(frames);
            glfwSetWindowTitle(window, title.c_str());
            secCounter = 0;
            frames = 0;
        }
        glfwSetTime(0);

        //////////////////
        // Update      //
        ////////////////

        // Update the player controller.
        controller.Update(window, viewportDimensions, mousePosition, dt);

        // Update the particle simulation
        // This is what runs the compute shader.
        particleSystem->Update(dt);

        /////////////////
        // Draw       //
        ///////////////

        // Calculate view-projection matrix.
        glm::mat4 viewMatrix = controller.GetTransform().GetInverseMatrix();
        glm::mat4 perspectiveProjection = glm::perspective(.75f, viewportDimensions.x / viewportDimensions.y, .1f, 100.f);
        glm::mat4 viewProjection = perspectiveProjection * viewMatrix;
        
        // The view projection matrix will be used in the vertex shader to move the particle.
        particleSystem->GetMaterial()->SetMatrix((char*)"cameraView", viewProjection);
        // The viewport dimensions are needed in the geometry shader to make a correctly sized quad.
        particleSystem->GetMaterial()->SetVec2((char*)"viewport", viewportDimensions);

		// Clear the screen.
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 0.0);

        // Tell Particle System to draw.
        particleSystem->Draw();


		// Swap the backbuffer to the front.
		glfwSwapBuffers(window);

		// Poll input and window events.
		glfwPollEvents();

	}


    delete particleSystem;

	// Free GLFW memory.
	glfwTerminate();


	// End of Program.
	return 0;
}
