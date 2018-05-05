#include "lbpFeatureSvm.h"
#include "ycbcr.h"

using namespace std;
using namespace cv;

void detectSingleScale(lbpFeatureSvm& svm,cv::Mat& image);
int detectMultiScale(lbpFeatureSvm& svm,cv::Mat& image,double scale = 1.1,int min_width=30,int max_width=100);

int main(int argc,const char* argv[]){

    lbpFeatureSvm svm;

    double t0,t1;
    double scale = 1;
    int found;
    char testData[PATH_MAX] = "test/001.jpg";
    Mat test = imread(testData);

    if(!test.data){
	cout << "no test data : " << testData << endl;
	return 2;
    }

    resize(test,test,Size(test.cols*scale,test.rows*scale) );
    Mat test2 = test.clone();
    //imshow("test",test);
    char resultName[PATH_MAX];
    char trainPath[PATH_MAX] = { "test/" };
    char trainFile[PATH_MAX] = { //"train3P1N3.xml"
                                 //"trainN3.xml"
                                 //"train3P0N3.xml"
                                 //"train3P0N5.xml"
                                 //"trainTP2N7.xml"
                                 "trainTP2N7_PCA.xml"
                                 };
    char pcaPath[PATH_MAX] = { "test/trainTP2N7_matrix.txt" };
    strcat(trainPath,trainFile);
    svm.load(trainPath,pcaPath);

    t0 = getTickCount();
    found = detectMultiScale(svm,test,1.1,40,60);
    t1 = getTickCount();
    cout << "time cost : " << (t1-t0)*1000/getTickFrequency() << " ms" <<endl;

    t0 = getTickCount();
    markSkinBlock(test2);
    t1 = getTickCount();
    cout << "mark time cost : " << (t1-t0)*1000/getTickFrequency() << " ms" <<endl;

    t0 = getTickCount();
    found = detectMultiScale(svm,test2,1.1,40,60);
    t1 = getTickCount();
    cout << "test2 time cost : " << (t1-t0)*1000/getTickFrequency() << " ms" <<endl;

    resize(test2,test2,Size(test.cols*0.8,test.rows*0.8) );
    imshow("result2",test2);


    trainFile[ strlen(trainFile)-4 ] = 0;
    sprintf(resultName,"./result/t_%s_%d.jpg",trainFile,found);
    cout << resultName << endl;
    imwrite(resultName,test);

    resize(test,test,Size(test.cols*0.8,test.rows*0.8) );
    imshow("result",test);

    while(waitKey(30) != 27);
return 0;
}
