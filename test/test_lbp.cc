#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "lbp.h"

int main(int argc, const char* argv[]) {
  if (argc != 2) return -1;
  auto m = cv::imread(argv[1]);
  cv::imshow("image", m);
  cv::cvtColor(m, m, CV_BGR2GRAY);
  cv::imshow("lbp", lbp::get(m));
  cv::waitKey(0);

  return 0;
}
