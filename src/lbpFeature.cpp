#include "lbpFeature.h"

using namespace std;
using namespace cv;

lbpFeature::lbpFeature(void){
    _saveCounter = 0;
}

lbpFeature::~lbpFeature(void){
    _outFile.close();
}

void lbpFeature::clear(void){
    height = 0;
    width = 0;
    memset( histogram, 0, sizeof(histogram) );
    memset( sortList, 0, sizeof(sortList) );

}

void lbpFeature::operator=(const lbpFeature& f){
   this->height = f.height;
   this->width = f.width;
   this->LBP = f.LBP;
   int i;
   for(i = 0;i<256;i++){
       this->histogram[i] = f.histogram[i];
       this->sortList[i] = f.sortList[i];
   }
}

bool lbpFeature::outputFile(const char* path){
    _outFile.open(path,ios::out);
    if(_outFile){
        return true;
    }
    else{
        cout << "could't open : " << path << endl;
        return false;
    }
}

int lbpFeature::k(int number){
    return histogram[ sortList[number] ];
}

bool lbpFeature::saveK(void){
    if(!_outFile) return false;
    _outFile << setw(3) << height << setw(3) << width;
    int i;
    for(i=0;i<20;i++){
        _outFile << setw(4) << sortList[i] << ":" << setw(3) << k(i);
    }
    _outFile << endl;
    return true;
}

void lbpFeature::drawLBP(const char* title){
    Mat img( height, width ,CV_8UC1 );
    int i,j;
    for(j=0; j<height; j++){
        for(i=0;i<width;i++){
            img.at<uchar>(j,i) = LBP.at(j*width+i);
        }
    }
    cv::imshow(title,img);
}

///////////////////////////////////////////////////////////

lbpFeatureEvaluator::lbpFeatureEvaluator(void){

}

void lbpFeatureEvaluator::loadImage(cv::Mat image,bool toLBP){
    feature.clear();
    //image.copyTo(_image);
    //_image = image.clone();
    cvtColor(image,_image,CV_BGR2GRAY );
	//imshow("hiiii",_image);
    feature.height = _image.rows-2;
    feature.width = _image.cols-2;
    if(toLBP)countLBP();
    //waitKey(30);
}

bool lbpFeatureEvaluator::loadMap(vector<unsigned char>& lbp, int rows, int cols){
    feature.height = rows;
    feature.width = cols;
    feature.LBP.clear();
    feature.LBP = lbp;

    return true;
}

inline unsigned char lbpFeatureEvaluator::countLBPat(int col,int row){
    unsigned char lbp = 0,center;
    const unsigned char two0 = 1;
    const unsigned char two1 = 2;
    const unsigned char two2 = 4;
    const unsigned char two3 = 8;
    const unsigned char two4 = 16;
    const unsigned char two5 = 32;
    const unsigned char two6 = 64;
    const unsigned char two7 = 128;

    center = _image.at<uchar>(row,col);

    if( _image.at<uchar>(row-1,col-1) >= center ) lbp |= two7;
    if( _image.at<uchar>(row-1,  col) >= center ) lbp |= two6;
    if( _image.at<uchar>(row-1,col+1) >= center ) lbp |= two5;
    if( _image.at<uchar>(  row,col+1) >= center ) lbp |= two4;
    if( _image.at<uchar>(row+1,col+1) >= center ) lbp |= two3;
    if( _image.at<uchar>(row+1,  col) >= center ) lbp |= two2;
    if( _image.at<uchar>(row+1,col-1) >= center ) lbp |= two1;
    if( _image.at<uchar>(  row,col-1) >= center ) lbp |= two0;
    return lbp;
}

void lbpFeatureEvaluator::countLBP(void){
    int i,j;
    feature.LBP.clear();

    for (j = 1; j <= feature.height; j++){
        for( i = 1; i <= feature.width; i++){
            feature.LBP.push_back( countLBPat(i,j) );
        }
    }
}

