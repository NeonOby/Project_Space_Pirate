//------------------------------------------------------------------------
//
//	SCMLAnimator : KickStarter project Spriter renderer for cocos2d-x.
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


#ifndef _CC_SPRITER_X_H_
#define _CC_SPRITER_X_H_

#include <vector>
#include <string>

#include "cocos2d.h"
#include <string>

#include "TinyXMl/tinyxml.h"


class CCSpriterX;
using namespace std;
USING_NS_CC;

namespace SCMLHelper
{
    class myCCSprite : public cocos2d::CCSprite
    {
    };

	struct File
	{
		File();
		~File();

		void Init(TiXmlNode *node);

		int id;
		std::string name;
		float width;
		float height;

		cocos2d::CCSprite *sprite;

	};

	class Folder
	{
	public:
		Folder();
		~Folder();

		void Init(TiXmlNode *node, const char* szPlist);

		int GetFileCount();
		File *GetFile(int index);

	private:
		int mId;
		std::string mName;

		std::vector <File *> mFiles;

	};







    // 2013-03-12 brucewang
    struct BoneRef
	{
		void Init(TiXmlNode *node);

		int id;
        // initially -1
        int parent;
		int timeline;
		int key;
	};

    // 2013-03-12 brucewang
    struct Bone
    {
        void Init(TiXmlNode *node);

        float x;
		float y;
		float angle;
        float scale_x;
        float scale_y;
    };








	struct ObjectRef
	{
		void Init(TiXmlNode *node);

		int id;
		int timeline;
		int key;
		int z_index;

        // 2013-03-12 bruceawng
        // initially -1
        int parent;
	};

	struct Object
	{
		void Init(TiXmlNode *node, CCSpriterX *animator);

		int folder;
		int file;
		float x;
		float y;
		float angle;
		float pivot_x;
		float pivot_y;
		int z_index;

        float scale_x;
        float scale_y;

        SCMLHelper::File* fileinfo;

		cocos2d::CCSprite *sprite;


        // pre-calculated values
//        float calculated_angle;
//        float calculated_x;
//        float calculated_y;
//        float parent_x;
//        float parent_y;
	};

	class Key
	{
	public:
		Key();
		~Key();

		void Init(TiXmlNode *node, CCSpriterX *animator);

		int GetObjectRefCount();
		ObjectRef *GetObjectRef(int index);
		int GetObjectCount();
		Object *GetObject(int index);

        // 2013-03-12 brucewang
        int GetBoneRefCount();
        BoneRef* GetBoneRef(int index);
        int GetBoneCount();
        Bone* GetBone(int index);

		float GetTime();

		bool IsSpinCounterClockwise();

	private:
		int mId;
		float mTime;
		bool mSpinCounterClockwise;

		std::vector <Object *> mObjects;
		std::vector <ObjectRef *> mObjectRefs;

        // Bones
        std::vector <Bone *> mBones;
		std::vector <BoneRef *> mBoneRefs;
	};

	class Timeline
	{
	public:
		Timeline();
		~Timeline();

		void Init(TiXmlNode *node, CCSpriterX *animator);
		int GetKeyframeCount();
		Key *GetKeyframe(int index);

	private:
		int mId;
		std::vector <Key *> mKeyframes;
	};


	class Animation
	{
	public:
		Animation();
		~Animation();

		void Update(float dt);
		void Init(TiXmlNode *node, CCSpriterX *animator);

		void Render();

		bool IsDone();

		void Restart();

        // 2013-02-09 brucewang
        std::string getName(){ return mName; }


	private:
		int mId;
		std::string mName;
		float mLength;
		bool mLooping;
		bool mDone;

		Timeline *mMainline;
		int mCurrKeyframe;

		std::vector <Timeline *> mTimelines;



		float mTimer;

		cocos2d::CCPoint mPosition;


        void calc_bone_position(Key *keyframe,
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
                                float* angle_totalNext);
	};


	class Entity
	{
	public:
		Entity();
		~Entity();

		void Update(float dt);
		void Render();

		void SetId(int id);
		void SetName(const char *name);
		void AddAnimation(Animation *animation);

		void NextAnimation();

        // 2013-02-09 brucewang
        void PlayAnimation(const char* animationName);


	private:

		int mId;
		std::string mName;

		std::vector <Animation *> mAnimations;

		int mCurrAnimation;

	};

}


class CCSpriterX : public cocos2d::CCNode
{
public:
	CCSpriterX();
	~CCSpriterX();

	bool initWithFile(const char *filename, const char *plist);

	virtual void draw(void);
	virtual void update(float dt);

	static CCSpriterX* create(const char *filename, const char *plist);

	cocos2d::CCSprite *getSprite(int folderId, int fileId);
    SCMLHelper::File* getFile(int folderId, int fileId);
    
    
    
	virtual void PlayNext();
    // 2013-02-09 brucewang
    virtual void PlayAnimation(const char* animationName);
    
    virtual void onEnter();
    virtual void onExit();
    
    
    //    CC_SYNTHESIZE(std::string, mstrPlist, PlistFile);
    
    
private:
    
	std::vector <SCMLHelper::Folder *> mFolders;
	std::vector <SCMLHelper::Entity *> mEntities;
    
	int mCurrEntity;
    
};



#endif
