#ifndef HISTOGRAM1D_H
#define HISTOGRAM1D_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <dirent.h>
#include <fstream>

using namespace cv;
using namespace std;

struct Hist_Points{
    int x;
    int y;
    int radius;
    int blk_siz;
};


class Histogram1D{

private:
    int histSize[1];
    float hranges[2];
    const float* ranges[1];
    int channels[1];

public:

    Histogram1D(bool u2, int max){
        if (u2)
        {
            //prepare arguments for 1D histogram:
            histSize[0]=max+1; //standardize*
            hranges[0]=0.0;
            hranges[1]=double(max); //standardize*
            ranges[0]=hranges;
            channels[0]=0; //by default look at channel 0
        }
        else
        {
            //prepare arguments for 1D histogram:
            histSize[0]=max+1;
            hranges[0]=0.0;
            hranges[1]=(double) max;
            ranges[0]=hranges;
            channels[0]=0; //by default look at channel 0
        }
    }

    //Computes the 1D histogram:
    Mat getHistogram(const Mat &image);
    //Outputs a histogram image:
    Mat getHistogramImage(bool u2, const Mat &image, int max);
};


#endif // HISTOGRAM1D_H

#ifndef LBPHISTOGRAM_H
#define LBPHISTOGRAM_H
class LBPHISTOGRAM{
public:
    bool is_rotated;

    LBPHISTOGRAM();

    Mat ROItoHistogram(Mat Result, bool u2, int max);

    int LBPFilter(Mat* input, Mat& dest, bool u2, int N, int R);

    int Local_LBPFilter(Mat *input, Mat& dest, bool u2, int N, int R, int X, int Y,int blk_size);

    Mat PatchHistogram(Mat *warp_dst);

    Mat rotate(Mat* image_input, int x_1, int y_1, int x_2, int y_2, int length=66 );

    int SVM_Predictor(Mat* test_image);
    int gender_predictor(Mat *test_image);
    int age_predictor(Mat *test_image);

    bool Load_Gender_Points(string file_name);

    bool Load_Age_Points(string file_name);


    Mat AGE_descriptor(Mat *warp_dst);
    Mat GENDER_descriptor(Mat *warp_dst);

private:

    int DEFAULT_BLOCK_SIZE; //height and width of the block to be LBPu'd
    int DEFAULT_NUM_BLOCKS; //There will be 100 10x10 blocks for a 100x100 size image; 10 blocks for the height and width each

    int XL_eye;
    int XR_eye;
    int YL_eye;
    int YR_eye;
    int numOPtch;
    int gender_numOPtch;
    int age_numOPtch;
    int nOtrain;

    Mat train_Data;
    string Train_File;
    string Train_LbL;
    CvSVM MYSVM;
    CvSVM GENDER_SVM;
    CvSVM AGE_SVM;
    CvSVMParams params;
    CvSVMParams gender_params;
    CvSVMParams age_params;
    vector<Hist_Points> gender_points;
    vector<Hist_Points> age_points;
    void SVM_LBP_Train();

    int checkIfLBPu2Value(int N, int out);

    vector<int> getAllUnifVals(int N);

    void norm_1(Mat input, Mat &output);

    void convertImageBins(Mat& dest, vector<int> bins);

};

#endif
