#version 300 es
// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE.txt for full details.
// ----------------------------------------------------------------

// WebGL requires specifying float precision
precision highp float;

// Tex coord input from vertex shader
in vec2 fragTexCoord;

// This corresponds to the output color to the color buffer
out vec4 outColor;

// This is used for the texture sampling
uniform sampler2D uTexture;
uniform sampler2D uMask;

void main()
{
	vec4 texColor = texture(uTexture, fragTexCoord);
	if (texture(uMask, fragTexCoord).r < 0.05f)
		discard;
	outColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
}
