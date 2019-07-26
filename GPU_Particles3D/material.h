/*
Title: GPU Simulated Particle System
File Name: material.h
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
#include "shaderProgram.h"
#include "texture.h"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>

class Material
{

private:
    // Shader program
    ShaderProgram* m_shaderProgram = nullptr;

    // Texture uniforms in use.
    std::vector<GLuint> m_textureUniforms;
    // Texture objects.
    std::vector<Texture*> m_textures;

    // Uniform for matrix.
    std::vector<GLuint> m_matrixUniforms;
    // Matrices to bind with material.
    std::vector<glm::mat4> m_matrices;

    // Uniform for vector.
    std::vector<GLuint> m_vec4Uniforms;
    std::vector<GLuint> m_vec3Uniforms;
    std::vector<GLuint> m_vec2Uniforms;
    std::vector<GLuint> m_floatUniforms;
    std::vector<GLuint> m_intUniforms;
    // vectors to bind with material.
    std::vector<glm::vec4> m_vec4s;
    std::vector<glm::vec3> m_vec3s;
    std::vector<glm::vec2> m_vec2s;
    std::vector<float> m_floats;
    std::vector<int> m_ints;

public:
    // Create a material using a given shader program.
    // If you want to use a different shader program, create a new material.
    Material(ShaderProgram* shaderProgram);
    ~Material();
    void SetTexture(char* name, Texture* texture);
    void SetMatrix(char* name, glm::mat4 matrix);
    void SetVec4(char* name, glm::vec4 vector);
    void SetVec3(char* name, glm::vec3 vector);
    void SetVec2(char* name, glm::vec2 vector);
    void SetFloat(char* name, float f);
    void SetInt(char* name, int i);

    void Bind();
    void Unbind();
};