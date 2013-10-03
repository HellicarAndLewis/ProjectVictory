uniform sampler2DRect diffuseTexture;

void main(void) {
    vec3 col = texture2DRect( diffuseTexture, gl_TexCoord[0].xy ).xyz;
    gl_FragColor = vec4( col, 1.0 );
}