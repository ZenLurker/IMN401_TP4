#version 460

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;
uniform vec3 PosCam;
uniform vec3 PosLum;
uniform float Time;

out gl_PerVertex {
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
};

layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec3 VertexNormal;
layout (location = 3) in vec3 VertexTextureCoordinate;
layout (location = 4) in vec4 VertexTangente;

out VTF {
    vec3 vL;
    vec3 vV;
    vec2 uv;
};

void main()
{
    gl_Position = Proj * View * Model * vec4(VertexPosition, 1.0);
    uv = VertexTextureCoordinate.xy;
    
    // Calculate binormal
    vec3 Binormal = cross(VertexNormal, VertexTangente.xyz); 

    // Create the TBN matrix
    mat3 TBN = mat3(normalize(VertexTangente.xyz), normalize(Binormal), normalize(VertexNormal)); 
    TBN = transpose(TBN);

    // Transform light and view vectors into the TBN space
    vL = TBN * (PosLum - VertexPosition);
    vV = TBN * (PosCam - VertexPosition);
}