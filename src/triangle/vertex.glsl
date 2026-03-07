#version 330 core
layout(location=0)in vec3 myVertex;
layout(location=1)in vec3 myColor;
out vec3 myOutColor;
out vec3 myNewVertex;
uniform float xOffset;
void main(){
    gl_Position=vec4(myVertex.x+xOffset,myVertex.y,myVertex.z,1.0);
    myOutColor=myColor;
    myNewVertex=myVertex;
};
