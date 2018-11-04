#version 330 core

uniform sampler2D myTextureSampler;
varying vec2 uv;
varying float diffuse;

void main()
{
	vec3 color = texture( myTextureSampler, uv ).rgb * diffuse;

    gl_FragColor = vec4(color, 1.0);
};