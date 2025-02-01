#version 330 core
out vec4 FragColor;

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 uv;
};

struct GLEPInfo{
    float time;
    float deltaTime;
};

struct Framebuffer{
    sampler2D color;
    sampler2D depth;
};

in Vertex v;
in GLEPInfo i;

uniform Framebuffer uFramebuffer;

void main()
{
    vec3 color = texture(uFramebuffer.color, v.uv).rgb;
    color.r *= (sin(i.time * 0.5f) + 1.0f) * 0.5f;
    color.g *= (sin(i.time * 1.0f) + 1.0f) * 0.5f;
    color.b *= (sin(i.time* 1.5f) + 1.0f) * 0.5f;
    
    FragColor = vec4(color, 1.0);
} 