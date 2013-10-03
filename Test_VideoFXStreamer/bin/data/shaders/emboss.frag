
uniform vec2 resolution;
uniform vec2 size;
uniform float time;
uniform vec2 mouse;
uniform sampler2DRect diffuseTexture;
uniform float intensity;
uniform float colorization;
uniform float amount;

uniform sampler2DRect vectorField;
uniform vec2 vectorFieldSize;
uniform float flowEffect;

float remap(float value, float inputMin, float inputMax, float outputMin, float outputMax ) {
    return ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
}

void main(void)
{
    // OPTICAL FLOW STUFF
    vec2 flow = texture2DRect( vectorField, gl_TexCoord[0].xy * (vectorFieldSize / size ) ).xy;
    float magnitude = length( flow );
    magnitude = remap( magnitude, 0.0, flowEffect, 0.0, 1.0 );
    magnitude = clamp( magnitude, 0.0, 1.0 );
    
    
    
	vec2 uv = gl_FragCoord.xy / resolution.xy;
	vec3 col = texture2DRect( diffuseTexture, gl_TexCoord[0].xy ).xyz;
    vec3 actualCol = col;
	float lum = dot(col,vec3(0.333));
	vec3 ocol = col;
    vec3  nor = normalize( vec3( dFdx(lum), intensity/resolution.x, dFdy(lum) ) );
    float lig = 0.5 + dot(nor,vec3(0.7,0.2,-0.7));
    col = vec3(lig);
	
    vec3 colorized = col * actualCol;
    if ( flowEffect != 0.0 ) {
        gl_FragColor = vec4( mix( actualCol, mix( col, colorized, colorization ), amount * magnitude ), 1.0 );
        
    }
    else {
        gl_FragColor = vec4( mix( actualCol, mix( col, colorized, colorization ), amount), 1.0 );
    }
    
    
//    gl_FragColor = vec4( actualCol * magnitude, 1.0 );
}