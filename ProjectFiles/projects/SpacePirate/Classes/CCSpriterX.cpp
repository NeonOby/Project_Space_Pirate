//------------------------------------------------------------------------
//
//	CCSpriterX : KickStarter project Spriter renderer for cocos2d-x.
//
//	Spriter website : http://www.kickstarter.com/projects/539087245/spriter
//
//	Licensed under the BSD license, see LICENSE in root for details.
//
//	Copyright (c) 2012 James Hui (a.k.a. Dr.Watson)
//
//	For latest updates, please visit http://jameshui.com
//
//------------------------------------------------------------------------

#include "CCSpriterX.h"




USING_NS_CC;



namespace SCMLHelper
{
#include <math.h>
#define PI 3.141592653589793
    double rad(double x)
    {
        return x*PI/180;
    }




    void calculate_rotation(float a, float b, float x, float y, float degree, float* px, float *py)
    {
        (*px) = (x-a)*cos(rad(degree)) - (y-b)*sin(rad(degree));
        (*py) = (x-a)*sin(rad(degree)) + (y-b)*cos(rad(degree));
    }


    double ArcCalc(double vec1X,double vec1Y , double vec2X ,double vec2Y, bool isClockWise=true)
    {
		double angle = 0.0f;

#if 0
		double inner = 0;
		double i1=	sqrt(vec1X*vec1X+ vec1Y*vec1Y);
		double i2=	sqrt(vec2X*vec2X+ vec2Y*vec2Y);
	    vec1X=(vec1X/i1);
	    vec1Y=(vec1Y/i1);
	    vec2X=(vec2X/i2);
	    vec2Y=(vec2Y/i2);
	    inner =(vec1X*vec2X + vec1Y*vec2Y); //다시 내적을 구한다.

	    angle = acos(inner)*180/PI;
#else
        double va = sqrt(vec1X*vec1X + vec1Y*vec1Y);
        double vb = sqrt(vec2X*vec2X + vec2Y*vec2Y);
        double dot = (vec1X*vec2X + vec1Y*vec2Y);
        angle = acos(  dot/(va*vb) );
        angle *= 180/PI;

#endif

		return angle;
	}








	///////////////////////////////////////////////////////////////////////////////////

	File::File()
    : sprite(NULL)
	{
	}


	File::~File()
	{
		if (sprite)
			sprite->release();

	}

	void File::Init(TiXmlNode *node)
	{
		TiXmlElement *element = node->ToElement();
		if (element)
		{
			int intValue;
			float floatValue;

			if (element->QueryIntAttribute("id", &intValue) == TIXML_SUCCESS)
				id = intValue;
			else
				id = 0;

			name = element->Attribute("name");

			if (element->QueryFloatAttribute("width", &floatValue) == TIXML_SUCCESS)
				width = floatValue;
			else
				width = 0;

			if (element->QueryFloatAttribute("height", &floatValue) == TIXML_SUCCESS)
				height = floatValue;
			else
				height = 0;

			if (name.size()>0)
			{

                //sprite = CCSprite::createWithSpriteFrameName(name.c_str());
			sprite = Sprite::create(name.c_str());
				//sprite = CCSprite::create(name.c_str());
				sprite->retain();
			}

		}

	}

	///////////////////////////////////////////////////////////////////////////////////

	Folder::Folder()
    : mId(0)
	{
		mFiles.reserve(50);
	}


	Folder::~Folder()
	{
		int count = mFiles.size();
		for (int i=0;i<count;i++)
			CC_SAFE_DELETE(mFiles[i]);

		mFiles.clear();

	}


	int Folder::GetFileCount()
	{
		return mFiles.size();

	}


	File *Folder::GetFile(int index)
	{
		if (index < (int)mFiles.size())
			return mFiles[index];

		return NULL;

	}


