#pragma once
#include "C:\\Users\\win\\Pictures\\blending\\MultiDrone-image_format\\Image.cpp"
struct Point {
	Point() {};
	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}
	int x;
	int y;
};
struct ImageCorners {
	ImageCorners(Point TL, Point TR, Point BL, Point BR) {
		this->TL.x = TL.x;
		this->TR.x = TR.x;
		this->BL.x = BL.x;
		this->BR.x = BR.x;
		this->TL.y = TL.y;
		this->TR.y = TR.y;
		this->BL.y = BL.y;
		this->BR.y = BR.y;
	}
	Point TL;
	Point TR;
	Point BL;
	Point BR;
};

Image blending(Image img1, Image img2, ImageCorners image1, ImageCorners image2);