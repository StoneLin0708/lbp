#ifndef DATASET_H
#define DATASET_H
#include <opencv2/core.hpp>
#include <opencv2/ml.hpp>
#include <string>
#include <tuple>
#include <vector>
#include "lbp.h"
#include "lbpFeature.h"

struct dataset {
  bool load(const std::string& dir_path, int h, int w);
  struct splitted_data {
    cv::Ptr<cv::ml::TrainData> train;
    cv::Ptr<cv::ml::TrainData> test;
  };

  splitted_data get(float test);

  std::vector<cv::Mat> image;
  std::vector<lbp::data_type> image_lbp;
  std::vector<histogram::data_type> hist;
  cv::Mat1i label;
};

#endif  // DATASET_H
