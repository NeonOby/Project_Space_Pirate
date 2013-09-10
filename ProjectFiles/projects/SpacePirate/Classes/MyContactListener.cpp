#include "MyContactListener.h"

USING_NS_CC;

MyContactListener::MyContactListener() : _contacts() {
	playerFootContacts=0;

	playerRightSideContacts=0;
	playerRightStartClimbContacts=0;

	playerLeftSideContacts=0;
	playerLeftStartClimbContacts=0;
}

MyContactListener::~MyContactListener() {
}

bool MyContactListener::addContact(b2Fixture *fixtureA, b2Fixture *fixtureB, int data_filter, int &sum){
	if((int)fixtureA->GetUserData() == data_filter || (int)fixtureB->GetUserData() == data_filter){
		sum++;
		return true;
	}
	return false;
}

bool MyContactListener::remContact(b2Fixture *fixtureA, b2Fixture *fixtureB, int data_filter, int &sum){
	if((int)fixtureA->GetUserData() == data_filter || (int)fixtureB->GetUserData() == data_filter){
		if(sum>0){
			sum--;
		}
		return true;
	}
	return false;
}

void MyContactListener::BeginContact(b2Contact* contact) {
    // We need to copy out the data because the b2Contact passed in
    // is reused.

	//If contact is with playerFoot don't save it, just add 1 to playerFootContacts
	if(addContact(contact->GetFixtureA(),contact->GetFixtureB(), PLAYER_FOOD, playerFootContacts)){
		return;
	}
	if(addContact(contact->GetFixtureA(),contact->GetFixtureB(), PLAYER_RIGHT_SIDE, playerRightSideContacts)){
		return;
	}
	if(addContact(contact->GetFixtureA(),contact->GetFixtureB(), PLAYER_RIGHT_START_CLIMB, playerRightStartClimbContacts)){
		return;
	}
	if(addContact(contact->GetFixtureA(),contact->GetFixtureB(), PLAYER_LEFT_SIDE, playerLeftSideContacts)){
		return;
	}
	if(addContact(contact->GetFixtureA(),contact->GetFixtureB(), PLAYER_LEFT_START_CLIMB, playerLeftStartClimbContacts)){
		return;
	}

	MyContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
	_contacts.push_back(myContact);
}

void MyContactListener::EndContact(b2Contact* contact) {
	//If contact is with playerFoot don't save it, just remove 1 to playerFootContacts
	if(remContact(contact->GetFixtureA(),contact->GetFixtureB(), PLAYER_FOOD, playerFootContacts)){
		return;
	}
	if(remContact(contact->GetFixtureA(),contact->GetFixtureB(), PLAYER_RIGHT_SIDE, playerRightSideContacts)){
		return;
	}
	if(remContact(contact->GetFixtureA(),contact->GetFixtureB(), PLAYER_RIGHT_START_CLIMB, playerRightStartClimbContacts)){
		return;
	}
	if(remContact(contact->GetFixtureA(),contact->GetFixtureB(), PLAYER_LEFT_SIDE, playerLeftSideContacts)){
		return;
	}
	if(remContact(contact->GetFixtureA(),contact->GetFixtureB(), PLAYER_LEFT_START_CLIMB, playerLeftStartClimbContacts)){
		return;
	}

    MyContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
    std::vector<MyContact>::iterator pos;
    pos = std::find(_contacts.begin(), _contacts.end(), myContact);
    if (pos != _contacts.end()) {
        _contacts.erase(pos);
    }
}

void MyContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
}

void MyContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
}