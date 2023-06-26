#include "demoGameLogic.hpp"

#include <random>

#include <audio/iAudioSystem.hpp>
#include <entities/components/physicsComponent.hpp>
#include <entities/components/renderComponent2d.hpp>
#include <entities/entity.hpp>
#include <entities/entityFactory.hpp>
#include <events/events/collisionEvent.hpp>
#include <events/events/inputEvent.hpp>
#include <events/events/quitGameEvent.hpp>
#include <events/iEventManager.hpp>
#include <input/inputManager.hpp>
#include <physics/iPhysicsSystem.hpp>
#include <physics/rigidBodyObject.hpp>
#include <renderer/scene_nodes/sceneNode.hpp>
#include <serviceProvider.hpp>
#include <utilities/logging/iLoggingSystem.hpp>
#include <utilities/logging/logger.hpp>
#include <views/iView.hpp>

#include "entity_components/aiComponent.hpp"

DemoGameLogic::DemoGameLogic()
{
	std::random_device rd;   // Will be used to obtain a seed for the random number engine
	this->_ball_wobble_seed =
		std::mt19937(rd());  // Standard mersenne_twister_engine seeded with rd()
	this->_ball_wobble_rng = std::uniform_real_distribution<>(-200.0, 200.0);
}

void DemoGameLogic::onUpdate(Milliseconds delta)
{
	switch (this->_state)
	{
	case GameState::Initializing:
	{
		for (const auto& view : this->_views)
			view->onRestore();

		registerEventSink("input-event", DemoGameLogic::onInput);
		this->changeState(GameState::LoadingMainMenu);
		break;
	}
	case GameState::LoadingMainMenu:
	{
		auto menu_image = this->createEntity("menu_title.json");
		this->_menu_image = menu_image->getId();
		this->changeState(GameState::MainMenu);
		break;
	}
	case GameState::MainMenu:
	{
		for (const auto& view : this->_views)
			view->onUpdate(delta);

		for (const auto& entity : this->_entities)
			entity.second->update(delta);

		break;
	}
	case GameState::LoadingPauseMenu:
	{
		auto menu_image = this->createEntity("menu_title.json");
		this->_menu_image = menu_image->getId();
		this->changeState(GameState::PauseMenu);
		break;
	}
	case GameState::PauseMenu:
	{
		ServiceProvider::getPhysicsSystem()->pause();

		for (const auto& view : this->_views)
			view->onUpdate(delta);

		for (const auto& entity : this->_entities)
			entity.second->update(delta);

		break;
	}
	case GameState::Loading:
	{
		this->createEntity("left_border.json");
		this->createEntity("right_border.json");
		this->createEntity("top_border.json");
		this->createEntity("bottom_border.json");

		auto ball = this->createEntity("ball.json");
		this->_ball = ball->getId();

		auto p1_paddle = this->createEntity("p1_paddle.json");
		this->_player1_paddle = p1_paddle->getId();

		auto p2_paddle = this->createEntity("p2_paddle.json");
		this->_player2_paddle = p2_paddle->getId();

		auto p1_goal = this->createEntity("p1_goal.json");
		this->_player1_goal = p1_goal->getId();

		auto p2_goal = this->createEntity("p2_goal.json");
		this->_player2_goal = p2_goal->getId();

		this->_p1_score_label.initialize();
		this->_p1_score_value.initialize();
		this->_p2_score_label.initialize();
		this->_p2_score_value.initialize();

		this->onResetGame();
		this->registerEventSinks();

		ServiceProvider::getInputManager()->loadConfiguration("./input_config.json");

		this->changeState(GameState::Running);
		break;
	}
	case GameState::Running:
	{
		for (const auto& view : this->_views)
			view->onUpdate(delta);

		for (const auto& entity : this->_entities)
			entity.second->update(delta);

		break;
	}
	default:
		throw std::exception("The game entered a state that could not be handled.");
	}
	// TODO: game states
}

bool DemoGameLogic::initialize()
{
	if (!BaseGameLogic::initialize())
		return false;

	this->_entity_factory->registerComponent("AiComponent", createAiComponent);

	return true;
}

void DemoGameLogic::registerEventSinks()
{
	registerEventSink("collision-event", DemoGameLogic::onCollision);
}

