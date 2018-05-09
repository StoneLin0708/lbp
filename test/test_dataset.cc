#include <fmt/format.h>
#include "dataset.h"

int main(int argc, const char* argv[]) {
  dataset data;

  if (argc != 2) {
    fmt::print("missing data dir\n");
    return -1;
  }

  data.load(argv[1], 48, 40);
  data.get(0.1);
  for (int i = 0; i < 10; ++i) {
    cv::imshow("image", data.image[i]);
    cv::imshow("lbp", data.image_lbp[i]);
    auto v = data.label.at<int>(0, i);
    std::cout << v << '\n';
    cv::waitKey(0);
  }

  return 0;
}
