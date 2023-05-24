#version 330
//  WE ARE IN VIEW COORDINATES

in vec3 vertexPos; // in view coordinates
in vec3 vertexNormal;
in vec2 texCoord;

uniform sampler2D uTexture;

out vec4 fFragColor;

// LIGHTS DATA
// Point
uniform vec3  w_i[32]; // position light
uniform vec3  L_i[32]; // light color
uniform float intensity[32];
uniform int   nb_light;

// Directionnal
uniform vec3  direction_i[32];
uniform vec3  L_i_direction[32];
uniform float intensity_direction[32];
uniform int   nb_light_directionnal;

// MATERIAL DATA
uniform vec3  K_d; // reflection coefficient (a color) DIFFUSE
uniform vec3  K_s; // glossy factor
uniform float shininess;
uniform float alpha;

vec3 w_o = normalize(vertexPos);

// Bling phong for i light
vec3 Blinn_Phong(int i)
{
    vec3 light_factor = vec3(0.0);

    vec3  light_dir;
    float light_distance_i;

    // Point light
    light_dir        = normalize(w_i[i] - vertexPos);
    light_distance_i = length(w_i[i] - vertexPos);
    light_factor     = intensity[i] * L_i[i] / light_distance_i;

    vec3  halfVector  = normalize(w_o + light_dir) / 2.;
    float diffuseTerm = max(dot(light_dir, vertexNormal), 0.0);
    vec3  diffuse     = K_d * diffuseTerm;

    float specularTerm = pow(max(dot(halfVector, vertexNormal), 0.0), shininess);
    vec3  specular     = K_s * specularTerm;

    // return w_i[i].y * vec3(1., 1., 1.);
    return light_factor * (diffuse + specular);
}

vec3 Blinn_Phong_directionnal(int i)
{
    vec3 light_dir = normalize(direction_i[i]);

    vec3 halfVector = normalize((w_o + light_dir) / 2.);

    vec3 diffuse = K_d * dot(light_dir, vertexNormal);
    vec3 glossy  = K_s * (pow(max(dot(halfVector, vertexNormal), 0.), shininess));

    vec3 light_factor = intensity_direction[i] * L_i_direction[i];

    // return glossy;
    return light_factor * (diffuse + glossy);
}

vec3 fog()
{
    float dist = length(vertexPos);
    dist /= 5.;

    vec3 col = mix(vec3(0, 0, 0), vec3(1, 0., 1.), dist);
    return col;
}

void main()
{
    vec3 point_light = vec3(0.);
    for (int i = 0; i < nb_light; i++)
    {
        point_light += Blinn_Phong(i);
    }

    vec3 dir_light = vec3(0.);
    for (int i = 0; i < nb_light_directionnal; i++)
    {
        dir_light += Blinn_Phong_directionnal(i);
    }

    fFragColor = vec4((point_light + dir_light) * texture(uTexture, texCoord).xyz, alpha);
    // fFragColor = vec4(point_light, 1.);

    // fFragColor = vec4(vertexNormal, alpha);
    // fFragColor = vec4(vertexPos, 1.);
}