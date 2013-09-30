#version 130
uniform bool pickingEnabled;

flat in vec4 uniqueColor;
flat in vec4 newColor;
out vec4 fragColor;


void main()
{	

		fragColor = newColor;

	if(pickingEnabled)
		fragColor = uniqueColor;
}