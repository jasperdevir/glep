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

struct Material {
    Framebuffer framebuffer;
};

uniform Material uMaterial;

void main()
{
    vec3 color = texture(uMaterial.framebuffer.color, v.uv).rgb;
    FragColor = vec4(color, 1.0);
} 