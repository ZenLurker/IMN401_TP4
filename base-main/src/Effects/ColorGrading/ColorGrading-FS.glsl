#version 460 core
out vec4 FragColor;
in vec2 uv;

layout (binding = 0) uniform sampler2D screenTexture;
layout (binding = 1) uniform sampler2D lut;

uniform float fLUT_TileSizeXY;
uniform float fLUT_TileAmount;
uniform float fLUT_LutAmount;
uniform int fLUT_LutSelector;
uniform float fLUT_AmountChroma;
uniform float fLUT_AmountLuma;

void main()
{
    vec3 color = texture(screenTexture, uv).rgb;
    vec2 texelsize = 1.0 / vec2(fLUT_TileSizeXY);
    texelsize.x /= fLUT_TileAmount;

    vec3 lutcoord = vec3((color.xy * fLUT_TileSizeXY - color.xy + 0.5) * texelsize.xy, color.z * fLUT_TileSizeXY - color.z);
    lutcoord.y /= fLUT_LutAmount;
    lutcoord.y += (float(fLUT_LutSelector) / fLUT_LutAmount);
    float lerpfact = fract(lutcoord.z);
    lutcoord.x += (lutcoord.z - lerpfact) * texelsize.y;

    vec3 lutcolor = mix(texture(lut, lutcoord.xy).xyz, texture(lut, vec2(lutcoord.x + texelsize.y, lutcoord.y)).xyz, lerpfact);

    color = mix(normalize(color), normalize(lutcolor), fLUT_AmountChroma) * 
            mix(length(color), length(lutcolor), fLUT_AmountLuma);

    FragColor = vec4(color, 1.0);
//    FragColor = vec4(uv.x,uv.y,0,1);
//    FragColor = texture(screenTexture, uv);

}
