#include "lbpFeatureSvm.h"

using namespace cv;
using namespace std;

bool lbpFeatureSvm::loadPosSamples(const char *path){
    pos.load(path,false,false);
    cout << " positive sample load : " << pos.size() << endl;

return true;
}

bool lbpFeatureSvm::loadNegSamples(const char *path){
    neg.load(path,false,false);
    cout << " negative sample load : " << neg.size() << endl;

return true;
}

void lbpFeatureSvm::load(const char* trainFilePath,const char* pcaFilePath){
    _svm.load( trainFilePath );
    _pca.load( pcaFilePath );
}

bool lbpFeatureSvm::train(const char* outFilePath,CvSVMParams& params){
    int number = pos.size() + neg.size();
    int i,j;
    Mat labels(number,1,CV_32FC1);

    float* dataPos = (float*)malloc( pos.size()*featureNumber * sizeof(float) );
    float* dataNeg = (float*)malloc( neg.size()*featureNumber * sizeof(float) );
    //generate labels
    for( i = 0; i < pos.size(); i++)
        labels.at<float>(i) = label_pos;
    for( i = pos.size(); i < number;i++)
        labels.at<float>(i) = label_neg;

    //calculate feature
    for( i = 0; i < pos.size(); i++){
        evaluator.loadImage( *pos.read(i) );
        evaluator.countHistogram();
        for(j=0;j<featureNumber;j++)
            *( dataPos + i*featureNumber +j) = evaluator.feature.histogram[j];
    }
    for( i = 0; i < neg.size(); i++){
        evaluator.loadImage( *neg.read(i) );
        evaluator.countHistogram();
        for(j=0;j<featureNumber;j++)
            *( dataNeg + i*featureNumber +j) = evaluator.feature.histogram[j];
    }

    //convert data to mat
    Mat trainData(number , featureNumber , CV_32FC1);
    Mat posData(pos.size(),featureNumber,CV_32FC1,dataPos);
    Mat negData(neg.size(),featureNumber,CV_32FC1,dataNeg);

    for(i=0;i<trainData.rows;i++){
        for(j=0;j<trainData.cols;j++){
            if(i < posData.rows )
                trainData.at<float>(i,j) = posData.at<float>(i,j);
            else
                trainData.at<float>(i,j) = negData.at<float>(i-posData.rows,j);
        }
    }

    //training and save
    CvSVM SVM;
    SVM.train( trainData, labels , Mat(), Mat(), params);
    SVM.save(outFilePath);
    //test result
    cout << "testing result : " << endl;
    Mat testMat(1,featureNumber,CV_32FC1);
    int detected = 0,success;

    //test positive samples
    for( i = 0; i < posData.rows; i++){
        testMat = posData( Rect(0,i,posData.cols,1) );
        if(SVM.predict( testMat ) == label_pos ) detected++;
    }
    cout << " positive sample detected : " << detected << " / " << posData.rows <<endl;
    success = detected;
    //test negative samples
    detected = 0;
    for( i = 0; i < negData.rows; i++){
        testMat = negData( Rect(0,i,negData.cols,1) );
        if(SVM.predict( testMat ) == label_pos ) detected++;
    }
    cout << " negative sample detected : " << detected << " / " << negData.rows <<endl;
    success += negData.rows - detected;
    //cout result
    cout << " success : " << success <<endl
         << " success rate : " << (float)success/ (float)( number ) <<endl;

    free(dataPos);
    free(dataNeg);
return true;
}

bool lbpFeatureSvm::trainP(const char* outFilePath,const char* pcaPath,CvSVMParams& params){

    int number = pos.size() + neg.size();
    int i,j;
    Mat labels(number,1,CV_32FC1);

    float* dataPos = (float*)malloc( pos.size()*featureNumber * sizeof(float) );
    float* dataNeg = (float*)malloc( neg.size()*featureNumber * sizeof(float) );
    //generate labels
    for( i = 0; i < pos.size(); i++){
        labels.at<float>(i) = label_pos;
    }
    for( i = pos.size(); i < number;i++){
        labels.at<float>(i) = label_neg;
    }
    //calculate feature
    for( i = 0; i < pos.size(); i++){
        evaluator.loadImage( *pos.read(i) );
        evaluator.countHistogram();
        for(j=0;j<featureNumber;j++)
            *( dataPos + i*featureNumber +j) = evaluator.feature.histogram[j];
    }

    for( i = 0; i < neg.size(); i++){
        evaluator.loadImage( *neg.read(i) );
        evaluator.countHistogram();
        for(j=0;j<featureNumber;j++)
            *( dataNeg + i*featureNumber +j) = evaluator.feature.histogram[j];
    }

    //convert data to mat
    Mat orgData(number , featureNumber , CV_32FC1);
    Mat trainData(number , featureNumberReduce , CV_32FC1);
    Mat posData(pos.size(),featureNumber,CV_32FC1,dataPos);
    Mat negData(neg.size(),featureNumber,CV_32FC1,dataNeg);

    for(i=0;i<orgData.rows;i++){
        for(j=0;j<orgData.cols;j++){
            if(i < posData.rows )
                orgData.at<float>(i,j) = posData.at<float>(i,j);
            else
                orgData.at<float>(i,j) = negData.at<float>(i-posData.rows,j);
        }
    }

    _pca.calculate(posData,featureNumberReduce);
    _pca.save(pcaPath);
    trainData = orgData*_pca.transMatrix;

    //training and save
    CvSVM SVM;
    SVM.train( trainData, labels , Mat(), Mat(), params);
    SVM.save(outFilePath);
    //test result
    cout << "testing result : " << endl;
    Mat testMat(1,featureNumber,CV_32FC1);
    int detected = 0,success;

    //test positive samples
    for( i = 0; i < posData.rows; i++){
        testMat = posData( Rect(0,i,posData.cols,1) );
        testMat = testMat*_pca.transMatrix;
        if(SVM.predict( testMat ) == label_pos )
            detected++;
    }
    cout << " positive sample detected : " << detected << " / " << posData.rows <<endl;
    success = detected;
    //test negative samples
    detected = 0;
    for( i = 0; i < negData.rows; i++){
        testMat = negData( Rect(0,i,negData.cols,1) );
        testMat = testMat*_pca.transMatrix;
        if(SVM.predict( testMat ) == label_pos )
            detected++;
    }
    cout << " negative sample detected : " << detected << " / " << negData.rows <<endl;
    success += negData.rows - detected;
    //cout result
    cout << " success : " << success <<endl
         << " success rate : " << (float)success/ (float)( number ) <<endl;

    free(dataPos);
    free(dataNeg);

return true;
}
label lbpFeatureSvm::testImage(cv::Mat& image){
    int i;
    float testData[featureNumber];

    evaluator.loadImage( image );
    evaluator.countHistogram();

    for(i=0;i<featureNumber;i++)
        testData[i] = evaluator.feature.histogram[i];
    Mat testMat(1,featureNumber,CV_32FC1,testData);
    label re = _svm.predict(testMat);
    return re;
}

