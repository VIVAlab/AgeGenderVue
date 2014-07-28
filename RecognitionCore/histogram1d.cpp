
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdint.h>
#include <opencv2/ml/ml.hpp>
#include "histogram1d.h"
using namespace cv;
using namespace std;

/**
 * @brief Histogram1D::getHistogram
 * @param image
 * @return the histogram
 */
Mat Histogram1D::getHistogram(const Mat &image)
{
    Mat hist;
    //compute histogram:

    cv::calcHist(
            &image,
            1,          //histog for 1 image only
            channels,   //channel used
            Mat(),      //no mask
            hist,       //the resulting histogram
            1,          //1D
            histSize,   //number of bins
            ranges      //pixel value range
            );
    return hist;
}

/**
 * @brief Histogram1D::getHistogramImage
 * @param u2 true if uniform LBP was used
 * @param image
 * @param max largest value possible for the histogram bins
 * @return a histogram image
 */
Mat Histogram1D::getHistogramImage(bool u2, const Mat &image, int max)
{
    Histogram1D h(u2,max);
    //Computes the 1D histogra
    Mat hist=h.getHistogram(image);
    //get min and max bin values:
    double maxVal=0;
    double minVal=0;
    minMaxLoc(hist, & minVal, & maxVal, 0,0);
    //image on which to display histogram:
    Mat histImg(histSize[0], histSize[0], CV_8U, Scalar(255));
    //set highest point at 90% of nbins:
    int hpt = static_cast<int>(0.9* histSize[0]);
    //draw a vertical line for each bin"
    float * basePtr_hist=(float*)hist.data;
    
    for(int h = 0; h < histSize[0];h++)
    {
        //float binVal = hist.at<float>(h);
        float binVal = *(basePtr_hist+h);
        int intensity = static_cast<int>(binVal * hpt/maxVal);
        //draws a line between two points:
        line(histImg, Point(h, histSize[0]), Point(h, histSize[0]-intensity),Scalar::all(0));

    }
    return histImg;
}


/**
 * Determines whether the 'out' value converted to binary is a 'uniform pattern'.
 * @param N number of neighbouring pixels
 * @param out LBP value of the pixel to be assessed
 * @return If value has <=2 changes: returns the value out    &    if not: returns -1
 */
int LBPHISTOGRAM::checkIfLBPu2Value(int N, int out){
    int count=0; //count number of binary switches
    int *bits=(int*)malloc(N*sizeof(int)); //used to hold each binary value
    for (int n=0;n<N;n++)
    {
        *(bits+n)=0;
    }
    //CONVERT TO BINARY:
    double div;
    div = ceil( (out/2.0) -0.5);
    *(bits+0) = out % 2;
    for (int ind = 1 ; div != 0.0 ; ind++)
    {
        *(bits+ind) = (int) div%2;
        div = ceil( (div / 2) -0.5 );
    }

    bool moreThan2 = false;

    for(int k = N-1; k > 0 && !moreThan2; k--){
        if(*(bits+k)!= *(bits+k-1)) //if there is a change
        {
            count++;
        }
        if (count>2)
        {
            moreThan2=true;
        }
    }
    delete(bits);
    if(count<=2)
    {
        return out;
    }
    else
    {
        return -1;    //set values that don't have <=2 changes to -1 and use the max value of possible <=2 changes +1 for these
    }
}

/**
 * @brief getAllUnifVals All possible binary numbers of size N are checked for whether they have <=2 changes
 * @param N number of neighbours (aka the length of the binary numbers)
 * @return vector containing the values of binary numbers of size N that have <=2 changes
 */
vector<int> LBPHISTOGRAM::getAllUnifVals(int N)
{
    vector<int> res; int index=0;
    for(int b=0; b <= pow(2,N); b++)
    {
        int value = checkIfLBPu2Value(N, b);
        if(value != -1)
        {
            res.insert( res.begin()+index, 1, value); //add to vector the value found to have <=2 changes
            index++;
        }
    }

    return res;
}

/**
 * @brief convertImageBins receives a vector of the values with <=2 changes for the particular N value and converts each non -1 value of the dest image to be its corresponding vector value. If the pixel = -1 then its value is set to the largest value.
 * @param dest image to be altered
 * @param bins values which have <=2 changes for the specified N
 */
