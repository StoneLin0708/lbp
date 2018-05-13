#ifndef DATASET_H
#define DATASET_H
#include <fmt/format.h>
#include <opencv2/core.hpp>
#include <opencv2/ml.hpp>
#include <random>
#include <string>
#include <tuple>
#include <vector>
#include "feature.h"

struct image_loader {
  using string = std::string;
  using image_set = std::vector<cv::Mat>;
  using two_set = std::pair<image_set, image_set>;
  using two_subfolder = std::pair<string, string>;
  static image_set load_image_set(const string& folder, int h, int w);
  static two_set load_two_classes(const string& folder, int h, int w,
                                  two_subfolder sub = {"pos", "neg"});
};

template <typename second_feature = feature::histogram>
struct dataset {
  bool load(const std::string& dir_path, int h, int w) {
    auto preprocess = [&](auto& image_set, int l) {
      for (auto& m : image_set) {
        image_lbp.push_back(feature::lbp::get(m).mat());
        feature.push_back(second_feature::get(image_lbp.back()));
      }
      image.insert(image.end(), image_set.begin(), image_set.end());
      label.push_back(cv::Mat1i(image_set.size(), 1, l));
    };
    auto r = image_loader::load_two_classes(dir_path, h, w);
    preprocess(r.first, 1);
    preprocess(r.second, -1);
    label = label.t();
    fmt::print("loaded pos : {} neg : {} memory_usage : {} KB\n",
               r.first.size(), r.second.size(),
               !image.empty()
                   ? ((image[0].total() * image[0].elemSize() +
                       image_lbp[0].total() * image_lbp[0].elemSize() +
                       feature[0].total() * feature[0].elemSize()) *
                          image.size() +
                      label.total() * label.elemSize()) /
                         1024
                   : 0);
    return (!r.first.empty()) && (!r.second.empty());
  };

  struct splitted_data {
    cv::Ptr<cv::ml::TrainData> train;
    cv::Ptr<cv::ml::TrainData> test;
  };

  splitted_data get(float test) {
    int size = image.size();
    std::mt19937 mt(123);
    std::uniform_int_distribution<int> dist(0, size - 1);
    for (int i = 0; i < size; ++i) {
      int r = dist(mt);
      std::swap(image[i], image[r]);
      std::swap(image_lbp[i], image_lbp[r]);
      std::swap(feature[i], feature[r]);
      std::swap(label.at<float>(0, i), label.at<float>(0, r));
    }

    cv::Mat1f hist_mat;
    for (int i = 0; i < size; ++i) {
      hist_mat.push_back(feature[i]);
    }

    return {
        cv::ml::TrainData::create(hist_mat.rowRange(0, size * (1 - test)),
                                  cv::ml::SampleTypes::ROW_SAMPLE,
                                  label.colRange(0, size * (1 - test))),
        cv::ml::TrainData::create(hist_mat.rowRange(size * (1 - test), size),
                                  cv::ml::SampleTypes::ROW_SAMPLE,
                                  label.colRange(size * (1 - test), size))};
  };

  std::vector<cv::Mat> image;
  std::vector<feature::lbp::data_type> image_lbp;
  std::vector<typename second_feature::data_type> feature;
  cv::Mat1i label;
};

#endif  // DATASET_H
