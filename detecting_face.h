#ifndef DETECTING_FACE_H
#define DETECTING_FACE_H

#include "opencv2/opencv.hpp"

using namespace cv;

#include <string.h>
#include <vector>
using namespace std;

#include <fstream>
#include <sstream>

class face_model
{
public:
    void train_model(int);
    int get_numb_people(Mat);
private:
    void read_csv(const string&, vector<Mat>&, vector<int>&);
    int recognition_threshold=3000;
public:
    void set_threshold(int val){recognition_threshold = val;}
};

#endif // DETECTING_FACE_H
