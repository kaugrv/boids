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

float linearize_depth(float d,float zNear,float zFar)
{
    return zNear * zFar / (zFar + d * (zNear - zFar));
}


// OUTLINE
mat2 robert_cross_X = mat2(vec2(1.,0.),vec2(0.,-1));
mat2 robert_cross_Y = mat2(vec2(0.,1.),vec2(-1.,0.));
const float delta = 0.0005f;

uniform float threshold;
uniform bool outline_is_activated;
uniform vec3 outline_color;

float[4] get_samples(sampler2D tex,vec2 coord){
    float[4] res;
    res[0] = texture(tex,coord-vec2(delta)).r;// top left
    res[1] = texture(tex,coord+vec2(delta)).r;// bottom right
    res[2] = texture(tex,coord+vec2(-delta,delta)).r;// bottom left
    res[3] = texture(tex,coord+vec2(delta,-delta)).r;//bottom right

    for(int i=0; i<=3; i++)
        res[i] = linearize_depth(res[i],near_plane,far_plane);
    return res;
}


float get_edge_factor(){
    float[4] samples = get_samples(depthTexture,TexCoords);
    float horizontal, vertical;

    horizontal += samples[0] * robert_cross_X[0][0]; // top left (factor +1)
    horizontal += samples[3] * robert_cross_X[1][1]; // bottom right (factor -1)

    vertical += samples[2] * robert_cross_Y[0][1]; // bottom left (factor -1)
    vertical += samples[1] * robert_cross_Y[1][0]; // top right (factor +1)

    float edge_value =  sqrt(dot(horizontal, horizontal) + dot(vertical, vertical));
    return (edge_value > threshold) && outline_is_activated ? 1. : 0.;
    return edge_value;
}



vec2 hash_0_to_1_2D_to_2D(vec2 p)
{
    vec3 a = fract(p.xyx * vec3(123.34, 234.34, 345.65));
    a += dot(a, a + 34.45);
    return fract(vec2(a.x * a.y, a.y * a.z));
}
// Thanks to CoolLab :)


float linear_fog(float factor){
    return (far_plane-factor)/(far_plane-near_plane);
}

float exp_fog(float factor, float density){
    return 1./exp(density*factor);
}
float exp_sqrd_fog(float factor,float density){
    return exp_fog(factor*factor,density*density);
}


float compute_fog_factor(int fog_type,float factor, float density){
    switch(fog_type){
        case 2: // linear
            return 1.-linear_fog(factor); // cassé :( a cause de imgui je crois 
            break;

        case 1: // exponential
            return 1.-exp_fog(factor, density);
            break;
        
        case 0: // exponential squared
            return 1.-exp_sqrd_fog(factor,density);
            break;
    }
}

void main()
{
    vec4  image = texture(screenTexture, TexCoords);
    float depth = abs(texture(depthTexture, TexCoords)).x;

    depth = linearize_depth(depth,near_plane,far_plane)*far_plane;

    float fog_factor = compute_fog_factor(fog_type,depth,fog_density); 

    vec2  rand        = hash_0_to_1_2D_to_2D(TexCoords * 100);
    float Density     = 0.2;
    float black_white = 0.2;

    float noise = rand.x < Density
                      ? rand.y * black_white
                      : 0.;

    // FragColor = image - noise;
    vec4 color = image - noise;
    vec4 bgrd_color = vec4(fog_background_color,1.);

    // apply outline
    color = mix(color,vec4(outline_color,1.),get_edge_factor());
    
    FragColor  = mix(color, bgrd_color, fog_factor);
    // FragColor = vec4(get_edge_factor());
    // FragColor = vec4(fog_factor,fog_factor,fog_factor,1.);
    // FragColor = vec4(depth,depth,depth,1.);
}