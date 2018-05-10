#include "detect.h"
#include <fmt/format.h>
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>
#include "util.h"
using cv::ml::SVM;

int main(int argc, const char* argv[]) {
  if (argc < 3) {
    fmt::format(" [model] [image]\n");
    return -1;
  }

  auto svm = SVM::load(argv[1]);
  auto m = cv::imread(argv[2]);
  auto s = m.size();
  auto scale = 1.2;
  cv::Mat filted;
  cv::resize(m, m, cv::Size(s.width * scale, s.height * scale));
  cv::bilateralFilter(m, filted, 9, 18, 4);
  cv::imshow("result", detect::get(svm, filted, 48, 40));
  cv::waitKey(0);
}
