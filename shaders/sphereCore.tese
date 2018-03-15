#version 430 core

layout (triangles, equal_spacing, cw) in;
in vec3 tcPosition[];

out vec3 FragPos;  
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float sphereRadius;

void main() 
{ 
    vec3 p0 = gl_TessCoord.x * tcPosition[0];
    vec3 p1 = gl_TessCoord.y * tcPosition[1];
    vec3 p2 = gl_TessCoord.z * tcPosition[2];

    vec3 tePosition = normalize(p0 + p1 + p2);
	tePosition *= sphereRadius;

    vec3 a = tcPosition[1] - tcPosition[0];
    vec3 b = tcPosition[2] - tcPosition[0];
    
    FragPos = vec3(model * vec4(tePosition, 1.0));

    Normal = vec3((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x *b.y) - (a.y *b.x));
    
    Normal = mat3(transpose(inverse(model))) * Normal;  

    gl_Position = projection * view * model * vec4(FragPos, 1.0f);
	
}