#include "MyOctant.h"
using namespace Simplex;
//MyOctant 
uint MyOctant::m_uMyOctantCount = 0;
uint MyOctant::m_uMaxLevel = 3; 
uint MyOctant::m_uIdealEntityCount = 5;
uint MyOctant::GetMyOctantCount(void) { return m_uMyOctantCount; }

//allocations
void MyOctant::Init(void)
{
	m_uChildren = 0;
	
	m_fSize = 0.0f;
	
	m_uID = m_uMyOctantCount;
	m_uLevel = 0;

	m_v3Center = vector3(0.0f, 0.0f, 0.0f);
	m_v3Min = vector3(0.0f, 0.0f, 0.0f);
	m_v3Max = vector3(0.0f, 0.0f, 0.0f);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	m_pRoot = nullptr;
	m_pParent = nullptr;
	for (uint n = 0; n < 8; n++)
	{
		m_pChild[n] = nullptr;
	}
}
void MyOctant::Swap(MyOctant& other)
{
	//swap all of the variables out for other's variables 
	std::swap(m_uChildren, other.m_uChildren);

	std::swap(m_fSize, other.m_fSize);

	std::swap(m_uID, other.m_uID);
	std::swap(m_uLevel, other.m_uLevel);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);

	std::swap(m_pMeshMngr, other.m_pMeshMngr);
	std::swap(m_pEntityMngr, other.m_pEntityMngr);

	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_pParent, other.m_pParent);
	for (uint n = 0; n < 8; n++)
	{
		std::swap(m_pChild[n], other.m_pChild[n]);
	}
}
void Simplex::MyOctant::Release(void)
{
	//if root, kill al the branches 
	if (m_uLevel == 0) 
	{
		KillBranches();
	}
	//clear everything else
	m_uChildren = 0;
	m_fSize = 0.0f;
	m_EntitiyList.clear();
	m_lChild.clear();
}


//Accessors
float Simplex::MyOctant::GetSize(void){return m_fSize;}
vector3 Simplex::MyOctant::GetCenterGlobal(void){return m_v3Center;}
vector3 Simplex::MyOctant::GetMaxGlobal(void){return m_v3Max;}
vector3 Simplex::MyOctant::GetMinGlobal(void){return m_v3Min;}
MyOctant* Simplex::MyOctant::GetParent(void){return m_pParent;}
MyOctant* Simplex::MyOctant::GetChild(uint a_nChild) 
{ 
	if (a_nChild > 7) 
	{
		return nullptr;
	}
	return m_pChild[a_nChild];
}


//the big three
//constructors
Simplex::MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	//set all of them
	Init();

	//reset the maxLevel and IdealEntityCount
	m_uMyOctantCount = 0;
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
	m_uID = m_uMyOctantCount;

	//set root
	m_pRoot = this;
	//clear child list
	m_lChild.clear();

	//define the rigid bodys 
	std::vector<vector3> MinMaxList; // hold the min max for the bounding objects

	//Set Rigid Body for collisions 
	uint nObjects = m_pEntityMngr->GetEntityCount();
	for (uint i = 0; i < nObjects; i++) 
	{
		MyEntity* l_pEntity = m_pEntityMngr->GetEntity(i);
		MyRigidBody* l_pRigidBody = l_pEntity->GetRigidBody();
		MinMaxList.push_back(l_pRigidBody->GetMinGlobal());
		MinMaxList.push_back(l_pRigidBody->GetMaxGlobal());
	}

	MyRigidBody* l_pRigidBody = new MyRigidBody(MinMaxList);

	vector3 v3HalfWidth = l_pRigidBody->GetHalfWidth();
	float fMax = v3HalfWidth.x;
	for (int i = 1; i < 3; i++) 
	{
		if (fMax < v3HalfWidth[i]) 
		{
			fMax = v3HalfWidth[i];
		}
	}
	//vector3 v3Center = l_pRigidBody->GetCenterLocal();
	MinMaxList.clear();
	SafeDelete(l_pRigidBody);

	//set the variables based on rigid body
	m_fSize = fMax * 2.0f;
	//m_v3Center = v3Center;
	m_v3Min = m_v3Center - (vector3(fMax));
	m_v3Max = m_v3Center + (vector3(fMax));

	//add the MyOctant count
	m_uMyOctantCount++;

	//construct tree
	ConstructTree(m_uMaxLevel);
}
Simplex::MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	//Set using init
	Init();

	//reset center  and size
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;

	//reset the min and max based on the new information
	m_v3Min = m_v3Center - (vector3(m_fSize) / 2.0f);
	m_v3Max = m_v3Center + (vector3(m_fSize) / 2.0f);

	//add MyOctantCount
	m_uMyOctantCount++;
}
//copy constructor 
Simplex::MyOctant::MyOctant(MyOctant const& other)
{
	//set all of the variables to the other's
	m_uChildren = other.m_uChildren;

	m_fSize = other.m_fSize;

	m_uID = other.m_uMyOctantCount;
	m_uLevel = other.m_uLevel;

	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	m_pRoot , other.m_pRoot;
	m_lChild, other.m_lChild;
	m_pParent = other.m_pParent;
	for (uint n = 0; n < 8; n++)
	{
		m_pChild[n] = other.m_pChild[n];
	}
}
MyOctant& Simplex::MyOctant::operator=(MyOctant const& other)
{
	//clear and replace
	if (this != &other) 
	{
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}
	//return
	return *this;
}
//destroy 
Simplex::MyOctant::~MyOctant(void) {Release();}

