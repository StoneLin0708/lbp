#ifndef UTIL_H
#define UTIL_H
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
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

inline auto draw_hist(cv::Mat1f hist, int h, int w) {
  int cols = hist.cols;
  cv::Mat m(h, cols, CV_8UC3, cv::Scalar(0, 0, 0));
  double minV, maxV;
  cv::minMaxLoc(hist, &minV, &maxV);
  for (int i = 0; i < hist.cols; ++i) {
    cv::line(
        m, cv::Point(i, h),
        cv::Point(i, h - (hist.at<float>(0, i) - minV) / (maxV - minV) * h),
        cv::Scalar(0, 255, 0));
  }
  cv::resize(m, m, cv::Size(w, h), cv::INTER_NEAREST);
  return m;
}

}  // namespace util

#endif  // UTIL_H
