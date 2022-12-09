#version 330 core

uniform sampler2D tex0, tex1, tex2, tex3, norm0;
uniform vec3 lightPos, lightColor, ambientColor, cameraPos;
uniform float ambientStr, specStr, specPhong, intensity;

in vec2 texCoord;
in vec3 normCoord;
in vec3 fragPos;

in mat3 TBN;

out vec4 FragColor;

void main(){
	vec4 pixelColor0 = texture(tex0, texCoords);
	vec4 pixelColor1 = texture(tex1, texCoords);
	vec4 pixelColor2 = texture(tex2, texCoords);
	vec4 pixelColor3 = texture(tex3, texCoords);

	if(pixelColor0.a < 0.1){
		discard;
	}
	if(pixelColor1.a < 0.1){
		discard;
	}
	if(pixelColor2.a < 0.1){
		discard;
	}
	if(pixelColor3.a < 0.1){
		discard;

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

	FragColor = vec4(atten * (specColor + diffuse + ambientCol), 1.0) * texture(tex0, texCoord) * texture(tex1, texCoord) * texture(tex2, texCoord) * texture(tex3, texCoord), texture(norm0, normCoord);
}