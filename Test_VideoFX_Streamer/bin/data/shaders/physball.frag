uniform sampler2DRect diffuseTexture;
uniform sampler2DRect displacementTexture;
uniform vec2 displacementTextureSize;
uniform float displacementScale;

uniform vec2 resolution;
uniform vec2 size;
uniform float time;
uniform vec2 mouse;

void main(void) {
    vec2 disp = texture2DRect( displacementTexture, gl_TexCoord[0].xy * ( displacementTextureSize / size ) ).xy;
    disp -= vec2( .5, .5 );
    vec3 col = texture2DRect( diffuseTexture, gl_TexCoord[0].xy + disp * displacementScale ).xyz;
    gl_FragColor = vec4( col, 1.0 );
}