	void Folder::Init(TiXmlNode *node, const char* szPlist)
	{
		TiXmlElement *element = node->ToElement();
		if (element)
		{
			int intValue;

			if (element->QueryIntAttribute("id", &intValue) == TIXML_SUCCESS)
				mId= intValue;


            const char* szname = element->Attribute("name");
			mName = szname==NULL?"":szname;

			for (TiXmlNode* fileNode = node->FirstChild(); fileNode; fileNode = fileNode->NextSibling())
			{
				File *file = new File();




				file->Init(fileNode);

				mFiles.push_back(file);
			}

		}

	}










    ///////////////////////////////////////////////////////////////////////////////////
    // 2013-03-12 brucewang
	void BoneRef::Init(TiXmlNode *node)
	{
		TiXmlElement *element = node->ToElement();
		if (element)
		{
			int intValue;

			if (element->QueryIntAttribute("id", &intValue) == TIXML_SUCCESS)
				id = intValue;
			else
				id = 0;

			if (element->QueryIntAttribute("timeline", &intValue) == TIXML_SUCCESS)
				timeline = intValue;
			else
				timeline = 0;

			if (element->QueryIntAttribute("key", &intValue) == TIXML_SUCCESS)
				key = intValue;
			else
				key = 0;

			if (element->QueryIntAttribute("parent", &intValue) == TIXML_SUCCESS)
				parent = intValue;
			else
				parent = -1;
		}
	}


    ///////////////////////////////////////////////////////////////////////////////////
    // 2013-03-12 brucewang
	void Bone::Init(TiXmlNode *node)
	{
		TiXmlElement *element = node->ToElement();
		if (element)
		{
            float scaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();
            float floatValue;

			if (element->QueryFloatAttribute("x", &floatValue) == TIXML_SUCCESS)
				x = floatValue/scaleFactor;
			else
				x = 0.f;

            if (element->QueryFloatAttribute("y", &floatValue) == TIXML_SUCCESS)
				y = floatValue/scaleFactor;
			else
				y = 0.f;

            if (element->QueryFloatAttribute("angle", &floatValue) == TIXML_SUCCESS)
				angle = floatValue;
			else
				angle = 0.f;

            if (element->QueryFloatAttribute("scale_x", &floatValue) == TIXML_SUCCESS)
				scale_x = floatValue;
			else
				scale_x = 1.f;

            if (element->QueryFloatAttribute("scale_y", &floatValue) == TIXML_SUCCESS)
				scale_y = floatValue;
			else
				scale_y = 1.f;
		}
	}















	///////////////////////////////////////////////////////////////////////////////////

	void ObjectRef::Init(TiXmlNode *node)
	{
		TiXmlElement *element = node->ToElement();
		if (element)
		{
			int intValue;
            //float floatValue;

			if (element->QueryIntAttribute("id", &intValue) == TIXML_SUCCESS)
				id = intValue;
			else
				id = 0;

			if (element->QueryIntAttribute("timeline", &intValue) == TIXML_SUCCESS)
				timeline = intValue;
			else
				timeline = 0;

			if (element->QueryIntAttribute("key", &intValue) == TIXML_SUCCESS)
				key = intValue;
			else
				key = 0;

			if (element->QueryIntAttribute("z_index", &intValue) == TIXML_SUCCESS)
				z_index = intValue;
			else
				z_index = 0;


            // 2013-03-12 brucewang
            if (element->QueryIntAttribute("parent", &intValue) == TIXML_SUCCESS)
				parent = intValue;
			else
				parent = -1;



		}
	}

	///////////////////////////////////////////////////////////////////////////////////

