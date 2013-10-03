//by mu6k
//License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
//A simple sharpening filter. Useful in some situations. Perfect for low resolution movies.
//muuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuusk!

uniform vec2 resolution;
uniform vec2 size;
uniform float time;
uniform vec2 mouse;
uniform sampler2DRect diffuseTexture;
uniform float theMix;
uniform float weirdness;
uniform float amount;

float strength = 9.0; //  effect strength

vec4 sharp( sampler2DRect sampler, vec2 uv )
{
	vec4 c0 = texture2DRect(sampler,uv * resolution);
	vec4 c1 = texture2DRect(sampler,(uv*resolution-vec2(1.0,.0)));
	vec4 c2 = texture2DRect(sampler,(uv*resolution+vec2(1.0,.0)));
	vec4 c3 = texture2DRect(sampler,(uv*resolution-vec2(.0,1.0)));
	vec4 c4 = texture2DRect(sampler,(uv*resolution+vec2(.0,1.0)));
	vec4 c5 = c0+c1+c2+c3+c4;
	c5*=0.2;
	vec4 mi = min(c0,c1); mi = min(mi,c2); mi = min(mi,c3); mi = min(mi,c4);
	vec4 ma = max(c0,c1); ma = max(ma,c2); ma = max(ma,c3); ma = max(ma,c4);
	return clamp(mi,(strength+1.0)*c0-c5*strength,ma);
}

void main(void)
{
	vec2 uv = gl_FragCoord.xy / resolution.xy;
	vec2 tex = vec2(1.0,1.0)/ resolution.xy;
	vec2 m = mouse.xy / resolution.xy;

	vec4 col0 = sharp(diffuseTexture,uv);
	vec4 col1 = texture2DRect(diffuseTexture, uv*resolution );
	
	vec4 weird = abs( col0 - col1 ) * weirdness * 100.0;
//	weird = vec4(1.0) - weird;
    weird.a = 1.0;
	gl_FragColor = mix( col1, mix( weird, col0, theMix ), amount );
}