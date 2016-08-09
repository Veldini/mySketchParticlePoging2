#include "particle.h"

//------------------------------------------------------------------
demoParticle::demoParticle() {
	attractPoints = NULL;
}

//------------------------------------------------------------------
void demoParticle::setMode(particleMode newMode) {
	mode = newMode;
}

//------------------------------------------------------------------
void demoParticle::setAttractPoints(vector <ofPoint> * attract) {
	attractPoints = attract;
}

//------------------------------------------------------------------
void demoParticle::reset() {
	//the unique val allows us to set properties slightly differently for each particle
	uniqueVal = ofRandom(-10000, 10000);

	pos.x = ofRandomWidth();
	pos.y = ofRandomHeight();

	vel.x = ofRandom(-3.9, 3.9);
	vel.y = ofRandom(-3.9, 3.9);

	frc = ofPoint(0, 0, 0);
	scale = ofRandom(1.0, 2.0);
	drag = ofRandom(0.95, 0.998);
	


}

//------------------------------------------------------------------
void demoParticle::update() {

	//1 - APPLY THE FORCES BASED ON WHICH MODE WE ARE IN 

	if (mode == PARTICLE_MODE_ATTRACT) {
		ofPoint attractPt(ofGetMouseX(), ofGetMouseY());
		frc = attractPt - pos; // we get the attraction force/vector by looking at the mouse pos relative to our pos
		frc.normalize(); //by normalizing we disregard how close the particle is to the attraction point 

		vel *= drag; //apply drag
		vel += frc * 0.6; //apply force
	}
	else if (mode == PARTICLE_MODE_REPEL) {
		ofPoint attractPt(ofGetMouseX(), ofGetMouseY());
		frc = attractPt - pos;

		//let get the distance and only repel points close to the mouse
		float dist = frc.length();
		frc.normalize();

		vel *= drag;
		if (dist < 150) {
			vel += -frc * 0.6; //notice the frc is negative 
		}
		else {
			//if the particles are not close to us, lets add a little bit of random movement using noise. this is where uniqueVal comes in handy. 			
			frc.x = ofSignedNoise(uniqueVal, pos.y * 0.01, ofGetElapsedTimef()*0.2);
			frc.y = ofSignedNoise(uniqueVal, pos.x * 0.01, ofGetElapsedTimef()*0.2);
			vel += frc * 0.04;
		}
	}

	//2 - UPDATE OUR POSITION
	pos += vel;

	//3 - (optional) LIMIT THE PARTICLES TO STAY ON SCREEN 
	//we could also pass in bounds to check - or alternatively do this at the ofApp level
	if (pos.x > ofGetWidth()) {
		pos.x = ofGetWidth();
		vel.x *= -1.0;
	}
	else if (pos.x < 0) {
		pos.x = 0;
		vel.x *= -1.0;
	}
	if (pos.y > ofGetHeight()) {
		pos.y = ofGetHeight();
		vel.y *= -1.0;
	}
	else if (pos.y < 0) {
		pos.y = 0;
		vel.y *= -1.0;
	}
}

//------------------------------------------------------------------
void demoParticle::draw() {
	if (mode == PARTICLE_MODE_ATTRACT) {
		ofSetColor(255, 63, 180);
	}
	else if (mode == PARTICLE_MODE_REPEL) {
		ofSetColor(ofRandom(150, 255), ofRandom(0), ofRandom(150, 255));

	}
	else if (mode == PARTICLE_MODE_REPEL) {
		ofSetColor(255, 63, 180);

	}


	float distance = ofDist(ofGetMouseX(), ofGetMouseY(), pos.x, pos.y); // a method oF gives us to check the distance between two coordinates

	//ofSetLineWidth(2);
	//glPushMatrix();
  //  glTranslatef(pos.x, pos.y, 0);
	//float alpha = atan(vel.y / vel.x) + (vel.x < 0 ? PI : 0);
	//glRotatef(ofGetMouseX(), 1, ofGetMouseY(), 0);
	//ofRotate(ofGetMouseX(),0,ofGetMouseY(),0);

	ofFill();
	//ofBeginShape();
//	ofVertex(pos.x - 15, pos.y - 5);
	//ofVertex(pos.x - 15, pos.y + 5);
	//ofVertex(pos.x + 15, pos.y - 5);
	//ofEndShape(true);
	//ofSetColor(128, 128, 128);
	
	//ofRotateX(ofGetMouseX());
	//ofRotateY(ofGetMouseY());

	ofDrawTriangle(pos.x-15, pos.y-5, pos.x-15, pos.y+5, pos.x+15, pos.y-5);
	
	//ofDrawCircle(pos.x, pos.y, scale * 5.0);
//	ofDrawTriangle(10, 10, 20, 30, 30, 25);
}