#version 330

out vec4 fragColor;
in vec3 v_color;

in vec2 v_uv;
in vec3 v_normal;
in vec3 v_vertex;
in vec4 FragPosLightSpace;

uniform vec3 u_color;
uniform sampler2D u_texture;
uniform sampler2D u_texture_normal;
uniform sampler2D u_texture_spec;
uniform sampler2D u_texture_diffuse;

uniform vec3 u_light_dir;
uniform vec3 u_cam_pos;

uniform vec3 u_ambient;
uniform vec3 u_diffuse;
uniform vec3 u_specular;
uniform float u_shininess;

uniform sampler2D shadowMap;

mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{
	// get edge vectors of the pixel triangle
	vec3 dp1 = dFdx( p );
	vec3 dp2 = dFdy( p );
	vec2 duv1 = dFdx( uv );
	vec2 duv2 = dFdy( uv );
	// solve the linear system
	vec3 dp2perp = cross( dp2, N );
	vec3 dp1perp = cross( N, dp1 );
	vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
	vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
			 
	// construct a scale-invariant frame 
	float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );
	return mat3( T * invmax, B * invmax, N );
}

// assume N, the interpolated vertex normal and 
// V, the view vector (vertex to eye)
vec3 perturbNormal( vec3 N, vec3 V, vec2 texcoord, vec3 normal_pixel )
{

	normal_pixel = normal_pixel * 2.0 - 1.0;
	mat3 TBN = cotangent_frame(N, V, texcoord);
	return normalize(TBN * normal_pixel);
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
     //perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth + 0.05 ? 0.8 : 0.0;

    return shadow;
}  

void main(void)
{
	//vec4 texture_color = texture(u_texture, v_uv);

	vec3 texture_normal = texture(u_texture_normal, v_uv).xyz;
	vec3 N = normalize(v_normal);

	//original normal 
	vec3 N_orig = N;
	// call the function to modify the normal
	N = perturbNormal(N, v_vertex, v_uv, texture_normal);
	// mix the original normal with the new normal
	N = mix(N_orig, N, 2.0f);

	vec3 texture_spec = texture(u_texture_spec, v_uv).xyz;
	vec3 texture_diffuse = texture(u_texture_diffuse, v_uv).xyz;


	//Phong Diffuse
	//vec3 N = normalize(v_normal);
	vec3 L = normalize(u_light_dir - v_vertex);
	float NdotL = max( dot(N,L) , 0.0 );
	vec3 texture_color = texture( u_texture , v_uv ).xyz;
	vec3 diffuse_color = texture_color * NdotL * u_diffuse;

	//Phong Specular
	//vec3 R = normalize(-reflect(L, N) );
	vec3 E = normalize(u_cam_pos - v_vertex);
	//float RdotE = max( dot(R,E) , 0.0 );
	//vec3 spec_color = texture_color * pow ( RdotE , u_shininess) * u_specular;

	//Blinn-Phong Specular
	vec3 H = normalize(L + E);
	float NdotH = max(dot(N,H), 0.0);
	vec3 spec_color = texture_color * pow ( NdotH , u_shininess) * u_specular;

	//Phong Ambient
	vec3 ambient_color = texture_color * u_ambient;

	//vec3 final_color = diffuse_color + spec_color + ambient_color; // phong shading equation
	float shadow = ShadowCalculation(FragPosLightSpace);
	vec3 final_color = ambient_color + (1.0 - shadow) * (diffuse_color * texture_diffuse + spec_color * texture_spec);
	//vec3 final_color = ambient_color + (diffuse_color * texture_diffuse + spec_color * texture_spec);

	//fragColor = vec4(texture_color.xyz, 1.0);
	fragColor = vec4(final_color, 1.0);
}
