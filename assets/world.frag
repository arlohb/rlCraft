#version 330

uniform sampler2D texture0;
in vec2 texCoord;
in vec3 normal;

out vec4 finalColor;

void main() {
    vec4 texColour = texture(texture0, texCoord);

    float ambient = 0.3;

    float directional = max(dot(normal, normalize(vec3(0.5, 1.0, 0.3))), 0);

    float lighting = 0.3 + directional;

    finalColor = vec4(vec3(texColour * lighting), 1.0);
}
