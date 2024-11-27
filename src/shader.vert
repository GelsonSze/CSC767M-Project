#version 330
 
in vec3 a_vertex;
in vec3 a_color;
in vec3 a_normal;

in vec2 a_uv;
out vec2 v_uv;

out vec3 v_normal;
out vec3 v_vertex;
uniform mat4 u_model;
uniform mat4 u_projection;
uniform mat4 u_view;

out vec3 v_color;

void main()
{
	// position of the vertex
	gl_Position = vec4( a_vertex , 1.0 );

	// pass the colour to the fragment shader
	// v_color = a_color;
}

