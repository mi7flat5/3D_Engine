#version 450 core
layout(location = 0) in vec3 VP;

uniform mat4 Projection;
uniform mat4 Model;
uniform mat4 View;


5
void main()
{	
	
	gl_Position =    vec4(VP,1.0f);

}5