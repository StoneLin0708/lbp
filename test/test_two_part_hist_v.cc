#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "feature.h"
#include "util.h"

int main(int argc, const char* argv[]) {
  if (argc != 2) return -1;
  auto m = cv::imread(argv[1]);
  cv::imshow("image", m);
  cv::cvtColor(m, m, CV_BGR2GRAY);
  auto lbp = feature::lbp::get(m).mat();
  cv::imshow("lbp", lbp);
  auto hist = feature::two_part_hist_v::get(lbp);
  auto draw = util::draw_hist(hist.colRange(0, 256), 300, 512);
  cv::vconcat(draw, util::draw_hist(hist.colRange(256, 512), 300, 512), draw);
  cv::imshow("hist", draw);
  cv::waitKey(0);

  return 0;
}
