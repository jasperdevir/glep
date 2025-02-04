#version 330 core
out vec4 FragColor;

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 uv;
};

in Vertex v;

struct Framebuffer{
    sampler2D color;
    sampler2D depth;
};

struct Material{
    float nearPlane;
    float farPlane;
    vec4 fogColor;

    Framebuffer framebuffer;
};

uniform Material uMaterial;

void main()
{
    float depth = texture(uMaterial.framebuffer.depth, v.uv).r;
    float z = depth * 2.0 - 1.0;
    float ndc = (2.0 * uMaterial.nearPlane * uMaterial.farPlane) / (uMaterial.farPlane + uMaterial.nearPlane - z * (uMaterial.farPlane - uMaterial.nearPlane));

    vec3 color = texture(uMaterial.framebuffer.color, v.uv).rgb;
    if(color.r <= 0.01 && color.g <= 0.01 && color.b <= 0.01 && ndc > 1.0) discard;
    
    color = mix(color, uMaterial.fogColor.rgb, ndc / uMaterial.farPlane);
    //color *= 1.0 - ndc;

    FragColor = vec4(color, 1.0);
} 