	void Object::Init(TiXmlNode *node, CCSpriterX *animator)
	{
		sprite = NULL;

        float scaleFactor = CCDirector::sharedDirector()->getContentScaleFactor();

		TiXmlElement *element = node->ToElement();
		if (element)
		{
			int intValue;
			float floatValue;

			if (element->QueryIntAttribute("folder", &intValue) == TIXML_SUCCESS)
				folder = intValue;
			else
				folder = 0;

			if (element->QueryIntAttribute("file", &intValue) == TIXML_SUCCESS)
				file = intValue;
			else
				file = 0;

			if (element->QueryFloatAttribute("x", &floatValue) == TIXML_SUCCESS)
				x = floatValue/scaleFactor;
			else
				x = 0;
			if (element->QueryFloatAttribute("y", &floatValue) == TIXML_SUCCESS)
				y = floatValue/scaleFactor;
			else
				y = 0;

			if (element->QueryFloatAttribute("angle", &floatValue) == TIXML_SUCCESS)
				angle = floatValue;
			else
				angle = 0;

			if (element->QueryFloatAttribute("pivot_x", &floatValue) == TIXML_SUCCESS)
				pivot_x = floatValue;
			else
				pivot_x = 0;

			if (element->QueryFloatAttribute("pivot_y", &floatValue) == TIXML_SUCCESS)
				pivot_y = floatValue;
			else
				pivot_y = 1;

			if (element->QueryIntAttribute("z_index", &intValue) == TIXML_SUCCESS)
				z_index = intValue;
			else
				z_index = 0;


            // 2013-03-13 brucewang
            if (element->QueryFloatAttribute("scale_x", &floatValue) == TIXML_SUCCESS)
				scale_x = floatValue;
			else
				scale_x = 1.f;

            if (element->QueryFloatAttribute("scale_y", &floatValue) == TIXML_SUCCESS)
				scale_y = floatValue;
			else
				scale_y = 1.f;


			sprite = animator->getSprite(folder, file);

            this->fileinfo = animator->getFile(folder, file);
		}
	}


	///////////////////////////////////////////////////////////////////////////////////

	Key::Key()
    : mId(0)
    , mTime(0)
    , mSpinCounterClockwise(true)
	{
		mObjects.reserve(50);
		mObjectRefs.reserve(50);
        mBones.reserve(50);
		mBoneRefs.reserve(50);
	}


	Key::~Key()
	{
		int count = mObjects.size();
		for (int i=0;i<count;i++)
			CC_SAFE_DELETE(mObjects[i]);

		mObjects.clear();

		count = mObjectRefs.size();
		for (int i=0;i<count;i++)
			CC_SAFE_DELETE(mObjectRefs[i]);

		mObjectRefs.clear();


        // 2013-03-12 brucewang
        count = mBones.size();
		for (int i=0;i<count;i++)
			CC_SAFE_DELETE(mBones[i]);
		mBones.clear();
		count = mBoneRefs.size();
		for (int i=0;i<count;i++)
			CC_SAFE_DELETE(mBoneRefs[i]);
		mBoneRefs.clear();
	}


	int Key::GetObjectRefCount()
	{
		return mObjectRefs.size();

	}


	ObjectRef *Key::GetObjectRef(int index)
	{
		if (index < (int)mObjectRefs.size())
			return mObjectRefs[index];

		return NULL;

	}


	int Key::GetObjectCount()
	{
		return mObjects.size();

	}


	Object *Key::GetObject(int index)
	{
		if (index < (int)mObjects.size())
			return mObjects[index];

		return NULL;

	}



    // 2013-03-12 brucewang
    int Key::GetBoneCount()
	{
		return mBones.size();
	}
    Bone *Key::GetBone(int index)
	{
		if (index < (int)mBones.size())
			return mBones[index];

		return NULL;

	}
    int Key::GetBoneRefCount()
    {
        return mBoneRefs.size();
    }
    BoneRef* Key::GetBoneRef(int index)
    {
        if (index < (int)mBoneRefs.size())
			return mBoneRefs[index];
		return NULL;
    }


	float Key::GetTime()
	{
		return mTime;
	}


	bool Key::IsSpinCounterClockwise()
	{
		return mSpinCounterClockwise;

	}


