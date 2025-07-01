attribute vec3 a_posL;
attribute vec3 a_norm;
attribute vec2 a_uv;
attribute vec3 a_color;

varying vec2 v_uv;
varying vec3 v_color;
varying vec3 v_worldPos;
varying vec3 v_normW;

uniform mat4 uMVP;
uniform mat4 modelMatrix;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	gl_Position = uMVP * vec4(a_posL, 1.0);
	v_uv = a_uv;
	v_color = a_color;

	vec4 worldPos = modelMatrix * vec4(a_posL,1.0);
	v_worldPos = worldPos.xyz;

	vec4 normWorld = modelMatrix * vec4(a_norm,1.0);
	v_normW = normWorld.xyz;

}
   