#pragma once

#include "../Utilities/Math.h"

struct Vertex 
{
	Vector3 pos;
	Vector3 color;
	Vector3 norm;
	Vector3 binorm;
	Vector3 tgt;
	Vector2 uv;

public:
	Vertex() {};

	Vertex(Vector3 pos, Vector3 color, Vector3 norm, Vector3 binorm, Vector3 tgt, Vector2 uv)
		:pos{pos},color(color),norm{norm},binorm{binorm},tgt{tgt},uv{uv}
	{};

	Vertex(const Vertex& other) 
		:pos( other.pos ), color(other.color),norm( other.norm ), binorm( other.binorm ), tgt (other.tgt ), uv( other.uv){};

};