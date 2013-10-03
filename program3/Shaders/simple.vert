#version 130

uniform float timeIn;

//tells the tranlate matrix where to translate
uniform float transInX;
uniform float transInY;

//tells us which color to send to frag
uniform int colorID;

in vec2 pos;
in vec3 colorIn;

//id for each picking object
in uint id;

smooth out vec4 smoothColor;
flat out vec4 uniqueColor;

//for the checker pieces
flat out vec4 newColor;

//generates transform matrix
mat4 translate(vec3 t)
{
	mat4 M = mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
	M[3] = vec4(t.xyz, 1);
	return M;
}

//grabs a color for picking
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
	
	//here we figure out how to color everything 
	//using a fancy collection of if statements
	
	//void square
	newColor = vec4(0,.4,0,1);
	//Cheat mode color
	if(colorID == -2)
		newColor = vec4(.7,.7,0,1);
	//squares the checker rests on
	if(colorID == -1 )
		newColor = vec4(0.0,0.5,0.9,1);
	//standby bottom checker
	if(colorID == 1 )
		newColor = vec4(.7,0,0,1);
	//selected bottom checker
	if(colorID == 2)
	    newColor = vec4(1,.1,0,1);
	//standby king bottom checker
	if(colorID == 3)
	    newColor = vec4(1, .4,.4,1);
	//selected king bottom checker
	if(colorID == 4)
		newColor = vec4(1,1,1,1);
	//standby top checker
	if(colorID == 5)
		newColor = vec4(0,0,0,1);
	//selected top checker
	if(colorID == 6)
		newColor = vec4(.2,.2,.2,1);
	//standby king top checker
	if(colorID == 7)
		newColor = vec4(.5, .5, .5, 1);
	//selected king top checker
	if(colorID == 8)
		newColor = vec4(.6, .6, .6, 1);
}