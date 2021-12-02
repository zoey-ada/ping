#include "demoGameLogic.h"

#include <platypus.hpp>
#include <utilities/logging/logger.hpp>
#include <views/humanView.hpp>

class DemoGame : public Platypus
{
public:
	DemoGame()
		: Platypus("Demo Game") { }
	virtual ~DemoGame() = default;

	const bool initialize() { return Platypus::initialize(); }
	const int run() { return Platypus::run(); }
	void shutdown() { return Platypus::shutdown(); }

protected:
	const std::shared_ptr<BaseGameLogic> createLogicAndView() override
	{
		auto logic = std::make_shared<DemoGameLogic>();
		logic->initialize();

		auto view = std::make_shared<HumanView>(this->_renderer, this->_settings);
		logic->addView(view);

		return logic;
	}
};

int main()
{
	auto game = std::make_shared<DemoGame>();
	//logInfo("starting game");
	game->initialize();
	logWarning("running game", "state");
	auto ret = game->run();
	logError("ending game");
	game->shutdown();

	return ret;
}

#ifdef _WIN32
	#include <Windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int)
{
	return main();
}
#endif
