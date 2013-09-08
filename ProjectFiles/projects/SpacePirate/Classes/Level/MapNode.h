/*****************************************************************************************
/* Desc: Node class. Each node is a backdrop of the map that contains and IndieLib entity
/* that holds all its attributes
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

#ifndef _MapNode_
#define _MapNode_

// ------ Includes -----
#include "cocos2d.h"

// --------------------------------------------------------------------------------
//										Node
// --------------------------------------------------------------------------------

class MapNode
{
public:

	MapNode					(int pX, int pY, int pZ, int pLayer, cocos2d::Texture2D *pTexture);
	~MapNode				();

	cocos2d::Point			GetPosition		()				{ return cocos2d::Point(mX,mY); };
	int						GetZOrder		()				{ return mZ; };
	int						GetLayer		()				{ return mLayer; };
	cocos2d::Texture2D		*GetTexture		()				{ return mTexture; };

private:

	int mX, mY, mZ;
	int mLayer;						// Layer where the node is created
	cocos2d::Texture2D *mTexture;


};

#endif // _NODE_