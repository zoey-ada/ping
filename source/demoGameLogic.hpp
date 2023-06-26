#pragma once

#include <random>

#include <baseGameLogic.hpp>
#include <events/dataField.hpp>
#include <ui/dynamicText.hpp>
#include <ui/staticText.hpp>

class IEvent;
class InputEvent;

class DemoGameLogic: public BaseGameLogic
{
public:
	DemoGameLogic();
	virtual ~DemoGameLogic() = default;

	void onUpdate(Milliseconds delta) override;

	bool initialize() override;

	void onResetGame();
	void onPlayer1Score();
	void onPlayer2Score();
	void onResetBall();
	void onPlayer1PaddleHit();
	void onPlayer2PaddleHit();

	void registerEventSinks();
	void onCollision(std::shared_ptr<IEvent> event);
	void onInput(std::shared_ptr<IEvent> event);
	void onRunningInput(std::shared_ptr<InputEvent> event);
	void onMainMenuInput(std::shared_ptr<InputEvent> event);
	void onPauseMenuInput(std::shared_ptr<InputEvent> event);

private:
	DataField<int> _p1_score {"player1-score", 0};
	DataField<int> _p2_score {"player2-score", 0};

	EntityId _player1_paddle {InvalidEntityId};
	EntityId _player1_goal {InvalidEntityId};

	EntityId _player2_paddle {InvalidEntityId};
	EntityId _player2_goal {InvalidEntityId};

	EntityId _ball {InvalidEntityId};

	StaticText _p1_score_label {Vec2(-27.5, -20), "Player 1:"};
	DynamicIntegerText _p1_score_value {"player1-score", Vec2(-20, -20), 0};
	StaticText _p2_score_label {Vec2(12.5, -20), "Player 2:"};
	DynamicIntegerText _p2_score_value {"player2-score", Vec2(20, -20), 0};

	std::uniform_real_distribution<> _ball_wobble_rng;
	std::mt19937 _ball_wobble_seed;

	EntityId _menu_image {InvalidEntityId};
};
