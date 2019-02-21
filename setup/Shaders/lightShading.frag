#version 130
// The fragment shader operates on each pixel in a given polygon.
// out for output and in for input

in vec4 fragmentColor;
in vec2 fragmentPosition;
in vec2 fragmentUV;

// This is the 3 component float vector that gets outputted to the 
// screen for each pixel.
out vec4 color;

void main() {	
	float distance = length(fragmentUV);
	float effect = smoothstep(0.0, 1.0, 1.0 - distance);
	color = vec4(fragmentColor.rgb, fragmentColor.a * (pow(0.01, distance) - 0.01));
}