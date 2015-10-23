#include "lbpFeatureSvm.h"
using namespace std;
using namespace cv;
void detectMultiScale(lbpFeatureSvm& svm,cv::Mat& image);
void detectSingleScale(lbpFeatureSvm& svm,cv::Mat& image);
int main(int argc,const char* argv[]){

    if(argc != 3 ) return 1;

    Mat test = imread( argv[2] );
    if(!test.data) return 2;
    lbpFeatureSvm svm;
    svm.load( argv[1] );

    double t1,t2;
    double scale = 0.8;
    int i;

    Mat image = test.clone() ;
    if(0){
        for ( i = 1; i < 3; i = i + 2 )
            bilateralFilter ( test, image, i, i*2, i/2 );
    }

    resize(image,image,Size(image.cols/scale,image.rows/scale));

    t1 = getTickCount();
    detectMultiScale(svm,image);
    t2 = getTickCount();

    cout << "time cost : " << (t2-t1)/getTickFrequency() << " sec" <<endl;

    imshow("resoult",image);
    imwrite("result.jpg",image);

    while(waitKey(30) != 27);
return 0;
}
