#version 330 core

struct Vertex {
    vec3 position;
    vec4 lightSpacePosition;
    vec3 normal;
    vec2 uv;
};

struct GLEPInfo {
    float time;
    float deltaTime;
    vec3 viewPos;
};

struct Material {
    samplerCube cubeMap;
    float refractiveIndex;
    vec4 tint;
};

in Vertex v;
in GLEPInfo i;
out vec4 FragColor;

uniform Material uMaterial;

void main()
{             
    float ratio = 1.00 / uMaterial.refractiveIndex;
    vec3 I = normalize(v.position - i.viewPos);
    vec3 R = refract(I, normalize(v.normal), ratio);

    vec3 finalColor = texture(uMaterial.cubeMap, R).rgb;
    finalColor *= uMaterial.tint.rgb;

    FragColor = vec4(finalColor, 1.0);
}