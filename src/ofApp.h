#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	bool loadVideo(const std::string& file);
	void loadVideoWithDialog();

	void resetVideoDims();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofVideoPlayer video;

	ofxPanel gui;
	ofParameter<glm::vec2> vidPos{ "video pos", {0,0}, {-2000,-2000}, {2000,2000} };
	ofParameter<glm::vec2> vidSize{ "video size", { 256, 512 }, {0,0}, {4000, 4000} };
	ofParameter<ofColor> bgColor{ "bg color", ofColor(0,0,0) };
	ofxButton resetVideoDimsButton;
	ofxButton loadButton;
	ofxLabel videoTitle;
	bool bShowGui = true;
	bool bIsPaused = false;
	float instructionsStartTime = 0;

	ofRectangle progressBar;
};
