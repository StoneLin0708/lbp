#include "lbpFeatureSvm.h"
using namespace std;
using namespace cv;
int main(int argc,const char* argv[]){
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
