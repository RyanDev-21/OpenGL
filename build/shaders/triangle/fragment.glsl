#version 330 core
out vec4 fragColor;
in vec3 myNewVertex;
void main(){
    fragColor=vec4(myNewVertex,1.0f);
};
