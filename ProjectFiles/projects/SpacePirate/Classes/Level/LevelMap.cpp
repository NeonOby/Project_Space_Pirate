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
LevelMap::LevelMap(b2World * pWorld) 
{
	// Get IndieLib instante
	_world = pWorld;
}

Point LevelMap::getSpawnPoint(){
	return spawnPoint;
}

/* 
======================================									
Create node
====================================== 
*/
void LevelMap::CreateNode (int pX, int pY, int pZ, float pAngle, Color3B pColor, GLubyte pOpacity, int pLayer, float pScale, char *pFileName, bool pFlipX, bool pFlipY)
{

	if(strcmp(pFileName,"resources\\images\\set1\\SpawnPoint.png")==0){
		log("SpawnPosition");
		spawnPoint = Point(pX,pY);
		return;
	}

	//Seems not to load Texture right :(
	Sprite *tmpSprite = Sprite::create(pFileName);
	if(!tmpSprite)
		return;

	tmpSprite->setPosition (Point((float) pX, (float) pY));
	tmpSprite->setScale(pScale);
	tmpSprite->setRotation(pAngle);
	tmpSprite->setColor(pColor);
	tmpSprite->setOpacity(pOpacity);
	

	//Set Flip
	if(pFlipX)
		tmpSprite->setScaleX(-pScale);
	if(pFlipY)
		tmpSprite->setScaleY(-pScale);

	if(pLayer==4){
		//If bestimmtes Bild mach dazu Body etc.
		if(strcmp(pFileName,"resources\\images\\set1\\Astgabelung_v3_256px.png")==0){
			//Normale Platform
			float diffY = (tmpSprite->getContentSize().height*pScale/7.0f);
			if(pFlipY)
				diffY = -diffY;
			createPlatform(pX,pY+diffY,tmpSprite->getContentSize().width*pScale/2,8*pScale, pAngle, pFlipX, pFlipY);
		}else if(strcmp(pFileName,"resources\\images\\set1\\Kiste.png")==0){
			//Normale Platform
			createKiste(pX,pY,tmpSprite->getContentSize().width*pScale/2,tmpSprite->getContentSize().height*pScale/2, tmpSprite, true, pAngle, pFlipX, pFlipY);
		}else if(strcmp(pFileName,"resources\\images\\set1\\Kiste-static.png")==0 || strcmp(pFileName,"resources\\images\\set1\\classic_box_v1_64px.png")==0){
			//Normale Platform
			createKiste(pX,pY,tmpSprite->getContentSize().width*pScale/2,tmpSprite->getContentSize().height*pScale/2, tmpSprite, false, pAngle, pFlipX, pFlipY);
		}else if(strcmp(pFileName,"resources\\images\\set6\\FelsPlatform.png")==0){
			float diffY = (tmpSprite->getContentSize().height*pScale/3.0f);
			if(pFlipY)
				diffY = -diffY;
			createPlatform(pX,pY+diffY,tmpSprite->getContentSize().width*pScale/2,8*pScale, pAngle, pFlipX, pFlipY);
		}else if(strcmp(pFileName,"resources\\images\\set6\\decke.png")==0){
			float diffY = (tmpSprite->getContentSize().height*pScale/2.0f);
			if(pFlipY)
				diffY = -diffY;
			createPlatform(pX,pY+diffY,tmpSprite->getContentSize().width*pScale/2,8*pScale, pAngle, pFlipX, pFlipY);
		}
	}

	if(pLayer > 5){
		pLayer = 5;
	}
	mLayerArray[pLayer]->addChild(tmpSprite, pZ);
}

