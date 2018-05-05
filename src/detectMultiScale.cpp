#include "lbpFeatureSvm.h"
#include <math.h>
using namespace std;
using namespace cv;

int detectMultiScale(lbpFeatureSvm& svm,cv::Mat& image,double scale = 1.1,int min_width = 30,int max_width = 100){
    int i;
    int times = 0;
    const int deltaO = 10,deltaM = 5;
    double scale_now = 1;
    vector <object>found;
    lbpFeatureEvaluator evaluator;
    Mat testImage = image.clone();
    int delta;
    cout << "image w = " << image.cols << " h = " << image.rows << endl;

    while( min_width >= (trainW+2) * pow( scale ,times ) )
        ++times;

    scale_now = pow(scale,times);
    resize(image,testImage,Size(image.cols/scale_now,image.rows/scale_now));
    delta = deltaO/scale_now;

    while( scale_now*trainW < max_width ){

        evaluator.loadImage( testImage ,true);
        svm.testMap(evaluator,found,delta,scale_now);

        //cout << "scale : " << setw(9) <<scale_now << "  find : " <<found.size() << endl;

        if( testImage.cols < trainW || testImage.rows < trainH ) break;
        //break;
        ++times;
        scale_now = pow(scale,times);

        resize(image,testImage,Size(image.cols/scale_now,image.rows/scale_now));
        delta = deltaO/scale_now;
        if(delta < deltaM) delta = deltaM;
    }

    int number = (int)found.size();
    Mat save,temp = image.clone();
    Rect foundRect;

    cout << " detect : " << number <<endl;
    char path[MAX_PATH];
    for( i=0; i < number; ++i){

        foundRect = Rect( found[i].x*found[i].scale,
                          found[i].y*found[i].scale,
                          (trainW+1)*found[i].scale,
                          (trainH+1)*found[i].scale
                        );

        sprintf(path,"test_pos/%03d.jpg",i);
        save = temp( foundRect );
        resize(save,save, Size(trainW+2,trainH+2) );
        imwrite(path,save);

        rectangle(image,foundRect,
                  Scalar( CV_RGB( 60*found[i].scale, 200/found[i].scale,0 ) ),1,8,0);

    }

    found.clear();
    return number;
}

//old
/*
void detectMultiScale(lbpFeatureSvm& svm,cv::Mat& image){
    int i;
    int x,y;
    int xMax,yMax;
    int delta = 20;
    const double scale=1.1;
    double scale_now = 1;
    vector <cv::Point2d>found;

    label respones;
    Rect area;
    lbpFeatureEvaluator evaluator;
    Mat testImage;

    while(1){
        resize(image,testImage,Size(image.cols/scale_now,image.rows/scale_now));

        if( testImage.cols < trainW || testImage.rows < trainH ) break;
        //imshow("test",testImage);

        evaluator.loadImage( testImage ,true);
        //evaluator.feature.drawLBP("testing");
        //waitKey(0);
        svm.setMap( evaluator.feature.LBP, evaluator.feature.height, evaluator.feature.width );
        //cout << " xmax : " << xMax << " ymax : " << yMax << endl;
        xMax = evaluator.feature.width - trainW;
        yMax = evaluator.feature.height - trainH;

        for( y=0; y<=yMax; y+=delta){
            for( x=0; x<=xMax; x+=delta){
                area =  Rect(x,y,trainW,trainH);
                respones = svm.testMap( area );
                if(respones == label_pos) {
                    found.push_back( Point2d( (int)( (double)x*scale_now), (int)( (double)y*scale_now) ) );
                }
                //cout << "x = " << x <<endl;
            }
            //cout << "y = " << y <<endl;
        }
        //cout << "scale : " << setw(9) <<scale_now << "  find : " <<found.size() << endl;
        scale_now = scale_now*scale;
    }

    int number = (int)found.size();

    cout << " detect : " << number <<endl;

    for( i=0; i < number; i++){
        circle(image,Point2d( found[i].x + trainW/2,found[i].y+trainH/2)   ,20,Scalar( CV_RGB( 255, 20, 20) ),1,8,0 );
    }

    found.clear();
}
*/
