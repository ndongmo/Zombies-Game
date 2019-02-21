#version 130
// The vertex shader operates on each vertex

// Input data from the VBO(Vertex Buffer Object). Each vertex is 2 floats
in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

out vec4 fragmentColor;
out vec2 fragmentPosition;
out vec2 fragmentUV;

uniform mat4 cameraMatrix;

void main() {
	// set the x,y position on thr screen
	gl_Position.xy = (cameraMatrix * vec4(vertexPosition, 0.0, 1.0)).xy;
	
	// the z position is zero since we are in 2D
	gl_Position.z = 0.0;
	
	//Indicate that the coordinates are normalized
	gl_Position.w = 1.0;
	
	fragmentColor = vertexColor;
	fragmentPosition = vertexPosition;
	fragmentUV = vec2(vertexUV.x, 1- vertexUV.y);
}