uniform float time;
uniform bool grayscale;
uniform float nIntensity;
uniform float sIntensity;
uniform float sCount;
uniform float amount;
uniform sampler2DRect diffuseTexture;

uniform vec2 size;
uniform vec2 resolution;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
void main() {
    vec4 cTextureScreen = texture2DRect( diffuseTexture, gl_TexCoord[0].xy );
    vec3 cResult = cTextureScreen.rgb;
    vec2 normCoords = gl_TexCoord[0].xy / size;
    
    vec2 sc = vec2( sin( normCoords.y * sCount * amount ), cos( normCoords.y * sCount * amount ) );
    cResult += cTextureScreen.rgb * vec3( sc.x, sc.y, sc.x ) * sIntensity * amount;
    cResult = cTextureScreen.rgb + clamp( nIntensity * amount, 0.0,1.0 ) * ( cResult - cTextureScreen.rgb );
    gl_FragColor =  vec4( cResult, cTextureScreen.a );
}