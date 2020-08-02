#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 VertColor;

uniform sampler2D Texture;
uniform bool OnlyVertColor = false;

void main()
{
    if(OnlyVertColor) {
        // Display no textures but mesh with vertex colors
        FragColor = vec4(VertColor, 1.0);
    } else {
        // Display mesh with texture
        FragColor = texture(Texture, TexCoords);
    }


}