void LBPHISTOGRAM::convertImageBins(Mat& dest, vector<int> bins)
{
    bool found=false;

    int *basePtr_dest=(int*)dest.data;
    
    for(int r=0; r < dest.rows; r++)
    {
        for(int c=0; c < dest.cols; c++)//navigate through the dest matrix pixels
        {
            int pixel = *(basePtr_dest+dest.cols*(r)+c);
            if (pixel != -1)// -1 would be a pixel with >2 changes
            {
                for(unsigned int i=0; i<bins.size() && !found; i++)//find the bin for the given pixel
                {
                    if (pixel==bins[i])
                    {
                        *(basePtr_dest+dest.cols*(r)+c)=i;
                        found = true;
                    }
                }
                found=false; //restore it for the next pixel
            }
            else
            {
                *(basePtr_dest+dest.cols*(r)+c) = bins.size();
            }
        }
    }
}

/**
 * Takes a ROI and outputs either the LBP or LBPu2 according to the desired radius and # of neighbors
 * @param input image for which the LBP/u2 is computed
 * @param dest holds the LBP/u2 values
 * @param u2 if true, LBPu is computed instead of non-uniform LBP
 * @param N number of neighbouring points to use
 * @param R radius within which to find the N neighbouring points
 * @return integer that is the largest pixel value possible in the output image matrix (dest)
 */
