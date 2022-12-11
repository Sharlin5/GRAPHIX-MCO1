#version 330 core

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D norm0;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 ambientColor;
uniform vec3 cameraPos;

uniform float ambientStr;
uniform float specStr;
uniform float specPhong;
uniform float intensity;

in vec2 texCoord;
in vec3 normCoord;
in vec3 fragPos;

in mat3 TBN;

out vec4 FragColor;

void main(){
	vec4 pixelColor0 = texture(tex0, texCoord);
	vec4 pixelColor1 = texture(tex1, texCoord);
	vec4 pixelColor2 = texture(tex2, texCoord);
	vec4 pixelColor3 = texture(tex3, texCoord);

	normal = texture(norm0, texCoord).rgb;
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

	FragColor = vec4(atten * (specColor + diffuse + ambientCol), 1.0) * pixelColor0 * pixelColor 1 * pixelColor2 * pixelColor3 * normal;
}