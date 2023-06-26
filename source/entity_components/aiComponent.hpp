#pragma once

#include <entities/components/entityComponent.hpp>

class AiNode;

namespace platypus
{
class AiComponent;
}

class AiComponent: public EntityComponent
{
public:
	AiComponent() = default;
	virtual ~AiComponent();

	[[nodiscard]] bool initialize(const std::shared_ptr<Message>& data) override;
	void postInitialize() override;

	void update(const Milliseconds delta) override;
	[[nodiscard]] const char* name() const override { return "ai_component"; }

private:
	std::shared_ptr<platypus::AiComponent> _ai_data;
	std::shared_ptr<AiNode> _brain;

	void createBrain();
};

[[nodiscard]] std::shared_ptr<EntityComponent> createAiComponent();
