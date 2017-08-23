#ifndef HELPERS_H
#define HELPERS_H

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <dirent.h>
#include <string>
#include <iostream>

using namespace cv;
using namespace std;

class Helpers {

public:

    // Gets list of images from folder
    static vector<string> findimages(string path);

    // Function that will threshold an image
    static Mat preprocess(Mat img);

    // Gets the circle in an image
    static pair<Point2f,float> findsun(Mat img, bool display);

    // Saves the image to file
    static void saveimage(size_t ct, Mat img, int x, int y, int width, int height);

};


#endif //HELPERS_H