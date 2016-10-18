/*
Title: GPU Simulated Particle System
File Name: vertex.glsl
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


#version 400 core

// camera view projection matrix.
uniform mat4 cameraView;

// Vertex attributes for every variable in the particle struct
layout(location = 0) in vec4 in_position;
layout(location = 1) in vec4 in_velocity;
layout(location = 2) in vec4 in_color;
layout(location = 3) in float in_rotation;
layout(location = 4) in float in_angular;
layout(location = 5) in float in_age;

out vec4 vertOutColor;
out float vertOutRotation;
out float vertOutAge;

void main(void)
{
	// Move the vertex position into clip space.
	gl_Position = cameraView * in_position;

	// Pass color, rotation, and age forward to the geometry shader.
	vertOutColor = in_color;
	vertOutRotation = in_rotation;
	vertOutAge = in_age;
}