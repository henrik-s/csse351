//#version 130

uniform mat4 M;

in vec3 norm;
in vec3 pos;

uniform float transInX;
uniform float transInY;
uniform float time;

varying vec4 normal;
varying vec4 light;
varying vec4 view;

mat4 translate(vec3 t)
{
	mat4 M = mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
	M[3] = vec4(t.xyz, 1);
	return M;
}
mat4 rotateY(float a)
{
	return mat4(
		cos(a), -sin(a), 0, 0,
		sin(a), cos(a), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}
void main()
{
	vec4 lightPos = vec4(0,2,2,1);

	mat4 R = rotateY(time);
	mat4 T = translate(vec3(transInX, transInY,0));	
	vec4 p = vec4(pos, 1);
	vec4 lpos = R*lightPos;
	
	normal = vec4(norm, 0);
	light = normalize(lpos - p);
	view = normalize(-p);
	
	gl_Position = M*T*p;
}
