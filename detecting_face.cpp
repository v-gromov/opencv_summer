#include "detecting_face.h"
#include "opencv2/face.hpp"
#include "opencv2/core/core.hpp"
#include <QDebug>
using namespace cv::face;

//Ptr <BasicFaceRecognizer> model;
Ptr <FaceRecognizer> model;

Mat img_for_database(Mat Img)
{
    if((Img.size().width>10)&&(Img.size().height>10))
    {
        Size ImgSize = Img.size();
        Mat src = Img;//src image
        Mat dst;

        double sclareOX = 92./ImgSize.width;
        double sclareOY = 112./ImgSize.height;

        if(sclareOX<sclareOY)//значит больше ширина и мы уменьшаем до нужной ширины пропорционально
        {
            resize(src,dst,Size(0, 0), sclareOX, sclareOX, INTER_LINEAR);//resize image
        }
        else//значит больше высота и мы уменьшаем до нужной высоты пропорционально
        {
            resize(src,dst,Size(0, 0), sclareOY, sclareOY, INTER_LINEAR);//resize image
        }
        Mat dst2(112, 92, CV_8UC3);
        Vec3b col;
        col[0] = 0; col[1] = 0; col[2] = 0;
        int detOX = (112 - dst.size().height) / 2;
        int detOY = (92 - dst.size().width) / 2;
        for(int h = 0; h < 112; h++)
        {
            for(int w = 0; w < 92; w++)
                dst2.at<Vec3b>(Point(w,h)) = col;
        }
        for(int h = 0; h < dst.size().height; h++)
        {
            for(int w = 0; w<dst.size().width; w++)
            {
                Vec3b color = dst.at<Vec3b>(Point(w,h));
                dst2.at<Vec3b>(Point(w+detOY,h+detOX)) = color;
            }
        }
        return dst2;
    }
    else
    {
        return Mat(10, 10, DataType<int>::type);
    }
}

int face_model:: get_numb_people(Mat Img)
{
    if((Img.size().width>10)&&(Img.size().height>10))
    {
        Mat img2 = img_for_database(Img);
        cvtColor(img2, img2,  CV_RGB2GRAY);//Выставляем нужный колор
        int predicted_label = -1;
        double predicted_confidence = 0.0;
        model->predict(img2, predicted_label, predicted_confidence);
        if(predicted_confidence< recognition_threshold)
            return predicted_label;
    }
    return -1;
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

void face_model::train_model(int set_model)
{
    string fn_csv = "/home/vgromov/Projects/build-opencv_summer-Desktop-Debug/database.csv";
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
    switch(set_model)
    {
    case 1:
        model = createEigenFaceRecognizer();
        model->train(images, labels);
        break;
    case 2:
        model =  createFisherFaceRecognizer();
       model->train(images, labels);
        break;
    case 3:
        model = createLBPHFaceRecognizer();
        model->train(images, labels);
        break;
    default:
        string error_message = "The method for detection face not selected!";
        CV_Error(Error::StsError, error_message);
        break;
    }
}