	void Key::Init(TiXmlNode *node, CCSpriterX *animator)
	{
		TiXmlElement *element = node->ToElement();
		if (element)
		{
			int intValue;
			float floatValue;
			if (element->QueryIntAttribute("id", &intValue) == TIXML_SUCCESS)
				mId = intValue;

			float time = 0;
			if (element->QueryFloatAttribute("time", &floatValue) == TIXML_SUCCESS)		// was in milliseconds, convert to seconds instead
				time = floatValue/1000.0f;
			mTime = time;

			if (element->QueryIntAttribute("spin", &intValue) == TIXML_SUCCESS)
				mSpinCounterClockwise = !(intValue == -1);

			for (TiXmlNode* objNode = node->FirstChild(); objNode; objNode = objNode->NextSibling())
			{
				element = objNode->ToElement();
				const char *tabObj = element->Value();

				if (strcmp(tabObj, "object_ref")==0)
				{
					ObjectRef *ref = new ObjectRef();
					ref->Init(objNode);

					mObjectRefs.push_back(ref);

				}
				else if (strcmp(tabObj, "object")==0)
				{
					Object *obj = new Object();
					obj->Init(objNode, animator);
					mObjects.push_back(obj);
				}
                // 2013-03-12 brucewang
                else if (strcmp(tabObj, "bone_ref")==0)
				{
					BoneRef *ref = new BoneRef();
					ref->Init(objNode);
					mBoneRefs.push_back(ref);
				}
                // 2013-03-12 brucewang
				else if (strcmp(tabObj, "bone")==0)
				{
					Bone *obj = new Bone();
					obj->Init(objNode);
					mBones.push_back(obj);
				}
			}
		}

	}


	///////////////////////////////////////////////////////////////////////////////////

	Timeline::Timeline()
    : mId(0)
	{
		mKeyframes.reserve(50);
	}


	Timeline::~Timeline()
	{
		int count = mKeyframes.size();
		for (int i=0;i<count;i++)
		{
			CC_SAFE_DELETE(mKeyframes[i]);

		}

	}


	int Timeline::GetKeyframeCount()
	{
		return mKeyframes.size();

	}


	Key *Timeline::GetKeyframe(int index)
	{
		if (index < (int)mKeyframes.size())
		{
			return mKeyframes[index];
		}

		return NULL;

	}

	void Timeline::Init(TiXmlNode *node, CCSpriterX *animator)
	{
		int intValue;

		TiXmlElement *element = node->ToElement();

		if (element)
		{
			if (element->QueryIntAttribute("id", &intValue) == TIXML_SUCCESS)
				mId = intValue;

			for (TiXmlNode* keyNode = node->FirstChild(); keyNode; keyNode = keyNode->NextSibling())
			{
				element = keyNode->ToElement();
				if (element)
				{
					Key *keyframe = new Key();

					keyframe->Init(keyNode, animator);

					mKeyframes.push_back(keyframe);
				}
			}

		}

	}

	///////////////////////////////////////////////////////////////////////////////////

	Animation::Animation()
    : mId(0)
    , mCurrKeyframe(0)
    , mMainline(NULL)
    , mDone(false)
    , mTimer(0)
	{
		mTimelines.reserve(50);

	}


	Animation::~Animation()
	{
		int count = mTimelines.size();
		for (int i=0;i<count;i++)
			CC_SAFE_DELETE(mTimelines[i]);
		mTimelines.clear();
	}


	void Animation::Init(TiXmlNode *node, CCSpriterX *animator)
	{
		int intValue;
		float floatValue;

		TiXmlElement *element = node->ToElement();

		if (element)
		{
			if (element->QueryIntAttribute("id", &intValue) == TIXML_SUCCESS)
				mId = intValue;

			mName = element->Attribute("name");

			if (element->QueryFloatAttribute("length", &floatValue) == TIXML_SUCCESS)
				mLength = floatValue/1000.0f;							// was in milliseconds, convert to seconds instead

			//const char *looping = element->Attribute("looping");		// was set to "false" in alpha, but in fact looping all the time
			mLooping = true;

			for (TiXmlNode* lineNode = node->FirstChild(); lineNode; lineNode = lineNode->NextSibling())
			{
				element = lineNode->ToElement();

				const char *tabLine = element->Value();
				if (strcmp(tabLine, "mainline")==0)						// 1 mainline only
				{
					mMainline = new Timeline();
					mMainline->Init(lineNode, animator);
				}
				else if (strcmp(tabLine, "timeline")==0)
				{
					Timeline *timeline = new Timeline();
					timeline->Init(lineNode, animator);

					mTimelines.push_back(timeline);
				}
			}// end 'for'
        }
	}


