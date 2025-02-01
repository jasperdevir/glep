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

uniform Framebuffer uFramebuffer;

void main()
{
    float depth = texture(uFramebuffer.depth, v.uv).r;
    FragColor = vec4(vec3(depth), 1.0);
} 