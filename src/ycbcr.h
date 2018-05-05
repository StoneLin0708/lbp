#include <iostream>
#include <opencv2/core/core.hpp>

void rgb2ycbcr(const cv::Mat& image,cv::Mat& result);
void markSkin(cv::Mat& image);
void markSkinBlock(cv::Mat& image);
