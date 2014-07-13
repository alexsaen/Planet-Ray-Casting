uniform mat4 WVP;
uniform vec3 cameraPos;
uniform samplerCube cubeTex;

interface VSOutput {
	vec3	wpos;
};

const vec3 vertex[] = {
	vec3(-1.0, -1.0, -1.0),
	vec3( 1.0, -1.0, -1.0),
	vec3(-1.0,  1.0, -1.0),
	vec3( 1.0,  1.0, -1.0),
	vec3(-1.0, -1.0,  1.0),
	vec3( 1.0, -1.0,  1.0),
	vec3(-1.0,  1.0,  1.0),
	vec3( 1.0,  1.0,  1.0),
}; 

shader VS(out VSOutput o) {
	o.wpos = vertex[gl_VertexID];	
	gl_Position = WVP * vec4(o.wpos, 1.0);
}

float rayIntersect(vec3 pos, vec3 dir, float radius, float outer) {
	float a = dot(dir, dir);
	float b = 2.0 * dot(pos, dir);
	float c = dot(pos, pos) - radius*radius;
  
	float discr = b * b - 4.0 * a * c;

	if(discr < 0.0) 
		return -1.0;

	discr = sqrt(discr);

	float t0 = (-b + discr) / 2.0 * a;
	float t1 = (-b - discr) / 2.0 * a;

	return mix(t0, t1, step(0, t0*outer));
}

shader FS(in VSOutput i, out vec4 fragColor) {
	vec3 ray = normalize(i.wpos - cameraPos);

	float dist = rayIntersect(cameraPos, ray, 1, 1);
	if(dist<0)
		discard;

	vec3 pos = cameraPos + ray*dist;
	vec3 normal = normalize(pos);
		
	fragColor = textureLod(cubeTex, normal, 0);
} 

program raycasting {
    vs(330) = VS();
    fs(330) = FS();
};