void lbpFeatureEvaluator::countHistogram(void){
    int max,temp,i,j;
    memset( feature.histogram, 0, sizeof(feature.histogram) );
    memset( feature.sortList, 0, sizeof(feature.sortList) );
    //calculate histogram
    max = feature.LBP.size();
    for(i=0;i<max;i++){
        feature.histogram[ feature.LBP[i] ] ++;
    }
    //sort histogram and save to feature.sortList
    for( i=0; i < 256; i++){
        feature.sortList[i] = i;
    }
    for( j=0; j < 256; j++){

        max = j;
        for( i=j+1; i< 256; i++){
            if ( feature.histogram[ feature.sortList[i] ] > feature.histogram[ feature.sortList[max] ] ) max = i;
        }
        temp = feature.sortList[j];
        feature.sortList[j] = feature.sortList[max];
        feature.sortList[max] = temp;
    }
}

void lbpFeatureEvaluator::countHistogram(cv::Rect area){
    int i,j,xmax,ymax,width = feature.width;
    memset( feature.histogram, 0, sizeof(feature.histogram) );
    //calculate histogram

    xmax = area.x + area.width;
    ymax = area.y + area.height;
    for( i=area.y; i<ymax; i++){
        for( j=area.x; j<xmax; j++){
            feature.histogram[ feature.LBP[ j + width*i ] ] ++;
        }
    }
}

void lbpFeatureEvaluator::countHistogramArea(void){
    int i,j,xmax,ymax,width = feature.width;
    memset( feature.histogram, 0, sizeof(feature.histogram) );
    //calculate histogram
    xmax = area.x + area.width;
    ymax = area.y + area.height;
    for( i=area.y; i<ymax; i++){
        for( j=area.x; j<xmax; j++){
            feature.histogram[ feature.LBP[ j + width*i ] ] ++;
        }
    }
}

void lbpFeatureEvaluator::histogramUp(int shift){
    int i,j,width = feature.width;
    //calculate histogram
    const int xmax = area.x + area.width;
    const int ymax = area.y + area.height;
    unsigned char* lbp = &feature.LBP[0] + (ymax -shift)*width;

    for( i=0; i<shift; ++i){
        for( j=area.x; j<xmax; ++j){
            --feature.histogram[ *(lbp + j) ];
        }
        lbp += width;
    }

    area.y -= shift;
    lbp = &feature.LBP[0] + (area.y)*width;
    for( i=0; i<shift; ++i){
        for( j=area.x; j<xmax; ++j){
           ++feature.histogram[ *(lbp + j) ];
        }
        lbp += width;
    }
}

void lbpFeatureEvaluator::histogramDown(int shift){
    int i,j,width = feature.width;
    //calculate histogram
    const int xmax = area.x + area.width;
    const int ymax = area.y + area.height;
    unsigned char* lbp = &feature.LBP[0] + area.y*width;

    for( i=0; i<shift; ++i){
        for( j=area.x; j<xmax; ++j){
            --feature.histogram[ *(lbp + j) ];
        }
        lbp += width;
    }

    area.y += shift;
    lbp = &feature.LBP[0] + ymax*width;
    for( i=0; i<shift; ++i){
        for( j=area.x; j<xmax; ++j){
            ++feature.histogram[ *(lbp + j) ] ;
        }
        lbp += width;
    }
}

void lbpFeatureEvaluator::histogramLeft(int shift){
    int i,j,width = feature.width;
    //calculate histogram
    const int xmax = area.x + area.width;
    const int xo = xmax - shift;
    unsigned char* lbp = &feature.LBP[0] + area.y*width;

    for( i=0; i<area.height; ++i){
        for( j=xo; j<xmax; ++j){
            --feature.histogram[ *(lbp + j) ];
        }
        lbp += width;
    }

    const int xorg = area.x;
    area.x -= shift;
    lbp = &feature.LBP[0] + area.y*width;
    for( i=0; i<area.height; ++i){
        for( j=area.x; j< xorg; ++j){
            ++feature.histogram[ *(lbp + j) ];
        }
        lbp += width;
    }
}

