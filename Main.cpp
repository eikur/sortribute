#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "3rdParty/SDL/include/SDL.h"
#pragma comment( lib, "3rdParty/SDL/libx86/SDL2.lib" )
#pragma comment( lib, "3rdParty/SDL/libx86/SDL2main.lib" )

enum class MainState
{
	Creation,
	Start,
	Update ,
	Finish,
	Exit
};

Application* App = nullptr;

int main(int argc, char ** argv)
{
	ReportMemoryLeaks();

	int main_return = EXIT_FAILURE;
	MainState state = MainState::Creation;

	while (state != MainState::Exit)
	{
		switch (state)
		{
		case MainState::Creation:

			LOG("Application Creation --------------");
			App = new Application();
			state = MainState::Start;
			break;

		case MainState::Start:

			LOG("Application Init --------------");
			if (App->Init() == false)
			{
				LOG("Application Init exits with error -----");
				state = MainState::Start;
			}
			else
			{
				state = MainState::Update;
				LOG("Application Update --------------");
			}

			break;

		case MainState::Update:
		{
			UpdateStatus updateReturn = App->Update();

			if (updateReturn == UpdateStatus::Error)
			{
				LOG("Application Update exits with error -----");
				state = MainState::Exit;
			}

			if (updateReturn == UpdateStatus::Stop)
				state = MainState::Finish;
		}
			break;

		case MainState::Finish:

			LOG("Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				LOG("Application CleanUp exits with error -----");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MainState::Exit;

			break;

		}
	}

	RELEASE( App);
	LOG("Bye :)\n");
	return main_return;
}