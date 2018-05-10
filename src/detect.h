#ifndef DETECT_H
#define DETECT_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ml.hpp>
#include "lbp.h"
#include "util.h"

struct detect {
  static auto get(cv::Ptr<cv::ml::SVM> svm, const cv::Mat& image, int height,
                  int width) {
    cv::Mat m = image.clone();
    cv::Mat ret = image.clone();
    int rows = m.rows;
    int cols = m.cols;

    cv::cvtColor(m, m, CV_BGR2GRAY);
    auto m_lbp = lbp::get(m);
    int stride = width / 5;
    for (int r = 0; r < rows - height; r += stride) {
      cv::Mat1f x;
      for (int c = 0; c < cols - width; c += stride) {
        auto subm = m_lbp(cv::Rect(c, r, width, height));
        auto hist = histogram::get(subm);
        x.push_back(hist);
      }
      cv::Mat p;
      svm->predict(x, p);
      int i = 0;
      for (int c = 0; c < cols - width; c += stride) {
        if (p.at<float>(0, i) == 1) {
          cv::rectangle(ret, cv::Rect(c, r, width, height),
                        cv::Scalar(0, 255, 0));
        }
        ++i;
      }
    }
    return ret;
  }
};

#endif  // DETECT_H
