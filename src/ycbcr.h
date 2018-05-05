#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
void rgb2ycbcr(cv::Mat& image,cv::Mat& result);
void markSkin(cv::Mat& image);
void markSkinBlock(cv::Mat& image);
