#include "multiImg.h"

using namespace std;
using namespace cv;
multiImg::multiImg(void){

}
int multiImg::load(const char* listPath,bool print,bool show){
    ifstream imgList;
    char filePath[PATH_MAX];
    char wName[20];
    int number;
    imgList.open(listPath,ios::in);
    if ( imgList.fail() ){
        printf("fail to open list file : %s\n",listPath);
        return 0;
    }
    while( imgList >> filePath ){

        number = _imgMap.size();
        _imgMap.push_back( cv::imread(filePath,1) );
        if( !_imgMap[number].data ){
            _imgMap.pop_back();
            printf("fail to load image : %s\n", filePath);
            break;
        }
        else{
            if( print) printf("loaded image : %s\n", filePath);
            if( show){
                sprintf (wName, "%d", _imgMap.size() );
                imshow( wName, _imgMap.back() );

                waitKey(0);
                destroyWindow( wName );
            }
        }
    }
    imgList.close();
    return _imgMap.size();
}
Mat* multiImg::read(int number){
    if( _imgMap.size() > (unsigned)number){
        return &_imgMap[number];
    }
    else{
        return NULL;
    }
}
