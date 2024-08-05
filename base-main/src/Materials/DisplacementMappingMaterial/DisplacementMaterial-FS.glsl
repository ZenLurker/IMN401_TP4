#version 460

uniform vec4 Phong;
uniform vec3 diffuseAlbedo;

layout (binding = 0) uniform sampler2D T;

in VTF {
    vec3 vL;
    vec3 vV;
    vec2 uv;
    vec3 N;  // Input the normal
};

layout (location = 0) out vec4 Color;

void main()
{
    vec4 Texture = texture(T, uv);

    vec3 L = normalize(vL);
    vec3 V = normalize(vV);
    vec3 N = normalize(N);  // Use the normal from the vertex shader
    vec3 R = reflect(-L, N);

    float Id = max(0, dot(L, N));
    float Is = pow(max(0, dot(R, V)), Phong.w);

    Color.xyz = Phong.x * diffuseAlbedo * Texture.xyz + Phong.y * diffuseAlbedo * Id * Texture.xyz + Phong.z * Is * vec3(1.0);
    Color.w = 1.0;  // Set the alpha value to 1.0
}
