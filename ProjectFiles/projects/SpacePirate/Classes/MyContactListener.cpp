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
	playerFootContacts=0;

	playerRightSideContacts=0;
	playerRightStartClimbContacts=0;

	playerLeftSideContacts=0;
	playerLeftStartClimbContacts=0;
}

MyContactListener::~MyContactListener() {
}

int *MyContactListener::AddListener(b2Body * pBody, int pFIXTURE_TAG){
	//Wenn noch keine Map für diese Fixture gemacht wurde, erstell sie
	if(!ContactList.count(pFIXTURE_TAG)){
		ContactList.emplace(pFIXTURE_TAG, new map<b2Body*, int*>());
	}

	int *contactCount(0);
	if(!ContactList.at(pFIXTURE_TAG)->count(pBody))
		ContactList.at(pFIXTURE_TAG)->emplace(pBody, contactCount);
	else{
		contactCount = ContactList.at(pFIXTURE_TAG)->at(pBody);
		log("You just wanted to add a Listener for the second time:\
			Adress Body: %p\
			Adress Int: %p", pBody, contactCount);
	}

	return contactCount;
	}

void MyContactListener::RemoveListener(b2Body * pBody, int pFIXTURE_TAG, int *pInt){		
	//Wenn es diesen Listener gibt, lösche Ihn
	if(ContactList.at(pFIXTURE_TAG)->count(pBody))
		ContactList.at(pFIXTURE_TAG)->erase(pBody);

	//Lösche die MAP aus dem HEAP, wenn sie leer ist
	if(ContactList.at(pFIXTURE_TAG)->empty()){
		map<b2Body*, int*> *mapPTR = ContactList.at(pFIXTURE_TAG);
		ContactList.erase(pFIXTURE_TAG);
		delete mapPTR;
	}

	delete pInt;
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

	if((int)contact->GetFixtureA()->GetUserData() == ENEMY_FOOT){
		for (std::map<b2Body *, int *>::iterator it = EnemyFootContacts.begin(); it != EnemyFootContacts.end(); ++it)
			{
			if((*it).first==contact->GetFixtureA()->GetBody()){
				(*(*it).second)+=1;
				}

			}
		return;
		}else if((int)contact->GetFixtureB()->GetUserData() == ENEMY_FOOT){
			for (std::map<b2Body *, int *>::iterator it = EnemyFootContacts.begin(); it != EnemyFootContacts.end(); ++it)
				{
				if((*it).first==contact->GetFixtureB()->GetBody()){
					(*(*it).second)+=1;
					}
				}
			return;
		}

	if((int)contact->GetFixtureA()->GetUserData() == BULLET){
		//Tell FuxtureB that he got Hit
		//e.g. Do Damage/Apply Forces
		//Make some fancy effects
		//Tell Logik to do things, because we can't delete Bodies while listening !!

		BulletHit myHit = { contact->GetFixtureA(), contact->GetFixtureB() };
		mBulletHits.push_back(myHit);

		return;
		}else if((int)contact->GetFixtureB()->GetUserData() == BULLET){
			//Tell FuxtureB that he got Hit
			//e.g. Do Damage/Apply Forces
			//Make some fancy effects
			//Tell Logik to do things, because we can't delete Bodies while listening !!

			BulletHit myHit = { contact->GetFixtureB(), contact->GetFixtureA() };
			mBulletHits.push_back(myHit);
			return;
		}

	//If contact is with playerFoot don't save it, just add 1 to playerFootContacts
	if(addContact(contact->GetFixtureA(),contact->GetFixtureB(), PLAYER_FOOD, playerFootContacts)){
		return;
		}
	if(addContact(contact->GetFixtureA(),contact->GetFixtureB(), PLAYER_RIGHT_SIDE, playerRightSideContacts)){
		return;
		}
	if(addContact(contact->GetFixtureA(),contact->GetFixtureB(), PLAYER_RIGHT_START_CLIMB, CLIMBFIXTURE, playerRightStartClimbContacts)){
		return;
		}
	if(addContact(contact->GetFixtureA(),contact->GetFixtureB(), PLAYER_LEFT_SIDE, playerLeftSideContacts)){
		return;
		}
	if(addContact(contact->GetFixtureA(),contact->GetFixtureB(), PLAYER_LEFT_START_CLIMB, CLIMBFIXTURE, playerLeftStartClimbContacts)){
		return;
		}

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

	if((int)contact->GetFixtureA()->GetUserData() == ENEMY_FOOT){
		for (std::map<b2Body *, int *>::iterator it = EnemyFootContacts.begin(); it != EnemyFootContacts.end(); ++it)
			{
			if((*it).first==contact->GetFixtureA()->GetBody()){
				(*(*it).second)-=1;
				}

			}
		return;
		}else if((int)contact->GetFixtureB()->GetUserData() == ENEMY_FOOT){
			for (std::map<b2Body *, int *>::iterator it = EnemyFootContacts.begin(); it != EnemyFootContacts.end(); ++it)
				{

				if((*it).first==contact->GetFixtureB()->GetBody()){
					(*(*it).second)-=1;
					}
				}
			return;
		}

	//If contact is with playerFoot don't save it, just remove 1 to playerFootContacts
	if(remContact(contact->GetFixtureA(),contact->GetFixtureB(), PLAYER_FOOD, playerFootContacts)){
		return;
		}
	if(remContact(contact->GetFixtureA(),contact->GetFixtureB(), PLAYER_RIGHT_SIDE, playerRightSideContacts)){
		return;
		}
	if(remContact(contact->GetFixtureA(),contact->GetFixtureB(), PLAYER_RIGHT_START_CLIMB, CLIMBFIXTURE, playerRightStartClimbContacts)){
		return;
		}
	if(remContact(contact->GetFixtureA(),contact->GetFixtureB(), PLAYER_LEFT_SIDE, playerLeftSideContacts)){
		return;
		}
	if(remContact(contact->GetFixtureA(),contact->GetFixtureB(), PLAYER_LEFT_START_CLIMB, CLIMBFIXTURE, playerLeftStartClimbContacts)){
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