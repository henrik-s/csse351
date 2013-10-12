//#version 130

uniform mat4 M;
uniform float time;

uniform float transInX;
uniform float transInY;

attribute vec3 pos;

mat4 translate(vec3 t)
{
	mat4 M = mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
	M[3] = vec4(t.xyz, 1);
	return M;
}

void main()
{
	mat4 T = translate(vec3(transInX, transInY,0));	
	vec4 p = vec4(pos, 1);
	gl_Position = M*T*p;
}
