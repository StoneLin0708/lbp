#include "dataset.h"
#include <fmt/format.h>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <random>

bool dataset::load(const std::string& dir_path, int h, int w) {
  fmt::print("loading ... {}\n", dir_path);
  namespace fs = boost::filesystem;
  std::vector<std::string> pos, neg;
  if (!fs::exists(dir_path)) {
    return false;
  }
  if (fs::is_directory(dir_path)) {
    for (auto& dir : fs::directory_iterator(dir_path)) {
      const std::string name = fs::path(dir).filename().string();

      auto add_files_to = [](auto& dir, auto& path_vec) {
        for (auto& x : fs::directory_iterator(dir)) {
          if (fs::is_regular_file(x)) {
            path_vec.push_back(x.path().string());
          }
        }
      };
      if (name == "pos") {
        add_files_to(dir, pos);
      } else if (name == "neg") {
        add_files_to(dir, neg);
      }
    }
  }

  auto preprocess = [&](auto d) {
    int loaded = 0;
    for (auto& i : d) {
      auto m = cv::imread(i);
      if (m.data != nullptr) {
        auto s = m.size();
        bool wider = (s.width / float(s.height)) > (w / float(h));
        int new_w = wider ? w * s.height / float(h) : s.width;
        int new_h = wider ? s.height : h * s.width / float(w);
        int x = wider ? (s.width - new_w) / 2.0 : 0;
        int y = wider ? 0 : (s.height - new_h) / 2.0;

        cv::resize(m(cv::Rect(x, y, new_w, new_h)), m, cv::Size(w, h));
        image.push_back(m);
        cv::cvtColor(m, m, CV_BGR2GRAY);
        image_lbp.push_back(lbp::get(m));
        hist.push_back(histogram::get(image_lbp.back()));
        ++loaded;
      }
    }
    return loaded;
  };

  auto n_pos = preprocess(pos);
  auto n_neg = preprocess(neg);

  label.push_back(cv::Mat1i(n_pos, 1, 1));
  label.push_back(cv::Mat1i(n_neg, 1, -1));
  label = label.t();
  fmt::print("loaded pos : {} neg : {} memory_usage : {} KB\n", n_pos, n_neg,
             !image.empty() ? ((image[0].total() * image[0].elemSize() +
                                image_lbp[0].total() * image_lbp[0].elemSize() +
                                hist[0].total() * hist[0].elemSize()) *
                                   image.size() +
                               label.total() * label.elemSize()) /
                                  1024
                            : 0);

  return (n_pos != 0) && (n_neg != 0);
}

dataset::splitted_data dataset::get(float test) {
  int size = image.size();
  std::mt19937 mt(123);
  std::uniform_int_distribution<int> dist(0, size - 1);
  for (int i = 0; i < size; ++i) {
    int r = dist(mt);
    std::swap(image[i], image[r]);
    std::swap(image_lbp[i], image_lbp[r]);
    std::swap(hist[i], hist[r]);
    std::swap(label.at<float>(0, i), label.at<float>(0, r));
  }

  cv::Mat1f hist_mat;
  for (int i = 0; i < size; ++i) {
    hist_mat.push_back(hist[i]);
  }

  return {cv::ml::TrainData::create(hist_mat.rowRange(0, size * (1 - test)),
                                    cv::ml::SampleTypes::ROW_SAMPLE,
                                    label.colRange(0, size * (1 - test))),
          cv::ml::TrainData::create(hist_mat.rowRange(size * (1 - test), size),
                                    cv::ml::SampleTypes::ROW_SAMPLE,
                                    label.colRange(size * (1 - test), size))};
}
