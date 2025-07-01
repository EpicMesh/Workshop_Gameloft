precision mediump float;

varying vec3 v_color;
varying vec2 v_uv;
varying vec2 v_uv2;
varying vec3 v_worldPos;
varying vec3 v_normW;


uniform sampler2D u_texture_0; //Grass
uniform sampler2D u_texture_1; //Dirt
uniform sampler2D u_texture_2; //Rock
uniform sampler2D u_texture_3; //Blend

//For Fog
uniform vec3 camPos;
uniform float smallR;
uniform float hugeR;
uniform vec3 fogColor;

void main()
{	
	
    vec4 grass = texture2D(u_texture_0, v_uv);
    vec4 dirt = texture2D(u_texture_1, v_uv);
    vec4 rock = texture2D(u_texture_2, v_uv);
    vec4 blend = texture2D(u_texture_3, v_uv2);

    vec4 finalColor = blend.r * rock + blend.g * grass + blend.b * dirt;

    finalColor.a = 1.0;

    
	float d = distance(camPos,v_worldPos); 
	float alpha = clamp((d - smallR) / (hugeR - smallR), 0.0, 1.0);

    //vec4 finalfinalColor = alpha * fogColor + (1.0-alpha) * finalColor;
     vec4 finalfinalColor = mix(finalColor, vec4(fogColor, 1.0), alpha);

	gl_FragColor = finalfinalColor;
	//gl_FragColor = vec4(v_color,1.0);
}
