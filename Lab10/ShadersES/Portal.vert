#version 300 es
#extension GL_ANGLE_clip_cull_distance : enable
// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE.txt for full details.
// ----------------------------------------------------------------

// Uniforms for world transform and view-proj
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// Attribute 0 is position, 1 is normal, 2 is tex coords.
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// Any vertex outputs (other than position)
out vec2 fragTexCoord;

// Clip plane
uniform vec4 uClipPlane;
out float gl_ClipDistance[1];

void main()
{
	// Convert position to homogeneous coordinates
	vec4 pos = vec4(inPosition, 1.0);
	// Transform to position world space, then clip space
	gl_Position = pos * uWorldTransform * uViewProj;

	// Pass along the texture coordinate to frag shader
	fragTexCoord = inTexCoord;
	
	if (dot(inNormal, vec3(0.0f, 0.0f, 1.0f)) > 0.99f)
	{
		gl_ClipDistance[0] = 1.0f;
	}
	else
	{
		gl_ClipDistance[0] = dot(pos * uWorldTransform, uClipPlane);
	}
}
