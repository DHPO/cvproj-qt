#include "./morphology_basic.h"
#include "../expect/expect.h"
#include "../matrix/matrix_map.h"
#include "../matrix/matrix_math.h"
#include "../filter/filter_difference.h"
#include <set>
using namespace std;

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

Mat dilate_b(const Mat &img, const Mat &kernel, Point anchor, uchar mark)
{
    expect(kernel.type() == CV_16SC1, "dilate - invalid kernel type");

    int halfRows = kernel.rows / 2;
    int halfCols = kernel.cols / 2;

    if (anchor == Point(-1, -1))
        anchor = Point(halfRows, halfCols);

    Mat result(img.rows, img.cols, CV_8UC1);
    
    for (int r = 0; r < img.rows; r++) {
        for (int c = 0; c < img.cols; c++) {
            int temp = 0;
            for (int rr = max(0, r - anchor.x); rr <= min(img.rows, r + anchor.x); rr ++) {
                for (int cc = max(0, c - anchor.y); cc <= min(img.cols, c + anchor.y); cc ++) {
                    if (kernel.at<short>(rr - r + anchor.x, cc - c + anchor.y) == 1 && img.at<uchar>(rr, cc) == 255) {
                        result.at<uchar>(r, c) = 255;
                        goto out;
                    }
                }
            }
            result.at<uchar>(r, c) = mark;
            out:;
        }
    }

    return result;
}

