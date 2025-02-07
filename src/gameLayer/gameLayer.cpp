#define GLM_ENABLE_EXPERIMENTAL
#include "gameLayer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "platformInput.h"
#include "imgui.h"
#include <iostream>
#include <sstream>
#include "imfilebrowser.h"
#include <gl2d/gl2d.h>
#include <platformTools.h>

struct GameData
{
	//glm::vec2 rectPos = {100,100};
	glm::vec2 playerPos = { 100, 100 };

}gameData;

gl2d::Renderer2D renderer;
gl2d::Texture playerTexture;

bool initGame()
{
	//initializing stuff for the renderer
	gl2d::init();
	renderer.create();

	playerTexture.loadFromFile(RESOURCES_PATH "frog.png", true);

	//loading the saved data. Loading an entire structure like this makes savind game data very easy.
	platform::readEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));

	return true;
}


//IMPORTANT NOTICE, IF YOU WANT TO SHIP THE GAME TO ANOTHER PC READ THE README.MD IN THE GITHUB
//https://github.com/meemknight/cmakeSetup
//OR THE INSTRUCTION IN THE CMAKE FILE.
//YOU HAVE TO CHANGE A FLAG IN THE CMAKE SO THAT RESOURCES_PATH POINTS TO RELATIVE PATHS
//BECAUSE OF SOME CMAKE PROGBLMS, RESOURCES_PATH IS SET TO BE ABSOLUTE DURING PRODUCTION FOR MAKING IT EASIER.

bool gameLogic(float deltaTime)
{
#pragma region init stuff
	int w = 0; int h = 0;
	w = platform::getFrameBufferSizeX(); //window w
	h = platform::getFrameBufferSizeY(); //window h
	
	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT); //clear screen

	renderer.updateWindowMetrics(w, h);
#pragma endregion


	if (platform::isButtonHeld(platform::Button::A))
	{
		gameData.playerPos.x -= deltaTime * 200;
	}
	if (platform::isButtonHeld(platform::Button::D))
	{
		gameData.playerPos.x += deltaTime * 200;
	}
	if (platform::isButtonHeld(platform::Button::W))
	{
		gameData.playerPos.y -= deltaTime * 200;
	}
	if (platform::isButtonHeld(platform::Button::S))
	{
		gameData.playerPos.y += deltaTime * 200;
	}

	//gameData.rectPos = glm::clamp(gameData.rectPos, glm::vec2{0,0}, glm::vec2{w - 100,h - 100});
	//renderer.renderRectangle({gameData.rectPos, 100, 100}, Colors_Blue);

	gameData.playerPos = glm::clamp(gameData.playerPos, glm::vec2{ 0,0 }, glm::vec2{ w - 100,h - 100 });

	renderer.renderRectangle({ gameData.playerPos, 100, 100}, playerTexture);

	renderer.flush();


	//ImGui::ShowDemoWindow();
	ImGui::Begin("Test Imgui");

	ImGui::DragFloat2("Positions", &gameData.playerPos[0]);

	ImGui::End();

	return true;
#pragma endregion

}

//This function might not be be called if the program is forced closed
void closeGame()
{

	//saved the data.
	platform::writeEntireFile(RESOURCES_PATH "gameData.data", &gameData, sizeof(GameData));

}