int LBPHISTOGRAM::LBPFilter(Mat *input, Mat& dest, bool u2, int N, int R)
{
    //pre-computed values for LBPU2 coding
    static int code8[] = {0,1,2,3,4,6,7,8,12,14,15,16,24,28,30,31,32,48,56,60,62,63,64,96,112,120,124,126,127,128,129,131,135,143,159,191,192,193,195,199,207,223,224,225,227,231,239,240,241,243,247,248,249,251,252,253,254,255};
    static int code16[] ={0, 1, 2, 3, 4, 6, 7, 8, 12, 14, 15, 16, 24, 28, 30, 31, 32, 48, 56, 60, 62, 63, 64, 96, 112, 120, 124, 126, 127, 128, 192, 224, 240, 248, 252, 254, 255, 256, 384, 448, 480, 496, 504, 508, 510, 511, 512, 768, 896, 960, 992, 1008, 1016, 1020, 1022, 1023, 1024, 1536, 1792, 1920, 1984, 2016, 2032, 2040, 2044, 2046, 2047, 2048, 3072, 3584, 3840, 3968, 4032, 4064, 4080, 4088, 4092, 4094, 4095, 4096, 6144, 7168, 7680, 7936, 8064, 8128, 8160, 8176, 8184, 8188, 8190, 8191, 8192, 12288, 14336, 15360, 15872, 16128, 16256, 16320, 16352, 16368, 16376, 16380, 16382, 16383, 16384, 24576, 28672, 30720, 31744, 32256, 32512, 32640, 32704, 32736, 32752, 32760, 32764, 32766, 32767, 32768, 32769, 32771, 32775, 32783, 32799, 32831, 32895, 33023, 33279, 33791, 34815, 36863, 40959, 49151, 49152, 49153, 49155, 49159, 49167, 49183, 49215, 49279, 49407, 49663, 50175, 51199, 53247, 57343, 57344, 57345, 57347, 57351, 57359, 57375, 57407, 57471, 57599, 57855, 58367, 59391, 61439, 61440, 61441, 61443, 61447, 61455, 61471, 61503, 61567, 61695, 61951, 62463, 63487, 63488, 63489, 63491, 63495, 63503, 63519, 63551, 63615, 63743, 63999, 64511, 64512, 64513, 64515, 64519, 64527, 64543, 64575, 64639, 64767, 65023, 65024, 65025, 65027, 65031, 65039, 65055, 65087, 65151, 65279, 65280, 65281, 65283, 65287, 65295, 65311, 65343, 65407, 65408, 65409, 65411, 65415, 65423, 65439, 65471, 65472, 65473, 65475, 65479, 65487, 65503, 65504, 65505, 65507, 65511, 65519, 65520, 65521, 65523, 65527, 65528, 65529, 65531, 65532, 65533, 65534, 65535, 65536};

    dest = Mat::zeros(input->rows,input->cols,DataType<int>::type);
    Mat temp=Mat::zeros(input->rows+2*R,input->cols+2*R,DataType<uchar>::type);
    input->copyTo(temp(Rect(R, R, input->cols, input->rows)));
    const double PI  =4.0*atan(1.0);
    double a =  (2*PI)/N;
    int max = 0;//largest possible pixel value for non-u2. Modified later if u2=true.
    uchar *basePtr_temp=(uchar*)temp.data;
    int *basePtr_dest=(int*)dest.data;
    for(int i=R;i<temp.rows-R;i++)
    {
        for(int j=R;j<temp.cols-R;j++)
        {
            int b = *(basePtr_temp+temp.cols*(i)+j);
            uchar out = 0;
            for (int ind =1; ind<=N; ind++)
            {
                int changeY = (int) ceil( ( (-R)* sin((ind-1)*a) )-0.5);
                int changeX = (int) ceil( ( (R) * cos((ind-1)*a) )-0.5);

                out += b > *(basePtr_temp+temp.cols*(i+changeY)+(j+changeX)) ? 0:(int)(pow(2,ind-1));
            }

            if(u2){
                if(N==8)
                {
                    *(basePtr_dest+dest.cols*(i-R)+(j-R))= 58;
                    for(int k = 0; k < 58; k++){
                        if(out == *(code8+k))
                        {
                            *(basePtr_dest+dest.cols*(i-R)+(j-R)) = k;
                            break;
                        }
                    }
                    max=58;
                }
                else if(N==16)
                {
                    *(basePtr_dest+dest.cols*(i-R)+(j-R)) = 243;
                    for(int k = 0; k < 243; k++){
                        if(out == *(code16+k))
                        {
                            *(basePtr_dest+dest.cols*(i-R)+(j-R)) = k;
                            break;
                        }
                    }
                    max = 243;
                }
                else //all other values of N
                {
                    out = checkIfLBPu2Value(N, out);//checkIfLBPu2Value returns the value (decimal) that in binary has <=2 changes OR -1 if it has >2 changes
                    *(basePtr_dest+dest.cols*(i-R)+(j-R)) =  out;
                }
            }
            else
            {
                *(basePtr_dest+dest.cols*(i-R)+(j-R)) =  out;
            }
        }
    }
    if (N!=8 && N!=16 && u2) //do the conversion for N!=8 or 16 since those are pre-computed
    {
        vector<int> possibleUniformBins = getAllUnifVals(N);
        max=possibleUniformBins.size();
        convertImageBins(dest, possibleUniformBins);
    }
    if(!u2)
    {
        max = pow(2,N)-1;
    }
    return max; //maximum value in the image pixels (for u2, the max value = the value given to all pixels with >2 changes)
}


