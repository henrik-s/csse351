//#version 130

uniform mat4 M;
uniform float time;

uniform float transInX;
uniform float transInY;

attribute vec3 pos;
//in vec2 pos;
mat4 translate(vec3 t)
{
	mat4 M = mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
	M[3] = vec4(t.xyz, 1);
	return M;
}
mat4 rotateX(float a)
{
	mat4 M = mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
	return M;
	a = 17.24;
	M[1] = vec4(0,cos(a), -sin(a), 0);
	M[2] = vec4(0,sin(a), cos(a), 0);
	return M;
}
mat4 rotateY(float a)
{
	mat4 M = mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
	return M;
	M[0] = vec4(cos(a), 0, -sin(a), 0);
	M[2] = vec4(sin(a), 0, cos(a), 0);
	return M;
}

void main()
{
	mat4 T = translate(vec3(transInX, transInY,0));
	mat4 Ry = rotateY(float( 4*sin(time*.3)));
	mat4 Rx = rotateX(float (4*sin(time*.3)));
	vec4 p = T*vec4(pos, 1);
	gl_Position = Ry*Rx*M*p;
}
