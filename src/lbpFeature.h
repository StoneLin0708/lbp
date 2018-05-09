#ifndef LBP_FEATURE_H
#define LBP_FEATURE_H
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <fmt/format.h>
#include <array>
#include <fstream>
#include <vector>

struct lbp {
  using data_type = cv::Mat1b;
  static data_type get(const cv::Mat& m) {
    if (m.type() != CV_8UC1) throw "type must be CV_8UC1";
    data_type r(m.size());
    auto lbp_at = [&](int row, int col) {
      unsigned char lbp = 0;
      const unsigned char two0 = 1;
      const unsigned char two1 = 2;
      const unsigned char two2 = 4;
      const unsigned char two3 = 8;
      const unsigned char two4 = 16;
      const unsigned char two5 = 32;
      const unsigned char two6 = 64;
      const unsigned char two7 = 128;

      const auto center = m.at<uchar>(row, col);

      if (m.at<uchar>(row - 1, col - 1) >= center) lbp |= two7;
      if (m.at<uchar>(row - 1, col) >= center) lbp |= two6;
      if (m.at<uchar>(row - 1, col + 1) >= center) lbp |= two5;
      if (m.at<uchar>(row, col + 1) >= center) lbp |= two4;
      if (m.at<uchar>(row + 1, col + 1) >= center) lbp |= two3;
      if (m.at<uchar>(row + 1, col) >= center) lbp |= two2;
      if (m.at<uchar>(row + 1, col - 1) >= center) lbp |= two1;
      if (m.at<uchar>(row, col - 1) >= center) lbp |= two0;
      return lbp;
    };

    for (int i = 1; i < m.size().height-1; i++) {
      for (int j = 1; j < m.size().width-1; j++) {
        r.at<uchar>(i, j) = lbp_at(i, j);
      }
    }
    return r;
  }
};

struct histogram {
  using data_type = cv::Mat1f;
  static data_type get(const cv::Mat& m) {
    if (m.type() != CV_8UC1) throw "type must be CV_8UC1";
    int hist[256];

    int rows = m.rows;
    int cols = m.cols;
    if (m.isContinuous()) {
      cols *= rows;
      rows = 1;
    }

    for (int i = 0; i < rows; ++i) {
      const auto p = m.ptr<uchar>(i);
      for (int j = 0; j < cols; ++j) {
        ++hist[p[j]];
      }
    }
    data_type hist_m(1, 256);
    std::copy(&hist[0], &hist[256], hist_m.ptr<float>(0));
    return hist_m;
  }
};


class lbpFeature {
 public:
  lbpFeature();

  lbpFeature& operator=(const lbpFeature&);

  bool outputFile(const char* path);
  bool saveK();
  int k(int number);
  int h(unsigned char number) { return histogram[number]; };
  void clear();
  //
  void drawLBP(const char* title);
  //
  int height;
  int width;
  int histogram[256];
  int sortList[256];

  std::vector<unsigned char> LBP;

 private:
  int _saveCounter;
  std::ofstream _outFile;
};

class lbpFeatureEvaluator {
 public:
  lbpFeatureEvaluator();
  void loadImage(const cv::Mat& image, bool toLBP = true);
  unsigned char countLBPat(int col, int row);
  void countLBP();
  // load lbpMap to feature
  bool loadMap(std::vector<unsigned char>& lbp, int rows, int cols);

  // count histogram and sort
  void countHistogram();
  //( six function below ) count histogram without sort
  void countHistogram(cv::Rect area);
  void countHistogramArea();
  //( four function below ) need count histogram at last once before
  // count histogram in new area(just shift area x,y) with out clear old data
  void histogramUp(int shift);
  void histogramDown(int shift);
  void histogramLeft(int shift);
  void histogramRight(int shift);

  void pcaCalculate();
  // Data
  lbpFeature feature;
  cv::Rect area;

 private:
  cv::Mat _image;
  std::ofstream _outFile;
};

class lbpFeatureGraph {
 public:
  lbpFeatureGraph();
  explicit lbpFeatureGraph(lbpFeature& feature);

  bool load(lbpFeature& feature);

  void drawLBPMap(const char* title);
  void drawHistogram(const char* title_histogram);
  void drawMapValue(const char* title, unsigned char lbpValue);
  void drawMapK(const char* title, unsigned char K_max, unsigned char K_min);

 private:
  lbpFeature _feature;
};

#endif  // LBP_FEATURE_H
