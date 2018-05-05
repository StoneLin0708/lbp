#ifndef MULTIIMG_H
#define MULTIIMG_H
#include <opencv2/core/core.hpp>
#include <vector>

class multiImg {
 public:
  multiImg();
  int load(const char* listPath, bool print = false, bool show = false);
  cv::Mat* read(int no);
  int size() { return _imgMap.size(); };

 private:
  std::vector<cv::Mat> _imgMap;
};

#endif  // MULTIIMG_H
