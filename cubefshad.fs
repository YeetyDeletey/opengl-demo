#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in mat4 Rotation;
in vec2 TexCoords;

uniform sampler2D atexture;     //teture vars
uniform sampler2D aspectexture;

uniform vec3 viewpos;
                                //multipliers
uniform float ambient;  //default 0.1
uniform float diffuse;  //default 1.0
uniform float specular; //default 0.5
uniform float shininess; //default 16
//shininess controls size, specular is strength

struct Dirlight {
    vec3 direction;

    vec3 color;
    float str;
}; uniform Dirlight dirlight;

struct Pointlight {
    vec3 position;

    float linear;
    float quadratic;

    vec3 color;
    float str;
}; uniform Pointlight pointlights[4];

struct Spotlight {
    vec3 position;
    vec3 direction;

    float inner;
    float outer;

    float linear;
    float quadratic;

    vec3 color;
    float str;
}; uniform Spotlight spotlight;


vec3 calcdirlight(Dirlight light, vec3 normal, vec3 viewdir) {
    vec3 lightdir = normalize(-light.direction);
    //diffuse
    float diff = max(dot(normal, lightdir), 0.0) * diffuse;
    //specular
    vec3 reflectdir = reflect(-lightdir, normal);
    float spec = pow(max(dot(viewdir, reflectdir), 0.0), shininess) * specular * texture(aspectexture, TexCoords).rgb.x;
    //combine, add ambient
    vec3 col = (ambient + diff + spec) * light.color * light.str * texture(atexture, TexCoords).rgb;
    return col;
}

vec3 calcpointlight(Pointlight light, vec3 normal, vec3 FragPos, vec3 viewdir) {
    vec3 lightdir = normalize(light.position - FragPos);
    //diffuse
    float diff = max(dot(normal, lightdir), 0.0) * diffuse;
    //specular
    vec3 reflectdir = reflect(-lightdir, normal);
    float spec = pow(max(dot(viewdir, reflectdir), 0.0), shininess) * specular * texture(aspectexture, TexCoords).rgb.x;
    //attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (1.0 + (light.linear * distance) + (light.quadratic * (distance * distance)));
    //combine, add ambient
    vec3 col = ((ambient + diff + spec) * attenuation) * light.color * light.str * texture(atexture, TexCoords).rgb;
    return col;
}

vec3 calcspotlight(Spotlight light, vec3 normal, vec3 FragPos, vec3 viewdir) {
    vec3 lightdir = normalize(light.position - FragPos);
    float theta = dot(lightdir, normalize(-light.direction));

    if (theta > light.outer) {
        float epsilon = light.inner - light.outer;
        float intensity = clamp((theta - light.outer) / epsilon, 0.0, 1.0);
        //diffuse
        float diff = max(dot(normal, lightdir), 0.0) * diffuse;
        //specular
        vec3 reflectdir = reflect(-lightdir, normal);
        float spec = pow(max(dot(viewdir, reflectdir), 0.0), shininess) * specular * texture(aspectexture, TexCoords).rgb.x;
        //attenuation
        float distance = length(light.position - FragPos);
        float attenuation = 1.0 / (1.0 + (light.linear * distance) + (light.quadratic * (distance * distance)));
        //combine, add ambient
        vec3 col = ((ambient + diff + spec) * attenuation) * intensity * light.color * light.str * texture(atexture, TexCoords).rgb;
        return col;
    } else {
        return vec3(0.0, 0.0, 0.0);
    }
}

void main()
{
    //vec3 norm = Normal; 
    vec3 norm = vec3(Rotation * vec4(Normal, 1.0));
    vec3 viewdir = normalize(viewpos - FragPos);
    
    //directional
    vec3 col = calcdirlight(dirlight, norm, viewdir);

    //point lights
    for (int i = 0; i < 4; i++) {
        col += calcpointlight(pointlights[i], norm, FragPos, viewdir);}
    
    //spot light
    col += calcspotlight(spotlight, norm, FragPos, viewdir);

    //int cel = 5;
    //col.x = round(col.x * cel) / cel;
    //col.y = round(col.y * cel) / cel;
    //col.z = round(col.z * cel) / cel;
    //float tot = round((col.x + col.y + col.z)) / 3;
    //FragColor = vec4(tot, tot, tot, 1.0);
    FragColor = vec4(col, 1.0);
    
//this comment is for bixby
}
