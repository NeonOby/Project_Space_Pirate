/*****************************************************************************************
/* Desc: Resources class, for loading the initial tileset of brushes and the editor graphics.
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

#ifndef _RESOURCES_
#define _RESOURCES_

// ------ Includes -----

#include "TinyXML/tinyxml.h"
#include "cocos2d.h"
#include <vector>
using namespace std; 


// --------------------------------------------------------------------------------
//									  Resources
// --------------------------------------------------------------------------------

class Resources
{
public:

	Resources											();
	void Free											();

	bool					LoadResources				(char *pResourceFile);
	bool					LoadTileset					(char *pTilesetFile);

	char					*GetImagePath				(int index);
private:

	vector <string> mTextureVector;

	char *mTilesetPath;
};

#endif // _RESOURCES_
