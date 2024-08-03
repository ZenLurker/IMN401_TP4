#version 460

uniform vec4 Phong;
uniform vec3 diffuseAlbedo;

// Samplers
layout (binding = 0) uniform sampler2D T;
layout (binding = 1) uniform sampler2D normalMap;

in VTF {
    vec3 vL;
    vec3 vV;
    vec2 uv;
};

layout (location = 0) out vec4 Color;

void main()
{
    // Sample textures
    vec4 Texture = texture(T, uv);
    vec3 normalMapSample = texture(normalMap, uv).xyz;

    // Convert from [0, 1] to [-1, 1]
    vec3 N = normalize(normalMapSample * 2.0 - 1.0);

    // Normalize the light and view vectors
    vec3 L = normalize(vL);
    vec3 V = normalize(vV);
    vec3 R = reflect(-L, N);

    // Phong lighting model
    float Id = max(0, dot(L, N));
    float Is = pow(max(0, dot(R, V)), Phong.w);

    // Calculate final color
    Color.xyz = Phong.x * diffuseAlbedo * Texture.xyz + Phong.y * diffuseAlbedo * Id * Texture.xyz  + Phong.z * Is * vec3(1.0);
    Color.w = 100.0;
}
