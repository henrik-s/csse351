//#version 130
uniform float r;
uniform float g;
uniform float b;

varying vec4 normal;
varying vec4 light;
varying vec4 view;

void main()
{
	
	vec4 surfaceColor = vec4(r, g, b, 1.0);
	vec4 white = vec4(1);
	
	vec4 aColor = surfaceColor * 0.2;
	vec4 dColor = clamp(dot(light, normal), 0, 1)*surfaceColor;
	vec4 sColor = pow(clamp(dot(view,reflect(-light,normal)), 0.0, 1.0),10.0) * white;
	gl_FragColor = aColor + dColor + sColor;
}
