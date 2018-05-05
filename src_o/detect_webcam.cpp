#include "lbpFeatureSvm.h"
using namespace std;
using namespace cv;
int detectMultiScale(lbpFeatureSvm& svm,cv::Mat& image,double scale = 1.1,int min_width = 30,int max_width = 100);

int main(int argc,const char* argv[]){

    VideoCapture cap(0);

    Mat test;
    lbpFeatureSvm svm;
    //svm.load("test\\train3P0N4.xml");
    svm.load("test\\trainTP2N6.xml");
    double t1,t2,scale = 0.6;
    int i;
    Mat img_;

    while(1){
        if( !cap.read(test) ) break;

        for ( i = 1; i < 3; i = i + 2 )
            bilateralFilter ( test, img_, i, i*2, i/2 );
        resize(img_,img_,Size(img_.cols*scale,img_.rows*scale) );

        //resize(test,img_,Size(test.cols*scale,test.rows*scale) );

        t1 = getTickCount();
        detectMultiScale(svm,img_,1.1,60,100);
        t2 = getTickCount();

        system("cls");
        cout << "time cost : " << (t2-t1)/getTickFrequency() << " sec" <<endl;
        resize(img_,img_,Size(img_.cols/scale,img_.rows/scale) );
        imshow("resoult",img_);
        //if(waitKey(0) == 27) break;
        if(waitKey(30) == 27) break;
    }

return 0;
}
