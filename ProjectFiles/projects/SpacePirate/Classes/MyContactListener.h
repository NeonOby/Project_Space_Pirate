#ifndef __MY_CONTACT_LISTENER_H__
#define __MY_CONTACT_LISTENER_H__

#include "Box2D\Box2D.h"
#include <vector>
#include <algorithm>
#include "Konstanten.h"
#include "cocos2d.h"

struct MyContact {
	b2Fixture *fixtureA;
	b2Fixture *fixtureB;
	bool operator==(const MyContact& other) const
	{
		return (fixtureA == other.fixtureA) && (fixtureB == other.fixtureB);
	}
};

//Bullet, HitBody, Point, Force
struct BulletHit {
	b2Fixture *bulletFixture;
	b2Fixture *hitFixture;

	bool operator==(const BulletHit& other) const
	{
		//Just for better readability
		return (\
				bulletFixture == other.bulletFixture) \
			&& (hitFixture == other.hitFixture);
	}
};

class MyContactListener : public b2ContactListener {
private:
	bool addContact(b2Fixture *fixtureA, b2Fixture *fixtureB, int data_filter, int &sum);
	bool remContact(b2Fixture *fixtureA, b2Fixture *fixtureB, int data_filter, int &sum);

	bool addContact(b2Fixture *fixtureA, b2Fixture *fixtureB, int data_filter1, int data_filter2, int &sum);
	bool remContact(b2Fixture *fixtureA, b2Fixture *fixtureB, int data_filter1, int data_filter2, int &sum);

	cocos2d::Sprite *mPlayer;

public:
	std::vector<MyContact>_contacts;
	
	std::vector<BulletHit>mBulletHits;

	int playerFootContacts;
	int playerRightSideContacts;
	int playerRightStartClimbContacts;

	int playerLeftSideContacts;
	int playerLeftStartClimbContacts;

	MyContactListener(cocos2d::Sprite * pPlayer);
	MyContactListener();
	~MyContactListener();
	
	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);    
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
	
};

#endif //__MY_CONTACT_LISTENER_H__