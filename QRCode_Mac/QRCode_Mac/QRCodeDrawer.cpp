// QRCodeDrawer.cpp

#include "QRCodeDrawer.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

cv::Mat QRCodeDrawer::drawQRMessage(const std::string& message, const cv::Size& size) {
    QRDataCoder coder;
    std::string str(message.c_str());
    QRData data = coder.encode(str);

    if (data.g1.r1.size() == 0) {
        return cv::Mat();  // Returning an empty Mat if data is not valid
    }

    int side = std::min(size.width, size.height);
    cv::Mat qrImage = cv::Mat::zeros(size, CV_8UC3);

    cv::Size drawSize(352, 352);
    cv::Mat transformationMatrix = cv::getRotationMatrix2D(cv::Point2f(size.width / 2, size.height / 2 - 22), 45, side / sqrt(2) / drawSize.width);
    cv::warpAffine(qrImage, qrImage, transformationMatrix, size);

    // Draw dots
    for (int i = 0; i < 4; i++) {
        cv::Point p(30, 30);
        if (i == 1 || i == 2) {
            p.x = drawSize.width - p.x;
        }
        if (i == 2 || i == 3) {
            p.y = drawSize.height - p.y;
        }
        drawDot(qrImage, p);
    }

    // Draw QR code groups
    for (int i = 0; i < 4; i++) {
        cv::Mat rotatedImage;
        if (i > 0) {
            cv::warpAffine(qrImage, rotatedImage, cv::getRotationMatrix2D(cv::Point2f(drawSize.width / 2, drawSize.height / 2), i * 90, 1.0), drawSize);
        } else {
            rotatedImage = qrImage.clone();
        }

        drawGroup(data.group(i), rotatedImage);
    }

    return qrImage;
}

void QRCodeDrawer::drawDot(cv::Mat& image, const cv::Point& point) {
    cv::circle(image, point, 3, cv::Scalar(0, 0, 0), cv::FILLED);
}

void QRCodeDrawer::drawGroup(const Group& group, cv::Mat& image) {
    for (int i = 0; i < 3; i++) {
        const RowData& row = group.row(i);

        if (!row.empty()) {
            for (size_t j = 0; j < row.size(); j++) {
                // Find the indices of true values in the row
                if (row[j]) {
                    // Convert the index to cv::Point coordinates
                    cv::Point point(static_cast<int>(j), static_cast<int>(i));

                    // Draw a dot at the calculated coordinates
                    drawDot(image, point);
                }
            }
        }
    }
}

