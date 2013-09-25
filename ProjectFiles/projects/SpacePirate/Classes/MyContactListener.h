#ifndef __MY_CONTACT_LISTENER_H__
#define __MY_CONTACT_LISTENER_H__

#include "Box2D\Box2D.h"
#include <vector>
#include <algorithm>
#include "Konstanten.h"
#include "cocos2d.h"

class MyContactListener : public b2ContactListener {
private:
	bool addContact(b2Fixture *fixtureA, b2Fixture *fixtureB, int data_filter, int &sum);
	bool remContact(b2Fixture *fixtureA, b2Fixture *fixtureB, int data_filter, int &sum);

	bool addContact(b2Fixture *fixtureA, b2Fixture *fixtureB, int data_filter1, int data_filter2, int &sum);
	bool remContact(b2Fixture *fixtureA, b2Fixture *fixtureB, int data_filter1, int data_filter2, int &sum);

	cocos2d::Sprite *mPlayer;

	std::map<int, std::map<b2Body*, ContactCounter*>*> ContactList;

	bool addContact(b2Body* pBody, int pFixture, int pOtherFixture);
	bool removeContact(b2Body* pBody, int pFixture, int pOtherFixture);
public:
	
	static MyContactListener * GetInstance();

	int *AddListener(b2Body * pBody, int pFIXTURE_TAG, int pFixtureMask);
	void RemoveListener(b2Body * pBody, int pFIXTURE_TAG);

	std::vector<MyContact>_contacts;
	
	std::vector<BulletHit>mBulletHits;


	MyContactListener();
	~MyContactListener();
	
	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);    
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
	
};

#endif //__MY_CONTACT_LISTENER_H__