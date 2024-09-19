#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <opencv2/opencv.hpp>
#include "doctest.h"
//#include "FlannMatcher.cpp"
#include <iostream>
#include "matchFeatures.h"
#ifdef _DEBUG
 //the debug version that corresponds to the opencv_world420d.dll:
#pragma comment(lib, "opencv_world4100d.lib")
#else
 //the release version that corresponds to the opencv_world420.dll:
#pragma comment(lib, "opencv_world4100.lib")
#endif
using namespace cv::flann;
using namespace cv;
using namespace std;

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
TEST_CASE("input is not empty") {

   Mat img1 = imread("C:\\Users\\win\\Pictures\\פרקטיקום\\pictures\\1.jpg", IMREAD_GRAYSCALE);
   Mat img2 = imread("C:\\Users\\win\\Pictures\\פרקטיקום\\pictures\\2.jpg", IMREAD_GRAYSCALE);
   std::vector<KeyPoint> keypoints1, keypoints2;
   Mat descriptors1, descriptors2;
   //find key point 
   detectAndComputeFeatures(img1, keypoints1, descriptors1);
   detectAndComputeFeatures(img2, keypoints2, descriptors2);
   CHECK_FALSE(keypoints1.empty());

   CHECK(keypoints2.empty() == false);

}
TEST_CASE("check Input for lsh table") {
    int table_number = 10;
    int key_size = 5;
    int multi_probe_level = 2;
    cout << "check Input for lsh table\n";
    CHECK_NE(table_number, 0);
    cout << "table number could not be 0\n";
    FlannBasedMatcher matcher(new flann::LshIndexParams(table_number, key_size, multi_probe_level));
}

TEST_CASE("Comparing the results with openCV") {
    for (int i = 1; i < 10; i += 2)
    {
        // Load the first image
        stringstream s;
        s << i << ".jpg";
        cout << i;
        Mat img1 = imread("C:\\Users\\win\\Pictures\\פרקטיקום\\pictures\\" + s.str(), IMREAD_GRAYSCALE);

        if (img1.empty()) {
            std::cout << "Could not open or find image!\n" << std::endl;
        }
        resize(img1, img1, img1.size() / 5);
        // Load the second image
        stringstream s2;
        s2 << i + 1 << ".jpg";
        Mat img2 = imread("C:\\Users\\win\\Pictures\\פרקטיקום\\pictures\\" + s2.str(), IMREAD_GRAYSCALE);
        if (img2.empty()) {
            std::cout << "Could not open or find image!\n" << std::endl;
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
        //cv matching
        matcher.knnMatch(descriptors1, descriptors2, knn_matches, 2);
        std::vector<DMatch> good;
        //filter the cv matching
        for (const auto& matches : knn_matches) {
            if (matches.size() > 1) {
                // Checking the distance ratio between the first match and the second
                if (matches[0].distance < 0.7 * matches[1].distance) {
                    good.push_back(matches[0]);
                }
            }
        }
        MatchFeaturers matcher1;
        std::vector<std::vector<PointMatch>> knn_matches_ = matcher1.knnMatch(descriptors1_, descriptors2_, 2);
        matcher1.matchFilter(knn_matches_);
        // return good_dmatches;
        std::vector<DMatch> dmatch;
        for (const auto& match : matcher1.good_matches) {
            dmatch.push_back(DMatch(match.queryIdx, match.trainIdx, match.distance));
        }
        Mat result;
       //Counts the number of our matches found in matches of openCV
        int count = 0;
        for (auto u : matcher1.good_matches) {
            for (auto p : good) {
                if (u.trainIdx == p.trainIdx && u.queryIdx == p.queryIdx) {
                    count++;
                    break;  
                }
            }
        }
        CHECK(matcher1.good_matches.size() * 0.96 <= count );
    }
}
