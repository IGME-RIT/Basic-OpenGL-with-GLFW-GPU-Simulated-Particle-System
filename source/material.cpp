/*
Title: GPU Simulated Particle System
File Name: material.cpp
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

#include "../header/material.h"

Material::Material(ShaderProgram * shaderProgram)
{
    // Increment the reference counter on the shader program.
    shaderProgram->IncRefCount();
    m_shaderProgram = shaderProgram;
}

Material::~Material()
{
    // Free shader program
    if (m_shaderProgram != nullptr)
        m_shaderProgram->DecRefCount();

    // Free textures
    for (int i = 0; i < m_textures.size(); i++)
    {
        m_textures[i]->DecRefCount();
    }
}

void Material::SetTexture(char* name, Texture* texture)
{
    // Bind shader program
    m_shaderProgram->Bind();

    // Request uniform from shader.
    GLint uniform = glGetUniformLocation(m_shaderProgram->GetGLShaderProgram(), name);

    // If there was no uniform location, print an error and return from the function.
    if (uniform == -1)
    {
        std::cout << "Uniform: " << name << " not found in shader program." << std::endl;
        return;
    }

    texture->IncRefCount();

    // Search through current texture uniforms to find a match.
    for (int i = 0; i < m_textureUniforms.size(); i++)
    {
        // If there's a match replace the texture.
        if (m_textureUniforms[i] == uniform)
        {
            m_textures[i]->DecRefCount();
            m_textures[i] = texture;
            return;
        }
    }

    // There is no match, add the new texture.
    m_textureUniforms.push_back(uniform);
    m_textures.push_back(texture);
}

void Material::SetMatrix(char* name, glm::mat4 matrix)
{
    // Bind shader program
    m_shaderProgram->Bind();

    // Request uniform
    GLint uniform = glGetUniformLocation(m_shaderProgram->GetGLShaderProgram(), name);

    // If there was no uniform location, print an error and return from the function.
    if (uniform == -1)
    {
        std::cout << "Uniform: " << name << " not found in shader program." << std::endl;
        return;
    }

    // Search through current matrix uniforms to find a match.
    for (int i = 0; i < m_matrixUniforms.size(); i++)
    {
        // If there's a match replace the matrix.
        if (m_matrixUniforms[i] == uniform)
        {
            m_matrices[i] = matrix;
            return;
        }
    }

    // There is no match, add the new matrix.
    m_matrixUniforms.push_back(uniform);
    m_matrices.push_back(matrix);
}

void Material::SetVec4(char * name, glm::vec4 vector)
{
    // Bind shader program
    m_shaderProgram->Bind();

    // Request uniform
    GLint uniform = glGetUniformLocation(m_shaderProgram->GetGLShaderProgram(), name);

    // If there was no uniform location, print an error and return from the function.
    if (uniform == -1)
    {
        std::cout << "Uniform: " << name << " not found in shader program." << std::endl;
        return;
    }

    // Search through current vector uniforms to find a match.
    for (int i = 0; i < m_vec4Uniforms.size(); i++)
    {
        // If there's a match replace the vector.
        if (m_vec4Uniforms[i] == uniform)
        {
            m_vec4s[i] = vector;
            return;
        }
    }

    // There is no match, add the new vector.
    m_vec4Uniforms.push_back(uniform);
    m_vec4s.push_back(vector);
}

void Material::SetVec3(char * name, glm::vec3 vector)
{
    // Bind shader program
    m_shaderProgram->Bind();

    // Request uniform
    GLint uniform = glGetUniformLocation(m_shaderProgram->GetGLShaderProgram(), name);

    // If there was no uniform location, print an error and return from the function.
    if (uniform == -1)
    {
        std::cout << "Uniform: " << name << " not found in shader program." << std::endl;
        return;
    }

    // Search through current vector uniforms to find a match.
    for (int i = 0; i < m_vec3Uniforms.size(); i++)
    {
        // If there's a match replace the vector.
        if (m_vec3Uniforms[i] == uniform)
        {
            m_vec3s[i] = vector;
            return;
        }
    }

    // There is no match, add the new vector.
    m_vec3Uniforms.push_back(uniform);
    m_vec3s.push_back(vector);
}

void Material::SetVec2(char * name, glm::vec2 vector)
{
    // Bind shader program
    m_shaderProgram->Bind();

    // Request uniform
    GLint uniform = glGetUniformLocation(m_shaderProgram->GetGLShaderProgram(), name);

    // If there was no uniform location, print an error and return from the function.
    if (uniform == -1)
    {
        std::cout << "Uniform: " << name << " not found in shader program." << std::endl;
        return;
    }

    // Search through current vector uniforms to find a match.
    for (int i = 0; i < m_vec2Uniforms.size(); i++)
    {
        // If there's a match replace the vector.
        if (m_vec2Uniforms[i] == uniform)
        {
            m_vec2s[i] = vector;
            return;
        }
    }

    // There is no match, add the new vector.
    m_vec2Uniforms.push_back(uniform);
    m_vec2s.push_back(vector);
}

void Material::SetFloat(char * name, float f)
{
    // Bind shader program
    m_shaderProgram->Bind();

    // Request uniform for matrix
    GLint uniform = glGetUniformLocation(m_shaderProgram->GetGLShaderProgram(), name);

    // If there was no uniform location, print an error and return from the function.
    if (uniform == -1)
    {
        std::cout << "Uniform: " << name << " not found in shader program." << std::endl;
        return;
    }

    // Search through current vector uniforms to find a match.
    for (int i = 0; i < m_floatUniforms.size(); i++)
    {
        // If there's a match replace the vector.
        if (m_floatUniforms[i] == uniform)
        {
            m_floats[i] = f;
            return;
        }
    }

    // There is no match, add the new vector.
    m_floatUniforms.push_back(uniform);
    m_floats.push_back(f);
}

void Material::SetInt(char * name, int newint)
{
    // Bind shader program
    m_shaderProgram->Bind();

    // Request uniform for matrix
    GLint uniform = glGetUniformLocation(m_shaderProgram->GetGLShaderProgram(), name);

    // If there was no uniform location, print an error and return from the function.
    if (uniform == -1)
    {
        std::cout << "Uniform: " << name << " not found in shader program." << std::endl;
        return;
    }

    // Search through current vector uniforms to find a match.
    for (int i = 0; i < m_intUniforms.size(); i++)
    {
        // If there's a match replace the vector.
        if (m_intUniforms[i] == uniform)
        {
            m_ints[i] = newint;
            return;
        }
    }

    // There is no match, add the new vector.
    m_intUniforms.push_back(uniform);
    m_ints.push_back(newint);
}


void Material::Bind()
{
    m_shaderProgram->Bind();

    // Bind all textures
    for (int i = 0; i < m_textureUniforms.size(); i++)
    {
        // This enum value can be incremented to bind to different texture locations
        glActiveTexture(GL_TEXTURE0 + i);

        // Bind the texture
        glBindTexture(GL_TEXTURE_2D, m_textures[i]->GetGLTexture());

        // Use the the texture from GL_TEXTURE0 + i at the given texture uniform location.
        glUniform1i(m_textureUniforms[i], i);
    }

    // Set all matrix data
    for (int i = 0; i < m_matrixUniforms.size(); i++)
    {
        glUniformMatrix4fv(m_matrixUniforms[i], 1, GL_FALSE, &(m_matrices[i][0][0]));
    }

    // Set all vector data
    for (int i = 0; i < m_vec4Uniforms.size(); i++)
    {
        glUniform4fv(m_vec4Uniforms[i], 1, &(m_vec4s[i][0]));
    }

    for (int i = 0; i < m_vec3Uniforms.size(); i++)
    {
        glUniform3fv(m_vec3Uniforms[i], 1, &(m_vec3s[i][0]));
    }

    for (int i = 0; i < m_vec2Uniforms.size(); i++)
    {
        glUniform2fv(m_vec2Uniforms[i], 1, &(m_vec2s[i][0]));
    }

    for (int i = 0; i < m_floatUniforms.size(); i++)
    {
        glUniform1fv(m_floatUniforms[i], 1, &(m_floats[i]));
    }

    for (int i = 0; i < m_intUniforms.size(); i++)
    {
        glUniform1iv(m_intUniforms[i], 1, &(m_ints[i]));
    }
}

void Material::Unbind()
{
    // Unbind all owned objects.
    for (int i = 0; i < m_textureUniforms.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    m_shaderProgram->Unbind();
}
