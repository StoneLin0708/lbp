#include <fmt/format.h>
#include <cmath>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include "lbpFeatureSvm.h"

using cv::Mat;
using cv::Rect;
using cv::Scalar;
using cv::Size;
using std::cout;
using std::vector;

int detect_face(lbpFeatureSvm& svm, cv::Mat& image, double scale = 1.1,
                     int min_width = 30, int max_width = 100) {
  fmt::print("image w={} h={}\n", image.cols, image.rows);

  const int deltaO = 10, deltaM = 5;
  lbpFeatureEvaluator evaluator;
  vector<object> found;

  double scale_now = scale != 1.0 ? double(min_width) / (trainW + 2) : 1.0;
  int delta = deltaO / scale_now;

  do {
    int w = image.cols / scale_now;
    int h = image.rows / scale_now;
    if (w < trainW || h < trainH) break;
    Mat testImage;
    resize(image, testImage, cv::Size(w, h));
    evaluator.loadImage(testImage, true);
    svm.testMap(evaluator, found, delta, scale_now);

    fmt::print("scale {:3.2f} stride {} : found {}\n", scale_now, delta,
               found.size());

    scale_now *= scale;

    delta = deltaO / scale_now > deltaM ? deltaO / scale_now : deltaM;
  } while ((trainW * scale_now < max_width) && (scale != 1));

  int number = found.size();
  Mat save, temp = image.clone();

  cout << " detect : " << number << '\n';
  for (int i = 0; i < number; ++i) {
    auto foundRect =
        Rect(found[i].x * found[i].scale, found[i].y * found[i].scale,
             (trainW + 1) * found[i].scale, (trainH + 1) * found[i].scale);

    save = temp(foundRect);
    resize(save, save, Size(trainW + 2, trainH + 2));
    cv::imwrite(fmt::format("test_pos/{:03d}.jpg", i), save);

    rectangle(image, foundRect,
              Scalar(CV_RGB(60 * found[i].scale, 200 / found[i].scale, 0)), 1,
              8, 0);
  }

  return number;
}
