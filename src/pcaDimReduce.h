#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/ml/ml.hpp>

class pcaDimReduce{
public:
    //pcaDimReduce();
    //~pcaDimReduce();
    void calculate(cv::Mat& data,int k);

    bool save(const char* path);
    bool load(const char* path);


    cv::Mat transMatrix;

    //cv::Mat eigenvectors;
    //cv::Mat eigenvalues;
};
