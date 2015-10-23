#include "lbpFeature.h"
#include "multiImg.h"
#include "pcaDimReduce.h"
#define label_pos 1.0f
#define label_neg -1.0f
/*
#define feature0 17
#define feature1 31
#define feature2 112
#define feature3 225
#define feature4 240
#define feature5 241
#define feature6 249
#define feature7 255
*/
#define featureNumber 256
#define featureNumberReduce 100

#define trainH 46
#define trainW 38
#define MAX_PATH 256

typedef float label;

typedef struct object{
    int x;
    int y;
    double scale;
}object;

class lbpFeatureSvm{
public:
    //lbpFeatureSvm(void);
    bool loadPosSamples(const char* path);
    bool loadNegSamples(const char* path);

    bool train(const char* outFilePath,CvSVMParams& params);
    bool trainP(const char* outFilePath,const char* pcaPath,CvSVMParams& params);
    //load svm trained data
    void load(const char* trainFilePath,const char* pcaFilePath);

    void setMap(std::vector<unsigned char>& lbpMap, int rows, int cols) { evaluator.loadMap( lbpMap ,rows,cols); };
    label testImage(cv::Mat& image);

    label testMap(cv::Rect& area);
    int testMap(lbpFeatureEvaluator& feature,std::vector<object>& detected,int delta,double scale);
    int testMapO(lbpFeatureEvaluator& feature,std::vector<object>& detected,int delta,double scale);

private:

    void reduceFeature();
    pcaDimReduce _pca;
    multiImg pos;
    multiImg neg;
    lbpFeatureEvaluator evaluator;
    CvSVM _svm;

};
