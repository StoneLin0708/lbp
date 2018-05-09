#ifndef LBP_H
#define LBP_H
#include <opencv2/core.hpp>

struct lbp {
  using data_type = cv::Mat1b;
  static data_type get(const cv::Mat& m) {
    if (m.type() != CV_8UC1) throw "type must be CV_8UC1";
    data_type r(m.size());
    auto lbp_at = [&](int row, int col) {
      unsigned char lbp = 0;
      const unsigned char two0 = 1;
      const unsigned char two1 = 2;
      const unsigned char two2 = 4;
      const unsigned char two3 = 8;
      const unsigned char two4 = 16;
      const unsigned char two5 = 32;
      const unsigned char two6 = 64;
      const unsigned char two7 = 128;

      const auto center = m.at<uchar>(row, col);

      if (m.at<uchar>(row - 1, col - 1) >= center) lbp |= two7;
      if (m.at<uchar>(row - 1, col) >= center) lbp |= two6;
      if (m.at<uchar>(row - 1, col + 1) >= center) lbp |= two5;
      if (m.at<uchar>(row, col + 1) >= center) lbp |= two4;
      if (m.at<uchar>(row + 1, col + 1) >= center) lbp |= two3;
      if (m.at<uchar>(row + 1, col) >= center) lbp |= two2;
      if (m.at<uchar>(row + 1, col - 1) >= center) lbp |= two1;
      if (m.at<uchar>(row, col - 1) >= center) lbp |= two0;
      return lbp;
    };

    for (int i = 1; i < m.size().height - 1; i++) {
      for (int j = 1; j < m.size().width - 1; j++) {
        r.at<uchar>(i, j) = lbp_at(i, j);
      }
    }
    return r;
  }
};

struct histogram {
  using data_type = cv::Mat1f;
  static data_type get(const cv::Mat& m) {
    if (m.type() != CV_8UC1) throw "type must be CV_8UC1";
    int hist[256];

    int rows = m.rows;
    int cols = m.cols;
    if (m.isContinuous()) {
      cols *= rows;
      rows = 1;
    }

    for (int i = 0; i < rows; ++i) {
      const auto p = m.ptr<uchar>(i);
      for (int j = 0; j < cols; ++j) {
        ++hist[p[j]];
      }
    }
    data_type hist_m(1, 256);
    std::copy(&hist[0], &hist[256], hist_m.ptr<float>(0));
    hist_m /= m.total();
    return hist_m;
  }
};

#endif  // LBP_H