	bool Animation::IsDone()
	{
		return mDone;

	}


	void Animation::Restart()
	{
		mDone = false;
		mTimer = 0;
		mCurrKeyframe = 0;

	}













    void Animation::calc_bone_position(Key *keyframe,
                                       Key *keyframeNext,
                                       float time,
                                       int parent,
                                       int parentNext,
                                       float* parentx,
                                       float* parenty,
                                       float* lscale_x,
                                       float* lscale_y,
                                       float* langle,
                                       float* angle_total,
                                       float* angle_totalNext)    {
        float scalex_parent = 1.f;
        float scaley_parent = 1.f;
        float angle_parent = 0;
        float x_parent = 0;
        float y_parent = 0;

        float scalex = 1;
        float scaley = 1;
        float angle = 0;
        float angleNext = 0;
        float x = 0;
        float y = 0;


        if( parent>=0 && parent<keyframe->GetBoneRefCount() )
        {
            BoneRef* boneref = keyframe->GetBoneRef( parent );
            BoneRef* bonerefNext = keyframeNext->GetBoneRef( parent );


            // get the parent information.
            calc_bone_position(keyframe,
                               keyframeNext,
                               time,
                               boneref->parent,
                               bonerefNext->parent,
                               &x_parent,
                               &y_parent,
                               &scalex_parent, &scaley_parent,
                               &angle_parent,
                               angle_total,
                               angle_totalNext);

            Key *keyBone = mTimelines[boneref->timeline]->GetKeyframe(boneref->key);
            Bone* bone = keyBone->GetBone(0);
            Key *keyBoneNext = mTimelines[bonerefNext->timeline]->GetKeyframe(bonerefNext->key);
            Bone* boneNext = keyBoneNext->GetBone(0);

            scalex = bone->scale_x;
            scaley = bone->scale_y;
            angle = bone->angle;
            angleNext = boneNext->angle;

            x = bone->x;
            y = bone->y;

            //CCLog( "### BONE x/y=(%3.1f/%3.1f)", x, y);



            float anglediff = (*angle_totalNext) - (*angle_total);
            float angle2 = (*angle_total) + anglediff*time;
            if (angle2 >= 360)
                angle2 -= 360;

            calculate_rotation(0, 0, (x)*scalex_parent, (y)*scaley_parent,
                               angle2,//*angle_total,
                               //*angle_total,
                               &x, &y);

            x += x_parent;
            y += y_parent;
            scalex *= scalex_parent;
            scaley *= scaley_parent;
        }

        *parentx = x;
        *parenty = y;
        *lscale_x *= scalex;
        *lscale_y *= scaley;
        *langle = angle;
        *angle_total += angle;
        *angle_totalNext += angleNext;
    }


#define LOOP 1

