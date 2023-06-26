#pragma once

#include <optional>

#include <serviceProvider.hpp>

#include "../aiNode.hpp"
#include "entities/components/transformComponent2d.hpp"
#include "entities/entity.hpp"
#include "entities/iEntityManager.hpp"

class ProximityNode: public AiNode
{
public:
	ProximityNode(std::shared_ptr<AiNode> on_true, std::shared_ptr<AiNode> on_false,
		float threshold, EntityId self_id, std::string entity_b_tag)
		: _on_true(on_true),
		  _on_false(on_false),
		  _threshold(threshold),
		  _self_id(self_id),
		  _entity_b_tag(std::move(entity_b_tag))
	{}
	virtual ~ProximityNode() = default;

	std::optional<Vec2> getEntityPosition(const EntityId id) const
	{
		std::optional<Vec2> found_position;

		auto entity_manager = ServiceProvider::getEntityManager();
		auto entity = entity_manager->getEntity(id);
		if (entity == nullptr)
			return found_position;

		auto transform_weak = entity->getComponent<TransformComponent2d>("transform_component_2d");
		if (transform_weak.expired())
			return found_position;
		auto transform = transform_weak.lock();

		found_position = transform->getPosition() + Vec2(0, 15);

		return found_position;
	}

	std::optional<Vec2> getEntityPosition(const std::string& tag) const
	{
		std::optional<Vec2> found_position;

		auto entity_manager = ServiceProvider::getEntityManager();
		auto entity = entity_manager->getEntity(tag);
		if (entity == nullptr)
			return found_position;

		auto transform_weak = entity->getComponent<TransformComponent2d>("transform_component_2d");
		if (transform_weak.expired())
			return found_position;
		auto transform = transform_weak.lock();

		found_position = transform->getPosition();

		return found_position;
	}

	std::optional<Vec2> getEntityPositionDifference() const
	{
		std::optional<Vec2> position_a = this->getEntityPosition(this->_self_id);
		std::optional<Vec2> position_b = this->getEntityPosition(this->_entity_b_tag);

		std::optional<Vec2> position_diff;

		if (position_a.has_value() && position_b.has_value())
		{
			position_diff = abs(position_b.value() - position_a.value());
		}

		return position_diff;
	}

	void think() override
	{
		auto position_diff = this->getEntityPositionDifference();
		if (!position_diff.has_value())
		{
			this->_on_false->think();
			return;
		}

		// this is detecting in a square around the entity, not a circle
		if (position_diff.value().x <= this->_threshold ||
			position_diff.value().y <= this->_threshold)
		{
			this->_on_true->think();
		}
		else
		{
			this->_on_false->think();
		}
	}

private:
	std::shared_ptr<AiNode> _on_true;
	std::shared_ptr<AiNode> _on_false;
	float _threshold {0.0};
	EntityId _self_id {InvalidEntityId};
	std::string _entity_b_tag;
};
