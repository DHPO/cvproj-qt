#include "./matrix_conv.h"
#include "../expect/expect.h"

Mat padding(const Mat &matrix, int paddingSize, PaddingMethod method)
{
    if (method == NONE)
        return matrix;

    int channels = matrix.channels();
    int originCols = matrix.cols;
    int originRows = matrix.rows;

    Mat result = Mat(originRows + 2 * paddingSize, originCols + 2 * paddingSize, matrix.type());

    /* copy matrix */
    for (int r = 0; r < originRows; r++)
    {
        const uchar *optr = matrix.ptr<uchar>(r);
        uchar *rptr = result.ptr<uchar>(r + paddingSize);
        for (int c = 0; c < originCols * channels; c++)
        {
            rptr[c + paddingSize * channels] = optr[c];
        }
        if (method == SAME)
        {
            for (int c = 0; c < paddingSize; c++)
            {
                for (int i = 0; i < channels; i++)
                {
                    rptr[c * channels + i] = rptr[paddingSize * channels + i];
                }
            }
            for (int c = paddingSize + originCols; c < originCols + 2 * paddingSize; c++)
            {
                for (int i = 0; i < channels; i++)
                {
                    rptr[c * channels + i] = rptr[(paddingSize + originCols - 1) * channels + i];
                }
            }
        }
        else
        {
            for (int c = 0; c < paddingSize; c++)
            {
                for (int i = 0; i < channels; i++)
                {
                    rptr[c * channels + i] = rptr[(2 * paddingSize - c) * channels + i];
                }
            }
            for (int c = paddingSize + originCols; c < originCols + 2 * paddingSize; c++)
            {
                for (int i = 0; i < channels; i++)
                {
                    rptr[c * channels + i] = rptr[(2 * (paddingSize + originCols - 1) - c) * channels + i];
                }
            }
        }
    }

    for (int r = 0; r < paddingSize; r++)
    {
        const uchar *optr;
        if (method == SAME)
            optr = result.ptr<uchar>(paddingSize);
        else
            optr = result.ptr<uchar>(2 * paddingSize - r);
        uchar *rptr = result.ptr<uchar>(r);
        for (int c = 0; c < (originCols + 2 * paddingSize) * channels; c++)
        {
            rptr[c] = optr[c];
        }
    }

    for (int r = originRows + paddingSize; r < originRows + 2 * paddingSize; r++)
    {
        const uchar *optr;
        if (method == SAME)
            optr = result.ptr<uchar>(paddingSize + originRows - 1);
        else
            optr = result.ptr<uchar>(2 * (paddingSize + originRows - 1) - r);
        uchar *rptr = result.ptr<uchar>(r);
        for (int c = 0; c < (originCols + 2 * paddingSize) * channels; c++)
        {
            rptr[c] = optr[c];
        }
    }

    return result;
}

float multiply_sum(const Mat &matrix, const Mat &kernel, int offsetR, int offsetC, int channel = 0)
{
    expect(kernel.type() % 8 == CV_32F, "multiply_sum - invalid kernel data type");
    expect(matrix.channels() == kernel.channels(), "multiply_sum - channels mismatch");
    expect(kernel.rows == kernel.cols, "multiply_sum - kernel is not square");
    expect(kernel.rows + offsetR <= matrix.rows, "multiply_sum - row overflow");
    expect(kernel.cols + offsetC <= matrix.cols, "multiply_sum - col overflow");
    expect(channel < matrix.channels(), "multiply_sum - channel overflow");

    float sum = 0;
    int channels = matrix.channels();

    for (int r = 0; r < kernel.rows; r++)
    {
        const uchar *mptr = matrix.ptr<uchar>(r + offsetR);
        const float *kptr = kernel.ptr<float>(r);
        for (int c = 0; c < kernel.cols; c++)
        {
            sum += mptr[(c + offsetC) * channels + channel] * kptr[c * channels + channel];
        }
    }

    return sum;
}

Mat conv(const Mat &matrix, const Mat &kernel, PaddingMethod method, int stride)
{
    expect(kernel.type() % 8 == CV_32F, "conv - invalid kernel data type");
    expect(matrix.channels() == kernel.channels(), "conv - channels mismatch");
    expect(kernel.rows == kernel.cols, "conv - kernel is not square");

    Mat matrix_pad;
    if (method == SAME || method == MIRROR)
        matrix_pad = padding(matrix, kernel.rows / 2, method);
    else
        matrix_pad = matrix.clone();

    Mat result(matrix_pad.rows - kernel.rows + 1, matrix_pad.cols - kernel.cols + 1, kernel.type());
    for (int r = 0; r < result.rows; r++)
    {
        float *ptr = result.ptr<float>(r);
        for (int c = 0; c < result.cols; c++)
        {
            for (int i = 0; i < result.channels(); i++)
            {
                float sum = multiply_sum(matrix_pad, kernel, r, c, i);
                ptr[c * result.channels() + i] = sum;
            }
        }
    }

    return result;
}