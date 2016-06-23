#ifndef CROP_IMAGE_H
#define CROP_IMAGE_H

#include "mainwindow.h"
QImage CropFace(QImage a, int left_x, int left_y, int right_x, int right_y, float offset_pct_1, float offset_pct_2, int dest_sz_x, int dest_sz_y);
QImage ScaleRotateTranclate(QImage, float , double* , int , float);

#endif // CROP_IMAGE_H
