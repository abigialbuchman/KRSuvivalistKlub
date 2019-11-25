//#pragma once
/*----------------------------------------------
Programmer: Abigail Buchman (amb5759 @rit.edu)
Date: 2017/06
----------------------------------------------*/
#ifndef _MYOCTANTCLASS_H_
#define _MYOCTANTCLASS_H_

#include "MyEntityManager.h"

namespace Simplex
{
	//systems class
	class MyOctant
	{
		//variables 
		static uint m_uMyOctantCount; // this will store the number of MyOctants instantiated
		static uint m_uMaxLevel; //will store the maxmium level an MyOctant can go
		static uint m_uIdealEntityCount; //will tell how many ideal Entities this object will contain

		uint m_uID = 0; //will store the current ID for this MyOctant
		uint m_uLevel = 0; // store the current level of the MyOctant 
		uint m_uChildren = 0; //Number of children on the MyOctant (either zero or eight)

		float m_fSize = 0.0f; //size of the MyOctant 

		MeshManager* m_pMeshMngr = nullptr; // MeshManager Singleton
		MyEntityManager* m_pEntityMngr = nullptr; //EntityManager Singleton

		vector3 m_v3Center = vector3(0.0f); // will store the center of the MyOctant
		vector3 m_v3Min = vector3(0.0f); //will store the minimum vector of the MyOctant
		vector3 m_v3Max = vector3(0.0f); // Will store the maximum vector of the MyOctant 

		MyOctant* m_pParent = nullptr; //Will store the parent of the current MyOctant
		MyOctant* m_pChild[8]; //will store the childre of the current MyOctant 

		std::vector<uint> m_EntitiyList = std::vector<uint>(); // list of all entities under this MyOctant
		MyOctant* m_pRoot = nullptr; // Root MyOctant
		std::vector<MyOctant*> m_lChild; // list of nodes that contain objects (this will be applied to the root only)

	public:
		/*
		USAGE: Constructor, will create an MyOctant containing all MagnaEntities Instances in the Mesh
		manager currently contains
		Arguements:
		-uint a_nMaxLevel = 2 ->sets the maxium level of subdivisions
		-uint nIdealEntityCount = 5 -> Sets the ideal level of objects per MyOctant
		OUTPUT: class object
		*/
		MyOctant(uint a_nMaxLevel = 2, uint a_nIdealEntityCount = 5);
		/*
		USAGE: Constructor
		ARGUEMENTS:
		-vector3 a_v3center -> center in global space
		-float a_fSize -> size of each side of the MyOctant volume
		OUTPUT: class object
		*/
		MyOctant(vector3 a_v3Center, float a_fSize);
		/*
		USAGE: Copy Constuctor
		ARGUEMENTS: Class Object to Copy
		OUTPUT: class object instance
		*/
		MyOctant(MyOctant const& other);
		/*
		USAGE: Copy Assignment Operator
		ARGUEMENT: class object to copy
		OUTPUT: ----
		*/
		MyOctant& operator=(MyOctant const& other);
		/*
		USAGE: Destructor
		AGRUEMENTS: ---
		OUTPUT: ---
		*/
		~MyOctant(void);
		/*
		USAGE: Changes object contents for other object's
		AGRUEMENTS:
		- MyOctant& other -> other to swap content from
		OUTPUT: ---
		*/
		void Swap(MyOctant& other);
		/*
		USAGE: Get this MyOctant's size
		ARGUEMENTS: ---
		OUTPUT: size od MyOctants
		*/
		float GetSize(void);
		/*
		USAGE: Gets the center of the MyOctant in global space
		ARGUEMENTS: ---
		OUTPUT: Center of the MyOctant in global space
		*/
		vector3 GetCenterGlobal(void);
		/*
		USAGE:Gets the min corner of the MyOctant in global space
		ARGUEMENTS: ---
		OUTPUT: Minimum in global space
		*/
		vector3 GetMaxGlobal(void);
		/*
		USAGE: Gets the max corner of the MyOctant in global space
		ARGUEMENTS: ---
		OUTPUT: Minimum in global space
		*/
		vector3 GetMinGlobal(void);
		/*
		USAGE:Displays the MyOctant volume specified by index including the objects underneath
		ARGUEMENTS:
		-uint a_nIndex -> MyOctant to be displayed
		-vector3 a_v3Color = REYELLOW -> Color of the volume to display.
		OUTPUT: ---
		*/
		void Display(uint a_nIndex, vector3 a_v3Color = C_YELLOW);
		void Display(vector3 a_v3Color = C_YELLOW);
		/*
		USAGE: Display the non empty leafs in the octree
		ARGUEMENTS:
		-vector3 a_v3color = R_Yellow -> color of the volume to display
		OUTPUT: ---
		*/
		void DisplaysLeafs(vector3 a_v3Color = C_YELLOW);
		/*
		USAGE: Clears the Entity list for each node
		ARGUEMENTS: ---
		OUTPUT: ---
		*/
		void ClearEntityList(void);
		/*
		USAGE: allocates 8 smaller MyOctants in the child pointers
		ARGUEMENTS: ---
		OUTPUT: ---
		*/
		void Subdivide(void);
		/*
		USAGE: returns the child specified in the index
		ARGUEMENTS:
		-uint a_nChild -> index of the child (from 0 to 7)
		OUTPUT: MyOctant object (child in index)
		*/
		MyOctant* GetChild(uint a_nChild);
		/*
		USAGE: returns the parent of the MyOctant
		ARGUEMENTS: ---
		OUTPUT: MyOctant object (parent)
		*/
		MyOctant* GetParent(void);
		/*
		USAGE: Asks the MyOctant if it does not contain any children (its a leaf)
		ARGUEMENTS: ---
		OUTPUT: It contains no children
		*/
		bool IsLeaf(void);
		/*
		USAGE: Asks the MyOctant if it contains more than this many bounding objects
		ARGUEMENTS:
		-uint a_nEntities -> number of Entities to query
		OUTPUT: It contains at least this many Entities
		*/
		bool ContainsMoreThan(uint a_nEntities);
		/*
		USAGE: Deletes all children and the children of their children
		ARGUEMENTS: ---
		OUTPUT: ---
		*/
		void KillBranches(void);
		/*
		USAGE: Creates a tree using subdivision, the max number of objects and levels
		ARGUEMENTS:
		-uint a_nMaxLevel = 3 -> Sets the maximum level of the tree while constucting it
		OUTPUT: ---
		*/
		void ConstructTree(uint a_nMaxLevel = 3);
		/*
		USAGE: traverse the tree up to the leafs and sets the objects in them to the index
		ARGUEMENTS: ---
		OUTPUT: ---
		*/
		void AssignIDtoEntity(void);
		/*
		USAGE: Gets the total number of MyOctants in the world
		ARGUEMENTS: ---
		OUTPUT: ---
		*/
		void AssignIDtoEntity(MyEntity* entity);
		uint GetMyOctantCount(void);
		bool IsColliding(uint a_uRBIndex);
		bool IsColliding(MyEntity* entity);

	private:
		/*
		USAGE: Deallocates member fields
		ARGUEMENTS: ---
		OUTPUT: ---
		*/
		void Release(void);
		/*
		USAGE: Allocates member fields
		ARGUEMENTS: ---
		OUTPUT: ---
		*/
		void Init(void);
		/*
		USAGE: creates the list of all leafs that contains objects
		ARGUEMENTS: ---
		OUTPUT: ---
		*/
		void ConstructList(void);
	};//class
}//namespace Simplex

#endif //_MyOctantCLASS_H_

/*
USAGE:
ARGUEMENTS:
OUTPUT:
*/