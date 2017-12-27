#include "./filter_basic.h"
#include "../expect/expect.h"
#include "../matrix/matrix_map.h"


namespace fb{
    template<typename T, int channels = 1>
    class Sum: public Summary<T, channels>
    {
        public:
            Vec<T, channels> sum;
            void record(Vec<T, channels> data){sum += data;}
    };

    template<typename T, int channels = 1>
    class Scale: public MatMapper<T, channels, T, channels>
    {
        protected:
            float scale;
        public:
            Scale(float scale):scale(scale){};
            Vec<T, channels> map(Vec<T, channels> data)
            {
                return data * scale;
            };
    };
}

Mat kernelUniform(Mat &kernel)
{
    expect(kernel.channels() == 1, "uniform - #channels not matches");

    fb::Sum<float, 1> sum;
    MatReducer<float, 1>().doReduce(kernel, sum);

    expect(sum.sum[0] != 0, "kernelUniform - can't uniform kernel whose sum is 0");
    Mat result = fb::Scale<float, 1>(1.0 / sum.sum[0]).doMap(kernel);

    return result;
}