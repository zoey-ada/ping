#pragma once

#include <optional>

#include <serviceProvider.hpp>

#include "../aiNode.hpp"

class WaitNode: public AiNode
{
public:
	WaitNode() = default;
	virtual ~WaitNode() = default;

	void think() override
	{
		// do nothing
	}
};
