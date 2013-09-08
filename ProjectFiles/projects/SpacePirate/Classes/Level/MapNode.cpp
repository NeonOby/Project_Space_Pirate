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

// ------ Includes -----

#include "MapNode.h"

USING_NS_CC;

/* 
======================================									
Init
====================================== 
*/
MapNode::MapNode (int pX, int pY, int pZ, int pLayer, Texture2D *pTexture)
{
	mX = pX;
	mY = pY;
	mZ = pZ;
	mLayer = pLayer;
	mTexture = pTexture;
}


/* 
======================================									
End
====================================== 
*/
MapNode::~MapNode() 
{

}