label lbpFeatureSvm::testMap(cv::Rect& area){
    int i;
    float testData[featureNumber];

    evaluator.countHistogram(area);

    for(i=0;i<featureNumber;i++)
        testData[i] = evaluator.feature.histogram[i];
    Mat testMat(1,featureNumber,CV_32FC1,testData);
    label re = _svm.predict(testMat);
    return re;
}

int lbpFeatureSvm::testMap(lbpFeatureEvaluator& in,std::vector<object> &detected,int delta,double scale){
    in.area = Rect(0,0,trainW,trainH);
    in.countHistogramArea();
    float testData[featureNumber];
    Mat testMat(1,featureNumber,CV_32FC1,&testData);
    object obj;
    int i,found = 0;
    obj.scale = scale;
    //double t0,t1;
    while(1){
        while(1){
            //cout << " in.area(x,y) = " << in.area.x << "," <<in.area.y << endl;
            //t0 = getTickCount();
            for(i=0;i<featureNumber;i++)
                testData[i] = in.feature.histogram[i];
            //t1 = getTickCount();
            //cout << "int to float :" << t1-t0 << ",";
            //t0 = getTickCount();
            testMat = Mat(1,featureNumber,CV_32FC1,testData);
            //t1 = getTickCount();
            //cout << "testMat set :" << t1-t0 << ",";
            //t0 = getTickCount();
            testMat = testMat*_pca.transMatrix;
            //t1 = getTickCount();
            //cout << "Mat mul :" << setw(3)<<t1-t0 << ",";
            //t0 = getTickCount();
            if(_svm.predict(testMat) == label_pos){
                obj.x = in.area.x;
                obj.y = in.area.y;
                detected.push_back( obj );
                ++found;
            }
            //t1 = getTickCount();
            //cout << "predict :" << t1-t0 << endl;
            if( (int)(in.area.x + delta+trainW) < in.feature.width )
                in.histogramRight(delta);
            else
                break;
        }
        if(in.area.y+delta+trainH <= in.feature.height)
            in.histogramDown(delta);
        else
            break;

        while(1){
            for(i=0;i<featureNumber;i++)
                testData[i] = in.feature.histogram[i];
            testMat = Mat(1,featureNumber,CV_32FC1,&testData);
            testMat = testMat*_pca.transMatrix;

            if(_svm.predict(testMat) == label_pos){
                obj.x = in.area.x;
                obj.y = in.area.y;
                detected.push_back( obj );
                ++found;
            }
            if( in.area.x - delta > 0 )
                in.histogramLeft(delta);
            else
                break;
        }
        if( (int)(in.area.y+delta+trainH) < in.feature.height)
            in.histogramDown(delta);
        else
            break;
    }
    return found;
}

int lbpFeatureSvm::testMapO(lbpFeatureEvaluator& in,std::vector<object> &detected,int delta,double scale){
    in.area = Rect(0,0,trainW,trainH);
    in.countHistogramArea();
    float testData[featureNumber];
    Mat testMat(1,featureNumber,CV_32FC1,&testData);
    object obj;
    int i,found = 0;
    obj.scale = scale;
    while(1){
        while(1){
            for(i=0;i<featureNumber;i++)
                testData[i] = in.feature.histogram[i];
            testMat = Mat(1,featureNumber,CV_32FC1,&testData);
            if(_svm.predict(testMat) == label_pos){
                obj.x = in.area.x;
                obj.y = in.area.y;
                detected.push_back( obj );
                ++found;
            }
            if( (int)(in.area.x + delta+trainW) < in.feature.width )
                in.histogramRight(delta);
            else
                break;
        }
        if(in.area.y+delta+trainH <= in.feature.height)
            in.histogramDown(delta);
        else
            break;

        while(1){
            for(i=0;i<featureNumber;i++)
                testData[i] = in.feature.histogram[i];
            testMat = Mat(1,featureNumber,CV_32FC1,&testData);
            if(_svm.predict(testMat) == label_pos){
                obj.x = in.area.x;
                obj.y = in.area.y;
                detected.push_back( obj );
                ++found;
            }
            if( in.area.x - delta > 0 )
                in.histogramLeft(delta);
            else
                break;
        }
        if( (int)(in.area.y+delta+trainH) < in.feature.height)
            in.histogramDown(delta);
        else
            break;
    }
    return found;
}
