/*
Title: GPU Simulated Particle System
File Name: particleSystem.h
Copyright ? 2016
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
#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "../header/material.h"

struct Particle
{
    glm::vec4 m_position;
    glm::vec4 m_velocity;
    glm::vec4 m_color;
    float m_rotation;
    float m_angularVelocity;
    float m_age;
    // This variable is required, can't input data that isn't a multiple of 4
    // IF you comment this out, undefined weird stuff will happen.
    float buffer;
};


class ParticleSystem
{
public:
    ParticleSystem(Texture* texture);
    ~ParticleSystem();

    Material* GetMaterial();
    void Update(float dt);
    void Draw();

    // Position of the system.
    glm::vec3 m_position;

    // Time in seconds until particles are recycled.
    float m_lifeTime = 1.f;
    
    // global acceleration applied to all particles, defaults to 0
    glm::vec3 m_acceleration = glm::vec3(0, 0, 0);

    // size of particles
    glm::vec2 m_particleSize = glm::vec2(100, 100);

private:
    // The particle system will work with a predefined pool of particles, this makes things way faster than having a dynamic list.
    // You may be able to increase this number depending on your hardware.
    // I was able to run it smoothly with 65536 particles on an NVIDIA GTX 680
    static const int MAX_PARTICLES = 16348;
    Particle m_particles[MAX_PARTICLES];
    float m_internalTimer = 0;

    Material* m_particleRenderMat;
    Material* m_particleSimulateMat;

    GLuint m_vertexBuffer;

};