uniform sampler3D video3D;
// vector field encoded into floating point texture
uniform sampler2DRect vectorField;
uniform vec2 vectorFieldSize;
// a mask texture
uniform sampler2DRect mask;
uniform vec2 maskSize;
// current offset for depth level in video
uniform float depthOffset;
// scale of the magnitude
uniform float magScaler;
uniform float amount;

// seconds since app started
uniform float time;
uniform vec2 size;


float remap(float value, float inputMin, float inputMax, float outputMin, float outputMax ) {
    return ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
}

void main() {
    vec2 flow = texture2DRect( vectorField, gl_TexCoord[0].xy * (vectorFieldSize / size ) ).rg;
//    flow.x = remap( flow.x, 0.0, 1.0, -1.0, 1.0 );
//    flow.y = remap( flow.y, 0.0, 1.0, -1.0, 1.0 );
    flow /= 100.0;
    float magnitude = length( flow ) * magScaler;
    
    
    vec3 passThruColor = texture3D( video3D, vec3( gl_TexCoord[0].x/size.x, gl_TexCoord[0].y/size.y, depthOffset  ) ).xyz;
    vec3 col = texture3D( video3D, vec3( gl_TexCoord[0].x/size.x, gl_TexCoord[0].y/size.y, depthOffset + magnitude ) ).xyz;
//    vec3 col = texture3D( video3D, vec3( gl_TexCoord[0].x/size.x, gl_TexCoord[0].y/size.y, depthOffset ) ).xyz;
    gl_FragColor = vec4( mix( passThruColor,col,amount), 1.0 );
//    gl_FragColor = vec4( 1.0, 1.0, 0.0, 1.0 );
}