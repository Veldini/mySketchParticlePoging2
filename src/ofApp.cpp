#include "ofApp.h"

#define PIN_LED 12
#define PIN_BUTTON 2
#define PIN_POTMETER 0
//--------------------------------------------------------------
void ofApp::setup() {
	// set listener - call ofApp::setupArduino when arduino is ready for use
	ofAddListener(arduino.EInitialized, this, &ofApp::setupArduino);

	// connect to the Arduino
	// use the same device name used in the Arduino IDE
	arduino.connect("COM3");
	arduino.sendFirmwareVersionRequest();
	
	ofSetVerticalSync(true);

	int num = 1500;
	p.assign(num, demoParticle());
	currentMode = PARTICLE_MODE_ATTRACT;

	currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";

	resetParticles();

}

//--------------------------------------------------------------
void ofApp::resetParticles() {

	//these are the attraction points used in the forth demo 
	attractPoints.clear();
	for (int i = 0; i < 4; i++) {
		attractPoints.push_back(ofPoint(ofMap(i, 0, 4, 100, ofGetWidth() - 100), ofRandom(100, ofGetHeight() - 100)));
	}

	attractPointsWithMovement = attractPoints;

	for (unsigned int i = 0; i < p.size(); i++) {
		p[i].setMode(currentMode);
		p[i].setAttractPoints(&attractPointsWithMovement);;
		p[i].reset();
	}
}

//--------------------------------------------------------------
void ofApp::update() {
	arduino.update();

	for (unsigned int i = 0; i < p.size(); i++) {
		p[i].setMode(currentMode);
		p[i].update();
	}

	//lets add a bit of movement to the attract points
	for (unsigned int i = 0; i < attractPointsWithMovement.size(); i++) {
		attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 12.0;
		attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackgroundGradient(ofColor(60, 60, 60), ofColor(10, 10, 10));

	ofSetColor(255, 0, 0);
	ofFill();
	//ofDrawCircle(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, potMeter);

	ofDrawTriangle(10, 10, 20, 30, 30, 25);
	


	ofDrawCircle(ofGetMouseX(), ofGetMouseY(), potMeter);



	for (unsigned int i = 0; i < p.size(); i++) {
		p[i].draw();
	}



	ofSetColor(230);
	ofDrawBitmapString(currentModeStr + "\n\nSpacebar to reset. \nKeys 1-2 to change mode.", 10, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == '1') {
		currentMode = PARTICLE_MODE_ATTRACT;
		currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";
	}
	if (key == '2') {
		currentMode = PARTICLE_MODE_REPEL;
		currentModeStr = "2 - PARTICLE_MODE_REPEL: repels from mouse";
	}



	if (key == ' ') {
		resetParticles();
	}



}

void ofApp::setupArduino(const int& version) {
	// remove listener because we don't need it anymore
	ofRemoveListener(arduino.EInitialized, this, &ofApp::setupArduino);

	// print firmware name and version to the console
	ofLogNotice() << "Arduino firmware found: " << arduino.getFirmwareName()
		<< " v" << arduino.getMajorFirmwareVersion() << "." << arduino.getMinorFirmwareVersion();

	arduino.sendDigitalPinMode(PIN_LED, ARD_OUTPUT);
	arduino.sendDigitalPinMode(PIN_BUTTON, ARD_INPUT);
	arduino.sendAnalogPinReporting(PIN_POTMETER, ARD_ANALOG);

	// set listeners for pin events
	ofAddListener(arduino.EDigitalPinChanged, this, &ofApp::digitalPinChanged);
	ofAddListener(arduino.EAnalogPinChanged, this, &ofApp::analogPinChanged);

}

void ofApp::digitalPinChanged(const int& pinNum) {
	// get value with arduino.getDigital(pinNum)
	ofLogNotice() << "Digital Pin " << pinNum << " value: " << arduino.getDigital(pinNum) << endl;
	knop = arduino.getDigital(pinNum);

	if (knop = 1) {
		currentMode = PARTICLE_MODE_REPEL;
		currentModeStr = "2 - PARTICLE_MODE_REPEL: repels from mouse";
	}
}

void ofApp::analogPinChanged(const int& pinNum) {
	// get value with arduino.getAnalog(pinNum));
	//	ofLogNotice() << "Analog Pin " << pinNum << " value: " << arduino.getAnalog(pinNum) << endl;
	potMeter = arduino.getAnalog(pinNum);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	// switch the LED on
	arduino.sendDigital(PIN_LED, ARD_HIGH);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	// switch the LED off
	arduino.sendDigital(PIN_LED, ARD_LOW);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}