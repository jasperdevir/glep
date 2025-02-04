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
    float offset;
    float kernel[9];

    Framebuffer framebuffer;
};

uniform Material uMaterial;

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-1.0f, 1.0f), 
        vec2(0.0f, 1.0f), 
        vec2(1.0f, 1.0f), 
        vec2(-1.0f, 0.0f),  
        vec2(0.0f, 0.0f),  
        vec2(1.0f, 0.0f), 
        vec2(-1.0f, -1.0f),
        vec2(0.0f, -1.0f), 
        vec2(1.0f, -1.0f) 
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(uMaterial.framebuffer.color, v.uv + offsets[i] * uMaterial.offset));
    }

    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++){
        col += sampleTex[i] * uMaterial.kernel[i];
    }
        
    FragColor = vec4(col, 1.0);
} 