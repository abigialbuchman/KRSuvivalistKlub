#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 0.0f, 100.0f), //Position
		vector3(0.0f, 0.0f, 99.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

	std::cout << m_pMeshMngr << std::endl;

#ifdef DEBUG
	uint uInstances = 900;
#else
	uint uInstances = 1849;
#endif
	int nSquare = static_cast<int>(std::sqrt(uInstances));
	m_uObjects = nSquare * nSquare;
	uint uIndex = -1;

	m_pEntityMngr->AddEntity("CustomModels\\ground.obj", "NA", "ground");
	m_pEntityMngr->SetModelMatrix(glm::translate(vector3(0.0f)) * glm::scale(vector3(forestSize, 1, forestSize)));

	for (int i = 0; i < numTrees; i++) {
		uIndex++;
		int tree = rand() % 5 + 1;
		switch (tree)
		{
		case 1:
			m_pEntityMngr->AddEntity("CustomModels\\tree1.obj", "NA", "tree");
			break;
		case 2:
			m_pEntityMngr->AddEntity("CustomModels\\tree2.obj", "NA", "tree");
			break;
		case 3:
			m_pEntityMngr->AddEntity("CustomModels\\tree3.obj", "NA", "tree");
			break;
		case 4:
			m_pEntityMngr->AddEntity("CustomModels\\tree4.obj", "NA", "tree");
			break;
		case 5:
			m_pEntityMngr->AddEntity("CustomModels\\tree5.obj", "NA", "tree");
			break;
		default:
			std::cout << "yonkers" << std::endl;
			break;
		}
		vector3 v3Position = vector3((rand() % forestSize) - forestSize / 2,
			0,
			(rand() % forestSize) - forestSize / 2);
		matrix4 m4Position = glm::translate(v3Position);
		m_pEntityMngr->SetModelMatrix(m4Position);
	}

	for (int i = 0; i < nSquare; i++)
	{
		for (int j = 0; j < nSquare; j++)
		{
			
		}
	}

	m_uOctantLevels = 1;
	m_pEntityMngr->Update();
	m_pRoot = new MyOctant(m_uOctantLevels, 5);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
	
	//Update Entity Manager
	m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//display octree
	
	

	if (m_uOctantID == -1) 
	{
		m_pRoot->Display();
	}
	else 
	{
		m_pRoot->Display(m_uOctantID);
	}
	//test render list 
	//m_pMeshMngr->AddWireCubeToRenderList(IDENTITY_M4,
		//C_YELLOW, RENDER_WIRE);

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();


	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui,
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//Release the octree
	SafeDelete(m_pRoot);
	//release GUI
	ShutdownGUI();
}