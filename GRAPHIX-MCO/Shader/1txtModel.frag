#version 330 core

uniform sampler2D tex0;
uniform vec3 lightPos, lightColor, ambientColor, cameraPos;
uniform float ambientStr, specStr, specPhong, intensity;

in vec2 texCoords;
in vec3 normCoords;
in vec3 fragPos;

in mat3 TBN;

out vec4 FragColor;

void main(){
	vec4 pixelColor0 = texture(tex0, texCoords);

	if(pixelColor0.a < 0.1){
		discard;
	}

	vec3 normal = normalize(normCoords);
	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(TBN * normal);

	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(dot(normal, lightDir), intensity);
	vec3 diffuse = diff * lightColor;

	vec3 ambientCol = ambientColor * ambientStr;

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
	vec3 specColor = specStr * lightColor;
	
	float distance = length(lightPos - fragPos);
	float atten = 1.0 / (distance * distance);

	//FragColor = vec4(atten * (specColor + diffuse + ambientCol), 1.0) * pixelColor0;
	FragColor = vec4(specColor + diffuse + ambientCol, 1.0) * pixelColor0;
}