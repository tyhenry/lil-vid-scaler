#include "ofApp.h"

const float INSTRUCTIONS_DUR = 5;

//--------------------------------------------------------------
void ofApp::setup() {

	ofBackground(0);
	ofSetVerticalSync(true);
	ofDisableArbTex();

	ofxGuiSetDefaultWidth(300);

	gui.setup("settings", "settings.json", 960, 200);

	gui.add(loadButton.setup("LOAD VIDEO"));
	gui.add(videoTitle.setup("video", "none loaded"));
	gui.add(resetVideoDimsButton.setup("ORIGINAL VIDEO DIMS"));
	gui.add(vidPos);
	gui.add(vidSize);
	gui.add(bgColor);

	resetVideoDimsButton.addListener(this, &ofApp::resetVideoDims);
	loadButton.addListener(this, &ofApp::loadVideoWithDialog);

	gui.loadFromFile("settings.json");

	progressBar.set(0, ofGetHeight() - 30, ofGetWidth(), 30);
}

//--------------------------------------------------------------
void ofApp::update() {

	video.update();
	if (video.isLoaded()) {
		video.getTexture().enableMipmap();
		video.getTexture().setTextureMinMagFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
		video.getTexture().generateMipmap();
	}

	glm::vec2 winSize = ofGetWindowSize();
	progressBar.width = winSize.x;
	progressBar.y = winSize.y - progressBar.height;
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(bgColor.get());

	float time = ofGetElapsedTimef();
	glm::vec2 winSize = ofGetWindowSize();

	if (video.isLoaded()) {
		video.draw(vidPos.get().x, vidPos.get().y, vidSize.get().x, vidSize.get().y);
	}

	if (bShowGui) {
		gui.draw();
		if (video.isLoaded()) {
			ofPushStyle();
			ofSetColor(127);
			ofDrawRectangle(progressBar);
			ofSetColor(255, 0, 0);
			float pct = video.getPosition();
			ofDrawRectangle(progressBar.x, progressBar.y, progressBar.width * pct, progressBar.height);
			ofPopStyle();
		}
	}

	float timeSinceInstructionsStarted = time - instructionsStartTime;
	if (bShowGui || timeSinceInstructionsStarted < INSTRUCTIONS_DUR) {
		float alpha = bShowGui ? 255 : ofMap(timeSinceInstructionsStarted, 0, INSTRUCTIONS_DUR, 255, 0, true);
		ofColor c{ 255,255,255, alpha };
		ofPushStyle();
		ofSetColor(c);
		ofDrawBitmapString("Keys:\n'G': toggle GUI\n'F': toggle fullscreen\nSPACE: toggle pause/play", winSize.x - 225, 25);
		ofPopStyle();
	}
}

//--------------------------------------------------------------
bool ofApp::loadVideo(const std::string& file)
{
	std::filesystem::path path = file;
	std::string ext = path.extension().string();
	std::string filename = path.filename().string();
	if (ext == ".mp4" || ext == ".mov") {
		if (video.load(path.string())) {
			videoTitle = filename;
			video.setLoopState(OF_LOOP_NORMAL);
			video.play();
			bIsPaused = false;
			return true;
		}
		else {
			videoTitle = "ERROR: " + filename;
		}
	}
	return false;
}

//--------------------------------------------------------------
void ofApp::loadVideoWithDialog()
{
	auto result = ofSystemLoadDialog("load a video (mp4, mov)");
	if (result.bSuccess) {
		loadVideo(result.getPath());
	}
}

void ofApp::resetVideoDims()
{
	vidPos = glm::vec2{ 0,0 };
	if (video.isLoaded()) {
		vidSize = glm::vec2(video.getWidth(), video.getHeight());
	}
	else {
		vidSize = glm::vec2(256, 512);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (key == 'g' || key == 'G') {
		bShowGui = !bShowGui;
		if (!bShowGui) {
			instructionsStartTime = ofGetElapsedTimef();
		}
	}
	else if (key == ' ' && video.isLoaded()) {
		if (bIsPaused) {
			video.play();
		}
		else {
			video.setPaused(true);
		}
		bIsPaused = !bIsPaused;
	}
	else if (key == 'f') {
		ofToggleFullscreen();
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	if (bShowGui && video.isLoaded()) {
		if (progressBar.inside(x, y)) {
			float pct = (x - progressBar.x) / progressBar.width;
			video.setPosition(pct);
		}
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	if (bShowGui && video.isLoaded()) {
		if (progressBar.inside(x, y)) {
			float pct = (x - progressBar.x) / progressBar.width;
			video.setPosition(pct);
		}
	}
	instructionsStartTime = ofGetElapsedTimef();
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
	for (auto& file : dragInfo.files) {
		if (loadVideo(file)) break;
	}
}