int LBPHISTOGRAM::Local_LBPFilter(Mat *input, Mat& dest, bool u2, int N, int R, int X, int Y,int blk_size)
{
    //pre-computed values for LBPU2 coding
    static int code8[] = {0,1,2,3,4,6,7,8,12,14,15,16,24,28,30,31,32,48,56,60,62,63,64,96,112,120,124,126,127,128,129,131,135,143,159,191,192,193,195,199,207,223,224,225,227,231,239,240,241,243,247,248,249,251,252,253,254,255};
    static int code16[] ={0, 1, 2, 3, 4, 6, 7, 8, 12, 14, 15, 16, 24, 28, 30, 31, 32, 48, 56, 60, 62, 63, 64, 96, 112, 120, 124, 126, 127, 128, 192, 224, 240, 248, 252, 254, 255, 256, 384, 448, 480, 496, 504, 508, 510, 511, 512, 768, 896, 960, 992, 1008, 1016, 1020, 1022, 1023, 1024, 1536, 1792, 1920, 1984, 2016, 2032, 2040, 2044, 2046, 2047, 2048, 3072, 3584, 3840, 3968, 4032, 4064, 4080, 4088, 4092, 4094, 4095, 4096, 6144, 7168, 7680, 7936, 8064, 8128, 8160, 8176, 8184, 8188, 8190, 8191, 8192, 12288, 14336, 15360, 15872, 16128, 16256, 16320, 16352, 16368, 16376, 16380, 16382, 16383, 16384, 24576, 28672, 30720, 31744, 32256, 32512, 32640, 32704, 32736, 32752, 32760, 32764, 32766, 32767, 32768, 32769, 32771, 32775, 32783, 32799, 32831, 32895, 33023, 33279, 33791, 34815, 36863, 40959, 49151, 49152, 49153, 49155, 49159, 49167, 49183, 49215, 49279, 49407, 49663, 50175, 51199, 53247, 57343, 57344, 57345, 57347, 57351, 57359, 57375, 57407, 57471, 57599, 57855, 58367, 59391, 61439, 61440, 61441, 61443, 61447, 61455, 61471, 61503, 61567, 61695, 61951, 62463, 63487, 63488, 63489, 63491, 63495, 63503, 63519, 63551, 63615, 63743, 63999, 64511, 64512, 64513, 64515, 64519, 64527, 64543, 64575, 64639, 64767, 65023, 65024, 65025, 65027, 65031, 65039, 65055, 65087, 65151, 65279, 65280, 65281, 65283, 65287, 65295, 65311, 65343, 65407, 65408, 65409, 65411, 65415, 65423, 65439, 65471, 65472, 65473, 65475, 65479, 65487, 65503, 65504, 65505, 65507, 65511, 65519, 65520, 65521, 65523, 65527, 65528, 65529, 65531, 65532, 65533, 65534, 65535, 65536};

    dest = Mat::zeros(blk_size,blk_size,DataType<int>::type);
	Mat temp=Mat::zeros(input->rows+2*R,input->cols+2*R,input->type());
    input->copyTo(temp(Rect(R, R, input->cols, input->rows)));
	temp.convertTo(temp,DataType<uchar>::type);
    const double PI  =4.0*atan(1.0);
    double a =  (2*PI)/N;
    int max = 0;//largest possible pixel value for non-u2. Modified later if u2=true.
    uchar *basePtr_temp=(uchar*)temp.data;
    int *basePtr_dest=(int*)dest.data;

    for(int i=R+Y;i<R+Y+blk_size;i++)
    {
        for(int j=R+X;j<R+X+blk_size;j++)
        {
            int b = *(basePtr_temp+temp.cols*(i)+j);
            uchar out = 0;
            for (int ind =1; ind<=N; ind++)
            {
                int changeY = (int) ceil( ( (-R)* sin((ind-1)*a) )-0.5);
                int changeX = (int) ceil( ( (R) * cos((ind-1)*a) )-0.5);

                out += b > *(basePtr_temp+temp.cols*(i+changeY)+(j+changeX)) ? 0:(int)(pow(2,ind-1));
            }

            if(u2){
                if(N==8)
                {
                    *(basePtr_dest+dest.cols*(i-R-Y)+(j-R-X))= 58;
                    for(int k = 0; k < 58; k++){
                        if(out == *(code8+k))
                        {
                            *(basePtr_dest+dest.cols*(i-R-Y)+(j-R-X)) = k;
                            break;
                        }
                    }
                    max=58;
                }
                else if(N==16)
                {
                    *(basePtr_dest+dest.cols*(i-R-Y)+(j-R-X)) = 243;
                    for(int k = 0; k < 243; k++){
                        if(out == *(code16+k))
                        {
                            *(basePtr_dest+dest.cols*(i-R-Y)+(j-R-X)) = k;
                            break;
                        }
                    }
                    max = 243;
                }
                else //all other values of N
                {
                    out = checkIfLBPu2Value(N, out);//checkIfLBPu2Value returns the value (decimal) that in binary has <=2 changes OR -1 if it has >2 changes
                    *(basePtr_dest+dest.cols*(i-R-Y)+(j-R-X)) =  out;
                }
            }
            else
            {
                *(basePtr_dest+dest.cols*(i-R-Y)+(j-R-X)) =  out;
            }
        }
    }
    if (N!=8 && N!=16 && u2) //do the conversion for N!=8 or 16 since those are pre-computed
    {
        vector<int> possibleUniformBins = getAllUnifVals(N);
        max=possibleUniformBins.size();
        convertImageBins(dest, possibleUniformBins);
    }
    if(!u2)
    {
        max = pow(2,N)-1;
    }
    return max; //maximum value in the image pixels (for u2, the max value = the value given to all pixels with >2 changes)
}




