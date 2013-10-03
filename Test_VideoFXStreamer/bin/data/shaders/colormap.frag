uniform vec2 resolution;
uniform vec2 size;
uniform float time;
uniform vec2 mouse;
uniform sampler2DRect diffuseTexture;
uniform sampler2DRect colorMap;
uniform vec2 colorMapSize;
uniform float amount;

// vector field encoded into floating point texture
uniform sampler2DRect vectorField;
uniform vec2 vectorFieldSize;

#define M_PI 3.1415926535897932384626433832795


float remap(float value, float inputMin, float inputMax, float outputMin, float outputMax ) {
    return ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
}

vec4 adjustHue( vec4 col, float amt ) {
    const vec4  kRGBToYPrime = vec4 (0.299, 0.587, 0.114, 0.0);
    const vec4  kRGBToI     = vec4 (0.596, -0.275, -0.321, 0.0);
    const vec4  kRGBToQ     = vec4 (0.212, -0.523, 0.311, 0.0);
    
    const vec4  kYIQToR   = vec4 (1.0, 0.956, 0.621, 0.0);
    const vec4  kYIQToG   = vec4 (1.0, -0.272, -0.647, 0.0);
    const vec4  kYIQToB   = vec4 (1.0, -1.107, 1.704, 0.0);
    
    // Convert to YIQ
    float   YPrime  = dot (col, kRGBToYPrime);
    float   I      = dot (col, kRGBToI);
    float   Q      = dot (col, kRGBToQ);
    
    // Calculate the hue and chroma
    float   hue     = atan (Q, I);
    float   chroma  = sqrt (I * I + Q * Q);
    
    hue += amt;
    
    // Convert back to YIQ
    Q = chroma * sin (hue);
    I = chroma * cos (hue);
    
    // Convert back to RGB
    vec4    yIQ   = vec4 (YPrime, I, Q, 0.0);
    
    vec4 finalColor;
    finalColor.r = dot (yIQ, kYIQToR);
    finalColor.g = dot (yIQ, kYIQToG);
    finalColor.b = dot (yIQ, kYIQToB);
    finalColor.a = col.a;
    
    return finalColor;
}

void main(void) {
    
    // get vector from vf
    vec2 flow = texture2DRect( vectorField, gl_TexCoord[0].xy * (vectorFieldSize / size ) ).rg;
    flow.x = remap( flow.x, 0.0, 1.0, -1.0, 1.0 );
    flow.y = remap( flow.y, 0.0, 1.0, -1.0, 1.0 );
    float magnitude = length( flow );
    
    
    
    // Sample the input pixel
    vec4    color   = texture2DRect(diffuseTexture, gl_TexCoord [ 0 ].xy);
    float   luma    = dot( vec3(0.2126, 0.7152, 0.0722), color.rgb );
    
    vec3 rampColor = texture2DRect( colorMap, vec2( mod(luma + time, 1.0), 0.0 ) * colorMapSize ).xyz;
    
    // Save the result
//    gl_FragColor = color;
    gl_FragColor = vec4( mix( color.xyz, rampColor, amount ), 1.0 );
//    gl_FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );
}