void lbpFeatureEvaluator::histogramRight(int shift){
    int i,j,width = feature.width;
    //calculate histogram
    const int xmax = area.x + shift;
    unsigned char* lbp = &feature.LBP[0] + area.y*width;

    for( i=0; i<area.height; ++i){
        for( j=area.x; j<xmax; ++j){
            --feature.histogram[ *(lbp + j) ];
        }
        lbp += width;
    }
    const int xo = area.x+area.width;
    const int xmax2 = xo+shift;
    area.x += shift;
    lbp = &feature.LBP[0] + area.y*width;
    for( i=0; i<area.height; ++i){
        for( j=xo; j< xmax2; ++j){
            ++feature.histogram[ *(lbp + j) ];
        }
        lbp += width;
    }
}
/*
void lbpFeatureEvaluator::drawHistogram(const char* title_histogram){
    Mat img( 320, 590, CV_8UC3,Scalar( CV_RGB( 20, 20, 20) ) );
    int* histogram;
    int maxValue;
    int i;
    double scale;

    histogram = feature.histogram;
    maxValue = feature.histogram[ feature.sortList[0] ];

    //scale line
    line(img,Point(20,19), Point(570,19),Scalar( CV_RGB(240,240,240) ),1,8,0);
    for( i=1; i<10; i++){
        line(img,Point(20,300-28*i), Point(570,300-28*i),Scalar( CV_RGB(240,240,240) ),1,8,0);
    }
    line(img,Point(20,301), Point(570,301),Scalar( CV_RGB(240,240,240) ),1,8,0);
    //histogram line
    for( i=0; i< 256; i++){
        scale = (double)*(histogram +i)/(double)maxValue;
        line(img,Point(30+i*2,300),Point( 30+i*2, 300- (int)(scale*280.0) ), Scalar( CV_RGB( 255*scale , 10, 255*(1.0-scale) ) ) ,2,8,0);
    }

    imshow(title_histogram,img);
}

void lbpFeatureEvaluator::drawMapValue(const char* title,unsigned char lbpValue){
        Mat img( feature.height, feature.width ,CV_8UC1 ,Scalar(255));
        int i,j;
        printf("%s : lbpValue = %d\n",title,lbpValue);
        for(j=0; j<feature.height; j++){
            for(i=0;i<feature.width;i++){
                if( feature.lbpMat.at<uchar>(j,i) == lbpValue) {
                    img.at<uchar>(j,i) = 0;
                }
            }
        }

        imshow(title,img);
}

void lbpFeatureEvaluator::drawMapK(const char* title,unsigned char K_max,unsigned char K_min){
        Mat img( feature.height, feature.width ,CV_8UC1 ,Scalar(255));

        int maxValue = feature.histogram[ feature.sortList[K_max] ];
        int minValue = feature.histogram[ feature.sortList[K_min] ];
        int histogramValue,i,j;

        printf("%s : minValue = %d to maxValue = %d\n",title,minValue,maxValue);

        for(j=0; j<feature.height; j++){
            for(i=0;i<feature.width;i++){

                histogramValue = feature.histogram[ feature.lbpMat.at<uchar>(j,i) ];

                if( histogramValue <= maxValue && histogramValue >= minValue){
                    img.at<uchar>(j,i) = 0;

                }
            }
        }
        imshow(title,img);
}
*/
void lbpFeatureEvaluator::pcaCalculate(void){
    Mat pcaSet;
    pcaSet = _image.clone();
    Mat imageOut;

    imshow("test in",pcaSet);

    pcaSet.reshape(1 ,pcaSet.cols * pcaSet.rows );

    pcaSet.convertTo(pcaSet,CV_32FC1,1/255);

    PCA pca(pcaSet,Mat(),CV_PCA_DATA_AS_COL);

    normalize( pca.eigenvectors , imageOut, 0, 255, NORM_MINMAX, CV_8UC1);
    imageOut.reshape(1,_image.rows);
    imshow("test out",imageOut);
    //imshow("test out",imageOut );
    //waitKey(30);
    //system("pause");
}

