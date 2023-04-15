#version 330

in vec3 vertexPos;  // in view coordinates
in vec3 vertexNormal;
in vec2 texCoord;

uniform sampler2D uEarthTexture;
uniform sampler2D uCloudTexture;

out vec3 fFragColor;

void main(){
    vec3 col = texture(uEarthTexture, texCoord).xyz;
    vec3 colCloud = texture(uCloudTexture, texCoord).xyz;

    fFragColor = colCloud;
    // fFragColor = vec3(texCoord,0);
    fFragColor = mix(col, colCloud, colCloud.x);
    // fFragColor = col;

}