/*
Title: GPU Simulated Particle System
File Name: particleSystem.cpp
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

#include "particleSystem.h"

ParticleSystem::ParticleSystem(Texture* texture)
{
    // Setup the compute shader material for the particlue simulation

    ShaderProgram* simulationProgram = new ShaderProgram();
    simulationProgram->AttachShader(new Shader("../Assets/compute.glsl", GL_COMPUTE_SHADER));
    m_particleSimulateMat = new Material(simulationProgram);

    // Setup shaders and shader program.
    ShaderProgram* program = new ShaderProgram();
    program->AttachShader(new Shader("../Assets/vertex.glsl", GL_VERTEX_SHADER));
    program->AttachShader(new Shader("../Assets/geometry.glsl", GL_GEOMETRY_SHADER));
    program->AttachShader(new Shader("../Assets/fragment.glsl", GL_FRAGMENT_SHADER));
    m_particleRenderMat = new Material(program);
    m_particleRenderMat->Bind();
    m_particleRenderMat->SetTexture((char*)"tex", texture);

    // Create particle data.
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        // Get a reference to that particle, not a copy.
        Particle& p = m_particles[i];
        p.m_age = (float)i / MAX_PARTICLES;
        p.m_position = glm::vec4(0, 0, 0, 0);
        p.m_velocity = glm::vec4(0, 0, 0, 0);
        p.m_angularVelocity = 0;
        p.m_rotation = 0;
        p.m_color = glm::vec4(1, 0, 1, 1);
    }

    // Make a buffer for our particle data.
    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(Particle), m_particles, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

ParticleSystem::~ParticleSystem()
{
    glDeleteBuffers(1, &m_vertexBuffer);
    delete m_particleSimulateMat;
    delete m_particleRenderMat;
}

Material * ParticleSystem::GetMaterial()
{
    return m_particleRenderMat;
}

void ParticleSystem::Update(float dt)
{
    // We are binding the vertex buffer from our square.
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_vertexBuffer);

    // Same as with drawing, but we bind a compute shader program instead.
    // Set a bunch of values in the compute shader to use.
    m_particleSimulateMat->SetFloat((char*)"dt", dt);
    m_particleSimulateMat->SetFloat((char*)"burnRate", 1 / (float)m_lifeTime);
    m_particleSimulateMat->SetVec3((char*)"basePosition", m_position);
    m_particleSimulateMat->SetVec3((char*)"acceleration", m_acceleration);
    
	// bind, execute the compute program, and unbind
	m_particleSimulateMat->Bind();
    glDispatchCompute(MAX_PARTICLES, 1, 1);
    m_particleSimulateMat->Unbind();
	
	// unbind vertex buffer
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
}

void ParticleSystem::Draw()
{
    // Enable blending when rendering particles
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // Bind the vertex buffer. and set up attributes
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(0));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(sizeof(float) * 4));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(sizeof(float) * 8));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(sizeof(float) * 12));
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(sizeof(float) * 13));
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(sizeof(float) * 14));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // use the vertex attributes we just declared
    for (int i = 0; i < 6; i++)
    {
        glEnableVertexAttribArray(i);
    }


    // The particle size is used in the geometry shader to create quads.
    m_particleRenderMat->SetVec2((char*)"particleSize", m_particleSize);

    // Bind material and draw
    m_particleRenderMat->Bind();

    // The geometry shader is expecting points, so we call draw with points, once for each particle.
    glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);



    // reset everything:
    m_particleRenderMat->Unbind();
    for (int i = 0; i < 6; i++)
    {
        glDisableVertexAttribArray(i);
    }
    glDisable(GL_BLEND);
}
