#ifndef QRCODEDRAWER_H
#define QRCODEDRAWER_H

#include <iostream>
#include <opencv2/core.hpp>
#include "qr_data_coder.hpp"
#include <opencv2/highgui.hpp>

class QRCodeDrawer {
public:
    static cv::Mat drawQRMessage(const std::string& message, const cv::Size& size);

private:
    static void drawDot(cv::Mat& image, const cv::Point& point);
    static void drawGroup(const Group& group, cv::Mat& image);
};

#endif // QRCODEDRAWER_H
