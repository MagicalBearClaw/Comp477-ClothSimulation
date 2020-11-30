#version 330 core
out vec4 color;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform sampler2D diffuse;

void main() {
    
	// Ambient
    float ambientStrength = 0.3f;
    vec3 ambient = ambientStrength * lightColor * texture(diffuse, TexCoords).rgb;

    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPosition - FragPos);
    float difference = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = difference * lightColor * texture(diffuse, TexCoords).rgb;

    vec3 result = (ambient + diffuse) * objectColor;
    color = vec4(result, 0.1f);
}
