#version 330 core
struct Light {
    vec3 position;
    vec3 color;
    float ambientStrength;
    float specularStrength;
    float specularPhong;
    float constant;
    float linear;
    float quadratic;
};

uniform Light lights[2]; // Adjust the size based on the number of lights
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

void main()
{
    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    for (int i = 0; i < 2; ++i) {
        vec3 lightDir;
        float attenuation = 1.0;
        float distance = length(lights[i].position - FragPos);
        attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * distance * distance);
        lightDir = normalize(lights[i].position - FragPos);

        // Ambient
        ambient += lights[i].ambientStrength * lights[i].color * attenuation;

        // Diffuse
        float diff = max(dot(Normal, lightDir), 0.0);
        diffuse += diff * lights[i].color * attenuation;

        // Specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, Normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), lights[i].specularPhong);
        specular += lights[i].specularStrength * spec * lights[i].color * attenuation;
    }

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
