#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <fstream>
#include <iostream>
#include <iomanip>

class multiImg{
public:
    multiImg(void);
    int load(const char* listPath,bool print =false,bool show=false);
    cv::Mat* read(int no);
    int size(void) {return (int)_imgMap.size() ;};
private:
    std::vector<cv::Mat> _imgMap;
};
