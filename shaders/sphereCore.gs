#version 330
// THIS IS EXTREMELY USELESS, (as of now)
// Hence I have not compiled or attached it to the sphereShader

layout (triangles) in;		//  The below can be used to tell the GS to run 3 times on each primitive
// -->layout(triangles, invocations = 3) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 tePosition[3];

void main()
{
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();
	EndPrimitive();
}