	void Animation::Update(float dt)
	{
		mTimer += dt;
		if (mTimer >= mLength)
		{
			mDone = true;
#if LOOP
			Restart();			// always looping for now
#endif
		}

		int count = mMainline->GetKeyframeCount();
		Key *keyframe = mMainline->GetKeyframe(mCurrKeyframe);
		float currTime = keyframe->GetTime();

		Key *keyframeNext = NULL;

		int next = mCurrKeyframe+1;

#if LOOP
		if (next > count-1)		// looping
			next = 0;
#endif

		keyframeNext = mMainline->GetKeyframe(next);

		if (keyframeNext)
		{
			float nextTime = keyframeNext->GetTime();
			if (next == 0)
				nextTime = mLength;

			if (mTimer >= nextTime)
			{

				mCurrKeyframe = next;

				keyframe = keyframeNext;
				currTime = keyframe->GetTime();

				next = mCurrKeyframe+1;
				if (next > count-1)				// looping
					next = 0;

				keyframeNext = mMainline->GetKeyframe(next);
				if (keyframeNext == NULL)
					return;

				nextTime = keyframeNext->GetTime();
				if (next == 0)
					nextTime = mLength;

			}


			float t = (mTimer-currTime)/(nextTime-currTime);


            // get the parent informatio


			count = keyframe->GetObjectRefCount();
			for (int i=0;i<count;i++)
			{
				ObjectRef *ref = keyframe->GetObjectRef(i);
				ObjectRef *refNext = keyframeNext->GetObjectRef(i);

				if (ref && refNext)
				{
					Key *keyRef = mTimelines[ref->timeline]->GetKeyframe(ref->key);
					Object *obj = keyRef->GetObject(0);	// should be only 1 object

					Key *keyRefNext = mTimelines[refNext->timeline]->GetKeyframe(refNext->key);
					Object *objNext = keyRefNext->GetObject(0);


                    if (ref->timeline != refNext->timeline)
						t = 0;


                    float x_parent = 0;
                    float y_parent = 0;
                    float scalex_parent = 1.f;
                    float scaley_parent = 1.f;
                    float angle_parent = 0;
                    float angle_total = 0;
                    float angle_totalNext = 0;

                    //--------------------------------------------------
                    // get the parent information.
                    BoneRef* boneref = keyframe->GetBoneRef( ref->parent );
                    BoneRef* bonerefNext = keyframeNext->GetBoneRef( refNext->parent );
                    if( boneref && bonerefNext )
                    {
                        calc_bone_position(keyframe,
                                           keyframeNext,
                                           t,
                                           ref->parent,
                                           refNext->parent,
                                           &x_parent,
                                           &y_parent,
                                           &scalex_parent,
                                           &scaley_parent,
                                           &angle_parent,
                                           &angle_total,
                                           &angle_totalNext);
                    }


                    
                    float anglenow = obj->angle + angle_total;
                    float anglenext = objNext->angle + angle_totalNext;
                    if( anglenow<0 ){
                        anglenow += 360;
                    }
                    if( anglenext<0 ){
                        anglenext += 360;
                    }



                    float anglediff = 0;
                    float angle = 0.f;









                    //------------------------------------------------------------
                    // Position
                    //------------------------------------------------------------
                    anglediff = objNext->angle - obj->angle;
                    if (keyRef->IsSpinCounterClockwise())
					{
						if (anglediff < 0)
							anglediff = 360 + (objNext->angle-obj->angle);
					}
					else
					{
						if (anglediff > 0)
							anglediff = -360 + (objNext->angle-obj->angle);

					}
                    angle = (angle_total) + ( anglediff )*t;


                    float x = obj->x;
                    float y = obj->y;
                    float x1 = obj->x;
                    float y1 = obj->y;
                    float x2 = objNext->x;
                    float y2 = objNext->y;
                    calculate_rotation(0, 0,
                                       (obj->x)*scalex_parent, (obj->y)*scaley_parent,
                                       angle_total,
                                       &x1, &y1);
                    calculate_rotation(0, 0,
                                       (objNext->x)*scalex_parent, (objNext->y)*scaley_parent,
                                       angle_totalNext,
                                       &x2, &y2);
                    x = x1 + (  (x2)-(x1)  )*t;
                    y = y1 + (  (y2)-(y1)  )*t;
                    obj->sprite->setPosition( ccp(
                                                  x + x_parent,
                                                  y + y_parent)
                                             );
                    //------------------------------------------------------------




                    //------------------------------------------------------------
                    // Rotation
                    //------------------------------------------------------------
                    
                    anglediff = (anglenext)-(anglenow);
//                    anglediff = objNext->angle - obj->angle;
//                    anglediff = (angle_totalNext)-(angle_total);
                    if (keyRef->IsSpinCounterClockwise())
					{
						if (anglediff < 0)
							anglediff = 360 + (anglenext-anglenow);
					}
					else
					{
						if (angle > 0)
							anglediff = -360 + (anglenext-anglenow);
					}


                    
                    //============================================================
                    // Minimize the absurd 'Angle' case
                    //============================================================
                    if( fabs(anglediff)>180 )
                    {
                        anglediff += anglediff>0 ? -360 : 360;
                    }
                    //============================================================




                    angle = (angle_total + obj->angle) + ( anglediff )*t;


                    

                    //CCLog( "#AngleDiff = %3.1f, diff_original=%3.1f,  TotalDiff = %3.1f", anglediff, anglediff_original, (anglenext - anglenow) );
                    //============================================================
                    // If you use this factor, you will see smooth transformation
                    float testangle1 = -angle;
                    // If you use this factor, You will see only the first position in each keyframe.
                    //float testangle2 = -(angle_total + obj->angle);
                    //============================================================
                    obj->sprite->setRotation( testangle1 );
                    //
                    //------------------------------------------------------------






                    float px = obj->pivot_x+(objNext->pivot_x-obj->pivot_x)*t;
					float py = obj->pivot_y+(objNext->pivot_y-obj->pivot_y)*t;
                    obj->sprite->setAnchorPoint(ccp(px,py));



                    obj->sprite->setScaleX( scalex_parent*obj->scale_x );//<0 ? -1.f : 1.f );
                    obj->sprite->setScaleY( scaley_parent*obj->scale_y );//<0 ? -1.f : 1.f );
				}
			}
		}
	}

