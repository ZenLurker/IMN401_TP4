#version 460

layout (binding = 0) uniform sampler2D myFBO;
uniform int blurAmount;

in vec2 uv;

layout(location = 0) out vec4 Color;

void main() {
    ivec2 tSize = textureSize(myFBO, 0);
    float texelSize = 1.0 / tSize.y;

    vec3 colorSum = vec3(0.0);
    int count = 0;

    for(int i = -blurAmount; i <= blurAmount; ++i) {
        vec2 offset = vec2(0.0, float(i) * texelSize);
        colorSum += texture(myFBO, uv + offset).rgb;
        count++;
    }

    vec3 averageColor = colorSum / float(count);
    Color = vec4(averageColor, 1.0);
}
