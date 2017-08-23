#include "helpers.h"


using namespace std;
using namespace cv;


/**
 * Given a directory and a fileending this will find all files that match
 * This allows us to load images later from multiple different files or directories
 */
vector<string> Helpers::findimages(string path) {

    // Directory objects
    DIR *dpdf;
    struct dirent *epdf;

    // Get files from the directory
    std::vector<std::string> paths;
    dpdf = opendir(path.c_str());
    if (dpdf != NULL) {
        while (epdf = readdir(dpdf)) {
            paths.push_back(path+std::string(epdf->d_name));
        }
    }
    else {
        cerr << "Unable to open the directory specified..." << endl;
    }
    closedir(dpdf);

    // Sort the files and return
    std::sort(paths.begin(), paths.end());
    return paths;
}


/**
 * This will take an incoming image and apply any need preprocessing to it
 * Right now we just inverse threshold the incoming image
 */
Mat Helpers::preprocess(Mat img) {
    // Convert image
    Mat gimg;
    cvtColor(img, gimg, CV_BGR2GRAY);
    // Config values (maybe allow these to be changed from cmd??)
    double thresh = 100;
    double maxValue = 255;
    // Threshold
    Mat img_thresh;
    threshold(gimg,img_thresh,thresh,maxValue, THRESH_BINARY_INV);
    return img_thresh;
}



/**
 * Will find the circle of the image that is passed
 */
pair<Point2f,float> Helpers::findsun(Mat img, bool display) {


    // Find contours
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);

    // Remove small and large contours
    // https://stackoverflow.com/a/18108466/7718197
    int contour_length_threshold = 50;
    for (vector<vector<Point> >::iterator it = contours.begin(); it!=contours.end(); ) {
        if (it->size()<contour_length_threshold)
            it=contours.erase(it);
        else
            ++it;
    }

    /// Approximate contours to polygons + get bounding rects and circles
    vector<vector<Point> > contours_poly(contours.size());
    vector<Point2f> center( contours.size());
    vector<float> radius(contours.size());
    for( int i = 0; i < contours.size(); i++ ) {
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true);
        minEnclosingCircle(contours_poly[i], center[i], radius[i]);
    }

    // Display if we want to
    if(display) {
        // Draw contours
        RNG rng(12345);
        // Find the minimun enclosing circle
        for(int i = 0; i< contours.size(); i++ ) {
            Scalar color = Scalar(rng.uniform(100, 255), rng.uniform(100,255), rng.uniform(100,255));
            drawContours(img, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
            circle(img, center[i], (int)radius[i], color, 2, 8, 0 );
        }
        // Display
        namedWindow("results", WINDOW_FREERATIO);
        imshow("results", img);
        waitKey(0);
    }


    // Debug
    cout << "found " << contours.size() << " contours" << endl;

    // Return zero if we have not found one
    if(contours.size() < 2) {
        return make_pair(Point2f(),0.0f);
    }

    // TODO: Find the sun circle
    Point2f origin = center[1];
    float rad = radius[1];
    return make_pair(origin,rad);

}







/**
 * Given the image bounds this will save the passed image to file
 * TODO: Make a directory to save the images into too
 */
void Helpers::saveimage(size_t ct, Mat img, int x, int y, int width, int height) {
    // Crop the image
    Mat cimg = img(Rect(x,y,width,height));
    // Display
    namedWindow("results", WINDOW_FREERATIO);
    imshow("results", cimg);
    waitKey(1);
    // Save to file
    string file = "../data_out/image_"+std::to_string(ct)+".jpg";
    imwrite(file,cimg);
}





