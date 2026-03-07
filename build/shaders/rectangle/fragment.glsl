#version 330 core
out vec4 ModFragColor;
in vec2 textCoord;
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float opacity;
void main(){
    ModFragColor=mix(texture(texture1,textCoord),texture(texture2,textCoord),opacity);
};
