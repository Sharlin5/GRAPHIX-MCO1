#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 aTex;

out vec2 texCoord;
out vec3 normCoord;
out vec3 fragPos;

//transform matrix
uniform mat4 transform;

//projection matrix
uniform mat4 projection;

//view matrix
uniform mat4 view;

//colors
uniform vec3 aColor;
out vec3 v_Color;

void main(){
	// color change values
	v_Color = aColor;

	gl_Position = projection * view * transform * vec4(aPos, 1.0);
	
	texCoord = aTex;
	
	normCoord = mat3(transpose(inverse (transform))) * vertexNormal;
	
	fragPos = vec3 (transform * vec4(aPos, 1.0));
}