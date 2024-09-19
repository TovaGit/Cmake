#include <opencv2/opencv.hpp>
#include "matchFeatures.h"
using namespace cv;
using namespace std;

#include <iostream>
#ifdef _DEBUG
// the debug version that corresponds to the opencv_world420d.dll:
#pragma comment(lib, "opencv_world4100d.lib")
#else
// the release version that corresponds to the opencv_world420.dll:
#pragma comment(lib, "opencv_world4100.lib")
#endif
void detectAndComputeFeatures(const Mat& img, std::vector<KeyPoint>& keypoints, Mat& descriptors) {
    // Create ORB object
    Ptr<ORB> orb = ORB::create();
    // Detect keypoints and compute descriptors
    orb->detectAndCompute(img, noArray(), keypoints, descriptors);
}
std::vector<std::vector<int>> matToVector(const cv::Mat& mat) {
    std::vector<std::vector<int>> vec(mat.rows, std::vector<int>(mat.cols));
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            vec[i][j] = static_cast<int>(*(mat.ptr(i, j)));
        }
    }
    return vec;
}
int main() {

    Mat img1 = imread("C:\\Users\\The user\\Downloads\\image_1.png", IMREAD_GRAYSCALE);
    if (img1.empty()) {
        std::cout << "Could not open or find image1.png!" << std::endl;
        return -1;
    }
    resize(img1, img1, img1.size() / 5);
    // Load the second image
    Mat img2 = imread("C:\\Users\\The user\\Downloads\\image_2.png", IMREAD_GRAYSCALE);
    if (img2.empty()) {
        std::cout << "Could not open or find image2!" << std::endl;
        return -1;
    }
    resize(img2, img2, img2.size() / 5);
    // Vectors to store keypoints for each image
    std::vector<KeyPoint> keypoints1, keypoints2;
    // Matrices to store descriptors for each image
    Mat descriptors1, descriptors2;
    // Detect keypoints and compute descriptors for the first image
    detectAndComputeFeatures(img1, keypoints1, descriptors1);
    // Detect keypoints and compute descriptors for the second image
    detectAndComputeFeatures(img2, keypoints2, descriptors2);
    // Convert cv::Mat to std::vector<std::vector<float>>
    std::vector<std::vector<int>> descriptors1_ = matToVector(descriptors1);
    std::vector<std::vector<int>> descriptors2_ = matToVector(descriptors2);
    //Perform matching
    FlannBasedMatcher matcher(new flann::LshIndexParams(20, 10, 2));
    std::vector<std::vector<DMatch>> knn_matches;
    std::vector<std::vector<DMatch>> s;
    matcher.knnMatch(descriptors2, descriptors1, knn_matches, 2);
    std::vector<DMatch> good;
    for (const auto& matches : knn_matches) {
        if (matches.size() > 1) {
            // Checking the distance ratio between the first match and the second
            if (matches[0].distance < 0.7 * matches[1].distance) {
                good.push_back(matches[0]);
            }
        }
    }
    Mat res;
    drawMatches(img1, keypoints1, img2, keypoints2, good, res);
    imwrite("open cv.jpg", res);
    MatchFeaturers matcher1;
    std::vector<std::vector<PointMatch>> knn_matches_ = matcher1.knnMatch(descriptors1_, descriptors2_, 2);
    matcher1.matchFilter(knn_matches_);
    // return good_dmatches;
    std::vector<DMatch> dmatch;
    for (const auto& match : matcher1.good_matches) {
        dmatch.push_back(DMatch(match.queryIdx, match.trainIdx, match.distance));
    }
    Mat result;
    drawMatches(img1, keypoints1, img2, keypoints2, dmatch, result);
    imshow("Matches", result);
    imwrite("Matches.jpg", result);
    waitKey(0);
    destroyAllWindows();
    return 0;
}