	void Animation::Render()
	{
        Key *keyframe = mMainline->GetKeyframe(mCurrKeyframe);
        int count = keyframe->GetObjectRefCount();
        for (int i=0;i<count;i++)
        {
            ObjectRef *ref = keyframe->GetObjectRef(i);
            if (ref)
            {
                Key *keyRef = mTimelines[ref->timeline]->GetKeyframe(ref->key);
                Object *obj = keyRef->GetObject(0); // should be only 1 object
                obj->sprite->visit();
            }
        }
	}


	///////////////////////////////////////////////////////////////////////////////////

	Entity::Entity()
    : mCurrAnimation(0)
    , mId(0)
	{
		mAnimations.reserve(50);
	};


	Entity::~Entity()
	{
		int count = mAnimations.size();
		for (int i=0;i<count;i++)
			CC_SAFE_DELETE(mAnimations[i]);

		mAnimations.clear();

	}


	void Entity::Update(float dt)
	{

		Animation *animation = mAnimations[mCurrAnimation];
		animation->Update(dt);

	}


	void Entity::Render()
	{
		Animation *animation = mAnimations[mCurrAnimation];
		animation->Render();

	}


	void Entity::NextAnimation()
	{
		mCurrAnimation++;
		if (mCurrAnimation >= (int)mAnimations.size())
			mCurrAnimation = 0;

		Animation *animation = mAnimations[mCurrAnimation];
		animation->Restart();

	}

    // 2013-02-09 brucewang
    void Entity::PlayAnimation(const char* animationName){
        int size = (int)mAnimations.size();
        for( int i=0; i<size; i++){
            Animation *animation = mAnimations[i];
            if( animation->getName().compare(animationName)== 0){
                animation->Restart();
                mCurrAnimation=i;
            }
        }
    }


	void Entity::SetId(int id)
	{
		mId = id;

	}


	void Entity::SetName(const char *name)
	{
		mName = name;

	}


	void Entity::AddAnimation(Animation *animation)
	{
		mAnimations.push_back(animation);

	}

}

///////////////////////////////////////////////////////////////////////////////////

using namespace SCMLHelper;


CCSpriterX::CCSpriterX()
{
	mFolders.reserve(50);
	mEntities.reserve(50);

}


CCSpriterX::~CCSpriterX()
{
}


