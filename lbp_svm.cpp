#include "lbpFeatureSvm.h"
#include "ycbcr.h"
#include <string>
/*struct trainingSetting{
	int trainImageH = 46;
	int trainImageW = 38;

	int feature0 = 17;
	int feature1 = 31;
	int feature2 = 112;
	int feature3 = 225;
	int feature4 = 240;
	int feature5 = 241;
	int feature6 = 249;
	int feature7 = 255;

	int featureNumber = 256;
	int featureNumberReduce = 100;

	int trainH = 46;
	int trainW = 38;
}
*/
void detectSingleScale(lbpFeatureSvm& svm,cv::Mat& image);
int detectMultiScale(lbpFeatureSvm& svm,cv::Mat& image,double scale = 1.1,int min_width=30,int max_width=100);

int test(int argc,const char* argv[]);
int detect(int argc,const char* argv[]);
int train(int argc,const char* argv[]);


using namespace std;
using namespace cv;

int main(int argc,const char* argv[]){
	string arg1(argv[1]);
	string s_test("test");
	string s_detect("detect");
	string s_train("train");
	string s_help("help");

	if(arg1==s_test){
		test(argc,argv);
	}
	else if(arg1==s_detect){
	}
	else if(arg1==s_train){
	}
	else if(arg1==s_help){
		cout<< "  [function] [operation]"<< endl;
		cout<< "   test arga argb argc"<< endl;
		cout<< "   detect arga argb argc"<< endl;
		cout<< "   train arga argb argc"<< endl;
	}
	else{
		cout<< " there is no function called:"<< arg1<< endl;
		cout<< " call help for more infomation"<< endl;
	}

	return 0;
}
int test(int argc,const char* argv[]){

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
}

int detect(int argc,const char* argv[]){

    if(argc != 3 ) return 1;

    Mat test = imread( argv[2] );
    if(!test.data) return 2;
    lbpFeatureSvm svm;
    svm.load( argv[1] ,argv[2]);

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


int train(int argc,const char* argv[]){
    lbpFeatureSvm lbpsSvm;
    double t0,t1;

    char pos[PATH_MAX] = {"D:\\coding\\lbp_svm\\test\\pos2.txt"};
    char neg[PATH_MAX] = {"D:\\coding\\lbp_svm\\test\\list_neg7.txt"};
    char out[PATH_MAX] = {"D:\\coding\\lbp_svm\\test\\trainTP2N7_PCA.xml"};
    char outPca[PATH_MAX] = {"D:\\coding\\lbp_svm\\test\\trainTP2N7_matrix.txt"};

    if( !lbpsSvm.loadPosSamples(pos) )return 1;
    if( !lbpsSvm.loadNegSamples(neg) ) return 2;

    CvSVMParams params;

    params.svm_type = CvSVM::NU_SVC;
    //params.svm_type = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    //params.kernel_type = CvSVM::RBF;
    params.degree = 0; // for poly
    params.gamma = 0;  // for poly/rbf/sigmoid
    params.coef0 = 0;  // for poly/sigmoid

    params.C = 0.019;  // for CV_SVM_C_SVC, CV_SVM_EPS_SVR and CV_SVM_NU_SVR
    params.nu = 0.15; // for CV_SVM_NU_SVC, CV_SVM_ONE_CLASS, and CV_SVM_NU_SVR
    params.p = 0; // for CV_SVM_EPS_SVR
    //CvMat*      class_weights; // for CV_SVM_C_SVC

    params.term_crit =  // termination criteria
        //cvTermCriteria( CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
        cvTermCriteria(CV_TERMCRIT_EPS, 1000, 1e-7/*DBL_EPSILON*/);
        //cvTermCriteria(CV_TERMCRIT_ITER, 90, 1e-8/*DBL_EPSILON*/);
    t0 = getTickCount();
    //lbpsSvm.train(out,params);
    lbpsSvm.trainP(out,outPca,params);
    t1 = getTickCount();
    cout << "training end in : " << ( (t1-t0)/getTickFrequency() ) << " sec"<<endl;
    /*if(argc != 4){
        cout << "[positive sample list][negative samples list] [train file output]" <<endl;
        return 1;
    }
    if( !lbpsSvm.loadPosSamples(argv[1]) )return 2;
    if( !lbpsSvm.loadNegSamples(argv[2]) ) return 3;
    lbpsSvm.train(argv[3]);*/

return 0;
}