b2Body * LevelMap::createPlatform(float x, float y, float width, float height, float pAngle, bool pFlipX, bool pFlipY){
	//Floor2 Body only (no Sprite yet)
	b2Body *_body;

	b2BodyDef ballBodyDef;
	//ballBodyDef2.type = b2_staticBody;
	ballBodyDef.position.Set(x/PTM_RATIO, y/PTM_RATIO);
	ballBodyDef.angle = -pAngle*MATH_TO_RADIANS;
	_body = _world->CreateBody(&ballBodyDef);

	b2PolygonShape shapeDef;
	shapeDef.SetAsBox(width/PTM_RATIO, height/PTM_RATIO);
 
	b2FixtureDef ballShapeDef;
	ballShapeDef.shape = &shapeDef;
	ballShapeDef.density = 0.0f;
	ballShapeDef.friction = 0.8f;
	ballShapeDef.restitution = 0.0f;
	_body->CreateFixture(&ballShapeDef)->SetUserData((void*)CLIMBFIXTURE);

	return _body;
}

b2Body * LevelMap::createKiste(float x, float y, float width, float height, Sprite* pSprite, bool dynamic, float pAngle, bool pFlipX, bool pFlipY){
	//Floor2 Body only (no Sprite yet)
	b2Body *_body;

	b2BodyDef ballBodyDef;
	if(dynamic)
		ballBodyDef.type = b2_dynamicBody;
	ballBodyDef.position.Set(x/PTM_RATIO, y/PTM_RATIO);
	ballBodyDef.angle = -pAngle*MATH_TO_RADIANS;
	_body = _world->CreateBody(&ballBodyDef);

	_body->SetUserData(pSprite);


	b2PolygonShape shapeDef;
	shapeDef.SetAsBox(width/PTM_RATIO, height/PTM_RATIO);
 
	b2FixtureDef ballShapeDef;
	ballShapeDef.shape = &shapeDef;
	ballShapeDef.density = 13.0f;
	ballShapeDef.friction = 1.0f;
	ballShapeDef.restitution = 0.15f;
	if(dynamic)
		_body->CreateFixture(&ballShapeDef)->SetUserData((void*)DYNAMIC_KISTE);
	else
		_body->CreateFixture(&ballShapeDef)->SetUserData((void*)KISTE);

	return _body;
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
	if (!strcmp (pMapXMLPath, "")){
		delete [] pMapXMLPath;
		return true;
	}

	// Initializa XML doc
	TiXmlDocument mXmlDoc (pMapXMLPath);

	// Fatal error, cannot load
	if (!mXmlDoc.LoadFile()){
		log("%s: %s","TinyXML", mXmlDoc.ErrorDesc());
		return false;
	}

	// Document root
	TiXmlElement *mXMap = 0;
	mXMap = mXmlDoc.FirstChildElement("map");

	if (!mXMap){
		// No "<map>" tag
		return false;
	}

	// ----------------- Parse tileset -----------------

	// Tileset
	TiXmlElement *mXTileset = 0;
	mXTileset = mXMap->FirstChildElement("tileset");

	if (!mXTileset){
		// No "<tileset>" tag
		return false;
	}

	// Id
	if (mXTileset->Attribute("tileset_file")){
		if (!pResources->LoadTileset ((char *)mXTileset->Attribute("tileset_file"))) return false;	// It tries to load the tiles
	}


	// ----------------- Parse nodes -----------------

	// Nodes
	TiXmlElement *mXNodes = 0;
	mXNodes = mXMap->FirstChildElement("nodes");

	if (!mXNodes){
		// No "<nodes>" tag
		return false;
	}

	TiXmlElement *mXNode = 0;
	mXNode = mXNodes->FirstChildElement("node");

	if (!mXNode){
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
			mMirrorY = (bool) atoi ((char *)mXNode->Attribute("mirror_y"));
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

		CreateNode(mX,-mY,mZ,mAngle, Color3B(mTintR, mTintG, mTintB), mTrans,mLayer,mScale,pResources->GetImagePath(mSurfaceId), mMirrorX, mMirrorY);
	
		// Move to the next declaration
		mXNode = mXNode->NextSiblingElement("node");
	}

	return true;
}

