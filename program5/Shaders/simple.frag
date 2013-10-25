//#version 130
uniform float r;
uniform float g;
uniform float b;

attribute vec3 fColor;
attribute float d;
attribute float s;

void main()
{
	vec4 blueColor = vec4(0.1, 0.6, 0.8, 1.0);
	vec4 white = vec4(1);
	
	vec4 aColor = blueColor * 0.2; //same color as diffuse, just darker
	vec4 dColor = d * blueColor;
	vec4 sColor = s * white;
	gl_FragColor = aColor + dColor + sColor;
	//gl_FragColor = vec4(r,g,b,1);
}
