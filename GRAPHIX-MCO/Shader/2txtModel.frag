#version 330 core

uniform sampler2D tex0, tex1, norm0;
uniform vec3 dirLightPos, pointLightPos, dirIntensity, pointIntensity;
uniform vec3 lightPos, lightColor, ambientColor, cameraPos;
uniform float ambientStr, specStr, specPhong, intensity;

in vec2 texCoords;
in vec3 normCoords;
in vec3 fragPos;
in mat3 TBN;

out vec4 FragColor;

void main(){
	vec4 pixelColor0 = texture(tex0, texCoords);
	vec4 pixelColor1 = texture(tex1, texCoords);

	if(pixelColor0.a < 0.1){
		discard;
	}
	if(pixelColor1.a < 0.1){
		discard;
	}

	vec3 normal = texture(norm0, texCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(TBN * normal);

	vec3 lightDir = normalize(lightPos - fragPos);
	//vec3 lightDir = normalize(dirLightPos - fragPos);

	float diff = max(dot(normal, lightDir), intensity);
	vec3 diffuse = diff * lightColor;

	vec3 ambientCol = ambientColor * ambientStr;

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
	//vec3 specColor = specStr * lightColor;
	vec3 specColor = spec * specStr * lightColor;

	vec4 result = vec4(specColor + diffuse + ambientCol, 1.0);

	// point light
	lightDir = normalize(pointLightPos - fragPos);
	diff = max(dot(normal, lightDir), intensity);
	diffuse = diff * lightColor;
	viewDir = normalize(cameraPos - fragPos);
	reflectDir = reflect(-lightDir, normal);
	spec = pow(max(dot(reflectDir, viewDir), 0.1f),specPhong);
	specColor = spec * specStr * lightColor;

	float distance = length(pointLightPos - fragPos);

	float attenuation = 1.0 / (1.0 + 0.0014 * distance + 0.000007 * (distance * distance));

	specColor *= attenuation;
	diffuse *= attenuation;
	ambientCol *= attenuation;
	//point light
	result += vec4(specColor + diffuse + ambientCol, 1.0);

	FragColor = result * pixelColor0 * pixelColor1;

	//FragColor = vec4(specColor + diffuse + ambientCol, 1.0) * pixelColor0 * pixelColor1;
}