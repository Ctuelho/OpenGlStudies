#version 330 core

uniform sampler2D myTextureSampler;
varying vec2 uv;

void main()
{
	vec3 color = texture( myTextureSampler, uv ).rgb;

    gl_FragColor = vec4(color, 1.0);
};