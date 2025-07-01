precision mediump float;
varying vec3 v_color;
varying vec2 v_uv;
varying vec3 v_worldPos;
varying vec3 v_normW;

uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;
uniform sampler2D u_texture_3;
uniform sampler2D u_texture_4;

uniform float u_DispMax;
uniform float u_Time;

//For Fog
uniform vec3 camPos;
uniform float smallR;
uniform float hugeR;
uniform vec3 fogColor;

void main()
{	

	float d = distance(camPos,v_worldPos); 
	float alpha = clamp((d - smallR) / (hugeR - smallR), 0.0, 1.0);

	vec2 fire_coords = vec2(v_uv.x,v_uv.y+u_Time);
	vec2 disp = texture2D(u_texture_2, fire_coords).rg;

	vec2 offset = (disp * 2.0 - 1.0) * u_DispMax;
	vec2 v_uv_displaced = v_uv + offset;

	vec4 c_fire = texture2D(u_texture_1,v_uv_displaced);
	vec4 c_alpha = texture2D(u_texture_0,v_uv);

	c_fire.a = c_fire.a * c_alpha.r;

	if (c_fire.a<0.1) discard;

	vec3 finalColor = alpha * fogColor + (1.0-alpha) * c_fire.xyz;
	gl_FragColor = vec4(finalColor,c_fire.a);
	
	
	
}
