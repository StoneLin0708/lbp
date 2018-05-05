#ifndef PCADIMREDUCE_H
#define PCADIMREDUCE_H
#include <opencv2/core/core.hpp>

class pcaDimReduce {
 public:
  void calculate(const cv::Mat& data, int k);

  bool save(const char* path);
  bool load(const char* path);

  cv::Mat transMatrix;

  // cv::Mat eigenvectors;
  // cv::Mat eigenvalues;
};

#endif  // PCADIMREDUCE_H
