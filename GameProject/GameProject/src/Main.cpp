#include "Header Files/Include.h"
#include "Header Files/Transform.h"
#include "Header Files/Game.h"
#include "Header Files/Scene.h"
#define STB_IMAGE_IMPLEMENTATION

/* 
inline void sleepSomeTime() { Sleep(100); }
ISoundEngine* SoundEngine = createIrrKlangDevice();
*/

Game* GAME;
// commetn for push test
int main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	srand(time(NULL));

	GAME = new Game();

	float _deltaTime = 0.0, _curTime = 0.0, _lastTime = 0.0;
	GLFWwindow* _window = GAME->GetWindow();

	/* SOUND RELATED STUFF. CREATE A CLASS FOR IT. */
	/*
	if (!SoundEngine)
		cout << "Problems creating a sound engine with irrKlang." << endl;

	ISound* music = SoundEngine->play3D("src/Audio/Music - 16bit Deja Vu.mp3", vec3df(0, 0, 0), true, false, true);
	// SoundEngine->play2D("src/Audio/Music - 16bit Deja Vu.mp3", GL_TRUE);

	if (music)
		music->setMinDistance(5.0f); // Distance at which the sound gets played at max volume. Default = 1.0.

	cout << "Playing streamed sound in 3D." << endl;
	cout << "Press ESCAPE to quit, any other key to play sound at random position." << endl << endl;

	cout << "+ = Listener position" << endl;
	cout << "o = Playing sound" << endl;

	float posOnCircle = 0;
	const float radius = 5;

	while (true)
	{
		posOnCircle += 0.04f;
		vec3df pos3d(radius * cosf(posOnCircle), 0, radius * sinf(posOnCircle * 0.5f));
		SoundEngine->setListenerPosition(vec3df(0, 18, 33), vec3df(0, -4, 3));

		if (music)
			music->setPosition(pos3d); // Set position in 3D-space

		char stringForDisplay[] = "          +         ";

		int charPos = (int)((pos3d.X + radius) / radius * 10.0f);
		if (charPos >= 0 && charPos < 20)
			stringForDisplay[charPos] = 'o';

		int playPos = music ? music->getPlayPosition() : 0; // Seconds into song!

		printf("\rx:(%s)   3dpos: %.1f %.1f %.1f, playpos:%d:%.2d    ",
			stringForDisplay, pos3d.X, pos3d.Y, pos3d.Z,
			playPos / 60000, (playPos % 60000) / 1000);

		sleepSomeTime();

		vec3df pos(fmodf((float)rand(), radius * 2) - radius, 0, 0);

		const char* filename;

		if (rand() % 2)
			filename = "src/audio/Player - Dying 1.mp3";
		else				
			filename = "src/audio/Player - Dying 2.mp3";

		SoundEngine->play3D(filename, pos);

		printf("\nplaying %s at %.1f %.1f %.1f\n",
			filename, pos.X, pos.Y, pos.Z);
	}
	*/

	while (!glfwWindowShouldClose(_window))
	{	
		_curTime = (float)glfwGetTime();
		_deltaTime = _curTime - _lastTime;
		_lastTime = _curTime;

		GAME->Update(_deltaTime);
		GAME->PlayWithLights(_curTime);
		GAME->Render();
	}
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	
	//delete wndw;
	delete GAME;

	exit(0); //Used to be called in window destructor -> caused memory leaks. This fixes it.

	/* SOUND RELATED DELETION */
	/* 
	if (music)
		music->drop();

	SoundEngine->drop(); 
	*/

	_CrtDumpMemoryLeaks();
	
	return 0;
}
