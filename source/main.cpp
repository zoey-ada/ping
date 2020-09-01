#include "demoGameLogic.h"

#include <platypus.h>
#include <views/humanView.h>

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
	game->initialize();
	auto ret = game->run();
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
