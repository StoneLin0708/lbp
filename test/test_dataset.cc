#include <fmt/format.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "dataset.h"
int main(int argc, const char* argv[]) {
  dataset<> data;

  if (argc != 2) {
    fmt::print("missing data dir\n");
    return -1;
  }

  constexpr int h = 48;
  constexpr int w = 48;
  data.load(argv[1], h, w);
  data.get(0.1);
  cv::Mat preview_pos(h * 2, 0, CV_8UC3);
  cv::Mat preview_neg(h * 2, 0, CV_8UC3);
  int pos = 0;
  int neg = 0;
  int i = 0;
  while ((pos < 10) || (neg < 10)) {
    // cv::imshow("image", data.image[i]);
    // cv::imshow("lbp", data.image_lbp[i]);
    cv::Mat m;
    cv::cvtColor(data.image_lbp[i], m, CV_GRAY2BGR);
    cv::vconcat(data.image[i], m, m);
    auto l = data.label.at<int>(0, i);
    if ((pos < 10) && (l == 1)) {
      cv::hconcat(preview_pos, m, preview_pos);
      ++pos;
    } else if (neg < 10 && (l == -1)) {
      cv::hconcat(preview_neg, m, preview_neg);
      ++neg;
    }
    ++i;
  }
  cv::Mat preview;
  cv::vconcat(preview_pos, preview_neg, preview);
  cv::imshow("preview", preview);
  cv::waitKey(0);

  return 0;
}
