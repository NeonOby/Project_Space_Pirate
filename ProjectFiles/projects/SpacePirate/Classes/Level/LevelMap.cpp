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

// ------ Includes -----

#include "LevelMap.h"

USING_NS_CC;

/* 
======================================									
Init
====================================== 
*/
LevelMap::LevelMap() 
{
	// Get IndieLib instante
	
}

/* 
======================================									
Create node
====================================== 
*/
void LevelMap::CreateNode (int pX, int pY, int pZ, int pLayer, float pScale, char *pFileName)
{
	//Seems not to load Texture right :(
	Sprite *tmpSprite = Sprite::create(pFileName);
	if(!tmpSprite)
		return;
	log("PosY: %i", -pY);
	tmpSprite->setPosition (Point((float) pX, (float) -pY));
	tmpSprite->setScale(pScale);

	if(pLayer > 5){
		pLayer = 5;
	}
	mLayerArray[pLayer]->addChild(tmpSprite, pZ);
}

void LevelMap::SetLayerArray (cocos2d::Layer **pLayerArray){
	mLayerArray[3] = pLayerArray[0];
	mLayerArray[4] = pLayerArray[1];
	mLayerArray[5] = pLayerArray[2];
}

void LevelMap::SetLayerArray(cocos2d::Layer *pLayer1, cocos2d::Layer *pLayer2, cocos2d::Layer *pLayer3){
	mLayerArray[3] = pLayer1;
	mLayerArray[4] = pLayer2;
	mLayerArray[5] = pLayer3;
}

void LevelMap::SetParallaxLayer(cocos2d::Layer *pLayer1, cocos2d::Layer *pLayer2, cocos2d::Layer *pLayer3){
	mLayerArray[0] = pLayer1;
	mLayerArray[1] = pLayer2;
	mLayerArray[2] = pLayer3;
}

void LevelMap::SetParallaxArray(cocos2d::Layer **pLayerArray){
	mLayerArray[0] = pLayerArray[0];
	mLayerArray[1] = pLayerArray[1];
	mLayerArray[2] = pLayerArray[2];
}

/* 
======================================									
Load a Map from a XML file and create a node vector
====================================== 
*/
bool LevelMap::LoadMap (char* pMapXMLPath)
{
	Resources *pResources = new Resources();

	//String compare
	if (!strcmp (pMapXMLPath, "")) 
	{
		delete [] pMapXMLPath;
		return true;
	}

	// Initializa XML doc
	TiXmlDocument mXmlDoc (pMapXMLPath);

	// Fatal error, cannot load
	if (!mXmlDoc.LoadFile())
	{
		log("%s: %s","TinyXML",mXmlDoc.ErrorDesc());
		return false;
	}

	// Document root
	TiXmlElement *mXMap = 0;
	mXMap = mXmlDoc.FirstChildElement("map");

	if (!mXMap)
	{
		// No "<map>" tag
		return false;
	}

	// ----------------- Parse tileset -----------------

	// Tileset
	TiXmlElement *mXTileset = 0;
	mXTileset = mXMap->FirstChildElement("tileset");

	if (!mXTileset)
	{
		// No "<tileset>" tag
		return false;
	}

	// Id
	if (mXTileset->Attribute("tileset_file"))
	{
		if (!pResources->LoadTileset ((char *)mXTileset->Attribute("tileset_file"))) return false;	// It tries to load the tiles
	}


	// ----------------- Parse nodes -----------------

	// Nodes
	TiXmlElement *mXNodes = 0;
	mXNodes = mXMap->FirstChildElement("nodes");

	if (!mXNodes)
	{
		// No "<nodes>" tag
		return false;
	}

	TiXmlElement *mXNode = 0;
	mXNode = mXNodes->FirstChildElement("node");

	if (!mXNode)
	{
		// No nodes to parse
		return false;
	}
	
	// Parse all the nodes


	while (mXNode)
	{
		// Parameters to parse
		int mSurfaceId, mX, mY, mZ, mLayer, mTrans, mTintR, mTintG, mTintB, mRegionWidth, mRegionHeight;
		bool mMirrorX, mMirrorY, mIfWrap;
		float mAngle, mScale;
	

		// Surface Id
		if (mXNode->Attribute("surface_id"))
		{
			mSurfaceId = atoi ((char *)mXNode->Attribute("surface_id"));
		}
		else
		{
			return false;
		}

		// Pos X
		if (mXNode->Attribute("x"))
		{
			mX = atoi ((char *)mXNode->Attribute("x"));
		}
		else
		{
			return false;
		}

		// Pos Y
		if (mXNode->Attribute("y"))
		{
			mY = atoi ((char *)mXNode->Attribute("y"));
		}
		else
		{
			return false;
		}

		// Pos Z
		if (mXNode->Attribute("z"))
		{
			mZ = atoi ((char *)mXNode->Attribute("z"));
		}
		else
		{
			return false;
		}

		// Layer
		if (mXNode->Attribute("layer"))
		{
			mLayer = atoi ((char *)mXNode->Attribute("layer"));
		}
		else
		{
			return false;
		}

		// Angle
		if (mXNode->Attribute("angle"))
		{
			mAngle = (float) atof ((char *)mXNode->Attribute("angle"));
		}
		else
		{
			return false;
		}

		// Scale
		if (mXNode->Attribute("scale"))
		{
			mScale = (float) atof ((char *)mXNode->Attribute("scale"));
		}
		else
		{
			return false;
		}

		// Transparency
		if (mXNode->Attribute("trans"))
		{
			mTrans = atoi ((char *)mXNode->Attribute("trans"));
		}
		else
		{
			return false;
		}

		// Mirror x
		if (mXNode->Attribute("mirror_x"))
		{
			mMirrorX = (bool) atoi ((char *)mXNode->Attribute("mirror_x"));
		}
		else
		{
			return false;
		}

		// Mirror y
		if (mXNode->Attribute("mirror_y"))
		{
			mMirrorY = atoi ((char *)mXNode->Attribute("mirror_y"));
		}
		else
		{
			return false;
		}

		// Tint r
		if (mXNode->Attribute("tint_r"))
		{
			mTintR = atoi ((char *)mXNode->Attribute("tint_r"));
		}
		else
		{
			return false;
		}

		// Tint g
		if (mXNode->Attribute("tint_g"))
		{
			mTintG = atoi ((char *)mXNode->Attribute("tint_g"));
		}
		else
		{
			return false;
		}

		// Tint b
		if (mXNode->Attribute("tint_b"))
		{
			mTintB = atoi ((char *)mXNode->Attribute("tint_b"));
		}
		else
		{
			return false;
		}

		// If wrap
		if (mXNode->Attribute("if_wrap"))
		{
			mIfWrap = atoi ((char *)mXNode->Attribute("if_wrap"));
		}
		else
		{
			return false;
		}

		// Region width
		if (mXNode->Attribute("region_width"))
		{
			mRegionWidth = atoi ((char *)mXNode->Attribute("region_width"));
		}
		else
		{
			return false;
		}

		// Region height
		if (mXNode->Attribute("region_height"))
		{
			mRegionHeight = atoi ((char *)mXNode->Attribute("region_height"));
		}
		else
		{
			return false;
		}

		//Load Sprite etc.

		CreateNode(mX,mY,mZ,mLayer,mScale,pResources->GetImagePath(mSurfaceId));
	
		// Move to the next declaration
		mXNode = mXNode->NextSiblingElement("node");
	}

	return true;
}

