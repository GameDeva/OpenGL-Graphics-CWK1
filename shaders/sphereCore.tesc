#version 430 core

// No. of vertices per patch
layout (vertices = 3) out;
in vec3 vPosition[];

out vec3 tcPosition[];

#define ID gl_InvocationID

uniform float tessLevel;

void main() 
{
	tcPosition[ID] = vPosition[ID];
	
	// This is done per patch
	if (ID == 0) {
        gl_TessLevelInner[0] = tessLevel;
        gl_TessLevelOuter[0] = tessLevel;
        gl_TessLevelOuter[1] = tessLevel;
        gl_TessLevelOuter[2] = tessLevel;
    } 
    
}