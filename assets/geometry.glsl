/*
Title: GPU Simulated Particle System
File Name: geometry.glsl
Copyright ? 2016, 2019
Author: David Erbelding, Niko Procopi
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

// Scale used to convert particle size based on screen dimensions and particle size.
uniform vec2 particleSize;
uniform vec2 viewport;

layout(points) in;

in vec4 vertOutColor[];
in float vertOutRotation[];
in float vertOutAge[];

layout(triangle_strip, max_vertices = 4) out;

out vec2 uv;
out vec4 color;

void main()
{
	// Make the color fade with age (in 1 to 0 range)
	color = vertOutColor[0];

	// Get the base position of the particle on screen.
	vec4 pos = gl_in[0].gl_Position;
	
	// Make a transform matrix that rotates and scales vertices.
	float c = cos(vertOutRotation[0]);
	float s = sin(vertOutRotation[0]);
	vec2 scale = particleSize / viewport;
	mat2 T = mat2(c * scale.x, s * scale.y, -s * scale.x, c * scale.y) * vertOutAge[0];

	// Make a quad:
	// Generate 4 vertices
	
	for(int i = 0; i < 4; i++)
	{
		uv = vec2(i % 2, i/2);
		vec2 offset = T * (uv - vec2(0.5));
		gl_Position = pos + vec4(offset, 0, 0);
		EmitVertex();
	}

	// Finished making quad
	EndPrimitive();
}