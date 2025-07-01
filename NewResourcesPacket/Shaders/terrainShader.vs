attribute vec3 a_posL;
attribute vec3 a_norm;
attribute vec2 a_uv;
attribute vec3 a_color;

varying vec2 v_uv;
varying vec2 v_uv2;
varying vec3 v_color;
varying vec3 v_worldPos;
varying vec3 v_normW;

uniform mat4 uMVP;
uniform vec3 u_height;
uniform float u_N;
uniform sampler2D u_texture_3;
uniform vec2 u_offset;

uniform mat4 modelMatrix;

void main()
{

	v_uv = a_uv;
	v_uv2 = (a_uv+u_offset) / u_N;

	vec4 posL = vec4(a_posL, 1.0);

	vec4 c_blend = texture2D(u_texture_3,v_uv2);

	posL.y += c_blend.r * u_height.r +
				c_blend.g * u_height.g +
				c_blend.b * u_height.b;

	vec4 worldPos = modelMatrix * vec4(a_posL,1.0);
	v_worldPos = worldPos.xyz;

	vec4 normWorld = modelMatrix * vec4(a_norm,1.0);
	v_normW = normWorld.xyz;

	gl_Position = uMVP * posL;


}
   