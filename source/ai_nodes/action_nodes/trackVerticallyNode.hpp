#pragma once

#include <optional>

#include <serviceProvider.hpp>

#include "../aiNode.hpp"
#include "entities/components/physicsComponent.hpp"
#include "entities/components/transformComponent2d.hpp"
#include "entities/entity.hpp"
#include "entities/iEntityManager.hpp"
#include "physics/rigidBodyObject.hpp"

class TrackVerticallyNode: public AiNode
{
public:
	// move entity a vertically towards entity b
	TrackVerticallyNode(EntityId entity_a_id, std::string entity_b_tag)
		: _entity_a_id(entity_a_id), _entity_b_tag(std::move(entity_b_tag))
	{}
	virtual ~TrackVerticallyNode() = default;

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

		found_position = transform->getPosition();

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

	Vec3 calculate_force_ammount(const Vec2& position_a, const Vec2& position_b)
	{
		auto difference = position_b - position_a - Vec2(0, 15);
		difference *= 20;

		return Vec3(0.0, difference.y, 0.0);
	}

	void think() override
	{
		auto position_a = this->getEntityPosition(this->_entity_a_id);
		auto position_b = this->getEntityPosition(this->_entity_b_tag);
		if (!position_a.has_value() || !position_b.has_value())
		{
			return;
		}

		auto entity_manager = ServiceProvider::getEntityManager();
		auto entity = entity_manager->getEntity(this->_entity_a_id);
		if (entity == nullptr)
			return;

		auto physics_weak = entity->getComponent<PhysicsComponent>("physics_component");
		if (physics_weak.expired())
			return;
		auto physics = physics_weak.lock();

		auto force = this->calculate_force_ammount(position_a.value(), position_b.value());
		physics->getPhysicsObject()->applyForce(force);
	}

private:
	EntityId _entity_a_id {InvalidEntityId};
	std::string _entity_b_tag;
};
