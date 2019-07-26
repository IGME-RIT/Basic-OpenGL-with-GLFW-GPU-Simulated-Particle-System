/*
Title: GPU Simulated Particle System
File Name: compute.glsl
Copyright © 2016
Original authors: David Erbelding
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

// Compute shaders are part of openGL core since version 4.3
#version 430


// Inputs from the particle system.
uniform vec3 basePosition;
uniform vec3 acceleration;
uniform float burnRate;
uniform float dt;


// A basic definition of what our vertex data looks like.
struct VertexData
{
    vec4 position;
    vec4 velocity;
    vec4 color;
    float rotation;
    float angularVelocity;
    float age;
};


// A layout describing the vertex buffer.
layout(binding = 0) buffer block
{
	VertexData data[];
} outBuffer;


// This will just run once for each particle.
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

// Declare main program function which is executed when
void main()
{

	// Get the index of this object into the buffer
	uint i = gl_GlobalInvocationID.x;

	// Increment particle life
    outBuffer.data[i].age -= dt * burnRate;

	float age = outBuffer.data[i].age;

	// If the particle has reached the end of its life, reset it.
	if (age < 0)
    {
		// An arbitrary "random" function found on the internet.
		// Gets a value between 0 and 1.
		float rand = fract(sin(dot(vec2(dt, i) ,vec2(12.9898,78.233))) * 43758.5453);
		
		// Has to increment by one instead of just setting to one.
		// Otherwise, when multiple particles reset in the same frame, they would become permanently synced.
		outBuffer.data[i].age += 1;

		// Starting rotation and angular velocity are distributed "randomly"
		outBuffer.data[i].rotation = i % 7;
		outBuffer.data[i].angularVelocity = i % 11;

		// Move the particle back to the center.
		outBuffer.data[i].position = vec4(basePosition, 1);

		// Send the particle in a random direction.
		outBuffer.data[i].velocity = vec4(cos(i + rand) * (5f), 0, sin(i + rand) * (5f), 0);
	}

	// Update the particle position
    outBuffer.data[i].position += outBuffer.data[i].velocity * dt;

	// Dampen Velocity over time.
	outBuffer.data[i].velocity -= outBuffer.data[i].velocity * dt * 5f;

	// Apply acceleration
    outBuffer.data[i].velocity += vec4(acceleration, 0) * dt;

	// Apply rotation
    outBuffer.data[i].rotation += outBuffer.data[i].angularVelocity * dt;

	// Arbitrary color change formula, makes colors look cool
	outBuffer.data[i].color = vec4(.2f / age, .1f / age, .6f * age, 1);
	
}