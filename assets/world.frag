#version 330

uniform sampler2D texture0;
in vec2 fragTexCoord;

out vec4 finalColor;

void main() {
    vec4 texColour = texture(texture0, fragTexCoord);

    finalColor = texColour;
}
