#ifndef LBP_FEATURE_H
#define LBP_FEATURE_H
#include <opencv2/core/core.hpp>

#include <fstream>
#include <vector>

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
