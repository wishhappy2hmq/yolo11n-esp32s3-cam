#include "dl_detect_yolo11_postprocessor.hpp"
#include "dl_math.hpp"
#include <algorithm>
#include <cmath>

namespace dl {
namespace detect {
template <typename T>
void yolo11PostProcessor::parse_stage(TensorBase *score, TensorBase *box, const int stage_index)
{
    int stride_y = m_stages[stage_index].stride_y;
    int stride_x = m_stages[stage_index].stride_x;

    int offset_y = m_stages[stage_index].offset_y;
    int offset_x = m_stages[stage_index].offset_x;

    int H = score->shape[1];
    int W = score->shape[2];
    int C = score->shape[3];

    T *score_ptr = (T *)score->data;
    T *box_ptr = (T *)box->data;
    float score_exp = DL_SCALE(score->exponent);
    float box_exp = DL_SCALE(box->exponent);
    T score_thr_quant = quantize<T>(dl::math::inverse_sigmoid(m_score_thr), 1.f / score_exp);
    float inv_resize_scale_x = m_image_preprocessor->get_resize_scale_x(true);
    float inv_resize_scale_y = m_image_preprocessor->get_resize_scale_y(true);
    int border_left = m_image_preprocessor->get_border_left();
    int border_top = m_image_preprocessor->get_border_top();

    int reg_max = 16;

    for (size_t y = 0; y < H; y++) {
        for (size_t x = 0; x < W; x++) {
            for (size_t c = 0; c < C; c++) {
                if (*score_ptr > score_thr_quant) {
                    int center_y = y * stride_y + offset_y;
                    int center_x = x * stride_x + offset_x;

                    float box_data[reg_max * 4];
                    for (int i = 0; i < reg_max * 4; i++) {
                        box_data[i] = dequantize(box_ptr[i], box_exp);
                    }

                    result_t new_box = {
                        (int)c,
                        dl::math::sigmoid(dequantize(*score_ptr, score_exp)),
                        {(int)(((center_x - dl::math::dfl_integral(box_data, reg_max - 1) * stride_x) - border_left) *
                               inv_resize_scale_x),
                         (int)(((center_y - dl::math::dfl_integral(box_data + reg_max, reg_max - 1) * stride_y) -
                                border_top) *
                               inv_resize_scale_y),
                         (int)(((center_x + dl::math::dfl_integral(box_data + 2 * reg_max, reg_max - 1) * stride_x) -
                                border_left) *
                               inv_resize_scale_x),
                         (int)(((center_y + dl::math::dfl_integral(box_data + 3 * reg_max, reg_max - 1) * stride_y) -
                                border_top) *
                               inv_resize_scale_y)},
                        {}};

                    m_box_list.insert(std::upper_bound(m_box_list.begin(), m_box_list.end(), new_box, greater_box),
                                      new_box);
                }
                score_ptr++;
            }

            box_ptr += 4 * reg_max;
        }
    }
}

template void yolo11PostProcessor::parse_stage<int8_t>(TensorBase *score, TensorBase *box, const int stage_index);
template void yolo11PostProcessor::parse_stage<int16_t>(TensorBase *score, TensorBase *box, const int stage_index);

void yolo11PostProcessor::postprocess()
{
    TensorBase *bbox0 = m_model->get_output("box0");
    TensorBase *score0 = m_model->get_output("score0");

    TensorBase *bbox1 = m_model->get_output("box1");
    TensorBase *score1 = m_model->get_output("score1");

    TensorBase *bbox2 = m_model->get_output("box2");
    TensorBase *score2 = m_model->get_output("score2");

    if (bbox0->dtype == DATA_TYPE_INT8) {
        parse_stage<int8_t>(score0, bbox0, 0);
        parse_stage<int8_t>(score1, bbox1, 1);
        parse_stage<int8_t>(score2, bbox2, 2);
    } else {
        parse_stage<int16_t>(score0, bbox0, 0);
        parse_stage<int16_t>(score1, bbox1, 1);
        parse_stage<int16_t>(score2, bbox2, 2);
    }
    nms();
}
} // namespace detect
} // namespace dl
