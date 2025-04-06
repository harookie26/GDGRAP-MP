#version 330 core

uniform sampler2D tex0;
uniform sampler2D norm_tex;

out vec4 FragColor;

in vec3 normCoord;
in vec3 fragPos;
in vec2 texCoord;
in mat3 TBN;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 lightColor2;
uniform float ambientStr;
uniform vec3 ambientColor;
uniform vec3 cameraPos;
uniform float specStr;
uniform float specPhong;
uniform float alpha;
uniform float brightness; // Brightness parameter

uniform int lightType; // 0 for point light, 1 for directional light
uniform vec3 lightDir; // Direction for directional light

void main()
{
    vec4 pixelColor = texture(tex0, texCoord);
    if (pixelColor.a < 0.1)
    {
        discard;
    }

    vec3 normal = texture(norm_tex, texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(TBN * normal);

    vec3 lightDirection;
    float attenuation = 1.0;
    if (lightType == 0) // Point light
    {
        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;

        float distance = length(lightPos - fragPos);
        attenuation = brightness / (constant + linear * distance + quadratic * (distance * distance));
        lightDirection = normalize(lightPos - fragPos);
    }
    else if (lightType == 1) // Directional light
    {
        lightDirection = normalize(-lightDir);
    }

    vec3 ambientCol = ambientColor * ambientStr * attenuation;
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor * attenuation;

    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDirection, normal);

    float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
    vec3 specColor = spec * specStr * lightColor2 * attenuation;

    FragColor = vec4(specColor + diffuse + ambientCol, alpha) * texture(tex0, texCoord);
}
