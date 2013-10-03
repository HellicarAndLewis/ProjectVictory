uniform sampler2DRect diffuseTexture;
uniform float amount;
uniform float rgbAmount;
uniform float angle;
uniform vec2 size;
uniform float time;

#define M_PI 3.1415926535897932384626433832795

void main() {
    vec2 offset = rgbAmount * amount * vec2( 1000.0, 1000.0 ) * vec2( cos(angle * amount * M_PI), sin(angle * amount * M_PI));
//    vec2 offset = 100.0 * vec2( .25, .25 );
//    offset *= size;
    
    vec4 cr = texture2DRect(diffuseTexture, gl_TexCoord[0].xy + offset );
    vec4 cga = texture2DRect(diffuseTexture, gl_TexCoord[0].xy);
    vec4 cb = texture2DRect(diffuseTexture, gl_TexCoord[0].xy - offset);
    gl_FragColor = vec4(cr.r, cga.g, cb.b, cga.a);
//    gl_FragColor = vec4( size.x, size.y, 0.0, 1.0 );
}