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
uniform mat4 lightSpaceMatrix;

out vec3 v_color;
out vec4 FragPosLightSpace;

void main()
{
	// position of the vertex
	gl_Position = u_projection* u_view * u_model * vec4( a_vertex , 1.0 );

	// pass the colour to the fragment shader
	v_color = a_color;

	// pass the uv coordinates to the fragment shader
	v_uv = a_uv;

	mat4 normal_matrix = transpose(inverse(u_model));
	v_normal = (normal_matrix * vec4(a_normal,1.0) ).xyz;

	v_vertex = (u_model * vec4(a_vertex, 1.0)).xyz;
	

	FragPosLightSpace  = lightSpaceMatrix * u_model * vec4( a_vertex , 1.0);
	//FragPosLightSpace  = lightSpaceMatrix * vec4(FragPos, 1.0);
}