/**
 * @brief ROItoHistogram Takes an input image ROI  and returns the histogram matrix values for the ROI LBP/LBPu2
 * @param ROI image for which LBP/u2 is computed
 * @param u2 true for LBPu instead of LBP
 * @param N number of neighbour points to use
 * @param R radius within which to find the N neighbours
 * @return normalized histogram of the LBP/u2 values computed
 */
Mat LBPHISTOGRAM::ROItoHistogram(Mat result, bool u2, int max) //bool u2, int N, int R)
{
    Mat histogram;
    ///    ROI.convertTo(ROI, CV_32SC1);

    //Create the histogram:
    Histogram1D h = Histogram1D(u2, max);
    histogram = h.getHistogram(result);

    return histogram;
}

LBPHISTOGRAM::LBPHISTOGRAM(){
    //Train_File="Train.bin";
    Train_LbL="Train_LbL2.txt";

	Load_Gender_Points("Models\\gender_par.txt");
    Load_Age_Points("Models\\age_par.txt");
	SVM_LBP_Train();
    is_rotated=false;
    


}


Mat LBPHISTOGRAM::PatchHistogram(Mat *warp_dst){


    //GET LBP RENDITION OF WARP_DST & FRAGMENT THE IMAGE INTO 10x10 BLOCKS:
    int w = DEFAULT_BLOCK_SIZE;
    int h = DEFAULT_BLOCK_SIZE;
    Mat histograms(DEFAULT_BLOCK_SIZE*DEFAULT_BLOCK_SIZE*59, 1, DataType<int>::type); //type code 0 = CV_8U (the int code value yielded by calling .type() for the hist returned from ROItoHistogram)
    int *basePtr_histograms = (int*)histograms.data;
    int histInd = 0;
    Mat LBP_image; //LBP rendition of warp_dst
    int max = LBPFilter(warp_dst,LBP_image, 1, 8, 4); //
    for(int i = 0; i < DEFAULT_NUM_BLOCKS; i++)
    {
        int y = i * DEFAULT_NUM_BLOCKS;
        for(int n = 0; n < DEFAULT_NUM_BLOCKS; n++)
        {
            int x = n * DEFAULT_NUM_BLOCKS;
            Mat ROI;
            Mat(LBP_image, Rect(x, y, w, h)).copyTo(ROI);
            //GET AND GATHER uLBP HISTOGRAMS FOR ALL BLOCKS:

            ROI.convertTo(ROI,CV_8UC1);
            Mat hist = ROItoHistogram(ROI, 1, max);
            hist.convertTo(hist, DataType<int>::type);
            int *basePtr_hist = (int*)hist.data;
            //Add the histogram to the rest:
            for(int d = 0; d < hist.rows && histInd < histograms.rows; d++)//each histogram bin
            {
                //store content of hist at d into histograms at histInd:
                *(basePtr_histograms + histInd) = *(basePtr_hist + d);
                histInd++;
            }
        }
    }

    return histograms;

}

Mat LBPHISTOGRAM::GENDER_descriptor(Mat *warp_dst){
    Mat histograms(Size(gender_points.size()*59, 1), DataType<float>::type); //type code 0 = CV_8U (the int code value yielded by calling .type() for the hist returned from ROItoHistogram)
    float *basePtr_histograms = (float*)histograms.data;
    int histInd = 0;
    Mat hist2;
    int max;
    for(int i=0;i<gender_points.size();i++){
        Mat ROI;
        int BLOCK_NUM=warp_dst->rows/gender_points[i].blk_siz;
        int offset=floor((warp_dst->rows-BLOCK_NUM*gender_points[i].blk_siz)/2);
        max = Local_LBPFilter(warp_dst,ROI, 1, 8, gender_points[i].radius,gender_points[i].x+offset,gender_points[i].y+offset,gender_points[i].blk_siz); //
        ROI.convertTo(ROI,CV_8UC1);
        Mat hist = ROItoHistogram(ROI, 0, max);

        cv::transpose(hist,hist);

        hist.convertTo(hist, DataType<int>::type);
        norm_1(hist, hist2);
        float *basePtr_hist2 = (float*)hist2.data;
        for(int d = 0; d < hist2.cols && histInd < histograms.cols; d++)//each histogram bin
        {
            //store content of hist at d into histograms at histInd:
            *(basePtr_histograms + histInd) = *(basePtr_hist2 + d);
            histInd++;
        }
    }
    return histograms;
}

