#version 130

uniform float timeIn;

uniform float transInX;
uniform float transInY;

uniform int colorID;

in vec2 pos;
in vec3 colorIn;
in vec2 checkPos;

in uint id;

smooth out vec4 smoothColor;
flat out vec4 uniqueColor;
flat out vec4 newColor;

mat4 translate(vec3 t)
{
	mat4 M = mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
	M[3] = vec4(t.xyz, 1);
	return M;
}
vec4 getColor(uint i)
{
	vec4 parts;
	parts.x = float(i)/100;
	parts.y = float(i)/100;
	parts.z = float(i)/100;
	parts.w = 1;
	return parts;
}

void main()
{
	vec4 parts = getColor(id);
	mat4 T = translate(vec3(transInX, transInY, 0));
	gl_Position = T*vec4(pos, 0, 1);
	smoothColor = vec4(colorIn, 1);
	uniqueColor = parts;
	newColor = vec4(0,.4,0,1);
	if(colorID == -2)
		newColor = vec4(.7,.7,0,1);
	if(colorID == -1 )
		newColor = vec4(0.0,0.5,0.9,1);
	if(colorID == 1 )
		newColor = vec4(.7,0,0,1);
	if(colorID == 2)
	    newColor = vec4(1,.1,0,1);
	if(colorID == 3)
	    newColor = vec4(1, .4,.4,1);
	if(colorID == 4)
		newColor = vec4(1,1,1,1);
	if(colorID == 5)
		newColor = vec4(0,0,0,1);
	if(colorID == 6)
		newColor = vec4(.2,.2,.2,1);
	if(colorID == 7)
		newColor = vec4(.5, .5, .5, 1);
	if(colorID == 8)
		newColor = vec4(.6, .6, .6, 1);
}