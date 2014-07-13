#ifndef SHADING_GLSL
#define SHADING_GLSL

//	color = vec4(lambert(vec3(0.7, 0.0, 0.0), l, n), 1);
//	color = vec4(wrap(vec3(0.7, 0.0, 0.0), 0.5, l, n), 1);
//	color = vec4(lommel_seeliger(vec3(0.7, 0.0, 0.0), l, n, v), 1);
//	color = vec4(phong(vec3(0.7, 0.0, 0.0), vec3(0.7, 0.7, 0.0), 30.0, l, n, v), 1);
//	color = vec4(blinn(vec3(0.7, 0.0, 0.0), vec3(0.7, 0.7, 0.0), 30.0, l, n, v), 1);
//	color = vec4(blinn_rim(vec3(0.7, 0.0, 0.0), vec3(0.7, 0.7, 0.0), 30.0, vec3(0.2, 0.0, 0.2), 8.0, 0.3, l, n, v), 1);
//	color = vec4(ward(vec3(0.7, 0.0, 0.0), vec3(0.7, 0.7, 0.0), 10.0, l, n, v), 1);
//	color = vec4(cook_torrance(vec3(0.7, 0.0, 0.0), vec3(0.7, 0.7, 0.0), 0.5, l, n, v), 1);
//	color = vec4(oren_nayar(vec3(0.7, 0.0, 0.0), 0.0, l, n, v), 1);
//	color = vec4(minnaert(vec3(1.0, 1.0, 0.0), 0.8, 0.9, l, n, v), 1);	
//	color = vec4(toon(vec3(0.7, 0.0, 0.0), l, n), 1);
//	color = vec4(gooch(vec3(0.7, 0.7, 0.0), vec3(0.0, 0.0, 0.7), l, n), 1);
//	color = vec4(bidirectional(vec3(0.7, 0.7, 0.0), vec3(0.0, 0.0, 0.7), l, n), 1);
//	color = vec4(trilight(vec3(0.7, 0.0, 0.0), vec3(0.0, 0.7, 0.0), vec3(0.0, 0.0, 0.7), l, n), 1);

vec3 lambert(vec3 diffColor, vec3 l, vec3 n) {
	vec3 diff = diffColor * max(dot(n, l), 0.0);
	return diff;
}

vec3 wrap(vec3 diffColor, float factor, vec3 l, vec3 n) {
	vec3 diff = diffColor * max(dot(n, l) + factor, 0.0) / (1.0 + factor);
	return diff;
}

vec3 lommel_seeliger(vec3 diffColor, vec3 l, vec3 n, vec3 v) {
	float a = max(0.0, dot(n, l));
	float b = max(0.0, dot(n, v));
	return diffColor * a / (a + b);	
}

vec3 phong(vec3 diffColor, vec3 specColor, float specPower, vec3 l, vec3 n, vec3 v) {
	vec3 r = reflect(-v, n);
	vec3 diff = diffColor * max(dot(n, l), 0.0);
    vec3 spec = specColor * pow(max(dot(l, r), 0.0), specPower);
	return diff + spec;
}

vec3 blinn(vec3 diffColor, vec3 specColor, float specPower, vec3 l, vec3 n, vec3 v) {
	vec3 h = normalize(l + v);
	vec3 diff = diffColor * max(dot(n, l), 0.0);
    vec3 spec = specColor * pow(max(dot(n, h), 0.0), specPower);
	return diff + spec;
}

vec3 blinn_rim(vec3 diffColor, vec3 specColor, float specPower, vec3 rimColor, float rimPower, float bias, vec3 l, vec3 n, vec3 v) {
	vec3 h = normalize(l + v);
	vec3 diff = diffColor * max(dot(n, l), 0.0);
    vec3 spec = specColor * pow(max(dot(n, h), 0.0), specPower);
    vec3 rim = rimColor * pow(1.0 + bias - max(dot(n, v), 0.0), rimPower);
    return diff + spec + rim;
}

vec3 ward(vec3 diffColor, vec3 specColor, float k, vec3 l, vec3 n, vec3 v) {
	vec3 h = normalize(l + v);
	float hn   = dot(h, n);
    float hn2  = hn * hn;
    vec3 diff = diffColor * max(dot(n, l), 0.0);
    vec3 spec = specColor * exp(-k * (1.0 - hn2) / hn2);
	return diff + spec;
}

vec3 cook_torrance(vec3 diffuse, vec3 specular, float r, vec3 l, vec3 n, vec3 v) {
   	vec3 h = normalize(v + l);
   	float nl = max(dot(n, l), 0.0);
   	float nv = max(dot(n, v), 0.0);
   	float nh = max(dot(n, h), 1.0e-7);
   	float vh = max(dot(v, h), 0.0);

   	float g = min(1.0, 2.0 * nh / vh * min(nv, nl));
   	float nh2 = nh * nh;
   	float nhr = 1.0 / (nh2 * r * r);
   	float roughness = exp((nh2 - 1.0) * nhr) * nhr / (4.0 * nh2);
	float fresnel = 1.0 / (1.0 + nv);
   	float rs = min(1.0, (fresnel * g * roughness) / (nv * nl + 1.0e-7));
	
	return (nl + 0.1) * (diffuse + specular * (rs + 0.1));
}

vec3 oren_nayar(vec3 diffColor, float a, float b, vec3 l, vec3 n, vec3 v) {
	float nl = dot(n, l);
	float nv = dot(n, v);
	vec3 lProj = normalize(l - n * nl);
	vec3 vProj = normalize(v - n * nv);
	float cx = max(dot(lProj, vProj), 0.0);
	float cosAlpha = nl > nv ? nl : nv;
	float cosBeta = nl > nv ? nv : nl;
	float dx = sqrt((1.0 - cosAlpha * cosAlpha) * (1.0 - cosBeta * cosBeta)) / cosBeta;
	return max(0.0, nl) * diffColor * (a + b * cx * dx);
}

vec3 oren_nayar(vec3 diffColor, float q, vec3 l, vec3 n, vec3 v) {
	float q2 = q*q;
	float a = 1.0 - 0.5*q2/(q2+0.33); 
	float b = 0.45*q2/(q2+0.09); 
	return oren_nayar(diffColor, a, b, l, n, v);
}

vec3 minnaert(vec3 diffColor, float k, float k1, vec3 l, vec3 n, vec3 v) {
	float d1 = pow(max (dot(n, l), 0.0 ), 1.0 + k);
	float d2 = pow(1.0 - dot(n, v)*k1, 1.0 - k);
	return diffColor * d1 * d2;
}

vec3 toon(vec3 diffColor, vec3 l, vec3 n) {
	float diff = 0.2 + max(dot(n, l), 0.0);
	vec3  clr;
	if(diff < 0.4)
		clr = diffColor * 0.3;
	else if(diff < 0.8)
		clr = diffColor;
	else
		clr = diffColor * 1.3;
	return clr;
}

vec3 gooch(vec3 color1, vec3 color2, vec3 l, vec3 n) {
	return mix(color2, color1, (dot(n, l) + 1.0) * 0.5);
}

vec3 bidirectional(vec3 color1, vec3 color2, vec3 l, vec3 n) {
	return color1 * max(dot(n, l), 0.0) + color2 * max(dot(n, -l), 0.0);
}

vec3 trilight(vec3 color0, vec3 color1, vec3 color2, vec3 l, vec3 n) {
	return color0 * max(dot(n, l), 0.0) + color1 * (1.0 - abs(dot(n, l))) + color2 * max(dot(-n, l), 0.0);
}

#endif

