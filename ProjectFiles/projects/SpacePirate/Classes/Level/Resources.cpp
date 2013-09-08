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

// ------ Includes -----

#include "Resources.h"

USING_NS_CC;

/* 
======================================									
Init
====================================== 
*/
Resources::Resources() 
{

}


/* 
======================================									
End
====================================== 
*/
void Resources::Free() 
{
	// Free all the loaded tiles
	FreeTileset();

	// Note, the font bitmap and cursor are freed in Main.cpp calling to mI->End()
}


/* 
======================================									
Load Resources
====================================== 
*/
bool Resources::LoadResources (char *pTilesetFile)
{
	if (!LoadEditorElements())							return false;
	if (!LoadTileset (pTilesetFile))					return false;

	return true;
}




/* 
======================================									
Parse file and load backdrop images (tiles)
====================================== 
*/
bool Resources::LoadTileset (char *pTilesetFile)
{
	// If there is a tileset already, free it
	

	strcpy (mTilesetPath, pTilesetFile);

	TiXmlDocument mXmlDoc (mTilesetPath);

	// Fatal error, cannot load
	if (!mXmlDoc.LoadFile())
	{
		return false;
	}

	// Document root
	TiXmlElement *mXResources = 0;
	mXResources = mXmlDoc.FirstChildElement("resources");

	if (!mXResources)
	{
		// No "<resources>" tag
		return false;
	}

	// ----------------- Parse surfaces from the file and load them into a vector -----------------

	// Surfaces
	TiXmlElement *mXSurfaces = 0;
	mXSurfaces = mXResources->FirstChildElement("surfaces");

	if (!mXSurfaces)
	{
		// No "<surfaces>" tag
		return false;
	}

	TiXmlElement *mXSurface = 0;
	mXSurface = mXSurfaces->FirstChildElement("surface");

	if (!mXSurface)
	{
		// No surfaces to parse
		return false;
	}
	
	// Parse all the surfaces
	char mFileName [1024];
	mFileName [0] = 0;

	while (mXSurface)
	{
		

		// Id, Ignorieren wir die mal erst
		/*
		if (mXSurface->Attribute("id"))
		{
			mNewSurface->mId = atoi (mXSurface->Attribute("id"));
		}
		else
		{
			delete mNewSurface;
			return false;
		}
		*/

		// Path to the image
		if (mXSurface->Attribute("image"))
		{
			strcpy (mFileName, mXSurface->Attribute("image"));	

			// Load Image into Texture2D oder SpriteCache wie auch immer
			mTextureVector.push_back (mFileName);
			
		}
		else
		{
			//delete mTexture;
			return false;
		}

		// Move to the next declaration (XML next Node sozusagen)
		mXSurface = mXSurface->NextSiblingElement("surface");
	}


	// Note: XML nodes are deleted by TiXmlDocument destructor

	return true;
}
