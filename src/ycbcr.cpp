#include "ycbcr.h"
using cv::Mat;
using cv::Vec3b;

void rgb2ycbcr(const cv::Mat& image,cv::Mat& result){
    int i,j;
    result = image.clone();
    for( j=0; j<image.rows; j++){
        for( i=0; i<image.cols; i++){
            result.at<Vec3b>(j,i)[0] =  16+
                                        image.at<Vec3b>(j,i)[2]*0.257+
                                        image.at<Vec3b>(j,i)[1]*0.564+
                                        image.at<Vec3b>(j,i)[0]*0.098;
            result.at<Vec3b>(j,i)[1] =  128+
                                        image.at<Vec3b>(j,i)[2]*-0.148-
                                        image.at<Vec3b>(j,i)[1]*0.291+
                                        image.at<Vec3b>(j,i)[0]*0.439;
            result.at<Vec3b>(j,i)[2] =  128+
                                        image.at<Vec3b>(j,i)[2]*0.439-
                                        image.at<Vec3b>(j,i)[1]*0.368-
                                        image.at<Vec3b>(j,i)[0]*0.071;
        }
    }

}
void markSkinBlock(cv::Mat& image){
    int i,j,x,y,counter;
    const int block = 100;
    const int countValue = block*block/100;

    int cb,cr;
    const int avg_cb = 114;
    const int avg_cr = 155;
    const int skinRange = 9;
    Mat image_;
    rgb2ycbcr(image,image_);
    int xMax,yMax;

    for( y=0; y<image_.rows+block;y +=block){
        for( x=0; x<image_.cols+block; x+=block){
            counter = 0;
            xMax = x+block;
            yMax = y+block;
            if(xMax > image_.cols) xMax = image_.cols;
            if(yMax > image_.rows) yMax = image_.rows;

            for( j=y; j<yMax; j++){
                for( i=x; i<xMax; i++){
                    cb = image_.at<Vec3b>(j,i)[1];
                    cr = image_.at<Vec3b>(j,i)[2];
                    if( (cb > avg_cb-skinRange && cb < avg_cb+skinRange)&&
                        (cr > avg_cr-skinRange && cr < avg_cr+skinRange)
                        ){
                        counter++;
                    }
                }
            }

            if(counter <= countValue){
                for( j=y; j<yMax; j++){
                    for( i=x; i<xMax; i++){
                        image.at<Vec3b>(j,i)[2]=255;
                        image.at<Vec3b>(j,i)[1]=255;
                        image.at<Vec3b>(j,i)[0]=255;
                    }
                }
            }

        }
    }

}
void markSkin(cv::Mat& image){
    int i,j;
    int cb,cr;
    const int avg_cb = 114;
    const int avg_cr = 155;
    const int skinRange = 9;

    for( j=0; j<image.rows; j++){
        for( i=0; i<image.cols; i++){
            cb = image.at<Vec3b>(j,i)[1];
            cr = image.at<Vec3b>(j,i)[2];
            if( (cb > avg_cb-skinRange && cb < avg_cb+skinRange)&&
                (cr > avg_cr-skinRange && cr < avg_cr+skinRange)
               ){

                image.at<Vec3b>(j,i)[2]=255;
                image.at<Vec3b>(j,i)[1]=255;
                image.at<Vec3b>(j,i)[0]=255;
            }
            else{
                image.at<Vec3b>(j,i)[2]=0;
                image.at<Vec3b>(j,i)[1]=0;
                image.at<Vec3b>(j,i)[0]=0;
            }
        }
    }

}


