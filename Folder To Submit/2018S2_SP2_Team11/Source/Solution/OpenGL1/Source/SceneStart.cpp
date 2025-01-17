#include "SceneStart.h"



SceneStart::SceneStart()
{
}

SceneStart::~SceneStart()
{
}


void SceneStart::InitDerived()
{
	//init objects
	std::cout << "Generating OBJs\n";
	background.Init("background", "OBJ//LevelsButton.obj", "Image//Background.tga", Vector3(orthSize.x * 0.5f, orthSize.y * 0.5f, -14), Vector3(0, 0, 0), Vector3(60, 30, 10));
	title.Init("title", "OBJ//LevelsButton.obj", "Image//title.tga", Vector3(orthSize.x * 0.5f,  25, -8), Vector3(0, 0, 0), Vector3(20, 20, 0));
	mouse.Init("mouse", MeshBuilder::GenerateCube(Color(1, 0, 0)), "", Vector3(orthSize.x * 0.5f, orthSize.y * 0.5f, 10), Vector3(0, 0, 0), Vector3(1, 1, 0));
	play.Init("play", "OBJ//LevelsButton.obj", "Image//play.tga", Vector3(8, 5, 0));
	playText.Init("playText", "OBJ//LevelsButton.obj", "Image//playText.tga", Vector3(8, 5, 1), Vector3(0, 0, 0), Vector3(20, 20, 0));

	float carY = orthSize.y * 0.5f - 3;
	garage.Init("garage", MeshBuilder::GenerateCube(Color(0, 0, 1)), "", Vector3(orthSize.x * 0.5f, carY, -15), Vector3(0, 0, 0), Vector3(19, 9, 0));

	existingCar = nullptr;
	existingCar = static_cast<Car*>(GetDontDestroyGameObject("car"));
	if (!existingCar)
	{
		existingCar = new Car;
		existingCar->Init("car", "OBJ//taxiNoWheels.obj", "Image//Green.tga");
		existingCar->SetMaterial(shiny);
		existingCar->GetPaint()->SetPaintColor(Color(0, 1, 0));

		RequestDontDestroy(static_cast<GameObject*>(existingCar));
	}
	existingCar->SetTranslate(Vector3(orthSize.x * 0.5f, carY, -2));
	existingCar->SetRotate(Vector3(0, 90, 0));
	existingCar->SetScale(Vector3(9, 9, 9));
	std::cout << "Done Generating OBJs\n";

	//initialising UI
	allButtons.push_back(&play);
	allButtons.push_back(&garage);
	allButtons.push_back(&exit);

	totalbuttons = 3;
	buttonindex = 0;

	mouse.SetOrthSize(orthSize);
	mouse.SetAllButton(allButtons);
	freeRoam.ToggleCamMovement();

	//background music
	PlaySound(TEXT("Music//background.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void SceneStart::RenderDerived()
{
	RenderObjectOnScreen(&background, false);
	RenderObjectOnScreen(&title, false);
	RenderObjectOnScreen(&play, false);
	RenderObjectOnScreen(&playText, false);
	RenderObjectOnScreen(&garage, false);
	RenderObjectOnScreen(existingCar, false);
	RenderObjectOnScreen(&mouse, false);
}

void SceneStart::UpdateDerived(double dt)
{
	mouse.Move(dt);
	mouse.CheckHover();

	existingCar->IncrementRotate(Vector3(0, 1, 0));



	for (Button* b : allButtons)	//for each button in the vector carryout the function
	{
		b->AnimateButton();
	}

	if (play.GetOnClickEvent())
	{
		allButtons[0]->SetHover(false);
		play.SetOnClickEvent(false);
		mouse.ResetMousePos();
		RequestChangeScene(3);
	}

	if (garage.GetOnClickEvent())
	{
		allButtons[1]->SetHover(false);
		garage.SetOnClickEvent(false);
		mouse.ResetMousePos();
		RequestChangeScene(6);
	}

	if (Application::leftMouseClick)
	{
		for (unsigned int i = 0; i < allButtons.size(); ++i)
		{
			if (allButtons[i]->GetHover())
			{
				allButtons[i]->DoAction();
				break;
			}
		}
	}
}

void SceneStart::UpdateDerivedBounced(double dt)
{
	////selector controls
	//if (Application::IsKeyPressed(VK_RETURN))
	//{
	//	allButtons[buttonindex]->DoAction();
	//}

	//if (Application::IsKeyPressed(VK_DOWN))
	//{
	//	allButtons[buttonindex]->SetHover(false);
	//	if (buttonindex >= totalbuttons - 1)
	//	{
	//		buttonindex = 0;
	//	}

	//	else
	//		buttonindex++;

	//	allButtons[buttonindex]->SetHover(true);
	//}

	//if (Application::IsKeyPressed(VK_UP))
	//{
	//	allButtons[buttonindex]->SetHover(false);
	//	if (buttonindex <= 0)
	//		buttonindex = totalbuttons - 1;
	//	else
	//		buttonindex--;

	//	allButtons[buttonindex]->SetHover(true);
	//}
}
