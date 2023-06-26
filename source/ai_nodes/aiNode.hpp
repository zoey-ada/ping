#pragma once

class AiNode
{
public:
	virtual ~AiNode() = default;

	virtual void think() = 0;
};