//other methods
void Simplex::MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{
	//check if the id is the same as the index
	if (m_uID == a_nIndex)
	{
		//display the wire cube
		matrix4 m4Matrix = glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize));
		m_pMeshMngr->AddWireCubeToRenderList(m4Matrix , a_v3Color, RENDER_WIRE);

		//return
		return;
	}

	//if not, loop through and call for each child
	for (uint i = 0; i < m_uChildren; i++) 
	{
		m_pChild[i]->Display(a_nIndex);
	}
	

}
void Simplex::MyOctant::Display(vector3 a_v3Color)
{
	

	//loop through and call for each child
	for (uint i = 0; i < m_uChildren; i++) 
	{
		m_pChild[i]->Display(a_v3Color);
	}
	
	//display the wire cube
	//m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), 
		//a_v3Color, RENDER_WIRE);
	matrix4 m4Matrix = glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize));
	m_pMeshMngr->AddWireCubeToRenderList(m4Matrix, a_v3Color, RENDER_WIRE);

	
}
void Simplex::MyOctant::DisplaysLeafs(vector3 a_v3Color)
{
	//number of leafs is equal to the number of children (should be eight)
	uint nLeafs = m_lChild.size();
	//loop through and call for each child
	for (uint i = 0; i < nLeafs; i++) 
	{
		m_lChild[i]->DisplaysLeafs(a_v3Color);
	}

	//render wire cube 
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
		glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
}

//clear the entity list
void Simplex::MyOctant::ClearEntityList(void)
{
	//go through and clear the list
	for (uint i = 0; i < m_uChildren; i++) 
	{
		m_pChild[i]->ClearEntityList();
	}
	m_EntitiyList.clear();
}

void Simplex::MyOctant::Subdivide(void)
{
	//if you are at the max level, do not subdivide
	if (m_uLevel >= m_uMaxLevel) 
	{
		return;
	}

	//if you have already subdivided, do not subdivide 
	if (m_uChildren != 0) 
	{
		return;
	}

	//make it not a leaf
	m_uChildren = 8;

	//go through and set all the new octants 
	float l_fSize = m_fSize / 4.0f;
	float fSizeDouble = l_fSize * 2.0f;
	vector3 v3Center;

	//MyOctant 0 
	v3Center = m_v3Center;
	v3Center.x -= l_fSize;
	v3Center.y -= l_fSize;
	v3Center.z -= l_fSize;
	m_pChild[0] = new MyOctant(v3Center, fSizeDouble);

	//MyOctant 1
	v3Center.x += fSizeDouble;
	m_pChild[1] = new MyOctant(v3Center, fSizeDouble);

	//MyOctant 2
	v3Center.z += fSizeDouble;
	m_pChild[2] = new MyOctant(v3Center, fSizeDouble);

	//MyOctant 3
	v3Center.x -= fSizeDouble;
	m_pChild[3] = new MyOctant(v3Center, fSizeDouble);

	//MyOctant 4
	v3Center.y += fSizeDouble;
	m_pChild[4] = new MyOctant(v3Center, fSizeDouble);

	//MyOctant 5
	v3Center.z -= fSizeDouble;
	m_pChild[5] = new MyOctant(v3Center, fSizeDouble);

	//MyOctant 6
	v3Center.x += fSizeDouble;
	m_pChild[6] = new MyOctant(v3Center, fSizeDouble);

	//MyOctant 7
	v3Center.z += fSizeDouble;
	m_pChild[7] = new MyOctant(v3Center, fSizeDouble);

	//loop through the list of children
	for (uint i = 0; i < 8; i++) 
	{
		m_pChild[i]->m_pRoot = m_pRoot;
		m_pChild[i]->m_pParent = this;
		m_pChild[i]->m_uLevel = m_uLevel + 1;
		if (m_pChild[i]->ContainsMoreThan(m_uIdealEntityCount)) 
		{
			m_pChild[i]->Subdivide();
		}
	}
}

