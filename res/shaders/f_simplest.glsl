#version 330

uniform sampler2D textureMap0;
uniform sampler2D textureMap1;

out vec4 pixelColor;

in vec4 lightVector;
in vec4 viewerVector;
in vec4 normalVector;
in vec2 iTexCoord0;

void main(void) {

	//Znormalizowane interpolowane wektory
	vec4 normalizedLightVector = normalize(lightVector);
	vec4 normalizedViewerVector = normalize(viewerVector);
	vec4 normalizedNormalVector = normalize(normalVector);

	//Wektor odbity
	vec4 mr = reflect(-normalizedLightVector, normalizedNormalVector);

	//Parametry powierzchni
	vec4 ambient = texture(textureMap0, iTexCoord0);
	vec4 kd = texture(textureMap1, iTexCoord0) * ambient.r;
	vec4 ks = vec4(1,1,1,1);

	//Obliczenie modelu oświetlenia
	float nl = clamp(dot(normalizedNormalVector, normalizedLightVector), 0, 1);
	float rv = pow(clamp(dot(mr, normalizedViewerVector), 0, 1), 50);
	pixelColor= vec4(kd.rgb * nl, kd.a) + vec4(ks.rgb*rv, 0);
}
//
//    #version 330 core
//out vec4 FragColor;
//
//in vec3 Normal;
//in vec3 Position;
//
//uniform vec3 cameraPos;
//uniform samplerCube skybox;
//
//void main()
//{
//    vec3 I = normalize(Position - cameraPos);
//    vec3 R = reflect(I, normalize(Normal));
//    FragColor = vec4(texture(skybox, R).rgb, 1.0);
//}
