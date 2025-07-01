attribute vec3 a_posL;
attribute vec2 a_uv;
varying vec2 v_uv;
uniform mat4 uMVP;

void main()
{
vec4 posL = vec4(a_posL, 1.0);
gl_Position = uMVP * vec4(a_posL, 1.0);
v_uv = a_uv;

}
   