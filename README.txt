Documentation Author: Niko Procopi 2019

This tutorial was designed for Visual Studio 2017 / 2019
If the solution does not compile, retarget the solution
to a different version of the Windows SDK. If you do not
have any version of the Windows SDK, it can be installed
from the Visual Studio Installer Tool

Welcome to the 3D Particles-Compute Tutorial!
Prerequesites: 3D Particles

In this tutorial, just like the last tutorial, we initialize 
our particles and putting them in a Vertex Buffer (in GPU memory).

In the last tutorial, we updated all the particles on the CPU
in C++, and then we copied the updated particles back into the 
Vertex Buffer, every frame. 

We will use a compute shader to edit the vertex buffer. The 
compute shader will run on the GPU with GLSL; it will read
each particle from the vertex buffer, update the particle,
and then write the new particle in the vertex buffer. The
entire C++ algorithm for updating particles will be running
on the GPU. The buffer that the compute shader receives does
not need to be a vertex buffer, it can be uniform, but
in this scenario, a vertex buffer is most convenient to us.

This does not involve destroying and rebuilding vertex buffers,
this is one vertex buffer that we create one time.
The Vertex Shader reads particles from the Vertex Buffer,
sends them to the Geometry Shader, then they get sent to the
Fragment Shader, and then to the screen. After that, the 
Compute Shader reads that same Vertex Buffer, and alters all
the data depending on the physics equations that we give it,
then the next frame, the Vertex Shader reads the same Vertex
Buffer (which now has altered data), and the cycle repeats.

Each frame, the compute shader needs to finish before the 
graphics pipeline can begin. Otherwise, the vertex shader
might accidentally read a vertex that the compute shader is
writing to, which can cause corruption.

The Vertex, Geometry, and Fragment shaders in this tutorial
are identical to the previous tutorial.

[ParticleSystem.cpp]
In ParticleSystem constructor, we initialize  a shader
program with the compute shader, and we make a material
for this program. We also make another shader program
with the vertex, geometry, and fragment shaders.
The compute program and the graphics program are two 
seperate programs, each with their own materials, which
will each be executed seperately.

We loop through all particles to initialize them all,
then we put them into a vertex buffer with type 
GL_STATIC_DRAW. We will be changing the vertex buffer
a lot, but we will not be changing due to CPU, so
we make it static. We only make it dynamic if the CPU
is changing the buffer often, not if the GPU is changing
the buffer.

In ParticleSystem::Update, we use the compute pipeline,
which is just the compute shader. On line 79, we give
the vertex buffer to the compute program. After that,
we provide the necessary uniforms (time, burn rate, etc).
After that, we execute the compute progrma, and then we
unbind the vertex shader.

In ParticleSystem::Draw, we use the graphics pipeline,
which is made of the vertex, geometry, and fragment shaders.
This program executes exactly the same as previous tutorials.

[Compute Shader]

First we take in Uniforms, which will be the same for every particle
	uniform vec3 basePosition;
	uniform vec3 acceleration;
	uniform float burnRate;
	uniform float dt;
	
We define the particle struct, just like previous tutorials

We tell the shader to expect a buffer. Keep in mind though,
this is a Vertex Buffer, not a Uniform Buffer, so we are 
not restricted to 65kb, we can have as many paraticles as we
want; we can have one million if we want
	layout(binding = 0) buffer block
	{
		VertexData data[];
	} outBuffer;
	
In main(), we update every particle. Given the
prerequisites, it is easy to understand how this
function works. It alters position and color, (etc), 
based on velocity and age of the particle.

How to improve:
Use Asynchronous Compute to run the compute shader and the 
graphics pipeline at the same time.
