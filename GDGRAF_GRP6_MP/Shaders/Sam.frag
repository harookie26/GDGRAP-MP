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

uniform float brightness; //declare a brightness parameter

void main()
{

vec4 pixelColor = texture(tex0, texCoord);
if (pixelColor.a < 0.1)
{
discard;
}

	//declare the following variables for solving attenuation
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;

	float distance = length(lightPos - fragPos); //create a variable distance which is the distance of the rendered object to the light source
	float attenuation = brightness / (constant + linear * distance + quadratic * (distance * distance)); //brightness parameter adjusts the brightness of the light 

	vec3 ambientCol = ambientColor * ambientStr * attenuation; //multiply attenuation to ambientCol
	vec3 normal = texture(norm_tex,texCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(TBN * normal);	

	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(normal, lightDir), 0.0); 
	vec3 diffuse = diff * lightColor * attenuation; //multiply attenuation to diffuse

    vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
	vec3 specColor = spec * specStr * lightColor2 * attenuation; //multiply attenuation to specColor

	FragColor = vec4(specColor + diffuse + ambientCol, 1.0) * texture(tex0, texCoord); //if need more lightsource make for loop for this line
}