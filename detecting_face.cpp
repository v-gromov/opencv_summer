#include "detecting_face.h"
#include "opencv2/face.hpp"
#include "opencv2/core/core.hpp"
#include <QDebug>
using namespace cv::face;

Ptr <BasicFaceRecognizer> model;

int face_model:: get_numb_people(Mat Img)
{
    Size ImgSize = Img.size();
    Mat src = Img;//src image
    Mat dst;
    Mat dst2;
    double sclareOX = 92./ImgSize.width;
    double sclareOY = 112./ImgSize.height;
    if(sclareOX<sclareOY)//значит больше ширина и мы уменьшаем до нужной ширины пропорционально
    {
        resize(src,dst,Size(0, 0), 92./ImgSize.width, 92./ImgSize.width, INTER_LINEAR );//resize image

        int diff = (112 - dst.size().height)/2;
        int diff2 = dst.size().height - diff;
        qDebug()<<diff;
        qDebug()<<diff2;

        Rect reg(0, diff, 92, diff2);
        dst(reg).copyTo(dst2);
    }
    else
    {
        //resize(src,dst,Size(0, 112));//resize image
        resize(src,dst,Size(0, 0), 112./ImgSize.height, 112./ImgSize.height, INTER_LINEAR );//resize image
    }
    //Mat dst1(dst);
    //Size size(92,112);//the dst image size,e.g.92x112
    //resize(src,dst,size);//resize image
    imshow( "Display window", dst2);

    //to nado snizu
    //sscvtColor(dst, dst,  CV_RGB2GRAY);//Выставляем нужный колор
    return 1;//model->predict(dst);
}

void face_model::read_csv(const string& filename, vector<Mat>& images, vector<int>& labels)
{
    char separator = ';';
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(Error::StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

face_model::face_model()
{
    string fn_csv = "/home/vgromov/tmp/testcv/test.csv";
    vector<Mat> images;
    vector<int> labels;
    try {
        read_csv(fn_csv, images, labels);
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
        return;
    }
    if(images.size() <= 1) {
        string error_message = "Error in data. No image for work!!";
        CV_Error(Error::StsError, error_message);
    }
    model = createEigenFaceRecognizer();
    model->train(images, labels);
}

Mat face_model::norm_0_255(InputArray _src) {
    Mat src = _src.getMat();
    // Create and return normalized image:
    Mat dst;
    switch(src.channels()) {
    case 1:
        cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
        break;
    case 3:
        cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
}

