//Evan Sinasac - 1081418
//INFO6044 Game Engine Frameworks and Patterns (Midterm)
//main.cpp description:
//				Making a little atari rip off tank game! :)

#include "GLCommon.h"
#include "GLMCommon.h"

#include "globalThings.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>


//float lastX = 600.0f;
//float lastY = 320.0f;
//bool firstMouse = true;
//float cameraYaw = 90.0f;
//float cameraPitch = 0.0f;
//
//enum LookDirection
//{
//	UP = 0,
//	DOWN = 180,
//	LEFT = -90,
//	RIGHT = 90
//};

double deltaTime = 0.0;

std::vector<std::string> modelLocations;

std::vector<cBulletShell*> pBullets;
std::vector<cLaser*> pLasers;

int x = 0;//100 - ceil(g_pPlayerTank->GetPosition().x);
int y = 0;// 60 - ceil(g_pPlayerTank->GetPosition().y);
float xPos = 0.0f;// g_pPlayerTank->GetPosition().x;
float yPos = 0.0f;// g_pPlayerTank->GetPosition().y;

//Function signature for DrawObject()
void DrawObject(
	cMesh* pCurrentMesh,
	glm::mat4 matModel,
	GLint matModel_Location,
	GLint matModelInverseTranspose_Location,
	GLuint program,
	cVAOManager* pVAOManager);

//callbacks
//static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

//bool loadWorldFile();
//bool loadModelsFromFile(GLuint& program);
bool loadMazeFile();

int main(int argc, char** argv)
{

	if (!loadMazeFile())
	{
		std::cout << "Closing because maze didn't load" << std::endl;
		return -1;
	}
	GLFWwindow* pWindow;

	GLuint program = 0;		//0 means no shader program

	GLint mvp_location = -1;
	std::stringstream ss;


	glfwSetErrorCallback(GLFW_error_callback);

	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	pWindow = glfwCreateWindow(1000, 1200, "Graphics Midterm!", NULL, NULL);

	if (!pWindow)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(pWindow, GLFW_key_callback);

	glfwSetCursorEnterCallback(pWindow, GLFW_cursor_enter_callback);
	glfwSetCursorPosCallback(pWindow, GLFW_cursor_position_callback);
	glfwSetScrollCallback(pWindow, GLFW_scroll_callback);
	glfwSetMouseButtonCallback(pWindow, GLFW_mouse_button_callback);
	glfwSetWindowSizeCallback(pWindow, GLFW_window_size_callback);


	//glfwSetCursorPosCallback(pWindow, mouse_callback);
	//glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(pWindow);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);


	GLint max_uniform_location = 0;
	GLint* p_max_uniform_location = NULL;
	p_max_uniform_location = &max_uniform_location;
	glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, p_max_uniform_location);

	std::cout << "GL_MAX_UNIFORM_LOCATIONS: " << max_uniform_location << std::endl;

	// Create global things
	::g_StartUp(pWindow);

	::g_pFlyCamera->setEye(glm::vec3(50.0f, 50.0f, -200.0f));
	std::cout << "Fly Camera At: " << ::g_pFlyCamera->getAt().x << " " << ::g_pFlyCamera->getAt().y << " " << ::g_pFlyCamera->getAt().z << std::endl;

	
	cShaderManager::cShader vertShader;
	ss.str("");
	ss << SOLUTION_DIR << "common\\assets\\shaders\\vertShader_01.glsl";
	vertShader.fileName = ss.str();

	cShaderManager::cShader fragShader;
	ss.str("");
	ss << SOLUTION_DIR << "common\\assets\\shaders\\fragShader_01.glsl";
	fragShader.fileName = ss.str();

	if (::g_pShaderManager->createProgramFromFile("Shader#1", vertShader, fragShader))
	{
		std::cout << "Shader compiled OK" << std::endl;
		// 
		// Set the "program" variable to the one the Shader Manager used...
		program = ::g_pShaderManager->getIDFromFriendlyName("Shader#1");
	}
	else
	{
		std::cout << "Error making shader program: " << std::endl;
		std::cout << ::g_pShaderManager->getLastError() << std::endl;
	}

	// Select the shader program we want to use
	// (Note we only have one shader program at this point)
	glUseProgram(program);


	//GLint mvp_location = -1;
	mvp_location = glGetUniformLocation(program, "MVP");

	// Get "uniform locations" (aka the registers these are in)
	GLint matModel_Location = glGetUniformLocation(program, "matModel");
	GLint matView_Location = glGetUniformLocation(program, "matView");
	GLint matProjection_Location = glGetUniformLocation(program, "matProjection");
	GLint matModelInverseTranspose_Location = glGetUniformLocation(program, "matModelInverseTranspose");

	//Lights stuff here

	//    	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
