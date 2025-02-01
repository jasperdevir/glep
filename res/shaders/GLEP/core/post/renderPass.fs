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
    vec3 color = texture(uFramebuffer.color, v.uv).rgb;
    FragColor = vec4(color, 1.0);
} 