/*****************************************************************************************
/* Desc: Map class, for loading and savin maps and creating, cloning and deleting nodes.
/*
/* gametuto.com - Javier López López (javilop.com)
/*
/*****************************************************************************************
/*
/* Creative Commons - Attribution 3.0 Unported
/* You are free:
/*	to Share — to copy, distribute and transmit the work
/*	to Remix — to adapt the work
/*
/* Under the following conditions:
/* Attribution. You must attribute the work in the manner specified by the author or licensor 
/* (but not in any way that suggests that they endorse you or your use of the work).
/*
/*****************************************************************************************/

#ifndef _MAP_
#define _MAP_

// ------ Includes -----

#include <vector>
#include "tinyxml.h"
#include "Resources.h"
#include "cocos2d.h"
#include "MapNode.h"
using namespace std; 


// --------------------------------------------------------------------------------
//								     	Map
// --------------------------------------------------------------------------------

class Map
{
public:

	Map												();
	void						Free				();

	void						CreateNode			(int pX, int pY, int pZ, int pLayer, cocos2d::Texture2D *pSurface);
	void						DeleteNode			(cocos2d::Node *pNode);
	void						CloneNode			(cocos2d::Node *pNode);
	bool						SaveMap				(char *pTilesetPath);
	char						*GetPathToTileset	();
	bool						LoadMap				(Resources *pResources);
	vector	<cocos2d::Node*>	*GetVectorNodes		()	{ return &mVectorNodes; };
	void						FreeMap				();

private:

	vector <cocos2d::Node*>		mVectorNodes;		// Map nodes

	// ----- Private methods -----

	char						*OpenFileDialog		(char *pFilter, bool pAction, char *pTitle);
};

#endif // _MAP_