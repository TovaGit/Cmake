#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <stdint.h>
#include <any>
#include <map>
#include "HashTable.h"
#include <algorithm>

// for each point from what img and the poin distance distance
struct PointMatch
{
	int queryIdx;
	int trainIdx;
	float distance;
};
class MatchFeaturers :public HashTable
{
private:
	std::vector<std::vector<int>> descriptors_1;
	std::vector<std::vector<int>> descriptors_2;
	std::vector<std::vector<PointMatch>> knn_matches;
public:
	std::vector<PointMatch>good_matches;
	std::vector<std::vector<PointMatch>> knnMatch(const std::vector<std::vector<int>>& descriptors1, const std::vector<std::vector<int>>& descriptors2, int k);
	void knnSearch(const std::vector<std::vector<int>>& descriptors1, const std::vector<std::vector<int>>& descriptors2, std::vector<std::vector<int>>& indices, std::vector<std::vector<float>>& dists, int knn);
	std::vector<PointMatch> matchFilter(const std::vector<std::vector<PointMatch>>& knn_matches);
	void findNeighbors(const std::vector<int>& vec, ResultSet& result, const std::vector<std::vector<int>>& descriptors2);
	std::vector<std::pair<std::vector<int>, std::vector<int>>>setForHomorgraphy(std::vector<PointMatch>good_matches);
};
