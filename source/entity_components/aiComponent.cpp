#include "aiComponent.hpp"

#include <platypus_proto/entity.hpp>

#include <ai_nodes/action_nodes/trackVerticallyNode.hpp>
#include <ai_nodes/action_nodes/waitNode.hpp>
#include <ai_nodes/decision_nodes/proximityNode.hpp>
// #include <serviceProvider.hpp>

AiComponent::~AiComponent()
{}

bool AiComponent::initialize(const std::shared_ptr<Message>& data)
{
	this->_ai_data = std::dynamic_pointer_cast<platypus::AiComponent>(data);
	return this->_ai_data != nullptr;
}

void AiComponent::postInitialize()
{
	this->createBrain();
}

void AiComponent::update(const Milliseconds /*delta*/)
{
	this->_brain->think();
}

void AiComponent::createBrain()
{
	// auto entity_manager = ServiceProvider::getEntityManager();
	// auto ball = entity_manager->getEntity("ball");

	this->_brain = std::make_shared<ProximityNode>(
		std::make_shared<TrackVerticallyNode>(this->_owner->getId(), "ball"),
		std::make_shared<WaitNode>(), 20.0, this->_owner->getId(), "ball");
}

std::shared_ptr<EntityComponent> createAiComponent()
{
	return std::make_shared<AiComponent>();
}
