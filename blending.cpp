#include <vector>
#include <iostream>
#include "blending.h"

using namespace std;

Image blending(Image img1, Image img2, ImageCorners image1, ImageCorners image2) {

	int width = image1.TR.x - image2.TL.x;
	int height = abs(image1.TR.y - image1.BR.y);

	Image mask(width, height);
	//build mask for blending- filling the mask from black to white gradually.
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int intensity = static_cast<int>((255.0 * i) / (width - 1));
			for (int k = 0; k < mask.channels; k++) {
				mask.setPixelCH(j, i, k, intensity);
			}
		}
	}
	//create a new image for storing the full image.
	Image blendImage(img1.width + image2.TR.x - image1.TR.x, image1.BL.y - image1.TL.y);
	//storing the left part of the first image that doesn't apear in the second image.
	for (int x = 0; x < image1.BL.y; x++) {
		for (int y = 0; y < image2.TL.x * img2.channels; y++) {
			blendImage.data[x * blendImage.width * blendImage.channels + y] = img1.data[x * img1.width * img1.channels + y];
		}
	}
	//storing the right part of the second image that doesn't apear in the first image.
	for (int x = 0; x < image1.BL.y; x++) {
		for (int y = 0; y < img2.width * img2.channels - width; y++) {
			blendImage.data[blendImage.width * x * blendImage.channels + image2.TL.x * img2.channels + width + y] = img2.data[x * img2.width * img2.channels + width + y];
		}
	}
	//storing the overlapping parts from both images depending on the mask.
	for (int x = 0; x < height; x++) {
		for (int y = 0; y < width; y++) {
			//conversion of the values in the mask to a range of numbers between 0 and 1
			float M = mask.data[x * width * mask.channels + y] / 255.0f;
			float weight1 = 1.0f - M;
			float weight2 = M;
			blendImage.data[blendImage.width * blendImage.channels * x + image2.TL.x * img2.channels + y] =
			weight2 * img2.data[x * img2.width * img2.channels + y] +
			weight1 * img1.data[x * img1.width * img1.channels + image2.TL.x * img2.channels + y];
		}
	}
	Image::writeImage("blending.BMP", blendImage);
	return blendImage;
}

int main() {

	Image img1 = Image::readImage("1.BMP");
	Image img2 = Image::readImage("2.BMP");
	ImageCorners imgCorner1(Point(0, 0), Point(1499, 0), Point(0, 1527), Point(1499, 1527));
	ImageCorners imgCorner2(Point(1000, 0), Point(2707, 0), Point(1000, 1527), Point(2707, 1527));
	Image b=blending(img1, img2, imgCorner1, imgCorner2);
	cout << "success";
	return 0;
}