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

#ifndef _LEVELMAP_
#define _LEVELMAP_

// ------ Includes -----

#include "TinyXML/tinyxml.h"
#include "Resources.h"
#include "cocos2d.h"

// --------------------------------------------------------------------------------
//								     	_LEVELMAP_
// --------------------------------------------------------------------------------

class LevelMap
{
public:

	LevelMap										();

	void						CreateNode			(int pX, int pY, int pZ, int pLayer, float pScale, char *pFileName);
	bool						LoadMap				(char* pMapXMLPath);

	cocos2d::Layer				**GetLayerArray		(){ return mLayerArray; };
	void						SetLayerArray		(cocos2d::Layer **pLayerArray);
	void						SetParallaxArray	(cocos2d::Layer **pLayerArray);
	void						SetParallaxLayer	(cocos2d::Layer *pLayer1, cocos2d::Layer *pLayer2, cocos2d::Layer *pLayer3);
	void						SetLayerArray		(cocos2d::Layer *pLayer1, cocos2d::Layer *pLayer2, cocos2d::Layer *pLayer3);
private:


	cocos2d::Layer* mLayerArray[6];
};

#endif // _LEVELMAP_