#version 330

in vec3 vertexPos;  // in view coordinates
in vec3 vertexNormal;
in vec2 texCoord;

uniform sampler2D uTexture;

out vec3 fFragColor;

void main(){
    fFragColor = normalize(vertexNormal);
    fFragColor = vec3(texCoord,0);
    fFragColor = texture(uTexture, texCoord).xyz;

}