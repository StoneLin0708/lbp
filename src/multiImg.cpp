#include "multiImg.h"
#include <opencv2/highgui/highgui.hpp>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

multiImg::multiImg() {}

int multiImg::load(const char* listPath, bool print, bool show) {
  std::ifstream imgList;
  char filePath[PATH_MAX];
  std::string title;
  int number;
  imgList.open(listPath, std::ios::in);
  if (imgList.fail()) {
    printf("fail to open list file : %s\n", listPath);
    return 0;
  }
  while (imgList >> filePath) {
    number = _imgMap.size();
    _imgMap.push_back(cv::imread(filePath, 1));
    if (_imgMap[number].data == nullptr) {
      _imgMap.pop_back();
      printf("fail to load image : %s\n", filePath);
      break;
    } else {
      if (print) printf("loaded image : %s\n", filePath);
      if (show) {
        title = std::to_string(_imgMap.size());
        cv::imshow(title, _imgMap.back());

        cv::waitKey(0);
        cv::destroyWindow(title);
      }
    }
  }
  imgList.close();
  return _imgMap.size();
}

cv::Mat* multiImg::read(int number) {
  if (_imgMap.size() > (unsigned)number) {
    return &_imgMap[number];
  } else {
    return NULL;
  }
}
