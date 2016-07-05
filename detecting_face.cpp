#include "detecting_face.h"

int face_model:: get_numb_people(QImage qImg)
{
    return model->predict(qimage2mat(qImg));
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

//Дублируется говнокод. Надо правильно подцепить find_face.h
Mat face_model::qimage2mat(const QImage& qimage) {
    cv::Mat mat = cv::Mat(qimage.height(), qimage.width(), CV_8UC3, (uchar*)qimage.bits(), qimage.bytesPerLine());
    cv::Mat mat2 = cv::Mat(mat.rows, mat.cols, CV_8UC3 );
    int from_to[] = { 0,0,  1,1,  2,2 };
    cv::mixChannels( &mat, 1, &mat2, 1, from_to, 3 );
    return mat2;
}

