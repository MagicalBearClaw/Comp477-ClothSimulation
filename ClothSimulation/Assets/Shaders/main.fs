#version 330 core
out vec4 color;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform sampler2D diffuse;

void main() {
    
	// Ambient
    float ambientStrength = 0.3f;
    vec3 ambient = ambientStrength * light_color * texture(diffuse, TexCoords).rgb;

    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light_color * texture(diffuse, TexCoords).rgb;

    vec3 result = (ambient + diffuse) * object_color;
    color = vec4(result, 0.1f);
}
