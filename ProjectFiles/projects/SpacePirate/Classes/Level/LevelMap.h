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
#include "Konstanten.h"
#include "Box2D\Box2D.h"

// --------------------------------------------------------------------------------
//								     	_LEVELMAP_
// --------------------------------------------------------------------------------

class LevelMap
{
public:

	LevelMap										(b2World * pworld);

	void						CreateNode			(int pX, int pY, int pZ, float pAngle, cocos2d::Color3B pColor, GLubyte pOpacity, int pLayer, float pScale, char *pFileName, bool pFlipX, bool pFlipY);
	bool						LoadMap				(char* pMapXMLPath);

	cocos2d::Layer				**GetLayerArray		(){ return mLayerArray; };
	void						SetLayerArray		(cocos2d::Layer **pLayerArray);
	void						SetParallaxArray	(cocos2d::Layer **pLayerArray);
	void						SetParallaxLayer	(cocos2d::Layer *pLayer1, cocos2d::Layer *pLayer2, cocos2d::Layer *pLayer3);
	void						SetLayerArray		(cocos2d::Layer *pLayer1, cocos2d::Layer *pLayer2, cocos2d::Layer *pLayer3);

	cocos2d::Point				getSpawnPoint		();
private:

	bool StringContains(const string &pString, const string &pFind);

	b2Body * createKiste(float x, float y, float width, float height, cocos2d::Sprite* pSprite, bool dynamic, float pAngle, bool pFlipX, bool pFlipY);
	b2Body * createPlatform(float x, float y, float width, float height, float pAngle, bool pFlipX, bool pFlipY);

	b2World * _world;

	cocos2d::Point spawnPoint;

	cocos2d::Layer* mLayerArray[6];
};

#endif // _LEVELMAP_