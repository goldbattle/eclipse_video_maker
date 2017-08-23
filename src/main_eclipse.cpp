#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include "helpers.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv) {


    // Get list of images in folder
    vector<string> paths = Helpers::findimages("/home/patrick/eclipse_dataset/");

    // Image information
    int imgw = 700;
    int imgh = 700;

    // Loop through each image
    size_t img_ct = 0;
    for(size_t i=0; i<paths.size(); i++) {
        // Read image in
        Mat img_original = imread(paths.at(i), IMREAD_COLOR);
        // Make sure that we have an image
        if(img_original.empty())
            continue;
        // Else we are good, preprocess it
        Mat img_threshold = Helpers::preprocess(img_original);
        // Now lets find the sun
        pair<Point2f,float> sunloc = Helpers::findsun(img_threshold,false);
        // Skip if we do not have a sun location
        if(sunloc.second <= 0)
            continue;
        // Calculate our transforms
        int x = (int)sunloc.first.x - imgw/2;
        int y = (int)sunloc.first.y - imgh/2;
        // Make sure they are bounded
        x = (x < 0)? 0 : x;
        y = (y < 0)? 0 : y;
        x = (x > img_original.cols-imgw)? img_original.cols-imgw : x;
        y = (y > img_original.rows-imgh)? img_original.rows-imgh : y;
        // Save the image
        Helpers::saveimage(img_ct++,img_original,x,y,imgw,imgh);
    }

    //todo: figure out how to save as a video


    // Done!
    return EXIT_SUCCESS;

}
