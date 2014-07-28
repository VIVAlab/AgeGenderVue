#ifndef MACROS_H
#define MACROS_H

#include <time.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>




//Macros used for output.
//Can easily mask all output by replacing these with the commented
//out macros below
//Note that the omission of parentheses around the parameters error and info is
//necessary to preserve cout style printing.
//This can lead to undefined behavior, so use with care.
#if VIVA_DEBUG
#define print_line() do {std::cout << "[HERE]" <<  __FILE__ << " " << __LINE__ << std::endl;} while(0)
#define print_err(error) do {std::cerr << "[ERROR]" <<  __FILE__ << " " << __LINE__ << " : " << error << std::endl;} while(0)
#define print_info(info) do {std::cout << "[INFO]" <<  __FILE__ << " " << __LINE__ << " : " << info << std::endl;} while(0)
#else
#define print_line() do {} while(0)
#define print_err(error) do {} while(0)
#define print_info(info) do {} while(0)
#endif

//Random utility function
std::string getCurrentDateTime();

void putBoldedText(cv::Mat frame, std::string text, cv::Point pos, double size);


#endif // MACROS_H