void DemoGameLogic::onCollision(std::shared_ptr<IEvent> event)
{
	auto collision_event = std::static_pointer_cast<CollisionEvent>(event);
	if (isCollisionBetween(collision_event, this->_ball, this->_player1_goal))
	{
		this->onPlayer2Score();
	}
	else if (isCollisionBetween(collision_event, this->_ball, this->_player2_goal))
	{
		this->onPlayer1Score();
	}
	else if (isCollisionBetween(collision_event, this->_ball, this->_player1_paddle))
	{
		this->onPlayer1PaddleHit();
	}
	else if (isCollisionBetween(collision_event, this->_ball, this->_player2_paddle))
	{
		this->onPlayer2PaddleHit();
	}
}

void DemoGameLogic::onInput(std::shared_ptr<IEvent> event)
{
	auto input_event = dynamic_pointer_cast<InputEvent>(event);

	switch (this->_state)
	{
	case GameState::Running:
		this->onRunningInput(input_event);
		break;

	case GameState::MainMenu:
		this->onMainMenuInput(input_event);
		break;

	case GameState::PauseMenu:
		this->onPauseMenuInput(input_event);
		break;
	default:
		break;
	}
}

void DemoGameLogic::onMainMenuInput(std::shared_ptr<InputEvent> event)
{
	if (event->_command_name.compare("select") == 0)
	{
		if (event->_action == InputActionType::Start)
		{
			this->removeEntity(this->_menu_image);
			this->_menu_image = InvalidDelegateId;
			this->changeState(GameState::Loading);
		}
	}
}

void DemoGameLogic::onPauseMenuInput(std::shared_ptr<InputEvent> event)
{
	if (event->_command_name.compare("menu") == 0)
	{
		if (event->_action == InputActionType::Start)
		{
			this->removeEntity(this->_menu_image);
			this->_menu_image = InvalidDelegateId;

			ServiceProvider::getPhysicsSystem()->resume();

			this->changeState(GameState::Running);
		}
	}
	else if (event->_command_name.compare("move-up") == 0)
	{
		if (event->_action == InputActionType::Start)
		{
			// this->changeState(GameState::Running);
		}
	}
	else if (event->_command_name.compare("move-down") == 0)
	{
		if (event->_action == InputActionType::Start)
		{
			// this->changeState(GameState::Running);
		}
	}
	else if (event->_command_name.compare("select") == 0)
	{
		if (event->_action == InputActionType::Start)
		{
			auto event = std::make_shared<QuitGameEvent>();
			ServiceProvider::getEventManager()->triggerEvent(event);
			// this->changeState(GameState::Running);
		}
	}
}

void DemoGameLogic::onRunningInput(std::shared_ptr<InputEvent> event)
{
	if (event->_command_name.compare("menu") == 0)
	{
		if (event->_action == InputActionType::Start)
		{
			this->changeState(GameState::LoadingPauseMenu);
		}
	}
}

void DemoGameLogic::onResetGame()
{
	this->_p1_score = 0;
	this->_p2_score = 0;
	this->onResetBall();
}

void DemoGameLogic::onPlayer1Score()
{
	this->_p1_score = this->_p1_score + 1;
	this->onResetBall();
}

void DemoGameLogic::onPlayer2Score()
{
	this->_p2_score = this->_p2_score + 1;
	this->onResetBall();
}

void DemoGameLogic::onPlayer1PaddleHit()
{
	// add randomness to ball direction on paddle hit
	ServiceProvider::getLoggingSystem()->error("paddle hit: start");
	Vec3 ball_wobble(0, this->_ball_wobble_rng(this->_ball_wobble_seed), 0);
	auto ball = this->_entities[this->_ball]->getComponent<PhysicsComponent>("physics_component");
	auto ball_physics = ball.lock();
	ball_physics->getPhysicsObject()->applyForce(ball_wobble);
	ServiceProvider::getLoggingSystem()->error("paddle hit: physics done");

	auto audio_system = ServiceProvider::getAudioSystem();
	audio_system->playSound("assets.zip/pop3.wav", false, 0);
	ServiceProvider::getLoggingSystem()->error("paddle hit: audio done");
}

void DemoGameLogic::onPlayer2PaddleHit()
{
	// add randomness to ball direction on paddle hit
	Vec3 ball_wobble(0, this->_ball_wobble_rng(this->_ball_wobble_seed), 0);
	auto ball = this->_entities[this->_ball]->getComponent<PhysicsComponent>("physics_component");
	auto ball_physics = ball.lock();
	ball_physics->getPhysicsObject()->applyForce(ball_wobble);

	auto audio_system = ServiceProvider::getAudioSystem();
	audio_system->playSound("assets.zip/pop3.wav", false, 0);
}

void DemoGameLogic::onResetBall()
{
	this->removeEntity(this->_ball);
	auto ball = this->createEntity("ball.json");
	this->_ball = ball->getId();
}
