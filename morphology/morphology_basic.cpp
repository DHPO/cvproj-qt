#include "./morphology_basic.h"
#include "../expect/expect.h"
#include "../matrix/matrix_map.h"

Mat kernelFlip(const Mat &kernel)
{
    expect(kernel.type() == CV_16SC1, "kernelFlip - invalid kernel type");
    int cols = kernel.cols;
    int rows = kernel.rows;

    Mat result = Mat(rows, cols, kernel.type());
    for (int r = 0; r < rows; r++) {
        const short *pk = kernel.ptr<short>(r);
        short *pr = result.ptr<short>(rows - r - 1);
        for (int c = 0; c < cols; c++) {
            pr[cols - c -1] = pk[c];
        }
    }

    return result;
}

Mat dilate(const Mat &img, const Mat &kernel, Point anchor)
{
    int halfRows = kernel.rows / 2;
    int halfCols = kernel.cols / 2;

    if (anchor == Point(-1, -1))
        anchor = Point(halfRows, halfCols);

    Mat result(img.rows, img.cols, CV_8UC1);
    
    for (int r = 0; r < img.rows; r++) {
        for (int c = 0; c < img.cols; c++) {
            int temp = 0;
            for (int rr = max(0, r - anchor.x); rr < min(img.rows, r + anchor.x); rr ++) {
                for (int cc = max(0, c - anchor.y); cc < min(img.cols, c + anchor.y); cc ++) {
                    if (kernel.at<short>(rr - r + anchor.x, cc - c + anchor.y) != -1) {
                        int tmp = kernel.at<short>(rr - r + anchor.x, cc - c + anchor.y) + img.at<uchar>(rr, cc);
                        temp = max(temp, tmp);
                    }
                }
            }
            result.at<uchar>(r, c) = temp > 0 ? temp - 1 : temp;
        }
    }

    return result;
}

Mat erode(const Mat &img, const Mat &kernel, Point anchor)
{
    int halfRows = kernel.rows / 2;
    int halfCols = kernel.cols / 2;

    if (anchor == Point(-1, -1))
        anchor = Point(halfRows, halfCols);

    Mat result(img.rows, img.cols, CV_8UC1);
    
    for (int r = 0; r < img.rows; r++) {
        for (int c = 0; c < img.cols; c++) {
            int temp = 255;
            for (int rr = max(0, r - anchor.x); rr < min(img.rows, r + anchor.x); rr ++) {
                for (int cc = max(0, c - anchor.y); cc < min(img.cols, c + anchor.y); cc ++) {
                    if (kernel.at<short>(rr - r + anchor.x, cc - c + anchor.y) != -1) {
                        int tmp = img.at<uchar>(rr, cc) - kernel.at<short>(rr - r + anchor.x, cc - c + anchor.y);
                        temp = min(temp, tmp);
                    }
                }
            }
            result.at<uchar>(r, c) = temp < 255 ? temp + 1 : temp;
        }
    }

    return result;
}

Mat open(const Mat &img, const Mat &kernel, Point anchor)
{
    return dilate(erode(img, kernel, anchor), kernel, anchor);
}

Mat close(const Mat &img, const Mat &kernel, Point anchor)
{
    return erode(dilate(img ,kernel, anchor), kernel, anchor);
}

class MatSub: public MatOperator<uchar, 1>
{
    Vec<uchar, 1> op(Vec<uchar, 1> d1, Vec<uchar, 1> d2)
    {
        return Vec<uchar, 1>(d1[0] - d2[0]);
    }
};

Mat morphGrad(const Mat &img, const Mat &kernel, Point anchor)
{
    return MatSub().doOp(dilate(img, kernel, anchor), erode(img, kernel, anchor));
}

Mat tophat(const Mat &img, const Mat &kernel, Point anchor) {
    return MatSub().doOp(img, open(img, kernel, anchor));
}

Mat blackhat(const Mat &img, const Mat &kernel, Point anchor) {
    return MatSub().doOp(close(img, kernel, anchor), img);
}