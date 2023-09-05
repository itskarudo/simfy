#include "opencv2/highgui.hpp"
#include <cstdio>
#include <iostream>
#include <opencv4/opencv2/opencv.hpp>

cv::Mat img, original;

int main(int argc, char *argv[]) {

  if (argc <= 1) {
    fprintf(stderr, "[!] Usage: %s <image>\n", argv[0]);
    return EXIT_FAILURE;
  }

  auto *filename = argv[1];

  img = cv::imread(filename);
  if (img.empty()) {

    fprintf(stderr, "[!] Could not open the provided image.\n");
    return EXIT_FAILURE;
  }

  original = img;

  cv::namedWindow(filename, cv::WINDOW_NORMAL);

  cv::imshow(filename, img);

  cv::createTrackbar("Scale", filename, nullptr, 100, [](int ratio, void *) {
    if (ratio == 0)
      ratio = 1;

    cv::resize(original, img, cv::Size(), 1.0f / ratio, 1.0f / ratio,
               cv::INTER_NEAREST_EXACT);
    cv::resize(img, img, cv::Size(), ratio, ratio, cv::INTER_NEAREST_EXACT);
  });

  while (1) {
    cv::imshow(filename, img);
    if (cv::waitKey(1) == 'q')
      break;
  }

  cv::destroyAllWindows();
  return EXIT_SUCCESS;
}