bool Simplex::MyOctant::IsColliding(uint a_uRBIndex)
{
	//get number of objects 
	uint nObjectCount = m_pEntityMngr->GetEntityCount();

	//if index given is larger than the number of element in the bounding object there is no collision
	if (a_uRBIndex >= nObjectCount) 
	{
		return false;
	}

	//AABB stuff
	MyEntity* pEntity = m_pEntityMngr->GetEntity(a_uRBIndex);
	MyRigidBody* pRigidBody = pEntity->GetRigidBody();
	vector3 v3MinO = pRigidBody->GetMinGlobal();
	vector3 v3MaxO = pRigidBody->GetMaxGlobal();

	//check for x
	if (m_v3Max.x < v3MinO.x) 
	{
		return false;
	}
	if (m_v3Min.x > v3MaxO.x) 
	{
		return false;
	}

	//check y
	if (m_v3Max.y < v3MinO.y) 
	{
		return false;
	}
	if (m_v3Min.y > v3MaxO.y) 
	{
		return false;
	}

	//check z
	if (m_v3Max.z < v3MinO.z) 
	{
		return false;
	}
	if (m_v3Min.z > v3MaxO.z) 
	{
		return false;
	}

	//once none cleared 
	return true;
}

bool Simplex::MyOctant::IsLeaf(void)
{
	//check if children exist
	if (m_uChildren == 0) 
	{
		return true;
	}
	return false;
}

bool Simplex::MyOctant::ContainsMoreThan(uint a_nEntities)
{

	uint nCount = 0;
	uint nObjectCount = m_pEntityMngr->GetEntityCount();

	for (uint n = 0; n < nObjectCount; n++) 
	{
		if (IsColliding(n)) 
		{
			nCount++;
		}
		if (nCount > a_nEntities) 
		{
			return true;
		}
	}

	return false;
}

void Simplex::MyOctant::KillBranches(void)
{
	//recursion
	for (uint nIndex = 0; nIndex < m_uChildren; nIndex++) 
	{
		m_pChild[nIndex]->KillBranches();
		delete m_pChild[nIndex];
		m_pChild[nIndex] = nullptr;
	}
	m_uChildren = 0;
}

void Simplex::MyOctant::ConstructTree(uint a_nMaxLevel)
{
	//check if its the root 
	if (m_uLevel != 0) 
	{
		return;
	}

	m_uMaxLevel = a_nMaxLevel;

	m_uMyOctantCount = 1;

	//clear entity list
	m_EntitiyList.clear();

	//clear the tree
	KillBranches();
	m_lChild.clear();

	//if the base tree
	if (ContainsMoreThan(m_uIdealEntityCount)) 
	{
		Subdivide();
	}

	//add MyOctant ID to entities 
	AssignIDtoEntity();

	//construct the list of objects 
	ConstructList();
}

void Simplex::MyOctant::AssignIDtoEntity(void)
{
	//go until you hit a list 
	for (uint nChild = 0; nChild < m_uChildren; nChild++) 
	{
		m_pChild[nChild]->AssignIDtoEntity();
	}
	if (m_uChildren == 0)  // if this is a leaf 
	{
		uint nEntities = m_pEntityMngr->GetEntityCount();
		for (uint nIndex = 0; nIndex < nEntities; nIndex++) 
		{
			if (IsColliding(nIndex)) 
			{
				m_EntitiyList.push_back(nIndex);
				m_pEntityMngr->AddDimension(nIndex, m_uID);
			}
		}
	}
}

void Simplex::MyOctant::ConstructList(void)
{
	for (uint nChild = 0; nChild < m_uChildren; nChild++) 
	{
		m_pChild[nChild]->ConstructList();
	}
	if (m_EntitiyList.size() > 0) 
	{
		m_pRoot->m_lChild.push_back(this);
	}
}