Mat LBPHISTOGRAM::AGE_descriptor(Mat *warp_dst){

    Mat histograms(Size(age_points.size()*59, 1), DataType<float>::type); //type code 0 = CV_8U (the int code value yielded by calling .type() for the hist returned from ROItoHistogram)
    float *basePtr_histograms = (float*)histograms.data;
    int histInd = 0;
    Mat hist2;
    int max;
    for(int i=0;i<age_points.size();i++){
        Mat ROI;
        int BLOCK_NUM=warp_dst->rows/age_points[i].blk_siz;
        int offset=floor((warp_dst->rows-BLOCK_NUM*age_points[i].blk_siz)/2);
        max = Local_LBPFilter(warp_dst,ROI, 1, 8, age_points[i].radius,age_points[i].x+offset,age_points[i].y+offset,age_points[i].blk_siz); //
        ROI.convertTo(ROI,CV_8UC1);
        Mat hist = ROItoHistogram(ROI, 0, max);

        cv::transpose(hist,hist);

        hist.convertTo(hist, DataType<int>::type);
        norm_1(hist, hist2);
        float *basePtr_hist2 = (float*)hist2.data;
        for(int d = 0; d < hist2.cols && histInd < histograms.cols; d++)//each histogram bin
        {
            //store content of hist at d into histograms at histInd:
            *(basePtr_histograms + histInd) = *(basePtr_hist2 + d);
            histInd++;
        }
    }
    return histograms;
}


Mat LBPHISTOGRAM::rotate(Mat *image_input, int x_1, int y_1, int x_2, int y_2, int length )
{
    is_rotated=false;
    Mat image;
    image=image_input->clone();
    double angle=-atan((double(y_2-y_1))/(double(x_2-x_1)))*(180.0/(4.0*atan(1.0)));

    Point2f src_center(x_1, y_1);

    Mat rot_matrix = getRotationMatrix2D(src_center, -angle, 1.0);

    Mat rotated_img(Size(image.size().width, image.size().height), image.type());

    warpAffine(image, rotated_img, rot_matrix, rotated_img.size());
    Mat scaled_img;
    int past_length=sqrt(pow((x_2-x_1),2)+pow((y_2-y_1),2));
    double ratio=((double)past_length/(double)length);
    int new_cols=((double)rotated_img.cols/ratio);
    int new_rows=((double)rotated_img.rows/ratio);

    int new_x_1=(double)x_1*((double)length/(double)past_length);
    int new_y_1=(double)y_1*((double)length/(double)past_length);

    cv::resize(rotated_img,scaled_img,Size(new_cols,new_rows),0,0,INTER_LINEAR);
    Mat ROI;
	int left_x,left_y, width,height;
	(new_x_1-XL_eye>=0)?left_x=(new_x_1-XL_eye):left_x=0;
	(new_y_1-YL_eye >=0)?left_y=(new_y_1-YL_eye):left_y=0;
	(left_x+200<scaled_img.cols)?(width=200):(width=scaled_img.cols-left_x);
	(left_y+200<scaled_img.rows)?(height=200):(height=scaled_img.rows-left_y);

	ROI=scaled_img(Rect(left_x,left_y,width,height));
    cv::resize(ROI,ROI,Size(100,100),0,0,INTER_LINEAR);
    is_rotated=true;
    
    image.release();
    return ROI;
}

void LBPHISTOGRAM::norm_1(Mat input, Mat &output){

    output=Mat::zeros(input.rows,input.cols,DataType<float>::type);
    int* basePtr_input=(int*)input.data;
    float* basePtr_output=(float*)output.data;
    for(int i=0;i<input.cols;i++)
        *(basePtr_output+i)=*(basePtr_input+i);//sum
}

