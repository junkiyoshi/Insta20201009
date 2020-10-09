#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(255);

	this->cap.open("D:\\MP4\\Pexels Videos 2921.mp4");
	this->cap_size = cv::Size(1280, 720);

	this->rect_size = 10;
	for (int x = 0; x < this->cap_size.width; x += this->rect_size) {

		for (int y = 0; y < this->cap_size.height; y += this->rect_size) {

			auto image = make_unique<ofImage>();
			image->allocate(this->rect_size, this->rect_size, OF_IMAGE_COLOR);
			cv::Mat frame = cv::Mat(cv::Size(image->getWidth(), image->getHeight()), CV_MAKETYPE(CV_8UC3, image->getPixels().getNumChannels()), image->getPixels().getData(), 0);
			cv::Rect rect = cv::Rect(x, y, this->rect_size, this->rect_size);

			this->rect_images.push_back(move(image));
			this->cv_rects.push_back(rect);
			this->rect_frames.push_back(frame);
			this->frame_number_list.push_back(0);
		}
	}

	this->number_of_frames = this->cap.get(cv::CAP_PROP_FRAME_COUNT);
	for (int i = 0; i < this->number_of_frames; i++) {

		cv::Mat src, tmp;
		this->cap >> src;
		if (src.empty()) {

			return;
		}

		cv::resize(src, tmp, this->cap_size);
		cv::cvtColor(tmp, tmp, cv::COLOR_BGR2RGB);

		this->frame_list.push_back(tmp);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = 0; i < this->rect_images.size(); i++) {

		int tmp = ofMap(ofNoise(this->cv_rects[i].x * 0.0005, this->cv_rects[i].y * 0.0005, ofGetFrameNum() * 0.008), 0, 1, 1, 3);
		this->frame_number_list[i] = (this->frame_number_list[i] + tmp) % this->number_of_frames;
		cv::Mat tmp_box_image(this->frame_list[this->frame_number_list[i]], this->cv_rects[i]);
		tmp_box_image.copyTo(this->rect_frames[i]);

		this->rect_images[i]->update();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	for (int i = 0; i < this->rect_images.size(); i++) {

		this->rect_images[i]->draw(this->cv_rects[i].x, this->cv_rects[i].y);
	}
}

//========================================================================
int main() {

	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}