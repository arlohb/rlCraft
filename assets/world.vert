#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;

uniform mat4 mvp;
uniform mat4 matModel;
uniform mat4 matNormal;

out vec3 position;
out vec2 texCoord;
out vec3 normal;

void main() {
    position = vec3(matModel * vec4(vertexPosition, 1.0));
    texCoord = vertexTexCoord;
    normal = normalize(vec3(matNormal * vec4(vertexNormal, 1.0)));

    gl_Position = mvp * vec4(vertexPosition, 1.0);
}
