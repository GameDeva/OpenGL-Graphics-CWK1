#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform int lightingFlag;

void main()
{
	if(lightingFlag == 1) 
	{
		FragColor = vec4(0.5f, 0.2f, 0.8f, 1.0f);
	} else {
		FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
} 