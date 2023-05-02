#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;



// FOG PARAMETERS

uniform float near_plane;
uniform float far_plane;
uniform int fog_type; // 0 linear / 1 exp / 2 sqrd_exp
uniform float fog_density;
uniform vec3 fog_background_color;


vec2 hash_0_to_1_2D_to_2D(vec2 p)
{
    vec3 a = fract(p.xyx * vec3(123.34, 234.34, 345.65));
    a += dot(a, a + 34.45);
    return fract(vec2(a.x * a.y, a.y * a.z));
}
// Thanks to CoolLab :)

float linearize_depth(float d,float zNear,float zFar)
{
    return zNear * zFar / (zFar + d * (zNear - zFar));
}


float exp_fog(float factor, float density){
    return 1./exp(density*factor);
}
float exp_sqrd_fog(float factor,float density){
    return exp_fog(factor*factor,density*density);
}


void main()
{
    vec4  image = texture(screenTexture, TexCoords);
    float depth = abs(texture(depthTexture, TexCoords)).x;

    depth = linearize_depth(depth,near_plane,far_plane)*far_plane;

    float fog_factor = 1-exp_sqrd_fog(depth,fog_density); 

    vec2  rand        = hash_0_to_1_2D_to_2D(TexCoords * 100);
    float Density     = 0.2;
    float black_white = 0.2;

    float noise = rand.x < Density
                      ? rand.y * black_white
                      : 0.;

    // FragColor = image - noise;
    vec4 color = image - noise;
    vec4 bgrd_color = vec4(fog_background_color,1.);

    FragColor  = mix(color, bgrd_color, fog_factor);
    // FragColor = vec4(fog_factor,fog_factor,fog_factor,1.);
    // FragColor = vec4(depth,depth,depth,1.);
}