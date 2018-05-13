#ifndef FEATURE_H
#define FEATURE_H
#include <array>
#include <opencv2/imgproc.hpp>

namespace feature {

struct lbp {
  using data_type = cv::Mat1b;
  data_type data;
  data_type& mat() { return data; }
  lbp(data_type data) : data(std::move(data)) {}
  static lbp get(const cv::Mat& img) {
    cv::Mat1b m;
    if (img.channels() == 3) {
      cv::cvtColor(img, m, CV_BGR2GRAY);
    } else if (img.channels() == 1) {
      m = img;
    } else {
      return lbp(cv::Mat());
    }
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
    return lbp(r);
  }
};

struct histogram {
  using data_type = cv::Mat1f;
  static data_type get(const cv::Mat& m) {
    if (m.type() != CV_8UC1) throw "type must be CV_8UC1";
    int hist[256] = {0};

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
    return hist_m / m.total();
  }
};

struct two_part_hist_v {
  using data_type = cv::Mat1f;
  static data_type get(const cv::Mat& m) {
    if (m.type() != CV_8UC1) throw "type must be CV_8UC1";
    int hist[256 * 2] = {0};
    int* hist_up = &hist[0];
    int* hist_down = &hist[256];

    int rows = m.rows / 2;
    int cols = m.cols;
    if (m.isContinuous()) {
      cols *= rows;
      rows = 1;
    }

    for (int i = 0; i < rows; ++i) {
      const auto p = m.ptr<uchar>(i);
      for (int j = 0; j < cols; ++j) {
        ++hist_up[p[j]];
      }
    }

    for (int i = 0; i < rows; ++i) {
      const auto p = m.ptr<uchar>(m.rows / 2 + i);
      for (int j = 0; j < cols; ++j) {
        ++hist_down[p[j]];
      }
    }

    data_type hist_m(1, 256 * 2);
    std::copy(&hist[0], &hist[256 * 2], hist_m.ptr<float>(0));
    return hist_m / (m.total() / 2);
  }
};

struct lbp_hist {
  using data_type = histogram::data_type;
  static data_type get(const cv::Mat& m) {
    cv::Mat gray;
    if (m.channels() == 3) {
      cv::cvtColor(m, gray, CV_BGR2GRAY);
    }
    return histogram::get(lbp::get(gray).mat());
  }
};

}  // namespace feature

#endif  // FEATURE_H
