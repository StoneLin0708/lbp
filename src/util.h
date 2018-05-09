#ifndef UTIL_H
#define UTIL_H
#include <opencv2/ml.hpp>

namespace util {
inline cv::Mat test(cv::Ptr<cv::ml::SVM> svm, cv::Ptr<cv::ml::TrainData> data) {
  cv::Mat y;
  svm->predict(data->getSamples(), y);
  cv::transpose(y, y);
  auto t = data->getResponses();
  y.convertTo(y, t.type());
  return y;
}

inline float error(const cv::Mat& pred, const cv::Mat& target) {
  int pred_false = cv::countNonZero(pred != target);
  return pred_false / float(pred.cols);
}
}  // namespace util

#endif  // UTIL_H
