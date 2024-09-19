# Drone Project - Photography and Panoramic Map Creation

This project was done under **Extra-Tech**.  
Extra-Tech is a company that helps students integrate into the industry and develop projects for high-tech companies.

## Project Description

The project involved creating a drone system that is sent on photography missions to produce a panoramic map.  
The drone system is managed by a master drone, which maps the area and identifies risk factors.

### Development Process

The development was done by teams focused on: mapping, drones, communication, encryption, and memory.  
I was involved in the development of image processing and the actual stitching of aligned images.

## Project Objective

The goal is to match points of interest between two different images to allow for accurate image stitching.

## Implementation

Initially, we researched what points of interest are and how to find matches using descriptors.  
For efficient algorithm implementation, we used the FLANN algorithm to search for the nearest neighbors, based on a hash table.

### Main Steps:

1. Fill each cell in the table with `descriptor1` points.
2. Iterate over `descriptor2` and for each key point, search in the table for the neighbor with the smallest Hamming distance.
3. For each point, store the K closest neighbors using the `addPoint` function.
4. Create a vector of matches with the distance between them.

### Infrastructure

The code was built and compiled using CMake, so it can easily run on different systems such as Linux and Windows.

## Filtering Matches

Some of the matches obtained are inaccurate and may cause distortions when stitching the images.  
Therefore, we apply filtering to get only the most accurate matches:

1. Perform a bidirectional check to ensure that matches between A -> B and B <- A are equal, and remove those that aren't.
2. Filter matches with large distances between K nearest neighbors (ratio test).

## Tests

To verify the correctness of the code, we created a test that calls the OpenCV function and checks whether all the matches found by our code are also present in their matches.

### For Image Stitching

**Objective**: Stitch two aligned images in such a way that the seam between them is not visible.  
**Implementation**: We created a mask that transitions from black to white gradually over the overlap area of the images.  
First, we copied the unique parts of each image into the final stitched image. In the overlapping area, we calculated the pixel values using the mask: we multiplied each pixel from the first image by a certain percentage of the mask, and the second image by the complementary percentage to 1.

### Tests

We verified the correctness of the code by stitching a black image and a white image, ensuring correctness due to the stark contrast between them.
