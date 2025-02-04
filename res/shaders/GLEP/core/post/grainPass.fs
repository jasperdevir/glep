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
    vec2 uvScale;
    float intensity;

    Framebuffer framebuffer;
};

uniform Material uMaterial;

vec2 random2(vec2 st){
    st = vec2( dot(st,vec2(127.1,311.7)),
            dot(st,vec2(269.5,183.3)) );
    return -1.0 + 2.0*fract(sin(st)*43758.5453123);
}

//2D Gradient Noise
float gnoise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    vec2 u = f*f*(3.0-2.0*f);

    return mix( mix( dot( random2(i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ),
                    dot( random2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), u.x),
                mix( dot( random2(i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ),
                    dot( random2(i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), u.x), u.y);
}

//Fractal Brownian Motion
float fbm (vec2 st, int octaves, float amplitude, float lacunarity) {
    float value = 0.;

    for (int i = 0; i < octaves; i++) {
    
        value += amplitude * gnoise(st);
        st *= lacunarity;
        amplitude *= .5;
    }
    return value;
}

void main(){
    float noise = (fbm(v.uv * uMaterial.uvScale, 8, 1.0f, 3.0f) + 1.0f) * 0.5f;
    noise *= uMaterial.intensity;
    vec3 col = texture(uMaterial.framebuffer.color, v.uv).rgb * noise;

    FragColor = vec4(col, 1.0f);
}