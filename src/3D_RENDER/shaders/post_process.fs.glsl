#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;

vec2 hash_0_to_1_2D_to_2D(vec2 p)
{
    vec3 a = fract(p.xyx * vec3(123.34, 234.34, 345.65));
    a += dot(a, a + 34.45);
    return fract(vec2(a.x * a.y, a.y * a.z));
}
// Thanks to CoolLab :)

void main()
{
    vec4  image = texture(screenTexture, TexCoords);
    float depth = abs(texture(depthTexture, TexCoords)).x;

    vec4 bgrd_color = vec4(0.23, 0.47, 0.90, 1.);

    vec2  rand        = hash_0_to_1_2D_to_2D(TexCoords * 100);
    float Density     = 0.2;
    float black_white = 0.2;

    float noise = rand.x < Density
                      ? rand.y * black_white
                      : 0.;

    // FragColor = image - noise;
    vec4 color = image - noise;
    FragColor  = mix(color, bgrd_color, depth);
}