CCSpriterX *CCSpriterX::create(const char *filename, const char *plist)
{
	CCSpriterX *animator = new CCSpriterX();
	if (animator && animator->initWithFile(filename, plist))
	{
		animator->autorelease();

		return animator;
	}
	CC_SAFE_DELETE(animator);
	return NULL;
}


void CCSpriterX::update(float dt)
{
	if (dt > 0.0167f)
		dt = 0.0167f;

	Entity *entity = mEntities[mCurrEntity];
	entity->Update(dt);

}


void CCSpriterX::draw(void)
{
	Entity *entity = mEntities[mCurrEntity];
	entity->Render();
}


void CCSpriterX::PlayNext()
{
	Entity *entity = mEntities[mCurrEntity];
	entity->NextAnimation();

}

// 2013-02-09 brucewang
void CCSpriterX::PlayAnimation(const char* animationName){
    Entity *entity = mEntities[mCurrEntity];
	entity->PlayAnimation(animationName);
}

CCSprite *CCSpriterX::getSprite(int folderId, int fileId)
{
	if (folderId < (int)mFolders.size())
	{
		Folder *folder = mFolders[folderId];
        
		if (folder)
		{
			File *file = folder->GetFile(fileId);
            
			if (file)
				return file->sprite;
		}
	}
    
	return NULL;
}

SCMLHelper::File* CCSpriterX::getFile(int folderId, int fileId){
    if (folderId < (int)mFolders.size())
	{
		Folder *folder = mFolders[folderId];
        
		if (folder)
		{
			File *file = folder->GetFile(fileId);
            return file;
		}
	}
    
	return NULL;
}



bool CCSpriterX::initWithFile(const char *filename, const char *plist)
{
	mCurrEntity = 0;
    //    mstrPlist = plist;
    
	unsigned long filesize;
	string st = CCFileUtils::getInstance()->fullPathForFilename(filename);
	 const char *path = st.c_str();
	char *buffer = (char *)CCFileUtils::getInstance()->getFileData(path, "rb", &filesize);
    
	if (buffer == NULL)
		return false;
    
    
    //CCLog("CCSpriterX::initWithFile (%s) : (%s)", filename, plist);
    CCSpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist);
	TiXmlDocument doc;
    
	doc.Parse(buffer);
    
	TiXmlNode *root = doc.FirstChild("spriter_data"); 
	if (root)
	{
		
		TiXmlElement *element = root->ToElement();
        
        //		const char *version = element->Attribute("scml_version");
        //		const char *generator = element->Attribute("generator");
        //		const char *generatorVersion = element->Attribute("generator_version");
        
        
        
		for (TiXmlNode* entityNode = root->FirstChild(); entityNode; entityNode = entityNode->NextSibling())
		{
			element = entityNode->ToElement();
            
			if (element)
			{
				const char *tab = element->Value();
                
				if (strcmp(tab, "folder")==0)
				{
					Folder *folder = new Folder();
                    
					folder->Init(entityNode, plist);
                    
					mFolders.push_back(folder);
                    
				}
				else if (strcmp(tab, "entity")==0)
				{
					int intValue;
					Entity *entity = new Entity();
                    
					if (element->QueryIntAttribute("id", &intValue) == TIXML_SUCCESS)
						entity->SetId(intValue);
                    
					entity->SetName(element->Attribute("name"));
                    
					for (TiXmlNode* animationNode = entityNode->FirstChild(); animationNode; animationNode = animationNode->NextSibling())
					{
						SCMLHelper::Animation *animation = new SCMLHelper::Animation();
						animation->Init(animationNode, this);
                        
						entity->AddAnimation(animation);
					}
                    
					mEntities.push_back(entity);
                    
				}
                
			}
		}
	}
    
	CC_SAFE_DELETE_ARRAY(buffer);
    
    //CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(plist);
    
	this->scheduleUpdate();
    
	return true;
    
}




void CCSpriterX::onEnter()
{
    CCNode::onEnter();
}

void CCSpriterX::onExit()
{
    CCNode::onExit();
}



