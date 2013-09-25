#include "MyContactListener.h"

USING_NS_CC;

static MyContactListener *ListenerInstance;

MyContactListener * MyContactListener::GetInstance(){
	if (!ListenerInstance)
		{
		ListenerInstance = new MyContactListener();
		}

	return ListenerInstance;
	}

MyContactListener::MyContactListener() : _contacts(), ContactList() {
}

MyContactListener::~MyContactListener() {
}

bool MyContactListener::addContact(b2Body *pBody, int pFixture, int pOtherFixture){
	if(!ContactList.count(pFixture)){
		return false;
	}
	if(!ContactList.at(pFixture)->count(pBody)){
		return false;
	}
	if((pOtherFixture & ContactList.at(pFixture)->at(pBody)->mFixtureMask)){
		//Here should be what we look for
		ContactList.at(pFixture)->at(pBody)->mCount++;
		return true;
	}
	return false;
}

bool MyContactListener::removeContact(b2Body *pBody, int pFixture, int pOtherFixture){
	if(!ContactList.count(pFixture)){
		return false;
	}
	if(!ContactList.at(pFixture)->count(pBody)){
		return false;
	}
	if((pOtherFixture & ContactList.at(pFixture)->at(pBody)->mFixtureMask)){
		//Here should be what we look for
		ContactList.at(pFixture)->at(pBody)->mCount--;
		return true;
	}
	return false;
}

int *MyContactListener::AddListener(b2Body * pBody, int pFIXTURE_TAG, int pFixtureMask){
	//Wenn noch keine Map für diese Fixture gemacht wurde, erstell sie
	if(!ContactList.count(pFIXTURE_TAG)){
		ContactList.emplace(pFIXTURE_TAG, new map<b2Body*, ContactCounter*>());
	}

	if(pFIXTURE_TAG == PLAYER_FOOT)
		log("Add Food Listener");

	ContactCounter *contactCount = new ContactCounter();
	contactCount->mCount = 0;
	contactCount->mFixtureMask = pFixtureMask;

	if(!ContactList.at(pFIXTURE_TAG)->count(pBody))
		ContactList.at(pFIXTURE_TAG)->emplace(pBody, contactCount);
	else{
		contactCount = ContactList.at(pFIXTURE_TAG)->at(pBody);
		log("You just wanted to add a Listener for the second time:\
			Adress Body: %p\
			Adress Int: %p", pBody, contactCount);
		}

	return &(contactCount->mCount);
	}

void MyContactListener::RemoveListener(b2Body * pBody, int pFIXTURE_TAG){		
	//Wenn es diesen Listener gibt, lösche Ihn
	if(ContactList.at(pFIXTURE_TAG)->count(pBody)){
		delete ContactList.at(pFIXTURE_TAG)->at(pBody);
		ContactList.at(pFIXTURE_TAG)->erase(pBody);
	}

	//Lösche die MAP aus dem HEAP, wenn sie leer ist
	if(ContactList.at(pFIXTURE_TAG)->empty()){
		map<b2Body*, ContactCounter*> *mapPTR = ContactList.at(pFIXTURE_TAG);
		ContactList.erase(pFIXTURE_TAG);
		delete mapPTR;
		}
	}

bool MyContactListener::addContact(b2Fixture *fixtureA, b2Fixture *fixtureB, int data_filter, int &sum){
	if((int)fixtureA->GetUserData() == data_filter || (int)fixtureB->GetUserData() == data_filter){
		sum++;
		return true;
		}
	return false;
	}

bool MyContactListener::addContact(b2Fixture *fixtureA, b2Fixture *fixtureB, int data_filter1, int data_filter2, int &sum){
	if((int)fixtureA->GetUserData() == data_filter1 && (int)fixtureB->GetUserData() == data_filter2){
		sum++;
		return true;
		}
	if((int)fixtureA->GetUserData() == data_filter2 && (int)fixtureB->GetUserData() == data_filter1){
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

bool MyContactListener::remContact(b2Fixture *fixtureA, b2Fixture *fixtureB, int data_filter1, int data_filter2, int &sum){
	if((int)fixtureA->GetUserData() == data_filter1 && (int)fixtureB->GetUserData() == data_filter2){
		if(sum>0){
			sum--;
			}
		return true;
		}
	if((int)fixtureA->GetUserData() == data_filter2 && (int)fixtureB->GetUserData() == data_filter1){
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

	bool ContactListUsed(false);

	if(addContact(contact->GetFixtureA()->GetBody(), (int)contact->GetFixtureA()->GetUserData(), (int)contact->GetFixtureB()->GetUserData())){
		ContactListUsed=true;
	}
	if(addContact(contact->GetFixtureB()->GetBody(), (int)contact->GetFixtureB()->GetUserData(), (int)contact->GetFixtureA()->GetUserData())){
		ContactListUsed=true;
	}

	if(ContactListUsed)
		return;

	if((int)contact->GetFixtureA()->GetUserData() == BULLET){
		BulletHit myHit = { contact->GetFixtureA(), contact->GetFixtureB() };
		mBulletHits.push_back(myHit);
		return;
	}else if((int)contact->GetFixtureB()->GetUserData() == BULLET){
		BulletHit myHit = { contact->GetFixtureB(), contact->GetFixtureA() };
		mBulletHits.push_back(myHit);
		return;
	}

	//If contact is with playerFoot don't save it, just add 1 to playerFootContacts

	if(mPlayer){
		//TODO: remove Player Instance and use Extra Sensor Fixture
		/*
		if((int)contact->GetFixtureA()->GetUserData() == KISTE && contact->GetFixtureB()->GetBody()->GetUserData() == mPlayer){
		b2Body * chest = contact->GetFixtureA()->GetBody();
		b2Body * player = contact->GetFixtureB()->GetBody();

		float diffX = chest->GetPosition().x - player->GetPosition().x;

		chest->ApplyForceToCenter(b2Vec2(diffX*chest->GetMass()*PLAYER_MAGNETIK_OBJECT_REJECT,0));
		}
		if((int)contact->GetFixtureB()->GetUserData() == KISTE && contact->GetFixtureA()->GetBody()->GetUserData() == mPlayer){
		b2Body * chest = contact->GetFixtureB()->GetBody();
		b2Body * player = contact->GetFixtureA()->GetBody();

		float diffX = chest->GetPosition().x - player->GetPosition().x;

		chest->ApplyForceToCenter(b2Vec2(diffX*chest->GetMass()*PLAYER_MAGNETIK_OBJECT_REJECT,0));
		}
		*/
		}


	MyContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
	_contacts.push_back(myContact);
	}

void MyContactListener::EndContact(b2Contact* contact) {

	bool ContactListUsed(false);

	if(removeContact(contact->GetFixtureA()->GetBody(), (int)contact->GetFixtureA()->GetUserData(), (int)contact->GetFixtureB()->GetUserData())){
		ContactListUsed=true;
	}
	if(removeContact(contact->GetFixtureB()->GetBody(), (int)contact->GetFixtureB()->GetUserData(), (int)contact->GetFixtureA()->GetUserData())){
		ContactListUsed=true;
	}

	if(ContactListUsed)
		return;

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