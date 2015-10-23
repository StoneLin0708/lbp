#include "lbpFeatureSvm.h"

using namespace std;
using namespace cv;
void detectSingleScale(lbpFeatureSvm& svm,cv::Mat& image){
    int i;
    //int times = 0;

    vector <object>found;

    Rect area;
    lbpFeatureEvaluator evaluator;
    Mat testImage = image.clone();
    int delta = 8;

    cout << "image w = " << image.cols << " h = " << image.rows << endl;

    evaluator.loadImage( testImage ,true);
    svm.testMap(evaluator,found,delta,1);


    int number = (int)found.size();
    Mat save,temp = image.clone();
    Rect foundRect;
    cout << " detect : " << number <<endl;
    //char path[MAX_PATH];
    for( i=0; i < number; ++i){

        foundRect = Rect( found[i].x,
                          found[i].y,
                          trainW+1,
                          trainH+1
                        );
        /*
        sprintf(path,"test_pos\\%3d.jpg",i);
        save = temp( foundRect );
        resize(save,save, Size(trainW+2,trainH+2) );
        imwrite(path,save);
        */

        rectangle(image,foundRect,Scalar( CV_RGB( 60, 200,0 ) ),1,8,0);

    }

    found.clear();
}