Mat dilate(const Mat &img, const Mat &kernel, Point anchor)
{
    expect(kernel.type() == CV_16SC1, "dilate - invalid kernel type");

    int halfRows = kernel.rows / 2;
    int halfCols = kernel.cols / 2;

    if (anchor == Point(-1, -1))
        anchor = Point(halfRows, halfCols);

    Mat result(img.rows, img.cols, CV_8UC1);
    
    for (int r = 0; r < img.rows; r++) {
        for (int c = 0; c < img.cols; c++) {
            int temp = 0;
            for (int rr = max(0, r - anchor.x); rr <= min(img.rows, r + anchor.x); rr ++) {
                for (int cc = max(0, c - anchor.y); cc <= min(img.cols, c + anchor.y); cc ++) {
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

Mat erode_b(const Mat &img, const Mat &kernel, Point anchor, uchar mark)
{
    expect(kernel.type() == CV_16SC1, "erode - invalid kernel type");

    int halfRows = kernel.rows / 2;
    int halfCols = kernel.cols / 2;

    if (anchor == Point(-1, -1))
        anchor = Point(halfRows, halfCols);

    Mat result(img.rows, img.cols, CV_8UC1, Scalar(0));
    
    for (int r = 0; r < img.rows; r++) {
        for (int c = 0; c < img.cols; c++) {
            int temp = 255;
            for (int rr = max(0, r - anchor.x); rr <= min(img.rows, r + anchor.x); rr ++) {
                for (int cc = max(0, c - anchor.y); cc <= min(img.cols, c + anchor.y); cc ++) {
                    if (kernel.at<short>(rr - r + anchor.x, cc - c + anchor.y) == 1 && img.at<uchar>(rr, cc) == 0) {
//                        result.at<uchar>(r, c) = 0;
                        goto out;
                    }
                }
            }
            result.at<uchar>(r, c) = mark;
            out:;
        }
    }

    return result;
}

Mat erode(const Mat &img, const Mat &kernel, Point anchor)
{
    expect(kernel.type() == CV_16SC1, "erode - invalid kernel type");

    int halfRows = kernel.rows / 2;
    int halfCols = kernel.cols / 2;

    if (anchor == Point(-1, -1))
        anchor = Point(halfRows, halfCols);

    Mat result(img.rows, img.cols, CV_8UC1);
    
    for (int r = 0; r < img.rows; r++) {
        for (int c = 0; c < img.cols; c++) {
            int temp = 255;
            for (int rr = max(0, r - anchor.x); rr <= min(img.rows, r + anchor.x); rr ++) {
                for (int cc = max(0, c - anchor.y); cc <= min(img.cols, c + anchor.y); cc ++) {
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

Mat open_b(const Mat &img, const Mat &kernel, Point anchor)
{
    return dilate_b(erode_b(img, kernel, anchor), kernel, anchor);
}

Mat close_b(const Mat &img, const Mat &kernel, Point anchor)
{
    return erode_b(dilate_b(img ,kernel, anchor), kernel, anchor);
}

Mat open_g(const Mat &img, const Mat &kernel, Point anchor)
{
    return dilate(erode(img, kernel, anchor), kernel, anchor);
}

Mat close_g(const Mat &img, const Mat &kernel, Point anchor)
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

Mat morphGrad_b(const Mat &img, const Mat &kernel, Point anchor)
{
    return MatSub().doOp(dilate_b(img, kernel, anchor), erode_b(img, kernel, anchor));
}

Mat tophat_b(const Mat &img, const Mat &kernel, Point anchor) {
    return MatSub().doOp(img, open_b(img, kernel, anchor));
}

Mat blackhat_b(const Mat &img, const Mat &kernel, Point anchor) {
    return MatSub().doOp(close_b(img, kernel, anchor), img);
}

Mat morphGrad(const Mat &img, const Mat &kernel, Point anchor)
{
    return MatSub().doOp(dilate(img, kernel, anchor), erode(img, kernel, anchor));
}

Mat tophat(const Mat &img, const Mat &kernel, Point anchor) {
    return MatSub().doOp(img, open_g(img, kernel, anchor));
}

Mat blackhat(const Mat &img, const Mat &kernel, Point anchor) {
    return MatSub().doOp(close_g(img, kernel, anchor), img);
}

bool hitormissKernelLoop(const Mat &img, const Mat &kernel, Point2i pos, Point anchor)
{
    int r = pos.x;
    int c = pos.y;
    if (r == 0 || r == img.rows - 1 || c == 0 || c == img.cols - 1)
        return false;
    for (int rr = max(0, r - anchor.x); rr <= min(img.rows, r + anchor.x); rr ++) {
        for (int cc = max(0, c - anchor.y); cc <= min(img.cols, c + anchor.y); cc ++) {
            short kv = kernel.at<short>(rr - r + anchor.x, cc - c + anchor.y);
            uchar iv = img.at<uchar>(rr, cc);
            if ((kv == 0 && iv == 255) || (kv == 1 && iv == 0)) {
                return false;
            }
        }
    }
    return true;
}

Mat hitormiss(const Mat &img, const vector<Mat> &kernels, Point anchor, uchar mark)
{
    expect(kernels[0].type() == CV_16SC1, "hitormiss - invalid kernel type");

    int halfRows = kernels[0].rows / 2;
    int halfCols = kernels[0].cols / 2;

    if (anchor == Point(-1, -1))
        anchor = Point(halfRows, halfCols);

    Mat result(img.rows, img.cols, CV_8UC1, Scalar(0));

    for (int r = 0; r < img.rows; r++) {
        for (int c = 0; c < img.cols; c++) {
            for (const Mat &kernel : kernels) {
                if (hitormissKernelLoop(img, kernel, Point2i(r, c), anchor)) {
                    result.at<uchar>(r, c) = mark;
                    goto out;
                }
            }
            result.at<uchar>(r, c) = 0;
            out:;
        }
    }

    return result;
}

void defaultKernels(vector<Mat> &result)
{
    result.clear();
    for (int i = 0; i < 8; i++) {
        result.push_back(Mat(3, 3, CV_16SC1));
    }
    short kernel0_data[]= {
         0, 0,  0,
        -1, 1, -1,
         1, 1,  1
    };
    Mat kernel0(3, 3, CV_16SC1, kernel0_data);
    kernel0.copyTo(result[0]);

    short kernel1_data[]= {
         -1, 0,  0,
          1, 1,  0,
          1, 1, -1
    };
    Mat kernel1(3, 3, CV_16SC1, kernel1_data);
    kernel1.copyTo(result[1]);

    short kernel2_data[]= {
         1, -1, 0,
         1,  1, 0,
         1, -1, 0
    };
    Mat kernel2(3, 3, CV_16SC1, kernel2_data);
    kernel2.copyTo(result[2]);

    short kernel3_data[]= {
        1, 1,-1,
        1, 1, 0,
       -1, 0, 0
    };

    Mat kernel3(3, 3, CV_16SC1, kernel3_data);
    kernel3.copyTo(result[3]);

    short kernel4_data[]= {
         1, 1,  1,
        -1, 1, -1,
         0, 0,  0
    };
    Mat kernel4(3, 3, CV_16SC1, kernel4_data);
    kernel4.copyTo(result[4]);

    short kernel5_data[]= {
       -1, 1,  1,
        0, 1,  1,
        0, 0, -1
    };
    Mat kernel5(3, 3, CV_16SC1, kernel5_data);
    kernel5.copyTo(result[5]);

    short kernel6_data[]= {
        0, -1, 1,
        0,  1, 1,
        0, -1, 1
    };
    Mat kernel6(3, 3, CV_16SC1, kernel6_data);
    kernel6.copyTo(result[6]);

    short kernel7_data[]= {
        0, 0, -1,
        0, 1,  1,
       -1, 1,  1
    };
    Mat kernel7(3, 3, CV_16SC1, kernel7_data);
    kernel7.copyTo(result[7]);
}


template<typename T, int channels>
class MatAdd: public MatOperator<T, channels>
{
    Vec<T, channels> op(Vec<T, channels> d1, Vec<T, channels> d2)
    {
        return d1 + d2;
    }
};

Mat thick(const Mat &img)
{
    Mat write(img.rows, img.cols, img.type(), Scalar::all(255));
    Mat reverse = MatSub().doOp(write, img);
    vector<Mat> kernels;
    defaultKernels(kernels);
    return MatAdd<uchar, 1>().doOp(img, hitormiss(reverse, kernels));
}

Mat thin(const Mat &img)
{
    vector<Mat> kernels;
    defaultKernels(kernels);
    return MatSub().doOp(img, hitormiss(img, kernels));
}

class MatMax: public MatOperator<uchar, 1>
{
    Vec<uchar, 1> op(Vec<uchar, 1> d1, Vec<uchar, 1> d2)
    {
        return Vec<uchar, 1>(max(d1[0], d2[0]));
    }
};

Mat distanceConvert(const Mat &img)
{
    short kernel_data[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Mat kernel(3, 3, CV_16SC1, kernel_data);

    Mat temp = img.clone();
    Mat black(img.rows, img.cols, CV_8UC1, Scalar(0));
    Mat result(img.rows, img.cols, CV_8UC1, Scalar(0));
    for(int i = 1; i < 255; i++) {
        temp = erode_b(temp, kernel, Point(-1, -1), i);
        result = MatMax().doOp(temp, result);
        if (matEqual(black, temp))
            break;
    }
    return result;
}

Mat bone(const Mat &img)
{
    /*Mat result = img.clone();
    Mat temp = result.clone();
    do {
        result = temp.clone();
        temp = thin(temp);
    }while (!matEqual(temp, result));*/

    Mat temp = distanceConvert(img);
    Mat result(temp.rows, temp.cols, CV_8UC1, Scalar(0));

    for (int r = 1; r < temp.rows - 1; r++) {
        for (int c = 1; c < temp.cols - 1; c++) {
            uchar v = temp.at<uchar>(r, c);
            float cnt =
                ((v > temp.at<uchar>(r + 1, c)) + (v >= temp.at<uchar>(r - 1, c))) +
                ((v > temp.at<uchar>(r + 1, c + 1)) * 0.4 + (v > temp.at<uchar>(r - 1, c - 1)) * 0.4) +
                ((v > temp.at<uchar>(r + 1, c - 1)) * 0.4 + (v > temp.at<uchar>(r - 1, c + 1)) * 0.4) +
                ((v > temp.at<uchar>(r, c + 1)) + (v >= temp.at<uchar>(r, c - 1)));
            if (cnt >= 4)
                result.at<uchar>(r, c) = 255;
        }
    }

    return result;
}

class MatIntersect : public MatOperator<uchar, 1>
{
    Vec<uchar, 1> op(Vec<uchar, 1> d1, Vec<uchar, 1> d2)
    {
        return Vec<uchar, 1>(d1[0] == 255 && d2[0] == 255 ? 255 : 0);
    }
};

Mat reconstruct(const Mat &img, const Mat &mark)
{
    short kernel_data[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Mat kernel(3, 3, CV_16SC1, kernel_data);

    Mat temp = mark.clone();
    Mat prev = temp.clone();
    do{
        prev = temp.clone();
        temp = dilate(temp, kernel);
        temp = MatIntersect().doOp(temp, img);
    } while (!matEqual(prev, temp));

    return temp;
}

vector<int> marksAround(const Mat &mark, Point2i point)
{
    int r = point.x, c = point.y;
    set<int> marks;
    vector<int> result;
    if (r > 0 && mark.at<int>(r - 1, c) != 0)
        marks.insert(mark.at<int>(r - 1, c));
    if (r < mark.rows - 1 && mark.at<int>(r + 1, c) != 0)
        marks.insert(mark.at<int>(r + 1, c));
    if (c > 0 && mark.at<int>(r, c - 1) != 0)
        marks.insert(mark.at<int>(r, c - 1));
    if (c < mark.cols - 1 && mark.at<int>(r, c + 1) != 0)
        marks.insert(mark.at<int>(r, c + 1));

    for (int mark : marks)
        result.push_back(mark);

    return result;
}

int getRoot(int mark, const map<int, int> &unions)
{
    int root = mark;
    while(unions.find(root) != unions.end()) {
        root = unions.at(root);
    }
    return root;
}

Mat watershed(const Mat &img, int threshold)
{
    vector<vector<Point2i>> pixels(256);

    for (int r = 0; r < img.rows; r++) {
        for (int c = 0; c < img.cols; c++) {
            uchar val = img.at<uchar>(r, c);
            pixels[val / threshold].push_back(Point2i(r, c));
        }
    }

    Mat mark(img.rows, img.cols, CV_32SC1, Scalar(0));
    Mat shed(img.rows, img.cols, CV_8UC1, Scalar(0));
    map<int, int> markUnion;
    vector<int> depth;
    int maxMark = 0;

    for (int w = 0; w <= 255/threshold; w += 1) {
        for (auto point: pixels[w]) {
            vector<int> marks = marksAround(mark, point);
            if (marks.size() == 0) {
                /*if (mark.at<int>(point.x, point.y) != 0)
                    continue;*/
                int newMark = (++maxMark);
                mark.at<int>(point.x, point.y) = newMark;
                depth.push_back(w);
                /*stack<Point2i> stk;
                stk.push(point);
                while (!stk.empty()) {
                    Point2i point = stk.top();
                    stk.pop();
                    int r = point.x, c = point.y;
                    if (r > 0 && mark.at<int>(r - 1, c) == 0 && img.at<uchar>(r - 1, c) <= w * threshold) {
                        mark.at<int>(r - 1, c) = newMark;
                        stk.push(Point2i(r - 1, c));
                    }
                    if (r < mark.rows - 1 && mark.at<int>(r + 1, c) == 0 && img.at<uchar>(r + 1, c) <= w * threshold ) {
                        mark.at<int>(r + 1, c) = newMark;
                        stk.push(Point2i(r + 1, c));
                    }
                    if (c > 0 && mark.at<int>(r, c - 1) == 0  && img.at<uchar>(r, c - 1) <= w * threshold) {
                        mark.at<int>(r, c - 1) = newMark;
                        stk.push(Point2i(r, c - 1));
                    }
                    if (c < mark.cols - 1 && mark.at<int>(r, c + 1) == 0  && img.at<uchar>(r, c + 1) <= w * threshold) {
                        mark.at<int>(r, c + 1) = newMark;
                        stk.push(Point2i(r, c + 1));
                    }
                }*/
            }
            else if (marks.size() == 1) {
                mark.at<int>(point.x, point.y) = marks[0];
            }
            else {
                int root0 = getRoot(marks[0], markUnion), root1 = getRoot(marks[1], markUnion);
                if (root0 == root1) {
                    mark.at<int>(point.x, point.y) = marks[0];
                }
                else if (depth[marks[0]] == w || depth[marks[1]] == w) {
                    mark.at<int>(point.x, point.y) = marks[0];
                    markUnion[root1] = root0;
                }
                else {
                  /* build shed */
                   mark.at<int>(point.x, point.y) = marks[0];
                   //shed.at<uchar>(point.x, point.y) = 255;
                }
           }
        }
    }

    for (int r = 0; r < mark.rows; r++) {
        for (int c = 0; c < mark.cols; c++) {
            vector<int> marks= marksAround(mark, Point2i(r, c));
            if (marks.size() <= 1)
                continue;
            set<int> roots;
            for (int r : marks) {
                roots.insert(getRoot(r, markUnion));
            }
            if (roots.size() >= 2)
                shed.at<uchar>(r, c) = 255;
        }
    }

    return shed;
}
