#include "dataset.h"
#include <fmt/format.h>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <random>
#include "feature.h"

image_loader::image_set image_loader::load_image_set(const std::string& folder,
                                                     int h, int w) {
  image_set one_set;
  namespace fs = boost::filesystem;
  for (auto& x : fs::directory_iterator(folder)) {
    if (fs::is_regular_file(x)) {
      auto m = cv::imread(x.path().string());
      if (m.data != nullptr) {
        auto s = m.size();
        bool wider = (s.width / float(s.height)) > (w / float(h));
        int new_w = wider ? w * s.height / float(h) : s.width;
        int new_h = wider ? s.height : h * s.width / float(w);
        int x = wider ? (s.width - new_w) / 2.0 : 0;
        int y = wider ? 0 : (s.height - new_h) / 2.0;

        cv::resize(m(cv::Rect(x, y, new_w, new_h)), m, cv::Size(w, h));
        one_set.push_back(m);
      }
    }
  }
  return one_set;
}

image_loader::two_set image_loader::load_two_classes(
    const std::string& folder, int h, int w, image_loader::two_subfolder sub) {
  fmt::print("loading image ... {}\n", folder);
  namespace fs = boost::filesystem;
  if (!fs::exists(folder)) {
    return {{}, {}};
  }

  std::string first, second;
  if (fs::is_directory(folder)) {
    for (auto& dir : fs::directory_iterator(folder)) {
      const std::string name = fs::path(dir).filename().string();

      if (name == sub.first) {
        first = fs::path(dir).string();
      } else if (name == sub.second) {
        second = fs::path(dir).string();
      }
    }
  }
  return {load_image_set(first, h, w), load_image_set(second, h, w)};
}
