# Basic OpenGL with GLFW: GPU Simulated Particle System

Looping over and updating particles wastes most of the potential we gain from being able to render them so quickly.
We also had to buffer our data every frame.
This example demonstrates the use of compute shaders to update and maintain a collection of particles in parallel. 

# Setup

You will need to have CMake installed on your computer, and properly added to your path.
In order to setup, run the following in a shell, then open the project in your preferred editor.
Windows setup has been configured for use with Visual Studio.

Windows:
```
cd path/to/folder
setup.cmd
```
Linux:
```
cd path/to/folder
./setup
```