void LBPHISTOGRAM::SVM_LBP_Train(){

    GENDER_SVM.load("Models//gender_svm.bin");
    AGE_SVM.load("Models//age_svm.bin");
}

int LBPHISTOGRAM::SVM_Predictor(Mat *test_image){

    int val=0;
    Mat testMat_=PatchHistogram(test_image);
    Mat testMat=Mat::zeros(1,numOPtch,DataType<float>::type);
    float* Ptr_testMat=(float*)testMat.data;
    int* Ptr_testMat_=(int*)testMat_.data;
    for(int j=0;j<numOPtch;j++){
        *(Ptr_testMat+j)=*(Ptr_testMat_+j);
    }
    float response=MYSVM.predict(testMat);
    if(response>0)
        val=1;
    else
        val=-1;
    return val;
}
int LBPHISTOGRAM::gender_predictor(Mat *test_image){

    int val=0;
    Mat testMat_=GENDER_descriptor(test_image);
    Mat testMat=Mat::zeros(1,gender_numOPtch,DataType<float>::type);
    float* Ptr_testMat=(float*)testMat.data;
    float* Ptr_testMat_=(float*)testMat_.data;
    for(int j=0;j<gender_numOPtch;j++){
        *(Ptr_testMat+j)=sqrt(*(Ptr_testMat_+j));
    }
    float response=GENDER_SVM.predict(testMat);
    if(response>0)
        val=1;
    else
        val=0;
    return val;
}
int LBPHISTOGRAM::age_predictor(Mat *test_image){

    Point2i val;
    val.x=0;
    val.y=0;
    Mat testMat_=AGE_descriptor(test_image);

    Mat testMat=Mat::zeros(1,age_numOPtch,DataType<float>::type);
    float* Ptr_testMat=(float*)testMat.data;
    float* Ptr_testMat_=(float*)testMat_.data;
    for(int j=0;j<age_numOPtch;j++){
        *(Ptr_testMat+j)=sqrt(*(Ptr_testMat_+j));
    }
    float response=AGE_SVM.predict(testMat);
    int response_i=-1;
    if(response<1.5)
        response_i=0;
    else if(response<2.5)
        response_i=1;
    else
        response_i=2;

    return response_i;
}


bool LBPHISTOGRAM::Load_Gender_Points(string file_name){
    ifstream input;
    input.open(file_name.c_str());
    if(!input.is_open())
    {
        printf("Gender-Descriptors' path is not correct \n");
        return false;
    }
    else
    {
        int counter=0;
        int ti,tj,tk;
        string str;
        while (getline( input, str))
        {
            if(counter>=2000)
                break;
            Hist_Points a;
            ti=str.find_first_of(",");
            tj=str.find_first_of(":");
            tk=str.find_first_of(";");
            a.y=atoi(str.substr(0,ti).c_str());
            a.x=atoi(str.substr(ti+1,tj-ti-1).c_str());
            a.radius=atoi(str.substr(tj+1,tk-ti-1).c_str());
            a.blk_siz=atoi(str.substr(tk+1,str.length()-tk-1).c_str());
            gender_points.push_back(a);
            counter++;
        }
        input.close();
    }
    gender_numOPtch=gender_points.size()*59;
    return true;
}


bool LBPHISTOGRAM::Load_Age_Points(string file_name){
    ifstream input;
    input.open(file_name.c_str());
    if(!input.is_open())
    {
        printf("Age-Descriptors' path is not correct \n");
        return false;
    }
    else
    {
        int counter=0;
        int ti,tj,tk;
        string str;
        while (getline( input, str))
        {
            if(counter>=2000)
                break;
            Hist_Points a;
            ti=str.find_first_of(",");
            tj=str.find_first_of(":");
            tk=str.find_first_of(";");
            a.y=atoi(str.substr(0,ti).c_str());
            a.x=atoi(str.substr(ti+1,tj-ti-1).c_str());
            a.radius=atoi(str.substr(tj+1,tk-ti-1).c_str());
            a.blk_siz=atoi(str.substr(tk+1,str.length()-tk-1).c_str());
            age_points.push_back(a);
            counter++;
        }
        input.close();
    }
    age_numOPtch=age_points.size()*59;
    return true;

}