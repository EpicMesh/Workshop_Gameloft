precision mediump float;
varying vec3 v_color;
varying vec2 v_uv;
uniform sampler2D u_texture;
vec4 color;

void main()
{	
	color = texture2D(u_texture,v_uv);
	if (color.a<0.1) discard;

	//gl_FragColor = color;
	gl_FragColor = color;
}
