#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in mat4 Rotation;
in vec2 TexCoords;

uniform sampler2D atexture;
uniform sampler2D aspectexture;

uniform vec3 lightpos;
uniform vec3 lightdirection;
uniform float lightcutoff;
uniform float outlightcutoff;
uniform vec3 lightColor;
uniform vec3 objColor;
uniform vec3 viewpos;

uniform float linear;
uniform float quadratic;

struct Dirlight {
    vec3 direction;
}; uniform Dirlight dirlight;

struct Pointlight {
    vec3 position;
    vec3 color;

    float linear;
    float quadratic;
}; uniform Pointlight pointlights[4];

struct Spotlight {
    vec3 position;
    vec3 direction;

    float innercutoff;
    float outercutoff;

    float linear;
    float quadratic;
}; uniform Spotlight spotlight;

uniform float ambient;  //default 0.1
uniform float diffuse;  //default 1.0
uniform float specular; //default 0.5
uniform float shininess; //default 16
//shininess controls size, specular is strength


vec3 calcdirlight(Dirlight light, vec3 normal, vec3 viewdir) {
    vec3 lightdir = normalize(-light.direction);
    //diffuse
    float diff = max(dot(normal, lightdir), 0.0);
    //specular
    vec3 reflectdir = reflect(-lightdir, normal);
    float spec = pow(max(dot(viewdir, reflectdir), 0.0), shinestr);
}

void main()
{
    //ambient
    float ambientstr = 0.1;
    if (ambient != 0) {ambientstr = ambient;}

    //diffuse
    //vec3 norm = normalize(vec3(Rotation * vec4(Normal, 1.0)));
    vec3 norm = normalize(Normal);
    vec3 lightdir = normalize(lightpos - FragPos); //this calculates lightdir as the angle the light is to the fragment
    //vec3 lightdir = normalize(-lightdirection);  //this is only for directional when always in parallel anyways
    float diffstr = max(dot(norm, lightdir), 0.0);
    if (diffuse != 0) {diffstr *= diffuse;}

    //specular
    float specstr = 0.5;
    if (specular != 0) {specstr = specular;}
    float shinestr = 16;
    if (shininess != 0) {shinestr = shininess;}
    vec3 viewdir = normalize(viewpos - FragPos);
    vec3 reflectdir = reflect(-lightdir, norm); //spec is the angle times strength times how strong the specular map says
    float spec = pow(max(dot(viewdir, reflectdir), 0.0), shinestr) * specstr * texture(aspectexture, TexCoords).rgb.x;

    //calculate attenuation
    float distance = length(lightpos - FragPos);
    float attenuation = 1.0 / (1.0 + (linear * distance) + (quadratic * (distance * distance)));

    //combine them
    float result = ambientstr + diffstr + spec;
    result *= attenuation;

    //spotlight stuff
    //float theta = dot(lightdir, normalize(-lightdirection));
    //float epsilon = lightcutoff - outlightcutoff;
    //float intensity = clamp((theta - outlightcutoff) / epsilon, 0.0, 1.0);
    //result = ambientstr + ((diffstr + spec) * intensity * attenuation);

    vec3 col = result * lightColor * texture(atexture, TexCoords).rgb;
    FragColor = vec4(col, 1.0);
}

