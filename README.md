# AssetLoader
This project is no longer being developed, but much of what was developed here is being used in another project in the repository https://github.com/mi7flat5/Infernal_Engine.

The origional purpose of this project was to learn the OpenGL API by creating an application that can load arbitrary 3D models for viewing. Mnay other areas of 3D graphics programming were touched during development, but because these features were not anticipated on the outset of development the applications design became brittle. 

## Features
A fairly comprehensive list of features:
* 3D model Loading
* Multiple element shader program loading (except geometry shader)
    * Vertex\Fragment
    * Vertex\Tesselation Evaluation\ Tesselation Control\Fragment
* Spherical 3rd person camera
* Multiple shaders
    * Reflection
    * Refraction
    * Diffuse
    * Diffuse Texture
    * Diffuse/ Normal-Bump map / Specular map
    * Paralax mapping
    * Tesselated heightmap
    * Instanced tesselated heightmap terrain 
    * Cube/Environment map
* Cubemap loading
* Terrain loading from arbitrary heightmap image
* Terrain detection
* Collision detection with GJK algorithm
* Collision data extraction through Expanding polytope algorithm
* Very simplistic collision reolution
* ray/sphere collision detection
* sphere/sphere collision detection
* Many Transofrmation and math functions
    * Matrix rotation
    * lookat and perspective matrix functions
    * linear interpolation
    * bilinear interpolation
    * Smoothstep and smootherstep curve functions
    * Barycentric coordinates 
    * Various templates for simple utilities (testing for NaN etc.)

## Examples

Several videos of this project in action can be found at https://www.youtube.com/playlist?list=PL7Ar0B8xTw3x3M0Y8fZdOPz4Woa1kU8rk
    