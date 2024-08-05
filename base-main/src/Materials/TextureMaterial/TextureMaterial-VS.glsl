#version 460

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;
uniform vec3 PosCam;
uniform vec3 PosLum;
uniform float Time;

layout (binding = 0) uniform sampler2D heightMap;  // Correct binding for height map
uniform float displacementScale;

out gl_PerVertex {
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
};

layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec3 VertexNormal;
layout (location = 3) in vec2 VertexTextureCoordinate;
layout (location = 4) in vec4 VertexTangent;

out VTF {
    vec3 vL;
    vec3 vV;
    vec2 uv;
    vec3 N;  // Output the new normal
};

void main()
{
    uv = VertexTextureCoordinate;

    // Sample the height map and apply displacement
    float height = texture(heightMap, uv).r;
    vec3 displacedPosition = VertexPosition + VertexNormal * height * displacementScale;

    // Calculate displaced tangent and binormal using height map derivatives
    float heightRight = texture(heightMap, uv + vec2(0.01, 0.0)).r;
    float heightUp = texture(heightMap, uv + vec2(0.0, 0.01)).r;
    
    vec3 displacedRight = VertexPosition + (VertexTangent.xyz + VertexNormal * (heightRight - height) * displacementScale);
    vec3 displacedUp = VertexPosition + (cross(VertexNormal, VertexTangent.xyz) + VertexNormal * (heightUp - height) * displacementScale);
    
    vec3 newTangent = normalize(displacedRight - displacedPosition);
    vec3 newBinormal = normalize(displacedUp - displacedPosition);
    vec3 newNormal = normalize(cross(newTangent, newBinormal));
    
    gl_Position = Proj * View * Model * vec4(displacedPosition, 1.0);

    // Transform light and view vectors into the new space
    vL = PosLum - displacedPosition;
    vV = PosCam - displacedPosition;
    N = newNormal;  // Output the recalculated normal
}
