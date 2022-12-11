#version 330 core //version
out vec4 FragColor;

//uniform sampler2D tex0;

//uniform vec3 dirLightPos;
//uniform vec3 pointLightPos;

//uniform float dirIntensity;
//uniform float pointIntensity;

//uniform vec3 lightColor;
//uniform float ambientStr;
//uniform vec3 ambientColor;

//uniform vec3 cameraPos;
//uniform float specStr;
//uniform float specPhong;

//in vec2 texCoords;
//in vec3 normCoords;
in vec3 fragPos;

// color change
in vec3 v_Color;

void main(){
	//vec3 normal = normalize(normCoords);
	
	//vec3 lightDir = normalize(dirLightPos - fragPos);

	//float diff = max(dot(normal, lightDir), dirIntensity);

	//vec3 diffuse = diff * lightColor;

	//vec3 ambientCol = ambientStr * ambientColor;

	//vec3 viewDir = normalize(cameraPos - fragPos);
	//vec3 reflectDir = reflect(-lightDir, normal);

	//float spec = pow(max(dot(reflectDir, viewDir), 0.1f),specPhong);

	//vec3 specCol = spec * specStr * lightColor;
	// direction light
	//vec4 result = vec4(specCol + diffuse + ambientCol, 1.0);

	//lightDir = normalize(pointLightPos - fragPos);
	//diff = max(dot(normal, lightDir), pointIntensity);
	//diffuse = diff * lightColor;
	//viewDir = normalize(cameraPos - fragPos);
	//reflectDir = reflect(-lightDir, normal);
	//spec = pow(max(dot(reflectDir, viewDir), 0.1f),specPhong);
	/specCol = spec * specStr * lightColor;

	//float distance = length(pointLightPos - fragPos);

	//float attenuation = 1.0 / (1.0 + 0.0014 * distance + 0.000007 * (distance * distance));

	//specCol *= attenuation;
	//diffuse *= attenuation;
	//ambientCol *= attenuation;
	//point light
	//result += vec4(specCol + diffuse + ambientCol, 1.0);

	//FragColor = result * texture(tex0, texCoords);
	// color change
	FragColor = vec4(v_Color, 1.0);

}
