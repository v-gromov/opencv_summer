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
    face_model();
    int get_numb_people(Mat);
private:
    void read_csv(const string&, vector<Mat>&, vector<int>&);
    Mat norm_0_255(InputArray);
};

#endif // DETECTING_FACE_H
