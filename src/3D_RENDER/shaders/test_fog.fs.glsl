#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    FragColor = vec4(1., 1., 1., 2.) - texture(screenTexture, 2. * TexCoords);
    // FragColor = vec4(1., 0.3, 0.1, 1.);
}