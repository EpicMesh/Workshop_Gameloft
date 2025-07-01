precision mediump float;

varying vec3 v_color;
varying vec2 v_uv;
varying vec3 v_worldPos;
varying vec3 v_normW;

uniform sampler2D u_texture_0;
uniform samplerCube u_cube_texture;

uniform int useColorUniform;
uniform vec3 u_color;
uniform int hasCubeMap;

// Fog
uniform vec3 camPos;
uniform float smallR;
uniform float hugeR;
uniform vec3 fogColor;
vec4 c_proprie;

// Light
uniform vec3 lightPos;
uniform vec3 c_amb;         // Ambient light
uniform float ratio;        // Ambient vs (diff + spec)
uniform vec3 c_luminaDiff;
uniform vec3 c_luminaSpec;
uniform int type;           // 0 = point, 1 = directional

void main()
{   
    vec3 normal = normalize(v_normW);
    vec3 viewDir = normalize(camPos - v_worldPos);
    vec3 dirReflect = reflect(viewDir, normal);
    vec4 c_reflect = textureCube(u_cube_texture, dirReflect);

    // Choose base color
    if (useColorUniform == 0) {
        c_proprie = texture2D(u_texture_0, v_uv);
    } else {
        c_proprie = vec4(u_color, 1.0);
    }

    if (useColorUniform == 0 && c_proprie.a < 0.1) discard;

    // Ambient component
    vec3 ambient = c_proprie.rgb * c_amb;
    vec4 Camb = vec4(ambient, c_proprie.a);

    // Light calculations
    vec3 lightDir = (type == 0) 
        ? normalize(lightPos - v_worldPos)   // Point light
        : normalize(lightPos);               // Directional light

    // Diffuse component
    float diffFactor = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = c_proprie.rgb * c_luminaDiff * diffFactor;

    // Specular component (Blinn-Phong)
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float specFactor = pow(max(dot(normal, halfwayDir), 0.0), 0.5);
    vec3 specular = c_luminaSpec * specFactor;

    
    // Final lighting mix
    float clampedRatio = clamp(ratio, 0.0, 1.0);
    vec3 lightingRGB = clampedRatio * ambient + 
                      (1.0 - clampedRatio) * (diffuse + specular);
    vec4 lighting = vec4(lightingRGB, c_proprie.a);
    
    vec4 final_Color = c_reflect * 0.5 + c_proprie*0.5;

    // Cube map reflection blend
    vec4 c_final = (hasCubeMap == 1)
        ? mix(lighting, final_Color, 0.5)
        : lighting;

    // Fog calculation
    float d = distance(camPos, v_worldPos);
    float fogFactor = clamp((d - smallR) / (hugeR - smallR), 0.0, 1.0);
    vec3 finalColor = mix(c_final.rgb, fogColor, fogFactor);

    gl_FragColor = vec4(finalColor.rgb, c_proprie.a);
}