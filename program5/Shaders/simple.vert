//#version 130

uniform mat4 M;

uniform float transInX;
uniform float transInY;

uniform float time;

vec4 lightPos = vec4(0,2,2,1);
attribute vec3 pos;
in vec3 norm;

varying vec3 fColor;
varying float d;
varying float s;

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
	mat4 R = rotateY(time*0.6);
	mat4 T = translate(vec3(transInX, transInY,0));	
	vec4 p = vec4(pos, 1);
	vec4 lpos = R*lightPos;
	
	//diffuse coefficient //
	vec4 l = normalize(lpos - p);
	vec4 n = vec4(norm, 0);
	d = clamp(dot(l,n), 0,1);
	
	//specular coefficient//
	vec4 camPos = vec4(0);  //camera is always at origin in view coords!
	vec4 v = normalize(camPos-p);  //view vector goes to camera, normalized
	vec4 r = reflect(-l,n);  //light vector points away from surface, but reflect() needs incident, so invert
	s = clamp(dot(v,r), 0, 1);  //only valid in range [0,1]
	s = pow(s, 10);  //exponent controls sharpness of specular highlight
	
	
	gl_Position = M*T*p;
	fColor = norm;
}
