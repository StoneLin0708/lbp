#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "pcaDimReduce.h"

using namespace std;
using namespace cv;
/*
pcaDimReduce::pcaDimReduce(){

}
*/
void pcaDimReduce::calculate(cv::Mat& data,int k){
    PCA pca(data,Mat(),CV_PCA_DATA_AS_ROW);

    transMatrix = pca.eigenvectors( Rect(0,0, pca.eigenvectors.cols, k) );
    transMatrix = transMatrix.t();
}
bool pcaDimReduce::save(const char* path){
    FileStorage fout(path,FileStorage::WRITE);
    if( !fout.isOpened() ) return false;
    fout << "transMatrix" << transMatrix;
    fout.release();
    return true;
}
bool pcaDimReduce::load(const char* path){
    FileStorage fin(path,FileStorage::READ);
    if( !fin.isOpened() ) return false;
    fin["transMatrix"] >> transMatrix;
    fin.release();
    return true;
}
