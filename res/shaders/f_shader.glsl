#version 330

uniform sampler2D textureMap0;
uniform sampler2D textureMap1;
//uniform samplerCube skybox;


out vec4 pixelColor;

in vec4 lightVector[5];
in vec4 viewerVector;
in vec4 normalVector;
in vec2 iTexCoord0;

//vec4 skyboxColors() {
//  vec4 normalizedViewerVector = normalize(viewerVector);
//  vec4 normalizedNormalVector = normalize(normalVector);
//	vec3 R = reflect(-normalizedViewerVector, normalize(normalizedNormalVector));
//	return vec4(texture(skybox, R).rgb, 0);
//}
vec4 color(vec4 lightVector){
    vec4 normalizedLightVector = normalize(lightVector);
    vec4 normalizedViewerVector = normalize(viewerVector);
    vec4 normalizedNormalVector = normalize(normalVector);

    //Wektor odbity
    vec4 reflectedVector = reflect(-normalizedLightVector, normalizedNormalVector);

    //Parametry powierzchni
    float ambient = texture(textureMap0, iTexCoord0).r;
    vec4 kd = texture(textureMap1, iTexCoord0) * ambient;
    vec4 ks = vec4(1, 1, 1, 1);

    //Obliczenie modelu oświetlenia
    float nl = clamp(dot(normalizedNormalVector, normalizedLightVector), 0, 1);
    float rv = pow(clamp(dot(reflectedVector, normalizedViewerVector), 0, 1), 100);
    return vec4(kd.rgb*nl,0) + vec4(ks.rgb*rv, 0);
}

void main(void) {
    pixelColor = vec4(0,0,0,1); // +skyboxColors();
    for (int i = 0; i< 5; i++){
        pixelColor += color(lightVector[i]);
    }
   // pixelColor = clamp(pixelColor,0,1);
}