//	                // 0 = pointlight
//					// 1 = spot light
//					// 2 = directional light

	::g_pTheLights->theLights[0].position = glm::vec4(20.0f, 0.0f, -100000.0f, 1.0f);
	::g_pTheLights->theLights[0].specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	::g_pTheLights->theLights[0].diffuse = glm::vec4(0.9922f, 0.9843f, 0.8275f, 1.0f);
	::g_pTheLights->theLights[0].param1.x = 2.0f;
	::g_pTheLights->theLights[0].direction = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	::g_pTheLights->theLights[0].param1.y = 360.0f;
	::g_pTheLights->theLights[0].param1.z = 360.0f;
	::g_pTheLights->theLights[0].atten.x = 1.0f;
	::g_pTheLights->theLights[0].atten.y = 0.1f;
	::g_pTheLights->theLights[0].atten.z = 0.001f;
	::g_pTheLights->TurnOnLight(0);



	// Get the uniform locations of the light shader values
	::g_pTheLights->SetUpUniformLocations(program);

	ss.str("");
	ss << SOLUTION_DIR << "common\\assets\\models\\";

	::g_pVAOManager->setFilePath(ss.str());

	modelLocations.push_back("mazeOuput_xyz_n_rgba.ply");
	modelLocations.push_back("Low_Poly_Tank_Model_3D_model_xyz_n_rgba.ply");
	modelLocations.push_back("Sphere_xyz_n_rgba.ply");

	unsigned int totalVerticesLoaded = 0;
	unsigned int totalTrianglesLoaded = 0;
	for (std::vector<std::string>::iterator itModel = modelLocations.begin(); itModel != modelLocations.end(); itModel++)
	{
		sModelDrawInfo theModel;
		std::string modelName = *itModel;
		std::cout << "Loading " << modelName << "...";
		if (!::g_pVAOManager->LoadModelIntoVAO(modelName, theModel, program))
		{
			std::cout << "didn't work because: " << std::endl;
			std::cout << ::g_pVAOManager->getLastError(true) << std::endl;
		}
		else
		{
			std::cout << "OK." << std::endl;
			std::cout << "\t" << theModel.numberOfVertices << " vertices and " << theModel.numberOfTriangles << " triangles loaded." << std::endl;
			totalTrianglesLoaded += theModel.numberOfTriangles;
			totalVerticesLoaded += theModel.numberOfVertices;
		}
	} //end of for (std::vector<std::string>::iterator itModel

	std::cout << "Done loading models." << std::endl;
	std::cout << "Total vertices loaded = " << totalVerticesLoaded << std::endl;
	std::cout << "Total triangles loaded = " << totalTrianglesLoaded << std::endl;


	//Make meshes
	cMesh* meshMaze = new cMesh();
	meshMaze->meshName = "mazeOuput_xyz_n_rgba.ply";
	meshMaze->orientationXYZ = glm::vec3(0.0f, 0.0f, glm::radians(90.0f));
	meshMaze->positionXYZ = glm::vec3(100.0, 0.0f, 0.0f);
	meshMaze->scale = 1.0f;
	meshMaze->bUseWholeObjectDiffuseColour = true;
	meshMaze->wholeObjectDiffuseRGBA = glm::vec4(0.0f, 1.0f, 0.5f, 1.0f);
	::g_vec_pMeshes.push_back(meshMaze);


	float scale = 0.25f;
	//Make tanks with their meshes (this can be moved into a builder at some point)
	cTank* playerTank = new cPlayerTank();
	::g_vec_pMeshes.push_back(playerTank->makeMesh(glm::vec3(15.0f, 15.0f, 0.0f), glm::vec3(glm::radians(-90.0f), 0.0f, glm::radians((float)UP)), scale, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), UP));
	::g_pPlayerTank = playerTank;
	//::g_vec_pTanks.push_back(playerTank);

	//cMesh* meshPlayerTank = new cMesh();
	//meshPlayerTank->meshName = "Low_Poly_Tank_Model_3D_model_xyz_n_rgba.ply";
	//meshPlayerTank->positionXYZ = glm::vec3(15.0f, 15.0f, 0.0f);
	////meshPlayerTank->orientationXYZ = glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f);	//Look up
	////meshPlayerTank->orientationXYZ = glm::vec3(glm::radians(-90.0f), 0.0f, glm::radians(180.0f));	//Look down
	////meshPlayerTank->orientationXYZ = glm::vec3(glm::radians(-90.0f), 0.0f, glm::radians(90.0f));	//Look right
	//meshPlayerTank->orientationXYZ = glm::vec3(glm::radians(-90.0f), 0.0f, glm::radians(-90.0f));	//Look left
	//meshPlayerTank->scale = 0.5f;
	//meshPlayerTank->bUseWholeObjectDiffuseColour = true;
	//meshPlayerTank->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);		//Red
	//::g_vec_pMeshes.push_back(meshPlayerTank);
	//::g_vec_pMeshes.push_back(playerTank->getMesh());

	cTank* tankOne = new cAITank(0);
	::g_vec_pMeshes.push_back(tankOne->makeMesh(glm::vec3(15.0f, 45.0f, 0.0f), glm::vec3(glm::radians(-90.0f), 0.0f, glm::radians((float)UP)), scale, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), UP));
	::g_vec_pTanks.push_back(tankOne);

	cTank* tankTwo = new cAITank(0);
	::g_vec_pMeshes.push_back(tankTwo->makeMesh(glm::vec3(50.0f, 15.0f, 0.0f), glm::vec3(glm::radians(-90.0f), 0.0f, glm::radians((float)DOWN)), scale, glm::vec4(0.13f, 0.13f, 0.13f, 1.0f), DOWN));
	::g_vec_pTanks.push_back(tankTwo);

	cTank* tankThree = new cAITank(0);
	::g_vec_pMeshes.push_back(tankThree->makeMesh(glm::vec3(50.0f, 45.0f, 0.0f), glm::vec3(glm::radians(-90.0f), 0.0f, glm::radians((float)RIGHT)), scale, glm::vec4(0.0f, 0.4f, 0.4f, 1.0f), RIGHT));
	::g_vec_pTanks.push_back(tankThree);

	cTank* tankFour = new cAITank(1);
	::g_vec_pMeshes.push_back(tankFour->makeMesh(glm::vec3(85.0f, 15.0f, 0.0f), glm::vec3(glm::radians(-90.0f), 0.0f, glm::radians((float)LEFT)), scale, glm::vec4(1.0f, 0.5f, 0.0f, 1.0f), LEFT));
	::g_vec_pTanks.push_back(tankFour);

	cTank* tankFive = new cAITank(1);
	::g_vec_pMeshes.push_back(tankFive->makeMesh(glm::vec3(85.0f, 45.0f, 0.0f), glm::vec3(glm::radians(-90.0f), 0.0f, glm::radians((float)UP)), scale, glm::vec4(0.6f, 0.0f, 0.6f, 1.0f), LEFT));
	::g_vec_pTanks.push_back(tankFive);

	const double MAX_DELTA_TIME = 0.1;	//100 ms
	double previousTime = glfwGetTime();

	::tanksStart = false;

	while (!glfwWindowShouldClose(pWindow))
	{
		float ratio;
		int width, height;
		glm::mat4 matModel;				// used to be "m"; Sometimes it's called "world"
		glm::mat4 matProjection;        // used to be "p";
		glm::mat4 matView;              // used to be "v";

		double currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime;
		deltaTime = (deltaTime > MAX_DELTA_TIME ? MAX_DELTA_TIME : deltaTime);
		previousTime = currentTime;

		glfwGetFramebufferSize(pWindow, &width, &height);
		ratio = width / (float)height;

		//Turn on the depth buffer
		glEnable(GL_DEPTH);         // Turns on the depth buffer
		glEnable(GL_DEPTH_TEST);    // Check if the pixel is already closer

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// *******************************************************
		// Screen is cleared and we are ready to draw the scene...
		// *******************************************************

		// Update the title text
		glfwSetWindowTitle(pWindow, ::g_TitleText.c_str());

		// Copy the light information into the shader to draw the scene
		::g_pTheLights->CopyLightInfoToShader();

		matProjection = glm::perspective(
			::g_pFlyCamera->FOV,
			ratio,
			::g_pFlyCamera->nearPlane,      // Near plane (as large as possible)
			::g_pFlyCamera->farPlane);      // Far plane (as small as possible)

		//matProjection = glm::perspective(0.6f,
		//	ratio,
		//	0.1f,
		//	1000.0f);

		::g_pFlyCamera->Update(deltaTime);

		glm::vec3 cameraEye = ::g_pFlyCamera->getEye();
		glm::vec3 cameraAt = ::g_pFlyCamera->getAtInWorldSpace();
		glm::vec3 cameraUp = ::g_pFlyCamera->getUpVector();


		matView = glm::mat4(1.0f);
		matView = glm::lookAt(cameraEye,   // "eye"
			cameraAt,    // "at"
			cameraUp);

		//matView = glm::mat4(1.0f);

		//matView = glm::lookAt(::cameraEye,
		//	::cameraEye + ::cameraTarget,
		//	upVector);

		glUniformMatrix4fv(matView_Location, 1, GL_FALSE, glm::value_ptr(matView));
		glUniformMatrix4fv(matProjection_Location, 1, GL_FALSE, glm::value_ptr(matProjection));


		//Update tanks
		((cPlayerTank*)::g_pPlayerTank)->updateTank(deltaTime);

		if (::tanksStart)
		{
			for (unsigned int index = 0; index < ::g_vec_pTanks.size(); index++)
			{
				((cAITank*)::g_vec_pTanks[index])->updateTank(deltaTime);
			}
			//Check if firing at anything
			for (unsigned int index1 = 0; index1 < ::g_vec_pTanks.size()-1; index1++)
			{
				LookDirection curDir = ::g_vec_pTanks[index1]->GetLookDirection();
				for (unsigned int index2 = 0; index2 < ::g_vec_pTanks.size()-1; index2++)
				{
					if (index1 != index2)
					{
						if (curDir == UP 
							&& g_vec_pTanks[index1]->GetPosition().x == g_vec_pTanks[index2]->GetPosition().x 
							&& g_vec_pTanks[index2]->GetPosition().y > g_vec_pTanks[index1]->GetPosition().y)
						{
							//Fire up
							if (((cAITank*)g_vec_pTanks[index1])->getWeaponType() == 0)
							{
								if (!((cAITank*)g_vec_pTanks[index1])->GetFiring())
								{
									cBulletShell* newShell = ((cAITank*)g_vec_pTanks[index1])->FireBullet();
									pBullets.push_back(newShell);
									g_vec_pMeshes.push_back(newShell->getMesh());
									((cAITank*)g_vec_pTanks[index1])->SetFiring(true);
								}
							}
							else
							{
								if (((cAITank*)g_vec_pTanks[index1])->GetFiringLaser())
								{
									x = 100 - ceil(g_vec_pTanks[index1]->GetPosition().x);
									y = 60 - ceil(g_vec_pTanks[index1]->GetPosition().y);
									xPos = g_vec_pTanks[index1]->GetPosition().x;
									yPos = g_vec_pTanks[index1]->GetPosition().y;
									y++;
									yPos++;
									while (!::g_Maze[y][x])
									{
										cLaser* newLaser = ((cAITank*)g_vec_pTanks[index1])->FireLaser(glm::vec3(xPos, yPos, 0.0f));
										pLasers.push_back(newLaser);
										g_vec_pMeshes.push_back(newLaser->getMesh());
										y++;
										yPos++;
									}
									((cAITank*)g_vec_pTanks[index1])->SetFiringAndChargingLaser(false, false, 0.0f);
								}

								if (!((cAITank*)g_vec_pTanks[index1])->GetCharging())
								{
									((cAITank*)g_vec_pTanks[index1])->SetFiringAndChargingLaser(false, true, 5.0f);
								}

							}
						}
						else if (curDir == DOWN 
							&& g_vec_pTanks[index1]->GetPosition().x == g_vec_pTanks[index2]->GetPosition().x
							&& g_vec_pTanks[index2]->GetPosition().y < g_vec_pTanks[index1]->GetPosition().y)
						{
							//Fire down
							if (((cAITank*)g_vec_pTanks[index1])->getWeaponType() == 0)
							{
								if (!((cAITank*)g_vec_pTanks[index1])->GetFiring())
								{
									cBulletShell* newShell = ((cAITank*)g_vec_pTanks[index1])->FireBullet();
									pBullets.push_back(newShell);
									g_vec_pMeshes.push_back(newShell->getMesh());
									((cAITank*)g_vec_pTanks[index1])->SetFiring(true);
								}
							}
							else
							{
								if (((cAITank*)g_vec_pTanks[index1])->GetFiringLaser())
								{
									x = 100 - ceil(g_vec_pTanks[index1]->GetPosition().x);
									y = 60 - ceil(g_vec_pTanks[index1]->GetPosition().y);
									xPos = g_vec_pTanks[index1]->GetPosition().x;
									yPos = g_vec_pTanks[index1]->GetPosition().y;
									y--;
									yPos--;
									while (!::g_Maze[y][x])
									{
										cLaser* newLaser = ((cAITank*)g_vec_pTanks[index1])->FireLaser(glm::vec3(xPos, yPos, 0.0f));
										pLasers.push_back(newLaser);
										g_vec_pMeshes.push_back(newLaser->getMesh());
										y--;
										yPos--;
									}
									((cAITank*)g_vec_pTanks[index1])->SetFiringAndChargingLaser(false, false, 0.0f);
								}

								if (!((cAITank*)g_vec_pTanks[index1])->GetCharging())
								{
									((cAITank*)g_vec_pTanks[index1])->SetFiringAndChargingLaser(false, true, 5.0f);
								}
							}
						}
						else if (curDir == RIGHT
							&& g_vec_pTanks[index1]->GetPosition().y == g_vec_pTanks[index2]->GetPosition().y
							&& g_vec_pTanks[index2]->GetPosition().x < g_vec_pTanks[index1]->GetPosition().x)
						{
							//Fire Right
							if (((cAITank*)g_vec_pTanks[index1])->getWeaponType() == 0)
							{
								if (!((cAITank*)g_vec_pTanks[index1])->GetFiring())
								{
									cBulletShell* newShell = ((cAITank*)g_vec_pTanks[index1])->FireBullet();
									pBullets.push_back(newShell);
									g_vec_pMeshes.push_back(newShell->getMesh());
									((cAITank*)g_vec_pTanks[index1])->SetFiring(true);
								}
							}
							else
							{
								if (((cAITank*)g_vec_pTanks[index1])->GetFiringLaser())
								{
									x = 100 - ceil(g_vec_pTanks[index1]->GetPosition().x);
									y = 60 - ceil(g_vec_pTanks[index1]->GetPosition().y);
									xPos = g_vec_pTanks[index1]->GetPosition().x;
									yPos = g_vec_pTanks[index1]->GetPosition().y;
									x--;
									xPos++;
									while (!::g_Maze[y][x])
									{
										cLaser* newLaser = ((cAITank*)g_vec_pTanks[index1])->FireLaser(glm::vec3(xPos, yPos, 0.0f));
										pLasers.push_back(newLaser);
										g_vec_pMeshes.push_back(newLaser->getMesh());
										x--;
										xPos++;
									}
									((cAITank*)g_vec_pTanks[index1])->SetFiringAndChargingLaser(false, false, 0.0f);
								}

								if (!((cAITank*)g_vec_pTanks[index1])->GetCharging())
								{
									((cAITank*)g_vec_pTanks[index1])->SetFiringAndChargingLaser(false, true, 5.0f);
								}
							}
						}
						else if (curDir == LEFT
							&& g_vec_pTanks[index1]->GetPosition().y == g_vec_pTanks[index2]->GetPosition().y
							&& g_vec_pTanks[index2]->GetPosition().x > g_vec_pTanks[index1]->GetPosition().x)
						{
							//Fire Left
							if (((cAITank*)g_vec_pTanks[index1])->getWeaponType() == 0)
							{
								if (!((cAITank*)g_vec_pTanks[index1])->GetFiring())
								{
									cBulletShell* newShell = ((cAITank*)g_vec_pTanks[index1])->FireBullet();
									pBullets.push_back(newShell);
									g_vec_pMeshes.push_back(newShell->getMesh());
									((cAITank*)g_vec_pTanks[index1])->SetFiring(true);
								}
							}
							else
							{
								if (((cAITank*)g_vec_pTanks[index1])->GetFiringLaser())
								{
									x = 100 - ceil(g_vec_pTanks[index1]->GetPosition().x);
									y = 60 - ceil(g_vec_pTanks[index1]->GetPosition().y);
									xPos = g_vec_pTanks[index1]->GetPosition().x;
									yPos = g_vec_pTanks[index1]->GetPosition().y;
									x++;
									xPos--;
									while (!::g_Maze[y][x])
									{
										cLaser* newLaser = ((cAITank*)g_vec_pTanks[index1])->FireLaser(glm::vec3(xPos, yPos, 0.0f));
										pLasers.push_back(newLaser);
										g_vec_pMeshes.push_back(newLaser->getMesh());
										x++;
										xPos--;
									}
									((cAITank*)g_vec_pTanks[index1])->SetFiringAndChargingLaser(false, false, 0.0f);
								}

								if (!((cAITank*)g_vec_pTanks[index1])->GetCharging())
								{
									((cAITank*)g_vec_pTanks[index1])->SetFiringAndChargingLaser(false, true, 5.0f);
								}
							}
						}
					} //end of index1 != index2
				} //end of for index2
				
				if (curDir == UP
					&& g_vec_pTanks[index1]->GetPosition().x == g_pPlayerTank->GetPosition().x
					&& g_pPlayerTank->GetPosition().y > g_vec_pTanks[index1]->GetPosition().y)
				{
					//Fire up
					if (((cAITank*)g_vec_pTanks[index1])->getWeaponType() == 0)
					{
						if (!((cAITank*)g_vec_pTanks[index1])->GetFiring())
						{
							cBulletShell* newShell = ((cAITank*)g_vec_pTanks[index1])->FireBullet();
							pBullets.push_back(newShell);
							g_vec_pMeshes.push_back(newShell->getMesh());
							((cAITank*)g_vec_pTanks[index1])->SetFiring(true);
						}
					}
					else
					{
						if (((cAITank*)g_vec_pTanks[index1])->GetFiringLaser())
						{
							x = 100 - ceil(g_vec_pTanks[index1]->GetPosition().x);
							y = 60 - ceil(g_vec_pTanks[index1]->GetPosition().y);
							xPos = g_vec_pTanks[index1]->GetPosition().x;
							yPos = g_vec_pTanks[index1]->GetPosition().y;
							y++;
							yPos++;
							while (!::g_Maze[y][x])
							{
								cLaser* newLaser = ((cAITank*)g_vec_pTanks[index1])->FireLaser(glm::vec3(xPos, yPos, 0.0f));
								pLasers.push_back(newLaser);
								g_vec_pMeshes.push_back(newLaser->getMesh());
								y++;
								yPos++;
							}
							((cAITank*)g_vec_pTanks[index1])->SetFiringAndChargingLaser(false, false, 0.0f);
						}

						if (!((cAITank*)g_vec_pTanks[index1])->GetCharging())
						{
							((cAITank*)g_vec_pTanks[index1])->SetFiringAndChargingLaser(false, true, 5.0f);
						}
					}
				}
				else if (curDir == DOWN
					&& g_vec_pTanks[index1]->GetPosition().x == g_pPlayerTank->GetPosition().x
					&& g_pPlayerTank->GetPosition().y < g_vec_pTanks[index1]->GetPosition().y)
				{
					//Fire down
					if (((cAITank*)g_vec_pTanks[index1])->getWeaponType() == 0)
					{
						if (!((cAITank*)g_vec_pTanks[index1])->GetFiring())
						{
							cBulletShell* newShell = ((cAITank*)g_vec_pTanks[index1])->FireBullet();
							pBullets.push_back(newShell);
							g_vec_pMeshes.push_back(newShell->getMesh());
							((cAITank*)g_vec_pTanks[index1])->SetFiring(true);
						}
					}
					else
					{
						if (((cAITank*)g_vec_pTanks[index1])->GetFiringLaser())
						{
							x = 100 - ceil(g_vec_pTanks[index1]->GetPosition().x);
							y = 60 - ceil(g_vec_pTanks[index1]->GetPosition().y);
							xPos = g_vec_pTanks[index1]->GetPosition().x;
							yPos = g_vec_pTanks[index1]->GetPosition().y;
							y--;
							yPos--;
							while (!::g_Maze[y][x])
							{
								cLaser* newLaser = ((cAITank*)g_vec_pTanks[index1])->FireLaser(glm::vec3(xPos, yPos, 0.0f));
								pLasers.push_back(newLaser);
								g_vec_pMeshes.push_back(newLaser->getMesh());
								y--;
								yPos--;
							}
							((cAITank*)g_vec_pTanks[index1])->SetFiringAndChargingLaser(false, false, 0.0f);
						}

						if (!((cAITank*)g_vec_pTanks[index1])->GetCharging())
						{
							((cAITank*)g_vec_pTanks[index1])->SetFiringAndChargingLaser(false, true, 5.0f);
						}
					}
				}
				else if (curDir == RIGHT
					&& g_vec_pTanks[index1]->GetPosition().y == g_pPlayerTank->GetPosition().y
					&& g_pPlayerTank->GetPosition().x < g_vec_pTanks[index1]->GetPosition().x)
				{
					//Fire Right
					if (((cAITank*)g_vec_pTanks[index1])->getWeaponType() == 0)
					{
						if (!((cAITank*)g_vec_pTanks[index1])->GetFiring())
						{
							cBulletShell* newShell = ((cAITank*)g_vec_pTanks[index1])->FireBullet();
							pBullets.push_back(newShell);
							g_vec_pMeshes.push_back(newShell->getMesh());
							((cAITank*)g_vec_pTanks[index1])->SetFiring(true);
						}
					}
					else
					{
						if (((cAITank*)g_vec_pTanks[index1])->GetFiringLaser())
						{
							x = 100 - ceil(g_vec_pTanks[index1]->GetPosition().x);
							y = 60 - ceil(g_vec_pTanks[index1]->GetPosition().y);
							xPos = g_vec_pTanks[index1]->GetPosition().x;
							x--;
							xPos++;
							yPos = g_vec_pTanks[index1]->GetPosition().y;
							while (!::g_Maze[y][x])
							{
								cLaser* newLaser = ((cAITank*)g_vec_pTanks[index1])->FireLaser(glm::vec3(xPos, yPos, 0.0f));
								pLasers.push_back(newLaser);
								g_vec_pMeshes.push_back(newLaser->getMesh());
								x--;
								xPos++;
							}
							((cAITank*)g_vec_pTanks[index1])->SetFiringAndChargingLaser(false, false, 0.0f);
						}

						if (!((cAITank*)g_vec_pTanks[index1])->GetCharging())
						{
							((cAITank*)g_vec_pTanks[index1])->SetFiringAndChargingLaser(false, true, 5.0f);
						}
					}
				}
				else if (curDir == LEFT
					&& g_vec_pTanks[index1]->GetPosition().y == g_pPlayerTank->GetPosition().y
					&& g_pPlayerTank->GetPosition().x > g_vec_pTanks[index1]->GetPosition().x)
				{
					//Fire Left
					if (((cAITank*)g_vec_pTanks[index1])->getWeaponType() == 0)
					{
						if (!((cAITank*)g_vec_pTanks[index1])->GetFiring())
						{
							cBulletShell* newShell = ((cAITank*)g_vec_pTanks[index1])->FireBullet();
							pBullets.push_back(newShell);
							g_vec_pMeshes.push_back(newShell->getMesh());
							((cAITank*)g_vec_pTanks[index1])->SetFiring(true);
						}
					}
					else
					{
						if (((cAITank*)g_vec_pTanks[index1])->GetFiringLaser())
						{
							x = 100 - ceil(g_vec_pTanks[index1]->GetPosition().x);
							y = 60 - ceil(g_vec_pTanks[index1]->GetPosition().y);
							xPos = g_vec_pTanks[index1]->GetPosition().x;
							yPos = g_vec_pTanks[index1]->GetPosition().y;
							x++;
							xPos--;
							while (!::g_Maze[y][x])
							{
								cLaser* newLaser = ((cAITank*)g_vec_pTanks[index1])->FireLaser(glm::vec3(xPos, yPos, 0.0f));
								pLasers.push_back(newLaser);
								g_vec_pMeshes.push_back(newLaser->getMesh());
								x++;
								xPos--;
							}
							((cAITank*)g_vec_pTanks[index1])->SetFiringAndChargingLaser(false, false, 0.0f);
						}

						if (!((cAITank*)g_vec_pTanks[index1])->GetCharging())
						{
							((cAITank*)g_vec_pTanks[index1])->SetFiringAndChargingLaser(false, true, 5.0f);
						}
					}
				}
			}//end of for index1
		} //end of if tanksStart
		
		//Question 5
		for (unsigned int index = 0; index < ::g_vec_pTanks.size(); index++)
		{
			LookDirection curDir = ::g_pPlayerTank->GetLookDirection();
			if (curDir == UP
				&& g_pPlayerTank->GetPosition().x == g_vec_pTanks[index]->GetPosition().x
				&& g_pPlayerTank->GetPosition().y < g_vec_pTanks[index]->GetPosition().y)
			{
				//Fire up
				
				if (((cPlayerTank*)g_pPlayerTank)->GetWeaponType() == 0)
				{
					if (!((cPlayerTank*)::g_pPlayerTank)->GetFiringBullet())
					{
						cBulletShell* newShell = new cBulletShell(UP);
						newShell->makeMesh(g_pPlayerTank->GetPosition(), 1.0f, glm::vec4(0.9f, 0.0f, 0.2f, 1.0f));
						pBullets.push_back(newShell);
						g_vec_pMeshes.push_back(newShell->getMesh());
						((cPlayerTank*)::g_pPlayerTank)->SetFiringBullet(true);
					}
				}
				else
				{
					if (((cPlayerTank*)g_pPlayerTank)->GetFiringLaser())
					{
						x = 100 - ceil(g_pPlayerTank->GetPosition().x);
						y = 60 - ceil(g_pPlayerTank->GetPosition().y);
						xPos = g_pPlayerTank->GetPosition().x;
						yPos = g_pPlayerTank->GetPosition().y;
						y--;
						yPos++;
						while (!::g_Maze[y][x])
						{
							cLaser* newLaser = new cLaser();
							newLaser->makeMesh(glm::vec3(xPos, yPos, 0.0f), 1.0f, glm::vec4(0.9f, 0.0f, 0.2f, 1.0f));
							pLasers.push_back(newLaser);
							g_vec_pMeshes.push_back(newLaser->getMesh());
							newLaser = nullptr;
							y--;
							yPos++;
						}
						((cPlayerTank*)g_pPlayerTank)->SetFiringAndChargingLaser(false, false, 0.0f);
					}

					if (!((cPlayerTank*)g_pPlayerTank)->GetCharging())
					{
						((cPlayerTank*)g_pPlayerTank)->SetFiringAndChargingLaser(false, true, 5.0f);
					}
					
				}
			}
			else if (curDir == DOWN
				&& g_pPlayerTank->GetPosition().x == g_vec_pTanks[index]->GetPosition().x
				&& g_pPlayerTank->GetPosition().y > g_vec_pTanks[index]->GetPosition().y)
			{
				//Fire down
				if (((cPlayerTank*)g_pPlayerTank)->GetWeaponType() == 0)
				{
					if (!((cPlayerTank*)::g_pPlayerTank)->GetFiringBullet())
					{
						cBulletShell* newShell = new cBulletShell(DOWN);
						newShell->makeMesh(g_pPlayerTank->GetPosition(), 1.0f, glm::vec4(0.9f, 0.0f, 0.2f, 1.0f));
						pBullets.push_back(newShell);
						g_vec_pMeshes.push_back(newShell->getMesh());
						((cPlayerTank*)::g_pPlayerTank)->SetFiringBullet(true);
					}
				}
				else
				{
					if (((cPlayerTank*)g_pPlayerTank)->GetFiringLaser())
					{
						x = 100 - ceil(g_pPlayerTank->GetPosition().x);
						y = 60 - ceil(g_pPlayerTank->GetPosition().y);
						xPos = g_pPlayerTank->GetPosition().x;
						yPos = g_pPlayerTank->GetPosition().y;
						y++;
						yPos--;
						while (!::g_Maze[y][x])
						{
							cLaser* newLaser = new cLaser();
							newLaser->makeMesh(glm::vec3(xPos, yPos, 0.0f), 1.0f, glm::vec4(0.9f, 0.0f, 0.2f, 1.0f));
							pLasers.push_back(newLaser);
							g_vec_pMeshes.push_back(newLaser->getMesh());
							y++;
							yPos--;
						}
						((cPlayerTank*)g_pPlayerTank)->SetFiringAndChargingLaser(false, false, 0.0f);
					}
					if (!((cPlayerTank*)g_pPlayerTank)->GetCharging())
					{
						((cPlayerTank*)g_pPlayerTank)->SetFiringAndChargingLaser(false, true, 5.0f);
					}
				}
			}
			else if (curDir == RIGHT
				&& g_pPlayerTank->GetPosition().y == g_vec_pTanks[index]->GetPosition().y
				&& g_pPlayerTank->GetPosition().x > g_vec_pTanks[index]->GetPosition().x)
			{
				//Fire Right
				if (((cPlayerTank*)g_pPlayerTank)->GetWeaponType() == 0)
				{
					if (!((cPlayerTank*)::g_pPlayerTank)->GetFiringBullet())
					{
						cBulletShell* newShell = new cBulletShell(RIGHT);
						newShell->makeMesh(g_pPlayerTank->GetPosition(), 1.0f, glm::vec4(0.9f, 0.0f, 0.2f, 1.0f));
						pBullets.push_back(newShell);
						g_vec_pMeshes.push_back(newShell->getMesh());
						((cPlayerTank*)::g_pPlayerTank)->SetFiringBullet(true);
					}
				}
				else
				{
					if (((cPlayerTank*)g_pPlayerTank)->GetFiringLaser())
					{
						x = 100 - ceil(g_pPlayerTank->GetPosition().x);
						y = 60 - ceil(g_pPlayerTank->GetPosition().y);
						xPos = g_pPlayerTank->GetPosition().x;
						yPos = g_pPlayerTank->GetPosition().y;
						x++;
						xPos--;
						while (!::g_Maze[y][x])
						{
							cLaser* newLaser = new cLaser();
							newLaser->makeMesh(glm::vec3(xPos, yPos, 0.0f), 1.0f, glm::vec4(0.9f, 0.0f, 0.2f, 1.0f));
							pLasers.push_back(newLaser);
							g_vec_pMeshes.push_back(newLaser->getMesh());
							x++;
							xPos--;
						}
						((cPlayerTank*)g_pPlayerTank)->SetFiringAndChargingLaser(false, false, 0.0f);
					}
					if (!((cPlayerTank*)g_pPlayerTank)->GetCharging())
					{
						((cPlayerTank*)g_pPlayerTank)->SetFiringAndChargingLaser(false, true, 5.0f);
					}
				}
			}
			else if (curDir == LEFT
				&& g_pPlayerTank->GetPosition().y == g_vec_pTanks[index]->GetPosition().y
				&& g_pPlayerTank->GetPosition().x < g_vec_pTanks[index]->GetPosition().x)
			{
				//Fire Left
				if (((cPlayerTank*)g_pPlayerTank)->GetWeaponType() == 0)
				{
					if (!((cPlayerTank*)::g_pPlayerTank)->GetFiringBullet())
					{
						cBulletShell* newShell = new cBulletShell(LEFT);
						newShell->makeMesh(g_pPlayerTank->GetPosition(), 1.0f, glm::vec4(0.9f, 0.0f, 0.2f, 1.0f));
						pBullets.push_back(newShell);
						g_vec_pMeshes.push_back(newShell->getMesh());
						((cPlayerTank*)::g_pPlayerTank)->SetFiringBullet(true);
					}
				}
				else
				{
					if (((cPlayerTank*)g_pPlayerTank)->GetFiringLaser())
					{
						x = 100 - ceil(g_pPlayerTank->GetPosition().x);
						y = 60 - ceil(g_pPlayerTank->GetPosition().y);
						xPos = g_pPlayerTank->GetPosition().x;
						yPos = g_pPlayerTank->GetPosition().y;
						x--;
						xPos++;
						while (!::g_Maze[y][x])
						{
							cLaser* newLaser = new cLaser();
							newLaser->makeMesh(glm::vec3(xPos, yPos, 0.0f), 1.0f, glm::vec4(0.9f, 0.0f, 0.2f, 1.0f));
							pLasers.push_back(newLaser);
							g_vec_pMeshes.push_back(newLaser->getMesh());
							x--;
							xPos++;
						}
						((cPlayerTank*)g_pPlayerTank)->SetFiringAndChargingLaser(false, false, 0.0f);
					}
					if (!((cPlayerTank*)g_pPlayerTank)->GetCharging())
					{
						((cPlayerTank*)g_pPlayerTank)->SetFiringAndChargingLaser(false, true, 5.0f);
					}
				}
			}
		}

		for (unsigned int i = 0; i < pBullets.size(); i++)
		{
			pBullets[i]->updatePos();
			if (!pBullets[i]->getAlive())
			{
				//Have to remove it
				pBullets.erase(pBullets.begin() + i);
				g_vec_pMeshes.erase(g_vec_pMeshes.begin() + i + 7);	//Maze and 6 tanks
				i--;
				((cPlayerTank*)::g_pPlayerTank)->SetFiringBullet(false);
			}
		}

		for (unsigned int i = 0; i < pLasers.size(); i++)
		{
			pLasers[i]->updatePos(deltaTime);
			if (!pLasers[i]->getAlive())
			{
				pLasers.erase(pLasers.begin() + i);
				g_vec_pMeshes.erase(g_vec_pMeshes.begin() + i + 7);	//Maze and 6 tanks
				i--;
			}
		}

		// **********************************************************************
		// Draw the "scene" of all objects.
		// i.e. go through the vector and draw each one...
		// **********************************************************************
		for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
		{
			// So the code is a little easier...
			cMesh* pCurrentMesh = ::g_vec_pMeshes[index];

			matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)
			//mat4x4_identity(m);

			DrawObject(pCurrentMesh,
				matModel,
				matModel_Location,
				matModelInverseTranspose_Location,
				program,
				::g_pVAOManager);


		}//for (unsigned int index
		// Scene is drawn
		// **********************************************************************

		// "Present" what we've drawn.
		glfwSwapBuffers(pWindow);        // Show what we've drawn

		// Process any events that have happened
		glfwPollEvents();

		// Handle OUR keyboard, mouse stuff
		handleAsyncKeyboard(pWindow, deltaTime);
		handleAsyncMouse(pWindow, deltaTime);
	}//while (!glfwWindowShouldClose(window))

	// All done, so delete things...
	::g_ShutDown(pWindow);


	glfwDestroyWindow(pWindow);

	glfwTerminate();
	exit(EXIT_SUCCESS);
} //end of main


bool loadMazeFile()
{
	std::ifstream theFile("Maze_1081418.txt");
	if (!theFile.is_open())
	{
		fprintf(stderr, "Could not open Maze file!");
		return false;
	}

	std::string nextLetter;
	for (unsigned int y = 0; y < 61; y++)	//for some reason my maze is 61 long?  idk, instructions said 60
	{
		for (unsigned int x = 0; x <= 100; x++)
		{
			nextLetter = theFile.get();
			::g_Maze[y][x] = (nextLetter == "X" ? true : false);
			//std::cout << "Maze X: " << x << " Y: " << y << " is " << (::g_Maze[y][x] ? "a wall" : "open space") << std::endl;
			//std::cout << nextLetter;
		}
		theFile.get();	//Newline
		//std::cout << std::endl;
	}

	theFile.close();
	return true;
}