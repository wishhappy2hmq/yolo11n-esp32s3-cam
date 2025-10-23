#pragma once

#include "dl_define.hpp"
#include "dl_tensor_base.hpp"
#include <assert.h>
#include <functional>
#include <stdint.h>
#include <vector>

namespace dl {
namespace base {
template <typename feature_t>
struct ArgsType {
    feature_t *input_element;           /*!<  0 */
    int input_channel;                  /*!<  1 */
    int input_stride_y_offset;          /*!<  2 input_width_with_padding * input_channel_with_padding * stride_y */
    int input_stride_x_offset;          /*!<  3 input_channel_with_padding * stride_x */
    int input_dilation_y_offset;        /*!<  4 input_width_with_padding * input_channel_with_padding * dilation_y */
    int input_dilation_x_offset;        /*!<  5 input_channel_with_padding * dilation_x */
                                        //
    feature_t *output_element;          /*!<  6 */
    int output_height;                  /*!<  7 */
    int output_width;                   /*!<  8 */
    int output_channel;                 /*!<  9 */
    int output_y_offset;                /*!< 10 output_width_with_padding * output_channel_with_padding */
    int output_x_offset;                /*!< 11 output_channel_with_padding */
                                        //
    const void *filter_element;         /*!< 12 */
    int filter_height;                  /*!< 13 */
    int filter_width;                   /*!< 14 */
    int filter_y_offset;                /*!< 15 filter_width * input_channel */
    int mac_shift;                      /*!< 16 mac_shift = output.exponent - filter.exponent - input.exponent */
                                        //
    const void *bias_element;           /*!< 17 */
                                        //
    activation_type_t activation_type;  /*!< 18 */
    int activation_alpha;               /*!< 19 */
    const void *activation_alpha_ptr;   /*!< 20 */
    int activation_shift;               /*!< 21 */
                                        //
    int c_rs1_1;                        /*!< 22 (input_channel >> 1) - 1 */
    int c_rs2_1;                        /*!< 23 (input_channel >> 2) - 1 */
    int n_div_x;                        /*!< 24 output_channel / (vector_width / element_width) */
    int c_div_x_1;                      /*!< 25 input_channel / (vector_width / element_width) - 1 */
    int16_t *tie_filter_channel_factor; /*!< 26 */
                                        //
    int xtensa_dilation_x_offset;       /*!< 27 (dilation_x * input_channel_with_padding - input_channel) *
                                           sizeof(feature_t)*/
    int xtensa_dilation_y_offset;       /*!< 28 */
                                        //
    int tie_conv2d_dilation_x_offset; /*!< 29 TODO: not used, to be deleted. | dilation_x * input_channel_with_padding *
                                         sizeof(feature_t) - (c_div_x_1 + 1) * 16 */
    int tie_conv2d_dilation_y_offset; /*!< 30 TODO: not used, to be deleted. | */
                                      //
    int tie_depth2d_dilation_x_offset; /*!< 31 */
    int tie_depth2d_dilation_y_offset; /*!< 32 */
    int tie_depth2d_next_hwx1;         /*!< 33 */
                                       //
    int c_remainder;                   /*!< 34 input_channel % (vector_width / element_width) * sizeof(feature_t) */
    int n_remainder;                   /*!< 35 */
    int filter_n_offset;               /*!< 36 filter_height * filter_width * input_channel */
    int filter_w_rs1_1;                /*!< 37 (filter_width >> 1) - 1 */
    int16_t *filter_channel_factor;    /*!< 38 */
    int input_channel_with_padding;    /*!< 39 */

    int filter_y_offset_unaligned;        /*!< 40 */
    int filter_n_offset_unaligned;        /*!< 41 */
    const void *filter_element_unaligned; /*!< 42 */

    int output_shift; /*!< 43 */
    int output_scale; /*!< 44 */

    int padding_h_head;                       /*!< 45 */
    int padding_h_tail;                       /*!< 46 */
    int padding_w_head;                       /*!< 47 */
    int padding_w_tail;                       /*!< 48 */
    int dilation_h;                           /*!< 49 */
    int dilation_w;                           /*!< 50 */
    int stride_x;                             /*!< 51 */
    int stride_y;                             /*!< 52 */
    int input_y_offset;                       /*!< 53 */
    int filter_c;                             /*!< 54 */
    int xtensa_dilation_y_offset_stable;      /*!< 55 */
    int tie_depth2d_dilation_y_offset_stable; /*!< 56 */
    int input_width;                          /*!< 57 */

    int filter_y_offset_c; /*!< 58 */
    int filter_n_offset_c; /*!< 59 */

    int element_num;   /*!< 60 */
    int input_height;  /*!< 61 */
    void *debug_value; /*!< 62 It will malloc 16 bytes memory if malloc_debug_memory = true */
    bool auto_split;
};

typedef void (*c_impl_func_s16_t)(DL_S16_BUFFER_TYPE *, int16_t *, const ArgsType<int16_t> &);
typedef void (*n_wise_func_s16_t)(int16_t *, DL_S16_BUFFER_TYPE *, const ArgsType<int16_t> &);

typedef void (*c_impl_func_s8_t)(int32_t *, int8_t *, const ArgsType<int8_t> &);
typedef void (*n_wise_func_s8_t)(int8_t *, int32_t *, const ArgsType<int8_t> &);

template <typename out_feature_t, typename in_feature_t, typename... feature_t>
using ImplFunc_t = std::function<void(out_feature_t *, in_feature_t *, feature_t *..., void *)>;

// TODO:剥离出多核时 input output 的指针分配
template <typename feature_t>
void load_input_output_ptr()
{
}

// Modifications:
// 1. Tensor, Filter, Bias, Activation -> TensorBase pointer
// 2. move dilations from Filter into function's argument
// 3. activation_alpha is used for PReLU and leaky ReLU
// TODO:: It is possible to remove the template for ArgsType

/**
 * @brief Get the conv operation args object
 *
 * @tparam feature_t
 * @param output
 * @param input
 * @param padding
 * @param filter
 * @param stride_y
 * @param stride_x
 * @param dilation_y
 * @param dilation_x
 * @param group
 * @param bias
 * @param activate
 * @param activation_alpha
 * @param runtime_mode
 * @param malloc_debug_memory
 * @return std::vector<ArgsType<feature_t>>
 */
template <typename feature_t>
std::vector<ArgsType<feature_t>> get_conv_operation_args(TensorBase *output,
                                                         TensorBase *input,
                                                         std::vector<int> &padding,
                                                         TensorBase *filter,
                                                         const std::vector<int> &strides,
                                                         const std::vector<int> &dilations,
                                                         const int group,
                                                         TensorBase *bias = NULL,
                                                         const activation_type_t activate = Linear,
                                                         TensorBase *activation_alpha = nullptr,
                                                         const runtime_mode_t runtime_mode = RUNTIME_MODE_AUTO,
                                                         bool malloc_debug_memory = false)
{
    ArgsType<feature_t> args;
    args.input_element = (feature_t *)input->get_element_ptr();
    args.output_element = (feature_t *)output->get_element_ptr();
    args.filter_element = filter->get_element_ptr();

    if (input->shape.size() == 3) {
        args.input_height = 1;
        args.input_width = input->shape[1];
        args.input_channel = input->shape[2];
        args.dilation_h = 1;
        args.dilation_w = dilations[0];
        args.stride_y = 1;
        args.stride_x = strides[0];

        args.output_height = 1;
        args.output_width = output->shape[1];
        args.output_channel = output->shape[2];

        args.filter_height = 1;
        args.filter_width = filter->shape[0];
        if (group == 1) {
            // conv
            args.filter_y_offset = 0;
            args.filter_c = filter->shape[1]; // dw: filter->shape[2]. conv: filter->shape[1].
        } else {
            // depthwise
            args.filter_y_offset = 16;
            args.filter_c = filter->shape[2]; // dw: filter->shape[2]. conv: filter->shape[1].
        }
        /* It's for c. We need to confirm whether the following definitions conform to the C logical implementation. */
        args.filter_y_offset_c = args.filter_width * filter->shape[1];

        args.padding_h_head = 0;
        args.padding_h_tail = 0;
        args.padding_w_head = padding[0];
        args.padding_w_tail = padding[1];
    } else if (input->shape.size() == 4) {
        args.input_height = input->shape[1];
        args.input_width = input->shape[2];
        args.input_channel = input->shape[3];
        args.dilation_h = dilations[0];
        args.dilation_w = dilations[1];
        args.stride_y = strides[0];
        args.stride_x = strides[1];

        args.output_height = output->shape[1];
        args.output_width = output->shape[2];
        args.output_channel = output->shape[3];

        args.filter_height = filter->shape[0];
        args.filter_width = filter->shape[1];
        if (group == 1) {
            // conv
            args.filter_y_offset = 0;
            args.filter_c = filter->shape[2]; // dw: filter->shape[3]. conv: filter->shape[2].
        } else {
            // depthwise
#if CONFIG_TIE728_BOOST || CONFIG_ESP32P4_BOOST
            args.filter_y_offset = 16;
#else
            args.filter_y_offset = 0;
#endif
            args.filter_c = filter->shape[3]; // dw: filter->shape[3]. conv: filter->shape[2].
        }
        /* It's for c. We need to confirm whether the following definitions conform to the C logical implementation. */
        args.filter_y_offset_c = args.filter_width * filter->shape[2];

        args.padding_h_head = padding[0];
        args.padding_h_tail = padding[1];
        args.padding_w_head = padding[2];
        args.padding_w_tail = padding[3];
    } else {
        ESP_LOGE(__FUNCTION__, "Do not support input shape.");
        return {};
    }

    args.filter_n_offset = 0;
    args.filter_n_offset_c = args.filter_y_offset_c * args.filter_height;

    args.input_stride_y_offset = args.input_width * args.input_channel * args.stride_y;
    args.input_stride_x_offset = args.input_channel * args.stride_x;
    args.input_dilation_y_offset = args.input_width * args.input_channel * args.dilation_h;
    args.input_dilation_x_offset = args.input_channel * args.dilation_w;

    args.output_y_offset = args.output_width * args.output_channel;
    args.output_x_offset = args.output_channel;

    args.input_y_offset = args.input_width * args.input_channel;
    args.input_channel_with_padding = args.input_channel;
    args.auto_split = true;
    // printf("input: %d, %d, %d, output: %d, %d, %d\n", input->shape[1], args.input_width, args.input_channel,
    // output->shape[1], args.output_width, args.output_channel);

    args.mac_shift = output->exponent - filter->exponent - input->exponent;

    args.bias_element = bias ? bias->get_element_ptr() : NULL; // TODO: auto_split
    args.activation_type = activate;

    switch (args.activation_type) {
    case ReLU:
        args.activation_alpha = 0;
        args.activation_shift = 0;
        args.activation_alpha_ptr = NULL;
        break;
    case LeakyReLU:
        // ESP_LOGE(__FUNCTION__, "Do not support Leaky ReLU");
        //     args.activation_alpha = activation_alpha->get_element_ptr()[0];
        //     args.activation_shift = -activation_alpha->exponent;
        //     args.activation_alpha_ptr = NULL;
        break;
    case PReLU:
        // ESP_LOGE(__FUNCTION__, "Do not support PReLU");
        // args.activation_alpha_ptr = activation_alpha->get_element_ptr(); //TODO: auto_split
        // args.activation_shift = -activation_alpha->exponent;
        break;
    default:
        args.activation_alpha_ptr = NULL;
        args.activation_shift = -1;
        break;
    }

    // for ISA
    args.c_rs1_1 = (args.input_channel >> 1) - 1;
    args.c_rs2_1 = (args.input_channel >> 2) - 1;
    int u = 16 / sizeof(feature_t);
    args.n_div_x = args.output_channel / u; // TODO: auto_split
    args.c_div_x_1 = args.input_channel / u - 1;

    args.c_remainder = args.input_channel % u * sizeof(feature_t);
    args.n_remainder = args.output_channel % u;

    args.xtensa_dilation_x_offset = (args.dilation_w * args.input_channel - args.input_channel) * sizeof(feature_t);
    args.xtensa_dilation_y_offset_stable = args.dilation_h * args.input_channel * args.input_width;
    args.xtensa_dilation_y_offset = (args.xtensa_dilation_y_offset_stable - args.input_channel -
                                     (args.filter_width - 1) * args.dilation_w * args.input_channel) *
        sizeof(feature_t);

    args.filter_y_offset_unaligned = 0;
    args.filter_n_offset_unaligned = 0;
    args.filter_element_unaligned = args.n_remainder
        ? ((feature_t *)args.filter_element + args.n_div_x * args.filter_height * args.filter_width * args.filter_c * u)
        : args.filter_element;

    if (group > 1) {
        args.filter_w_rs1_1 = (args.filter_width >> 1) - 1;
        args.tie_depth2d_dilation_x_offset = args.dilation_w * args.input_channel * sizeof(feature_t);
        args.tie_depth2d_dilation_y_offset_stable = args.dilation_h * args.input_channel * args.input_width;
        args.tie_depth2d_dilation_y_offset = (args.tie_depth2d_dilation_y_offset_stable -
                                              (args.filter_width - 1) * args.dilation_w * args.input_channel) *
            sizeof(feature_t);

        args.tie_depth2d_next_hwx1 =
            (args.filter_width - 1) * args.dilation_w + (args.filter_height - 1) * args.dilation_h * args.input_width;
        args.tie_depth2d_next_hwx1 = 16 - args.tie_depth2d_next_hwx1 * args.input_channel * sizeof(feature_t);
    }
    args.debug_value = nullptr;
    if (malloc_debug_memory) {
        args.debug_value = tool::calloc_aligned(16, 16, 1, MALLOC_CAP_DEFAULT);
    }
    std::vector<ArgsType<feature_t>> m_args(1, args);
    if (args.input_height > 4 * args.dilation_h * args.filter_height) {
        if (runtime_mode == RUNTIME_MODE_MULTI_CORE ||
            (runtime_mode == RUNTIME_MODE_AUTO && args.input_height >= 100 && args.input_width >= 50)) {
            m_args.push_back(args);

            // Divide this convolution into two tasks by splitting the input height.
            // up
            int dilation_filter_height = args.dilation_h * (args.filter_height - 1) + 1;
            int half_step = (args.padding_h_head + args.padding_h_tail + args.input_height - dilation_filter_height) /
                args.stride_y / 2;
            m_args[0].input_height = dilation_filter_height - args.padding_h_head + half_step * args.stride_y;
            m_args[0].padding_h_tail = 0;
            m_args[0].output_height = half_step + 1;
            // bottom
            m_args[1].padding_h_head = 0;
            m_args[1].input_height =
                dilation_filter_height - args.stride_y + args.input_height - m_args[0].input_height;
            m_args[1].input_element +=
                (args.input_height - m_args[1].input_height) * args.input_width * args.input_channel;
            m_args[1].output_height = args.output_height - m_args[0].output_height;
            m_args[1].output_element +=
                (args.output_height - m_args[1].output_height) * args.output_width * args.output_channel;
        }
    }

    return m_args;
}

template <typename feature_t, typename buffer_t>
void conv_operation_shell(ArgsType<feature_t> &args,
                          ImplFunc_t<feature_t, feature_t> i_impl_func,
                          ImplFunc_t<feature_t, feature_t> i_impl_func_sp,
                          void (*c_impl_func)(buffer_t *, feature_t *, const ArgsType<feature_t> &),
                          void (*c_impl_func_sp)(buffer_t *, feature_t *, const ArgsType<feature_t> &),
                          void (*n_wise_tail)(feature_t *, buffer_t *, const ArgsType<feature_t> &))
{
    feature_t *input_ptr = (feature_t *)args.input_element;
    feature_t *output_ptr = (feature_t *)args.output_element;
    if (args.padding_h_head || args.padding_w_head || args.padding_h_tail || args.padding_w_tail) { // padding same
        int n_h_head = (args.padding_h_head + args.stride_y - 1) / args.stride_y;
        int n_w_head = (args.padding_w_head + args.stride_x - 1) / args.stride_x;
        int n_h_body = ((args.input_height + args.padding_h_head - args.dilation_h * (args.filter_height - 1) - 1) /
                            args.stride_y +
                        1) -
            n_h_head;
        if (n_h_body < 0)
            n_h_body = 0;
        int n_w_body =
            ((args.input_width + args.padding_w_head - args.dilation_w * (args.filter_width - 1) - 1) / args.stride_x +
             1) -
            n_w_head;
        if (n_w_body < 0)
            n_w_body = 0;

        int n_h_tail = args.output_height - n_h_head - n_h_body;
        int n_w_tail = args.output_width - n_w_head - n_w_body;
        int filter_h = args.filter_height;
        int filter_w = args.filter_width;
        feature_t *filter_ptr = (feature_t *)(args.filter_element);

        if (i_impl_func_sp) {
            feature_t *input_y_real;
            feature_t *input_x_real;
            feature_t *filter_ptr_y;
            feature_t *output_yx = output_ptr;
            feature_t *filter_ptr_unaligned = (feature_t *)(args.filter_element_unaligned);
            feature_t *filter_ptr_y_unaligned;
            int unaligned_filter_c_n_offset = args.filter_c * sizeof(feature_t);
#if CONFIG_TIE728_BOOST || CONFIG_ESP32P4_BOOST
            int filter_c_n_offset = args.n_div_x ? args.filter_c * 16 : unaligned_filter_c_n_offset;
#else
            int filter_c_n_offset = unaligned_filter_c_n_offset;
#endif
            int filter_c_n_ptr_offset = filter_c_n_offset / sizeof(feature_t);

            if (n_wise_tail) {
                for (size_t output_y = 0; output_y < n_h_head; output_y++) {
                    args.filter_height = filter_h -
                        ((args.padding_h_head - output_y * args.stride_y) + args.dilation_h - 1) / args.dilation_h;
                    // Fix for filter_size > input_size
                    int filter_height_excess = filter_h -
                        (args.input_height + (args.padding_h_head - output_y * args.stride_y) + args.dilation_h - 1) /
                            args.dilation_h;
                    if (filter_height_excess > 0) {
                        args.filter_height -= filter_height_excess;
                    } else {
                        filter_height_excess = 0;
                    }

                    input_y_real = input_ptr +
                        args.input_y_offset *
                            ((args.stride_y * output_y +
                              (filter_h - args.filter_height - filter_height_excess) * args.dilation_h) -
                             args.padding_h_head);
                    filter_ptr_y = filter_ptr +
                        (filter_h - args.filter_height - filter_height_excess) * filter_w * filter_c_n_ptr_offset;
                    filter_ptr_y_unaligned = filter_ptr_unaligned +
                        (filter_h - args.filter_height - filter_height_excess) * filter_w * args.filter_c;
                    args.filter_n_offset = (filter_w * (filter_h - args.filter_height)) * filter_c_n_offset;
                    args.filter_n_offset_unaligned =
                        (filter_w * (filter_h - args.filter_height)) * unaligned_filter_c_n_offset;

                    for (size_t output_x = 0; output_x < n_w_head; output_x++) {
                        args.filter_width = filter_w -
                            ((args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) / args.dilation_w;
                        // Fix for filter_size > input_size
                        int filter_width_excess = filter_w -
                            (args.input_width + (args.padding_w_head - output_x * args.stride_x) + args.dilation_w -
                             1) /
                                args.dilation_w;
                        if (filter_width_excess > 0) {
                            args.filter_width -= filter_width_excess;
                        } else {
                            filter_width_excess = 0;
                        }

                        input_x_real = input_y_real +
                            args.input_channel *
                                ((args.stride_x * output_x +
                                  (filter_w - args.filter_width - filter_width_excess) * args.dilation_w) -
                                 args.padding_w_head);
                        args.filter_y_offset =
                            (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.xtensa_dilation_y_offset =
                            (args.xtensa_dilation_y_offset_stable - args.input_channel -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        args.filter_element =
                            filter_ptr_y + (filter_w - args.filter_width - filter_width_excess) * filter_c_n_ptr_offset;
                        args.filter_element_unaligned = filter_ptr_y_unaligned +
                            (filter_w - args.filter_width - filter_width_excess) * args.filter_c;

                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        n_wise_tail(output_yx, NULL, args);
                        output_yx += args.output_x_offset;
                    }

                    input_x_real = input_y_real + args.input_channel * (args.stride_x * n_w_head - args.padding_w_head);
                    args.filter_width = filter_w;
                    args.xtensa_dilation_y_offset =
                        (args.xtensa_dilation_y_offset_stable - args.input_channel -
                         (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                        sizeof(feature_t);
                    args.filter_y_offset = 0; // ??? c， xtensa， tie 顺序不同
                    args.filter_y_offset_unaligned = 0;
                    args.filter_element = filter_ptr_y;
                    args.filter_element_unaligned = filter_ptr_y_unaligned;
                    for (size_t output_x = 0; output_x < n_w_body; output_x++) {
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        n_wise_tail(output_yx, NULL, args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }

                    for (size_t output_x = 0; output_x < n_w_tail; output_x++) {
                        args.filter_width = (args.padding_w_head + args.input_width -
                                             (n_w_head + n_w_body + output_x) * args.stride_x + args.dilation_w - 1) /
                            args.dilation_w;
                        args.filter_y_offset =
                            (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.xtensa_dilation_y_offset =
                            (args.xtensa_dilation_y_offset_stable - args.input_channel -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        n_wise_tail(output_yx, NULL, args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }
                }

                args.filter_height = filter_h;
                input_y_real = input_ptr + args.input_y_offset * ((args.stride_y * n_h_head) - args.padding_h_head);
                filter_ptr_y = filter_ptr;
                filter_ptr_y_unaligned = filter_ptr_unaligned;
                args.filter_n_offset = 0;
                args.filter_n_offset_unaligned = 0;
                for (size_t output_y = 0; output_y < n_h_body; output_y++) {
                    for (size_t output_x = 0; output_x < n_w_head; output_x++) {
                        args.filter_width = filter_w -
                            ((args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) / args.dilation_w;
                        // Fix for filter_size > input_size
                        int filter_width_excess = filter_w -
                            (args.input_width + (args.padding_w_head - output_x * args.stride_x) + args.dilation_w -
                             1) /
                                args.dilation_w;
                        if (filter_width_excess > 0) {
                            args.filter_width -= filter_width_excess;
                        } else {
                            filter_width_excess = 0;
                        }

                        input_x_real = input_y_real +
                            args.input_channel *
                                ((args.stride_x * output_x +
                                  (filter_w - args.filter_width - filter_width_excess) * args.dilation_w) -
                                 args.padding_w_head);
                        args.filter_y_offset =
                            (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.xtensa_dilation_y_offset =
                            (args.xtensa_dilation_y_offset_stable - args.input_channel -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        args.filter_element =
                            filter_ptr_y + (filter_w - args.filter_width - filter_width_excess) * filter_c_n_ptr_offset;
                        args.filter_element_unaligned = filter_ptr_y_unaligned +
                            (filter_w - args.filter_width - filter_width_excess) * args.filter_c;
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        n_wise_tail(output_yx, NULL, args);
                        output_yx += args.output_x_offset;
                    }

                    input_x_real = input_y_real + args.input_channel * (args.stride_x * n_w_head - args.padding_w_head);
                    args.filter_width = filter_w;
                    args.xtensa_dilation_y_offset =
                        (args.xtensa_dilation_y_offset_stable - args.input_channel -
                         (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                        sizeof(feature_t);
                    args.filter_y_offset = 0; // ??? c， xtensa， tie 顺序不同
                    args.filter_y_offset_unaligned = 0;
                    args.filter_element = filter_ptr_y;
                    args.filter_element_unaligned = filter_ptr_y_unaligned;
                    for (size_t output_x = 0; output_x < n_w_body; output_x++) {
                        i_impl_func_sp(output_yx, input_x_real, (void *const)&args);
                        n_wise_tail(output_yx, NULL, args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }

                    for (size_t output_x = 0; output_x < n_w_tail; output_x++) {
                        args.filter_width = (args.padding_w_head + args.input_width -
                                             (n_w_head + n_w_body + output_x) * args.stride_x + args.dilation_w - 1) /
                            args.dilation_w;
                        args.filter_y_offset =
                            (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.xtensa_dilation_y_offset =
                            (args.xtensa_dilation_y_offset_stable - args.input_channel -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        n_wise_tail(output_yx, NULL, args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }
                    input_y_real += args.input_stride_y_offset;
                }

                for (size_t output_y = 0; output_y < n_h_tail; output_y++) {
                    args.filter_height = (args.padding_h_head + args.input_height -
                                          (n_h_head + n_h_body + output_y) * args.stride_y + args.dilation_h - 1) /
                        args.dilation_h;
                    args.filter_n_offset = (filter_w * (filter_h - args.filter_height)) * filter_c_n_offset;
                    args.filter_n_offset_unaligned =
                        (filter_w * (filter_h - args.filter_height)) * unaligned_filter_c_n_offset;

                    for (size_t output_x = 0; output_x < n_w_head; output_x++) {
                        args.filter_width = filter_w -
                            ((args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) / args.dilation_w;
                        // Fix for filter_size > input_size
                        int filter_width_excess = filter_w -
                            (args.input_width + (args.padding_w_head - output_x * args.stride_x) + args.dilation_w -
                             1) /
                                args.dilation_w;
                        if (filter_width_excess > 0) {
                            args.filter_width -= filter_width_excess;
                        } else {
                            filter_width_excess = 0;
                        }

                        input_x_real = input_y_real +
                            args.input_channel *
                                ((args.stride_x * output_x +
                                  (filter_w - args.filter_width - filter_width_excess) * args.dilation_w) -
                                 args.padding_w_head);
                        args.filter_y_offset =
                            (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.xtensa_dilation_y_offset =
                            (args.xtensa_dilation_y_offset_stable - args.input_channel -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        args.filter_element =
                            filter_ptr_y + (filter_w - args.filter_width - filter_width_excess) * filter_c_n_ptr_offset;
                        args.filter_element_unaligned = filter_ptr_y_unaligned +
                            (filter_w - args.filter_width - filter_width_excess) * args.filter_c;
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        n_wise_tail(output_yx, NULL, args);
                        output_yx += args.output_x_offset;
                    }

                    input_x_real = input_y_real + args.input_channel * (args.stride_x * n_w_head - args.padding_w_head);
                    args.filter_width = filter_w;
                    args.filter_y_offset = 0; // ??? c， xtensa， tie 顺序不同
                    args.filter_y_offset_unaligned = 0;
                    args.xtensa_dilation_y_offset =
                        (args.xtensa_dilation_y_offset_stable - args.input_channel -
                         (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                        sizeof(feature_t);
                    args.filter_element = filter_ptr_y;
                    args.filter_element_unaligned = filter_ptr_y_unaligned;
                    for (size_t output_x = 0; output_x < n_w_body; output_x++) {
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        n_wise_tail(output_yx, NULL, args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }

                    for (size_t output_x = 0; output_x < n_w_tail; output_x++) {
                        args.filter_width = (args.padding_w_head + args.input_width -
                                             (n_w_head + n_w_body + output_x) * args.stride_x + args.dilation_w - 1) /
                            args.dilation_w;
                        args.filter_y_offset =
                            (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.xtensa_dilation_y_offset =
                            (args.xtensa_dilation_y_offset_stable - args.input_channel -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        n_wise_tail(output_yx, NULL, args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }
                    input_y_real += args.input_stride_y_offset;
                }
            } else {
                for (size_t output_y = 0; output_y < n_h_head; output_y++) {
                    args.filter_height = filter_h -
                        ((args.padding_h_head - output_y * args.stride_y) + args.dilation_h - 1) / args.dilation_h;
                    // Fix for filter_size > input_size
                    int filter_height_excess = filter_h -
                        (args.input_height + (args.padding_h_head - output_y * args.stride_y) + args.dilation_h - 1) /
                            args.dilation_h;
                    if (filter_height_excess > 0) {
                        args.filter_height -= filter_height_excess;
                    } else {
                        filter_height_excess = 0;
                    }

                    input_y_real = input_ptr +
                        args.input_y_offset *
                            ((args.stride_y * output_y +
                              (filter_h - args.filter_height - filter_height_excess) * args.dilation_h) -
                             args.padding_h_head);
                    filter_ptr_y = filter_ptr +
                        (filter_h - args.filter_height - filter_height_excess) * filter_w * filter_c_n_ptr_offset;
                    filter_ptr_y_unaligned = filter_ptr_unaligned +
                        (filter_h - args.filter_height - filter_height_excess) * filter_w * args.filter_c;
                    args.filter_n_offset = (filter_w * (filter_h - args.filter_height)) * filter_c_n_offset;
                    args.filter_n_offset_unaligned =
                        (filter_w * (filter_h - args.filter_height)) * unaligned_filter_c_n_offset;

                    for (size_t output_x = 0; output_x < n_w_head; output_x++) {
                        args.filter_width = filter_w -
                            ((args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) / args.dilation_w;
                        // Fix for filter_size > input_size
                        int filter_width_excess = filter_w -
                            (args.input_width + (args.padding_w_head - output_x * args.stride_x) + args.dilation_w -
                             1) /
                                args.dilation_w;
                        if (filter_width_excess > 0) {
                            args.filter_width -= filter_width_excess;
                        } else {
                            filter_width_excess = 0;
                        }

                        input_x_real = input_y_real +
                            args.input_channel *
                                ((args.stride_x * output_x +
                                  (filter_w - args.filter_width - filter_width_excess) * args.dilation_w) -
                                 args.padding_w_head);
                        args.filter_y_offset =
                            (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.xtensa_dilation_y_offset =
                            (args.xtensa_dilation_y_offset_stable - args.input_channel -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        args.filter_element =
                            filter_ptr_y + (filter_w - args.filter_width - filter_width_excess) * filter_c_n_ptr_offset;
                        args.filter_element_unaligned = filter_ptr_y_unaligned +
                            (filter_w - args.filter_width - filter_width_excess) * args.filter_c;

                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        output_yx += args.output_x_offset;
                    }

                    input_x_real = input_y_real + args.input_channel * (args.stride_x * n_w_head - args.padding_w_head);
                    args.filter_width = filter_w;
                    args.xtensa_dilation_y_offset =
                        (args.xtensa_dilation_y_offset_stable - args.input_channel -
                         (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                        sizeof(feature_t);
                    args.filter_y_offset = 0; // ??? c， xtensa， tie 顺序不同
                    args.filter_y_offset_unaligned = 0;
                    args.filter_element = filter_ptr_y;
                    args.filter_element_unaligned = filter_ptr_y_unaligned;
                    for (size_t output_x = 0; output_x < n_w_body; output_x++) {
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }

                    for (size_t output_x = 0; output_x < n_w_tail; output_x++) {
                        args.filter_width = (args.padding_w_head + args.input_width -
                                             (n_w_head + n_w_body + output_x) * args.stride_x + args.dilation_w - 1) /
                            args.dilation_w;
                        args.filter_y_offset =
                            (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.xtensa_dilation_y_offset =
                            (args.xtensa_dilation_y_offset_stable - args.input_channel -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }
                }

                args.filter_height = filter_h;
                input_y_real = input_ptr + args.input_y_offset * ((args.stride_y * n_h_head) - args.padding_h_head);
                filter_ptr_y = filter_ptr;
                filter_ptr_y_unaligned = filter_ptr_unaligned;
                args.filter_n_offset = 0;
                args.filter_n_offset_unaligned = 0;

                for (size_t output_y = 0; output_y < n_h_body; output_y++) {
                    for (size_t output_x = 0; output_x < n_w_head; output_x++) {
                        args.filter_width = filter_w -
                            ((args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) / args.dilation_w;
                        // Fix for filter_size > input_size
                        int filter_width_excess = filter_w -
                            (args.input_width + (args.padding_w_head - output_x * args.stride_x) + args.dilation_w -
                             1) /
                                args.dilation_w;
                        if (filter_width_excess > 0) {
                            args.filter_width -= filter_width_excess;
                        } else {
                            filter_width_excess = 0;
                        }

                        input_x_real = input_y_real +
                            args.input_channel *
                                ((args.stride_x * output_x +
                                  (filter_w - args.filter_width - filter_width_excess) * args.dilation_w) -
                                 args.padding_w_head);
                        args.filter_y_offset =
                            (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.xtensa_dilation_y_offset =
                            (args.xtensa_dilation_y_offset_stable - args.input_channel -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        args.filter_element =
                            filter_ptr_y + (filter_w - args.filter_width - filter_width_excess) * filter_c_n_ptr_offset;
                        args.filter_element_unaligned = filter_ptr_y_unaligned +
                            (filter_w - args.filter_width - filter_width_excess) * args.filter_c;
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        output_yx += args.output_x_offset;
                    }

                    input_x_real = input_y_real + args.input_channel * (args.stride_x * n_w_head - args.padding_w_head);
                    args.filter_width = filter_w;
                    args.xtensa_dilation_y_offset =
                        (args.xtensa_dilation_y_offset_stable - args.input_channel -
                         (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                        sizeof(feature_t);
                    args.filter_y_offset = 0; // ??? c， xtensa， tie 顺序不同
                    args.filter_y_offset_unaligned = 0;
                    args.filter_element = filter_ptr_y;
                    args.filter_element_unaligned = filter_ptr_y_unaligned;
                    for (size_t output_x = 0; output_x < n_w_body; output_x++) {
                        i_impl_func_sp(output_yx, input_x_real, (void *const)&args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }

                    for (size_t output_x = 0; output_x < n_w_tail; output_x++) {
                        args.filter_width = (args.padding_w_head + args.input_width -
                                             (n_w_head + n_w_body + output_x) * args.stride_x + args.dilation_w - 1) /
                            args.dilation_w;
                        args.filter_y_offset =
                            (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.xtensa_dilation_y_offset =
                            (args.xtensa_dilation_y_offset_stable - args.input_channel -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }
                    input_y_real += args.input_stride_y_offset;
                }

                for (size_t output_y = 0; output_y < n_h_tail; output_y++) {
                    args.filter_height = (args.padding_h_head + args.input_height -
                                          (n_h_head + n_h_body + output_y) * args.stride_y + args.dilation_h - 1) /
                        args.dilation_h;
                    args.filter_n_offset = (filter_w * (filter_h - args.filter_height)) * filter_c_n_offset;
                    args.filter_n_offset_unaligned =
                        (filter_w * (filter_h - args.filter_height)) * unaligned_filter_c_n_offset;

                    for (size_t output_x = 0; output_x < n_w_head; output_x++) {
                        args.filter_width = filter_w -
                            ((args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) / args.dilation_w;
                        // Fix for filter_size > input_size
                        int filter_width_excess = filter_w -
                            (args.input_width + (args.padding_w_head - output_x * args.stride_x) + args.dilation_w -
                             1) /
                                args.dilation_w;
                        if (filter_width_excess > 0) {
                            args.filter_width -= filter_width_excess;
                        } else {
                            filter_width_excess = 0;
                        }

                        input_x_real = input_y_real +
                            args.input_channel *
                                ((args.stride_x * output_x +
                                  (filter_w - args.filter_width - filter_width_excess) * args.dilation_w) -
                                 args.padding_w_head);
                        args.filter_y_offset =
                            (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.xtensa_dilation_y_offset =
                            (args.xtensa_dilation_y_offset_stable - args.input_channel -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        args.filter_element =
                            filter_ptr_y + (filter_w - args.filter_width - filter_width_excess) * filter_c_n_ptr_offset;
                        args.filter_element_unaligned = filter_ptr_y_unaligned +
                            (filter_w - args.filter_width - filter_width_excess) * args.filter_c;
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        output_yx += args.output_x_offset;
                    }

                    input_x_real = input_y_real + args.input_channel * (args.stride_x * n_w_head - args.padding_w_head);
                    args.filter_width = filter_w;
                    args.filter_y_offset = 0; // ??? c， xtensa， tie 顺序不同
                    args.filter_y_offset_unaligned = 0;
                    args.xtensa_dilation_y_offset =
                        (args.xtensa_dilation_y_offset_stable - args.input_channel -
                         (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                        sizeof(feature_t);
                    args.filter_element = filter_ptr_y;
                    args.filter_element_unaligned = filter_ptr_y_unaligned;
                    for (size_t output_x = 0; output_x < n_w_body; output_x++) {
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }

                    for (size_t output_x = 0; output_x < n_w_tail; output_x++) {
                        args.filter_width = (args.padding_w_head + args.input_width -
                                             (n_w_head + n_w_body + output_x) * args.stride_x + args.dilation_w - 1) /
                            args.dilation_w;
                        args.filter_y_offset =
                            (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.xtensa_dilation_y_offset =
                            (args.xtensa_dilation_y_offset_stable - args.input_channel -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }
                    input_y_real += args.input_stride_y_offset;
                }
            }
        } else // run c_impl_func
        {
            buffer_t *buffer = (buffer_t *)heap_caps_calloc(args.output_channel, sizeof(buffer_t), MALLOC_CAP_DEFAULT);
            feature_t *input_y_real;
            feature_t *input_x_real;
            feature_t *filter_ptr_y;
            feature_t *output_yx = output_ptr;
            int filter_c_n_offset = args.input_channel;
            int filter_c_n_ptr_offset = filter_c_n_offset;

            for (size_t output_y = 0; output_y < n_h_head; output_y++) {
                args.filter_height = filter_h -
                    ((args.padding_h_head - output_y * args.stride_y) + args.dilation_h - 1) / args.dilation_h;
                // Fix for filter_size > input_size
                int filter_height_excess = filter_h -
                    (args.input_height + (args.padding_h_head - output_y * args.stride_y) + args.dilation_h - 1) /
                        args.dilation_h;
                if (filter_height_excess > 0) {
                    args.filter_height -= filter_height_excess;
                } else {
                    filter_height_excess = 0;
                }

                input_y_real = input_ptr +
                    args.input_y_offset *
                        ((args.stride_y * output_y +
                          (filter_h - args.filter_height - filter_height_excess) * args.dilation_h) -
                         args.padding_h_head);
                filter_ptr_y = filter_ptr +
                    (filter_h - args.filter_height - filter_height_excess) * filter_w * filter_c_n_ptr_offset;
                args.filter_n_offset = (filter_w * (filter_h - args.filter_height)) * filter_c_n_offset;

                for (size_t output_x = 0; output_x < n_w_head; output_x++) {
                    args.filter_width = filter_w -
                        ((args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) / args.dilation_w;
                    // Fix for filter_size > input_size
                    int filter_width_excess = filter_w -
                        (args.input_width + (args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) /
                            args.dilation_w;
                    if (filter_width_excess > 0) {
                        args.filter_width -= filter_width_excess;
                    } else {
                        filter_width_excess = 0;
                    }

                    input_x_real = input_y_real +
                        args.input_channel *
                            ((args.stride_x * output_x +
                              (filter_w - args.filter_width - filter_width_excess) * args.dilation_w) -
                             args.padding_w_head);
                    args.filter_y_offset =
                        (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                    args.filter_element =
                        filter_ptr_y + (filter_w - args.filter_width - filter_width_excess) * filter_c_n_ptr_offset;
                    c_impl_func(buffer, input_x_real, args);
                    n_wise_tail(output_yx, buffer, args);
                    output_yx += args.output_x_offset;
                }

                input_x_real = input_y_real + args.input_channel * (args.stride_x * n_w_head - args.padding_w_head);
                args.filter_width = filter_w;
                args.filter_y_offset = 0; // ??? c， xtensa， tie 顺序不同
                args.filter_element = filter_ptr_y;
                for (size_t output_x = 0; output_x < n_w_body; output_x++) {
                    c_impl_func(buffer, input_x_real, args);
                    n_wise_tail(output_yx, buffer, args);
                    output_yx += args.output_x_offset;
                    input_x_real += args.input_stride_x_offset;
                }

                for (size_t output_x = 0; output_x < n_w_tail; output_x++) {
                    args.filter_width = (args.padding_w_head + args.input_width -
                                         (n_w_head + n_w_body + output_x) * args.stride_x + args.dilation_w - 1) /
                        args.dilation_w;
                    args.filter_y_offset =
                        (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                    c_impl_func(buffer, input_x_real, args);
                    n_wise_tail(output_yx, buffer, args);
                    output_yx += args.output_x_offset;
                    input_x_real += args.input_stride_x_offset;
                }
            }

            args.filter_height = filter_h;
            input_y_real = input_ptr + args.input_y_offset * ((args.stride_y * n_h_head) - args.padding_h_head);
            filter_ptr_y = filter_ptr;
            args.filter_n_offset = 0;

            for (size_t output_y = 0; output_y < n_h_body; output_y++) {
                for (size_t output_x = 0; output_x < n_w_head; output_x++) {
                    args.filter_width = filter_w -
                        ((args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) / args.dilation_w;
                    // Fix for filter_size > input_size
                    int filter_width_excess = filter_w -
                        (args.input_width + (args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) /
                            args.dilation_w;
                    if (filter_width_excess > 0) {
                        args.filter_width -= filter_width_excess;
                    } else {
                        filter_width_excess = 0;
                    }

                    input_x_real = input_y_real +
                        args.input_channel *
                            ((args.stride_x * output_x +
                              (filter_w - args.filter_width - filter_width_excess) * args.dilation_w) -
                             args.padding_w_head);
                    args.filter_y_offset =
                        (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                    args.filter_element =
                        filter_ptr_y + (filter_w - args.filter_width - filter_width_excess) * filter_c_n_ptr_offset;
                    c_impl_func(buffer, input_x_real, args);
                    n_wise_tail(output_yx, buffer, args);
                    output_yx += args.output_x_offset;
                }

                input_x_real = input_y_real + args.input_channel * (args.stride_x * n_w_head - args.padding_w_head);
                args.filter_width = filter_w;
                args.filter_y_offset = 0; // ??? c， xtensa， tie 顺序不同
                args.filter_element = filter_ptr_y;
                for (size_t output_x = 0; output_x < n_w_body; output_x++) {
                    c_impl_func_sp(buffer, input_x_real, args);
                    n_wise_tail(output_yx, buffer, args);
                    output_yx += args.output_x_offset;
                    input_x_real += args.input_stride_x_offset;
                }

                for (size_t output_x = 0; output_x < n_w_tail; output_x++) {
                    args.filter_width = (args.padding_w_head + args.input_width -
                                         (n_w_head + n_w_body + output_x) * args.stride_x + args.dilation_w - 1) /
                        args.dilation_w;
                    args.filter_y_offset =
                        (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                    c_impl_func(buffer, input_x_real, args);
                    n_wise_tail(output_yx, buffer, args);
                    output_yx += args.output_x_offset;
                    input_x_real += args.input_stride_x_offset;
                }
                input_y_real += args.input_stride_y_offset;
            }

            for (size_t output_y = 0; output_y < n_h_tail; output_y++) {
                args.filter_height = (args.padding_h_head + args.input_height -
                                      (n_h_head + n_h_body + output_y) * args.stride_y + args.dilation_h - 1) /
                    args.dilation_h;
                args.filter_n_offset = (filter_w * (filter_h - args.filter_height)) * filter_c_n_offset;

                for (size_t output_x = 0; output_x < n_w_head; output_x++) {
                    args.filter_width = filter_w -
                        ((args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) / args.dilation_w;
                    // Fix for filter_size > input_size
                    int filter_width_excess = filter_w -
                        (args.input_width + (args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) /
                            args.dilation_w;
                    if (filter_width_excess > 0) {
                        args.filter_width -= filter_width_excess;
                    } else {
                        filter_width_excess = 0;
                    }

                    input_x_real = input_y_real +
                        args.input_channel *
                            ((args.stride_x * output_x +
                              (filter_w - args.filter_width - filter_width_excess) * args.dilation_w) -
                             args.padding_w_head);
                    args.filter_y_offset =
                        (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                    args.filter_element =
                        filter_ptr_y + (filter_w - args.filter_width - filter_width_excess) * filter_c_n_ptr_offset;
                    c_impl_func(buffer, input_x_real, args);
                    n_wise_tail(output_yx, buffer, args);
                    output_yx += args.output_x_offset;
                }

                input_x_real = input_y_real + args.input_channel * (args.stride_x * n_w_head - args.padding_w_head);
                args.filter_width = filter_w;
                args.filter_y_offset = 0; // ??? c， xtensa， tie 顺序不同
                args.filter_element = filter_ptr_y;
                for (size_t output_x = 0; output_x < n_w_body; output_x++) {
                    c_impl_func(buffer, input_x_real, args);
                    n_wise_tail(output_yx, buffer, args);
                    output_yx += args.output_x_offset;
                    input_x_real += args.input_stride_x_offset;
                }

                for (size_t output_x = 0; output_x < n_w_tail; output_x++) {
                    args.filter_width = (args.padding_w_head + args.input_width -
                                         (n_w_head + n_w_body + output_x) * args.stride_x + args.dilation_w - 1) /
                        args.dilation_w;
                    args.filter_y_offset =
                        (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                    c_impl_func(buffer, input_x_real, args);
                    n_wise_tail(output_yx, buffer, args);
                    output_yx += args.output_x_offset;
                    input_x_real += args.input_stride_x_offset;
                }
                input_y_real += args.input_stride_y_offset;
            }
            heap_caps_free(buffer);
        }
    } else { // padding valid
        if (i_impl_func_sp) {
            if (n_wise_tail) {
                for (size_t output_y = 0; output_y < args.output_height; output_y++) {
                    feature_t *input_syx = input_ptr;
                    feature_t *output_yx = output_ptr;

                    for (size_t output_x = 0; output_x < args.output_width; output_x++) {
                        i_impl_func_sp(output_yx, input_syx, (void *const)&args);
                        n_wise_tail(output_yx, NULL, args);

                        input_syx += args.input_stride_x_offset;
                        output_yx += args.output_x_offset;
                    }
                    input_ptr += args.input_stride_y_offset;
                    output_ptr += args.output_y_offset;
                }
            } else {
                for (size_t output_y = 0; output_y < args.output_height; output_y++) {
                    feature_t *input_syx = input_ptr;
                    feature_t *output_yx = output_ptr;

                    for (size_t output_x = 0; output_x < args.output_width; output_x++) {
                        i_impl_func_sp(output_yx, input_syx, (void *const)&args);
                        input_syx += args.input_stride_x_offset;
                        output_yx += args.output_x_offset;
                    }
                    input_ptr += args.input_stride_y_offset;
                    output_ptr += args.output_y_offset;
                }
            }
        } else // run c_impl_func
        {
            buffer_t *buffer = (buffer_t *)heap_caps_calloc(args.output_channel, sizeof(buffer_t), MALLOC_CAP_DEFAULT);
            for (size_t output_y = 0; output_y < args.output_height; output_y++) {
                feature_t *input_syx = input_ptr;
                feature_t *output_yx = output_ptr;

                for (size_t output_x = 0; output_x < args.output_width; output_x++) {
                    c_impl_func_sp(buffer, input_syx, args);
                    n_wise_tail(output_yx, buffer, args);

                    input_syx += args.input_stride_x_offset;
                    output_yx += args.output_x_offset;
                }
                input_ptr += args.input_stride_y_offset;
                output_ptr += args.output_y_offset;
            }
            heap_caps_free(buffer);
        }
    }

    if (args.mac_shift == INT_MIN) {
        heap_caps_free(args.tie_filter_channel_factor);
        heap_caps_free(args.filter_channel_factor);
    }

    if (args.debug_value) {
        heap_caps_free(args.debug_value);
        args.debug_value = nullptr;
    }

    return;
}
/**
 * @brief
 *
 * @tparam feature_t
 * @tparam buffer_t
 * @param args
 * @param i_impl_func
 * @param i_impl_func_sp
 * @param c_impl_func
 * @param c_impl_func_sp
 * @param n_wise_tail
 */
template <typename feature_t, typename buffer_t>
void dwconv_operation_shell(ArgsType<feature_t> &args,
                            ImplFunc_t<feature_t, feature_t> i_impl_func,
                            ImplFunc_t<feature_t, feature_t> i_impl_func_sp,
                            void (*c_impl_func)(buffer_t *, feature_t *, const ArgsType<feature_t> &),
                            void (*c_impl_func_sp)(buffer_t *, feature_t *, const ArgsType<feature_t> &),
                            void (*n_wise_tail)(feature_t *, buffer_t *, const ArgsType<feature_t> &))
{
    feature_t *input_ptr = (feature_t *)args.input_element;
    feature_t *output_ptr = (feature_t *)args.output_element;
    if (args.padding_h_head || args.padding_w_head || args.padding_h_tail || args.padding_w_tail) { // padding same
        int n_h_head = (args.padding_h_head + args.stride_y - 1) / args.stride_y;
        int n_w_head = (args.padding_w_head + args.stride_x - 1) / args.stride_x;
        int n_h_body = ((args.input_height + args.padding_h_head - args.dilation_h * (args.filter_height - 1) - 1) /
                            args.stride_y +
                        1) -
            n_h_head;
        if (n_h_body < 0)
            n_h_body = 0;
        int n_w_body =
            ((args.input_width + args.padding_w_head - args.dilation_w * (args.filter_width - 1) - 1) / args.stride_x +
             1) -
            n_w_head;
        if (n_w_body < 0)
            n_w_body = 0;
        int n_h_tail = args.output_height - n_h_head - n_h_body;
        int n_w_tail = args.output_width - n_w_head - n_w_body;
        int filter_h = args.filter_height;
        int filter_w = args.filter_width;
        feature_t *filter_ptr = (feature_t *)(args.filter_element);

        if (i_impl_func_sp) {
            feature_t *input_y_real;
            feature_t *input_x_real;
            feature_t *filter_ptr_y;
            feature_t *output_yx = output_ptr;
            feature_t *filter_ptr_unaligned = (feature_t *)(args.filter_element_unaligned);
            feature_t *filter_ptr_y_unaligned;
            int unaligned_filter_c_n_offset = args.c_remainder;
            int c_remainder_num = args.c_remainder / sizeof(feature_t);
            int filter_c_n_offset = args.n_div_x ? args.filter_c * 16 : unaligned_filter_c_n_offset;
            int filter_c_n_ptr_offset = filter_c_n_offset / sizeof(feature_t);

            if (n_wise_tail) {
                for (size_t output_y = 0; output_y < n_h_head; output_y++) {
                    args.filter_height = filter_h -
                        ((args.padding_h_head - output_y * args.stride_y) + args.dilation_h - 1) / args.dilation_h;
                    // Fix for filter_size > input_size
                    int filter_height_excess = filter_h -
                        (args.input_height + (args.padding_h_head - output_y * args.stride_y) + args.dilation_h - 1) /
                            args.dilation_h;
                    if (filter_height_excess > 0) {
                        args.filter_height -= filter_height_excess;
                    } else {
                        filter_height_excess = 0;
                    }

                    input_y_real = input_ptr +
                        args.input_y_offset *
                            ((args.stride_y * output_y +
                              (filter_h - args.filter_height - filter_height_excess) * args.dilation_h) -
                             args.padding_h_head);
                    filter_ptr_y = filter_ptr +
                        (filter_h - args.filter_height - filter_height_excess) * filter_w * filter_c_n_ptr_offset;
                    filter_ptr_y_unaligned = filter_ptr_unaligned +
                        (filter_h - args.filter_height - filter_height_excess) * filter_w * c_remainder_num;
                    args.filter_n_offset = (filter_w * (filter_h - args.filter_height)) * filter_c_n_offset;

                    for (size_t output_x = 0; output_x < n_w_head; output_x++) {
                        args.filter_width = filter_w -
                            ((args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) / args.dilation_w;
                        // Fix for filter_size > input_size
                        int filter_width_excess = filter_w -
                            (args.input_width + (args.padding_w_head - output_x * args.stride_x) + args.dilation_w -
                             1) /
                                args.dilation_w;
                        if (filter_width_excess > 0) {
                            args.filter_width -= filter_width_excess;
                        } else {
                            filter_width_excess = 0;
                        }

                        args.filter_w_rs1_1 = (args.filter_width >> 1) - 1;
                        input_x_real = input_y_real +
                            args.input_channel *
                                ((args.stride_x * output_x +
                                  (filter_w - args.filter_width - filter_width_excess) * args.dilation_w) -
                                 args.padding_w_head);
                        args.filter_y_offset =
                            (filter_w - args.filter_width + 1) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.tie_depth2d_dilation_y_offset =
                            (args.tie_depth2d_dilation_y_offset_stable -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        args.tie_depth2d_next_hwx1 = 16 -
                            ((args.filter_width - 1) * args.dilation_w +
                             (args.filter_height - 1) * args.dilation_h * args.input_width) *
                                args.input_channel_with_padding * sizeof(feature_t);
                        args.filter_element =
                            filter_ptr_y + (filter_w - args.filter_width - filter_width_excess) * filter_c_n_ptr_offset;
                        args.filter_element_unaligned = filter_ptr_y_unaligned +
                            (filter_w - args.filter_width - filter_width_excess) * c_remainder_num;

                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        n_wise_tail(output_yx, NULL, args);
                        output_yx += args.output_x_offset;
                    }

                    input_x_real = input_y_real + args.input_channel * (args.stride_x * n_w_head - args.padding_w_head);
                    args.filter_width = filter_w;
                    args.filter_w_rs1_1 = (args.filter_width >> 1) - 1;
                    args.tie_depth2d_dilation_y_offset =
                        (args.tie_depth2d_dilation_y_offset_stable -
                         (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                        sizeof(feature_t);
                    args.tie_depth2d_next_hwx1 = 16 -
                        ((args.filter_width - 1) * args.dilation_w +
                         (args.filter_height - 1) * args.dilation_h * args.input_width) *
                            args.input_channel_with_padding * sizeof(feature_t);
                    args.filter_y_offset = filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                    args.filter_y_offset_unaligned = 0;
                    args.filter_element = filter_ptr_y;
                    args.filter_element_unaligned = filter_ptr_y_unaligned;
                    for (size_t output_x = 0; output_x < n_w_body; output_x++) {
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        n_wise_tail(output_yx, NULL, args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }

                    for (size_t output_x = 0; output_x < n_w_tail; output_x++) {
                        args.filter_width = (args.padding_w_head + args.input_width -
                                             (n_w_head + n_w_body + output_x) * args.stride_x + args.dilation_w - 1) /
                            args.dilation_w;
                        args.filter_w_rs1_1 = (args.filter_width >> 1) - 1;
                        args.filter_y_offset =
                            (filter_w - args.filter_width + 1) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.tie_depth2d_dilation_y_offset =
                            (args.tie_depth2d_dilation_y_offset_stable -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        args.tie_depth2d_next_hwx1 = 16 -
                            ((args.filter_width - 1) * args.dilation_w +
                             (args.filter_height - 1) * args.dilation_h * args.input_width) *
                                args.input_channel_with_padding * sizeof(feature_t);
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        n_wise_tail(output_yx, NULL, args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }
                }

                // h_body
                args.filter_height = filter_h;
                input_y_real = input_ptr + args.input_y_offset * ((args.stride_y * n_h_head) - args.padding_h_head);
                filter_ptr_y = filter_ptr;
                filter_ptr_y_unaligned = filter_ptr_unaligned;
                args.filter_n_offset = 0;
                for (size_t output_y = 0; output_y < n_h_body; output_y++) {
                    for (size_t output_x = 0; output_x < n_w_head; output_x++) {
                        args.filter_width = filter_w -
                            ((args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) / args.dilation_w;
                        // Fix for filter_size > input_size
                        int filter_width_excess = filter_w -
                            (args.input_width + (args.padding_w_head - output_x * args.stride_x) + args.dilation_w -
                             1) /
                                args.dilation_w;
                        if (filter_width_excess > 0) {
                            args.filter_width -= filter_width_excess;
                        } else {
                            filter_width_excess = 0;
                        }

                        args.filter_w_rs1_1 = (args.filter_width >> 1) - 1;
                        input_x_real = input_y_real +
                            args.input_channel *
                                ((args.stride_x * output_x +
                                  (filter_w - args.filter_width - filter_width_excess) * args.dilation_w) -
                                 args.padding_w_head);
                        args.filter_y_offset =
                            (filter_w - args.filter_width + 1) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.tie_depth2d_dilation_y_offset =
                            (args.tie_depth2d_dilation_y_offset_stable -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        args.tie_depth2d_next_hwx1 = 16 -
                            ((args.filter_width - 1) * args.dilation_w +
                             (args.filter_height - 1) * args.dilation_h * args.input_width) *
                                args.input_channel_with_padding * sizeof(feature_t);
                        args.filter_element =
                            filter_ptr_y + (filter_w - args.filter_width - filter_width_excess) * filter_c_n_ptr_offset;
                        args.filter_element_unaligned = filter_ptr_y_unaligned +
                            (filter_w - args.filter_width - filter_width_excess) * c_remainder_num;
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        n_wise_tail(output_yx, NULL, args);
                        output_yx += args.output_x_offset;
                    }

                    input_x_real = input_y_real + args.input_channel * (args.stride_x * n_w_head - args.padding_w_head);
                    args.filter_width = filter_w;
                    args.filter_w_rs1_1 = (args.filter_width >> 1) - 1;
                    args.tie_depth2d_dilation_y_offset =
                        (args.tie_depth2d_dilation_y_offset_stable -
                         (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                        sizeof(feature_t);
                    args.tie_depth2d_next_hwx1 = 16 -
                        ((args.filter_width - 1) * args.dilation_w +
                         (args.filter_height - 1) * args.dilation_h * args.input_width) *
                            args.input_channel_with_padding * sizeof(feature_t);
                    args.filter_y_offset = filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                    args.filter_y_offset_unaligned = 0;
                    args.filter_element = filter_ptr_y;
                    args.filter_element_unaligned = filter_ptr_y_unaligned;

                    for (size_t output_x = 0; output_x < n_w_body; output_x++) {
                        i_impl_func_sp(output_yx, input_x_real, (void *const)&args);
                        n_wise_tail(output_yx, NULL, args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }

                    for (size_t output_x = 0; output_x < n_w_tail; output_x++) {
                        args.filter_width = (args.padding_w_head + args.input_width -
                                             (n_w_head + n_w_body + output_x) * args.stride_x + args.dilation_w - 1) /
                            args.dilation_w;
                        args.filter_w_rs1_1 = (args.filter_width >> 1) - 1;
                        args.filter_y_offset =
                            (filter_w - args.filter_width + 1) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.tie_depth2d_dilation_y_offset =
                            (args.tie_depth2d_dilation_y_offset_stable -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        args.tie_depth2d_next_hwx1 = 16 -
                            ((args.filter_width - 1) * args.dilation_w +
                             (args.filter_height - 1) * args.dilation_h * args.input_width) *
                                args.input_channel_with_padding * sizeof(feature_t);
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        n_wise_tail(output_yx, NULL, args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }
                    input_y_real += args.input_stride_y_offset;
                }

                // h_tail
                for (size_t output_y = 0; output_y < n_h_tail; output_y++) {
                    args.filter_height = (args.padding_h_head + args.input_height -
                                          (n_h_head + n_h_body + output_y) * args.stride_y + args.dilation_h - 1) /
                        args.dilation_h;
                    args.filter_n_offset = (filter_w * (filter_h - args.filter_height)) * filter_c_n_offset;

                    for (size_t output_x = 0; output_x < n_w_head; output_x++) {
                        args.filter_width = filter_w -
                            ((args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) / args.dilation_w;
                        // Fix for filter_size > input_size
                        int filter_width_excess = filter_w -
                            (args.input_width + (args.padding_w_head - output_x * args.stride_x) + args.dilation_w -
                             1) /
                                args.dilation_w;
                        if (filter_width_excess > 0) {
                            args.filter_width -= filter_width_excess;
                        } else {
                            filter_width_excess = 0;
                        }

                        args.filter_w_rs1_1 = (args.filter_width >> 1) - 1;
                        input_x_real = input_y_real +
                            args.input_channel *
                                ((args.stride_x * output_x +
                                  (filter_w - args.filter_width - filter_width_excess) * args.dilation_w) -
                                 args.padding_w_head);
                        args.filter_y_offset =
                            (filter_w - args.filter_width + 1) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.tie_depth2d_dilation_y_offset =
                            (args.tie_depth2d_dilation_y_offset_stable -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        args.tie_depth2d_next_hwx1 = 16 -
                            ((args.filter_width - 1) * args.dilation_w +
                             (args.filter_height - 1) * args.dilation_h * args.input_width) *
                                args.input_channel_with_padding * sizeof(feature_t);
                        args.filter_element =
                            filter_ptr_y + (filter_w - args.filter_width - filter_width_excess) * filter_c_n_ptr_offset;
                        args.filter_element_unaligned = filter_ptr_y_unaligned +
                            (filter_w - args.filter_width - filter_width_excess) * c_remainder_num;
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        n_wise_tail(output_yx, NULL, args);
                        output_yx += args.output_x_offset;
                    }

                    input_x_real = input_y_real + args.input_channel * (args.stride_x * n_w_head - args.padding_w_head);
                    args.filter_width = filter_w;
                    args.filter_w_rs1_1 = (args.filter_width >> 1) - 1;
                    args.filter_y_offset = filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                    args.filter_y_offset_unaligned = 0;
                    args.tie_depth2d_dilation_y_offset =
                        (args.tie_depth2d_dilation_y_offset_stable -
                         (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                        sizeof(feature_t);
                    args.tie_depth2d_next_hwx1 = 16 -
                        ((args.filter_width - 1) * args.dilation_w +
                         (args.filter_height - 1) * args.dilation_h * args.input_width) *
                            args.input_channel_with_padding * sizeof(feature_t);
                    args.filter_element = filter_ptr_y;
                    args.filter_element_unaligned = filter_ptr_y_unaligned;
                    for (size_t output_x = 0; output_x < n_w_body; output_x++) {
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        n_wise_tail(output_yx, NULL, args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }

                    for (size_t output_x = 0; output_x < n_w_tail; output_x++) {
                        args.filter_width = (args.padding_w_head + args.input_width -
                                             (n_w_head + n_w_body + output_x) * args.stride_x + args.dilation_w - 1) /
                            args.dilation_w;
                        args.filter_w_rs1_1 = (args.filter_width >> 1) - 1;
                        args.filter_y_offset =
                            (filter_w - args.filter_width + 1) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.tie_depth2d_dilation_y_offset =
                            (args.tie_depth2d_dilation_y_offset_stable -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        args.tie_depth2d_next_hwx1 = 16 -
                            ((args.filter_width - 1) * args.dilation_w +
                             (args.filter_height - 1) * args.dilation_h * args.input_width) *
                                args.input_channel_with_padding * sizeof(feature_t);
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        n_wise_tail(output_yx, NULL, args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }
                    input_y_real += args.input_stride_y_offset;
                }
            } else // without n_wise_tail
            {
                // h_head
                for (size_t output_y = 0; output_y < n_h_head; output_y++) {
                    args.filter_height = filter_h -
                        ((args.padding_h_head - output_y * args.stride_y) + args.dilation_h - 1) / args.dilation_h;
                    // Fix for filter_size > input_size
                    int filter_height_excess = filter_h -
                        (args.input_height + (args.padding_h_head - output_y * args.stride_y) + args.dilation_h - 1) /
                            args.dilation_h;
                    if (filter_height_excess > 0) {
                        args.filter_height -= filter_height_excess;
                    } else {
                        filter_height_excess = 0;
                    }

                    input_y_real = input_ptr +
                        args.input_y_offset *
                            ((args.stride_y * output_y +
                              (filter_h - args.filter_height - filter_height_excess) * args.dilation_h) -
                             args.padding_h_head);
                    filter_ptr_y = filter_ptr +
                        (filter_h - args.filter_height - filter_height_excess) * filter_w * filter_c_n_ptr_offset;
                    filter_ptr_y_unaligned = filter_ptr_unaligned +
                        (filter_h - args.filter_height - filter_height_excess) * filter_w * c_remainder_num;
                    args.filter_n_offset = (filter_w * (filter_h - args.filter_height)) * filter_c_n_offset;

                    for (size_t output_x = 0; output_x < n_w_head; output_x++) {
                        args.filter_width = filter_w -
                            ((args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) / args.dilation_w;
                        // Fix for filter_size > input_size
                        int filter_width_excess = filter_w -
                            (args.input_width + (args.padding_w_head - output_x * args.stride_x) + args.dilation_w -
                             1) /
                                args.dilation_w;
                        if (filter_width_excess > 0) {
                            args.filter_width -= filter_width_excess;
                        } else {
                            filter_width_excess = 0;
                        }

                        args.filter_w_rs1_1 = (args.filter_width >> 1) - 1;
                        input_x_real = input_y_real +
                            args.input_channel *
                                ((args.stride_x * output_x +
                                  (filter_w - args.filter_width - filter_width_excess) * args.dilation_w) -
                                 args.padding_w_head);
                        args.filter_y_offset =
                            (filter_w - args.filter_width + 1) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.tie_depth2d_dilation_y_offset =
                            (args.tie_depth2d_dilation_y_offset_stable -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        args.tie_depth2d_next_hwx1 = 16 -
                            ((args.filter_width - 1) * args.dilation_w +
                             (args.filter_height - 1) * args.dilation_h * args.input_width) *
                                args.input_channel_with_padding * sizeof(feature_t);
                        args.filter_element =
                            filter_ptr_y + (filter_w - args.filter_width - filter_width_excess) * filter_c_n_ptr_offset;
                        args.filter_element_unaligned = filter_ptr_y_unaligned +
                            (filter_w - args.filter_width - filter_width_excess) * c_remainder_num;

                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        output_yx += args.output_x_offset;
                    }

                    input_x_real = input_y_real + args.input_channel * (args.stride_x * n_w_head - args.padding_w_head);
                    args.filter_width = filter_w;
                    args.filter_w_rs1_1 = (args.filter_width >> 1) - 1;
                    args.tie_depth2d_dilation_y_offset =
                        (args.tie_depth2d_dilation_y_offset_stable -
                         (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                        sizeof(feature_t);
                    args.tie_depth2d_next_hwx1 = 16 -
                        ((args.filter_width - 1) * args.dilation_w +
                         (args.filter_height - 1) * args.dilation_h * args.input_width) *
                            args.input_channel_with_padding * sizeof(feature_t);
                    args.filter_y_offset = filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                    args.filter_y_offset_unaligned = 0;
                    args.filter_element = filter_ptr_y;
                    args.filter_element_unaligned = filter_ptr_y_unaligned;

                    for (size_t output_x = 0; output_x < n_w_body; output_x++) {
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }

                    for (size_t output_x = 0; output_x < n_w_tail; output_x++) {
                        args.filter_width = (args.padding_w_head + args.input_width -
                                             (n_w_head + n_w_body + output_x) * args.stride_x + args.dilation_w - 1) /
                            args.dilation_w;
                        args.filter_w_rs1_1 = (args.filter_width >> 1) - 1;
                        args.filter_y_offset =
                            (filter_w - args.filter_width + 1) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.tie_depth2d_dilation_y_offset =
                            (args.tie_depth2d_dilation_y_offset_stable -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        args.tie_depth2d_next_hwx1 = 16 -
                            ((args.filter_width - 1) * args.dilation_w +
                             (args.filter_height - 1) * args.dilation_h * args.input_width) *
                                args.input_channel_with_padding * sizeof(feature_t);
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }
                }

                // h_body
                args.filter_height = filter_h;
                input_y_real = input_ptr + args.input_y_offset * ((args.stride_y * n_h_head) - args.padding_h_head);
                filter_ptr_y = filter_ptr;
                filter_ptr_y_unaligned = filter_ptr_unaligned;
                args.filter_n_offset = 0;
                for (size_t output_y = 0; output_y < n_h_body; output_y++) {
                    for (size_t output_x = 0; output_x < n_w_head; output_x++) {
                        args.filter_width = filter_w -
                            ((args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) / args.dilation_w;
                        // Fix for filter_size > input_size
                        int filter_width_excess = filter_w -
                            (args.input_width + (args.padding_w_head - output_x * args.stride_x) + args.dilation_w -
                             1) /
                                args.dilation_w;
                        if (filter_width_excess > 0) {
                            args.filter_width -= filter_width_excess;
                        } else {
                            filter_width_excess = 0;
                        }

                        args.filter_w_rs1_1 = (args.filter_width >> 1) - 1;
                        input_x_real = input_y_real +
                            args.input_channel *
                                ((args.stride_x * output_x +
                                  (filter_w - args.filter_width - filter_width_excess) * args.dilation_w) -
                                 args.padding_w_head);
                        args.filter_y_offset =
                            (filter_w - args.filter_width + 1) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.tie_depth2d_dilation_y_offset =
                            (args.tie_depth2d_dilation_y_offset_stable -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        args.tie_depth2d_next_hwx1 = 16 -
                            ((args.filter_width - 1) * args.dilation_w +
                             (args.filter_height - 1) * args.dilation_h * args.input_width) *
                                args.input_channel_with_padding * sizeof(feature_t);
                        args.filter_element =
                            filter_ptr_y + (filter_w - args.filter_width - filter_width_excess) * filter_c_n_ptr_offset;
                        args.filter_element_unaligned = filter_ptr_y_unaligned +
                            (filter_w - args.filter_width - filter_width_excess) * c_remainder_num;
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        output_yx += args.output_x_offset;
                    }

                    input_x_real = input_y_real + args.input_channel * (args.stride_x * n_w_head - args.padding_w_head);
                    args.filter_width = filter_w;
                    args.filter_w_rs1_1 = (args.filter_width >> 1) - 1;
                    args.tie_depth2d_dilation_y_offset =
                        (args.tie_depth2d_dilation_y_offset_stable -
                         (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                        sizeof(feature_t);
                    args.tie_depth2d_next_hwx1 = 16 -
                        ((args.filter_width - 1) * args.dilation_w +
                         (args.filter_height - 1) * args.dilation_h * args.input_width) *
                            args.input_channel_with_padding * sizeof(feature_t);
                    args.filter_y_offset = filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                    args.filter_y_offset_unaligned = 0;
                    args.filter_element = filter_ptr_y;
                    args.filter_element_unaligned = filter_ptr_y_unaligned;

                    for (size_t output_x = 0; output_x < n_w_body; output_x++) {
                        i_impl_func_sp(output_yx, input_x_real, (void *const)&args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }

                    for (size_t output_x = 0; output_x < n_w_tail; output_x++) {
                        args.filter_width = (args.padding_w_head + args.input_width -
                                             (n_w_head + n_w_body + output_x) * args.stride_x + args.dilation_w - 1) /
                            args.dilation_w;
                        args.filter_w_rs1_1 = (args.filter_width >> 1) - 1;
                        args.filter_y_offset =
                            (filter_w - args.filter_width + 1) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.tie_depth2d_dilation_y_offset =
                            (args.tie_depth2d_dilation_y_offset_stable -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        args.tie_depth2d_next_hwx1 = 16 -
                            ((args.filter_width - 1) * args.dilation_w +
                             (args.filter_height - 1) * args.dilation_h * args.input_width) *
                                args.input_channel_with_padding * sizeof(feature_t);
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }
                    input_y_real += args.input_stride_y_offset;
                }

                // h_tail
                for (size_t output_y = 0; output_y < n_h_tail; output_y++) {
                    args.filter_height = (args.padding_h_head + args.input_height -
                                          (n_h_head + n_h_body + output_y) * args.stride_y + args.dilation_h - 1) /
                        args.dilation_h;
                    args.filter_n_offset = (filter_w * (filter_h - args.filter_height)) * filter_c_n_offset;

                    for (size_t output_x = 0; output_x < n_w_head; output_x++) {
                        args.filter_width = filter_w -
                            ((args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) / args.dilation_w;
                        // Fix for filter_size > input_size
                        int filter_width_excess = filter_w -
                            (args.input_width + (args.padding_w_head - output_x * args.stride_x) + args.dilation_w -
                             1) /
                                args.dilation_w;
                        if (filter_width_excess > 0) {
                            args.filter_width -= filter_width_excess;
                        } else {
                            filter_width_excess = 0;
                        }

                        args.filter_w_rs1_1 = (args.filter_width >> 1) - 1;
                        input_x_real = input_y_real +
                            args.input_channel *
                                ((args.stride_x * output_x +
                                  (filter_w - args.filter_width - filter_width_excess) * args.dilation_w) -
                                 args.padding_w_head);
                        args.filter_y_offset =
                            (filter_w - args.filter_width + 1) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.tie_depth2d_dilation_y_offset =
                            (args.tie_depth2d_dilation_y_offset_stable -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        args.tie_depth2d_next_hwx1 = 16 -
                            ((args.filter_width - 1) * args.dilation_w +
                             (args.filter_height - 1) * args.dilation_h * args.input_width) *
                                args.input_channel_with_padding * sizeof(feature_t);
                        args.filter_element =
                            filter_ptr_y + (filter_w - args.filter_width - filter_width_excess) * filter_c_n_ptr_offset;
                        args.filter_element_unaligned = filter_ptr_y_unaligned +
                            (filter_w - args.filter_width - filter_width_excess) * c_remainder_num;
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        output_yx += args.output_x_offset;
                    }

                    input_x_real = input_y_real + args.input_channel * (args.stride_x * n_w_head - args.padding_w_head);
                    args.filter_width = filter_w;
                    args.filter_w_rs1_1 = (args.filter_width >> 1) - 1;
                    args.filter_y_offset = filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                    args.filter_y_offset_unaligned = 0;
                    args.tie_depth2d_dilation_y_offset =
                        (args.tie_depth2d_dilation_y_offset_stable -
                         (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                        sizeof(feature_t);
                    args.tie_depth2d_next_hwx1 = 16 -
                        ((args.filter_width - 1) * args.dilation_w +
                         (args.filter_height - 1) * args.dilation_h * args.input_width) *
                            args.input_channel_with_padding * sizeof(feature_t);
                    args.filter_element = filter_ptr_y;
                    args.filter_element_unaligned = filter_ptr_y_unaligned;
                    for (size_t output_x = 0; output_x < n_w_body; output_x++) {
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }

                    for (size_t output_x = 0; output_x < n_w_tail; output_x++) {
                        args.filter_width = (args.padding_w_head + args.input_width -
                                             (n_w_head + n_w_body + output_x) * args.stride_x + args.dilation_w - 1) /
                            args.dilation_w;
                        args.filter_w_rs1_1 = (args.filter_width >> 1) - 1;
                        args.filter_y_offset =
                            (filter_w - args.filter_width + 1) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                        args.filter_y_offset_unaligned = (filter_w - args.filter_width) * unaligned_filter_c_n_offset;
                        args.tie_depth2d_dilation_y_offset =
                            (args.tie_depth2d_dilation_y_offset_stable -
                             (args.filter_width - 1) * args.dilation_w * args.input_channel_with_padding) *
                            sizeof(feature_t);
                        args.tie_depth2d_next_hwx1 = 16 -
                            ((args.filter_width - 1) * args.dilation_w +
                             (args.filter_height - 1) * args.dilation_h * args.input_width) *
                                args.input_channel_with_padding * sizeof(feature_t);
                        i_impl_func(output_yx, input_x_real, (void *const)&args);
                        output_yx += args.output_x_offset;
                        input_x_real += args.input_stride_x_offset;
                    }
                    input_y_real += args.input_stride_y_offset;
                }
            }
        } else // run c_impl_func
        {
            buffer_t *buffer = (buffer_t *)heap_caps_calloc(args.output_channel, sizeof(buffer_t), MALLOC_CAP_DEFAULT);
            feature_t *input_y_real;
            feature_t *input_x_real;
            feature_t *filter_ptr_y;
            feature_t *output_yx = output_ptr;
            int filter_c_n_offset = args.input_channel;
            int filter_c_n_ptr_offset = filter_c_n_offset;

            for (size_t output_y = 0; output_y < n_h_head; output_y++) {
                args.filter_height = filter_h -
                    ((args.padding_h_head - output_y * args.stride_y) + args.dilation_h - 1) / args.dilation_h;
                // Fix for filter_size > input_size
                int filter_height_excess = filter_h -
                    (args.input_height + (args.padding_h_head - output_y * args.stride_y) + args.dilation_h - 1) /
                        args.dilation_h;
                if (filter_height_excess > 0) {
                    args.filter_height -= filter_height_excess;
                } else {
                    filter_height_excess = 0;
                }

                input_y_real = input_ptr +
                    args.input_y_offset *
                        ((args.stride_y * output_y +
                          (filter_h - args.filter_height - filter_height_excess) * args.dilation_h) -
                         args.padding_h_head);
                filter_ptr_y = filter_ptr +
                    (filter_h - args.filter_height - filter_height_excess) * filter_w * filter_c_n_ptr_offset;

                for (size_t output_x = 0; output_x < n_w_head; output_x++) {
                    args.filter_width = filter_w -
                        ((args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) / args.dilation_w;
                    // Fix for filter_size > input_size
                    int filter_width_excess = filter_w -
                        (args.input_width + (args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) /
                            args.dilation_w;
                    if (filter_width_excess > 0) {
                        args.filter_width -= filter_width_excess;
                    } else {
                        filter_width_excess = 0;
                    }

                    input_x_real = input_y_real +
                        args.input_channel *
                            ((args.stride_x * output_x +
                              (filter_w - args.filter_width - filter_width_excess) * args.dilation_w) -
                             args.padding_w_head);
                    args.filter_y_offset =
                        (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                    args.filter_element =
                        filter_ptr_y + (filter_w - args.filter_width - filter_width_excess) * filter_c_n_ptr_offset;
                    c_impl_func(buffer, input_x_real, args);
                    n_wise_tail(output_yx, buffer, args);
                    output_yx += args.output_x_offset;
                }

                input_x_real = input_y_real + args.input_channel * (args.stride_x * n_w_head - args.padding_w_head);
                args.filter_width = filter_w;
                args.filter_y_offset = 0; // ??? c， xtensa， tie 顺序不同
                args.filter_element = filter_ptr_y;
                for (size_t output_x = 0; output_x < n_w_body; output_x++) {
                    c_impl_func(buffer, input_x_real, args);
                    n_wise_tail(output_yx, buffer, args);
                    output_yx += args.output_x_offset;
                    input_x_real += args.input_stride_x_offset;
                }

                for (size_t output_x = 0; output_x < n_w_tail; output_x++) {
                    args.filter_width = (args.padding_w_head + args.input_width -
                                         (n_w_head + n_w_body + output_x) * args.stride_x + args.dilation_w - 1) /
                        args.dilation_w;
                    args.filter_y_offset =
                        (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                    c_impl_func(buffer, input_x_real, args);
                    n_wise_tail(output_yx, buffer, args);
                    output_yx += args.output_x_offset;
                    input_x_real += args.input_stride_x_offset;
                }
            }

            args.filter_height = filter_h;
            input_y_real = input_ptr + args.input_y_offset * ((args.stride_y * n_h_head) - args.padding_h_head);
            filter_ptr_y = filter_ptr;

            for (size_t output_y = 0; output_y < n_h_body; output_y++) {
                for (size_t output_x = 0; output_x < n_w_head; output_x++) {
                    args.filter_width = filter_w -
                        ((args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) / args.dilation_w;
                    // Fix for filter_size > input_size
                    int filter_width_excess = filter_w -
                        (args.input_width + (args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) /
                            args.dilation_w;
                    if (filter_width_excess > 0) {
                        args.filter_width -= filter_width_excess;
                    } else {
                        filter_width_excess = 0;
                    }

                    input_x_real = input_y_real +
                        args.input_channel *
                            ((args.stride_x * output_x +
                              (filter_w - args.filter_width - filter_width_excess) * args.dilation_w) -
                             args.padding_w_head);
                    args.filter_y_offset =
                        (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                    args.filter_element =
                        filter_ptr_y + (filter_w - args.filter_width - filter_width_excess) * filter_c_n_ptr_offset;
                    c_impl_func(buffer, input_x_real, args);
                    n_wise_tail(output_yx, buffer, args);
                    output_yx += args.output_x_offset;
                }

                input_x_real = input_y_real + args.input_channel * (args.stride_x * n_w_head - args.padding_w_head);
                args.filter_width = filter_w;
                args.filter_y_offset = 0; // ??? c， xtensa， tie 顺序不同
                args.filter_element = filter_ptr_y;
                for (size_t output_x = 0; output_x < n_w_body; output_x++) {
                    c_impl_func_sp(buffer, input_x_real, args);
                    n_wise_tail(output_yx, buffer, args);
                    output_yx += args.output_x_offset;
                    input_x_real += args.input_stride_x_offset;
                }

                for (size_t output_x = 0; output_x < n_w_tail; output_x++) {
                    args.filter_width = (args.padding_w_head + args.input_width -
                                         (n_w_head + n_w_body + output_x) * args.stride_x + args.dilation_w - 1) /
                        args.dilation_w;
                    args.filter_y_offset =
                        (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                    c_impl_func(buffer, input_x_real, args);
                    n_wise_tail(output_yx, buffer, args);
                    output_yx += args.output_x_offset;
                    input_x_real += args.input_stride_x_offset;
                }
                input_y_real += args.input_stride_y_offset;
            }

            for (size_t output_y = 0; output_y < n_h_tail; output_y++) {
                args.filter_height = (args.padding_h_head + args.input_height -
                                      (n_h_head + n_h_body + output_y) * args.stride_y + args.dilation_h - 1) /
                    args.dilation_h;

                for (size_t output_x = 0; output_x < n_w_head; output_x++) {
                    args.filter_width = filter_w -
                        ((args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) / args.dilation_w;
                    // Fix for filter_size > input_size
                    int filter_width_excess = filter_w -
                        (args.input_width + (args.padding_w_head - output_x * args.stride_x) + args.dilation_w - 1) /
                            args.dilation_w;
                    if (filter_width_excess > 0) {
                        args.filter_width -= filter_width_excess;
                    } else {
                        filter_width_excess = 0;
                    }

                    input_x_real = input_y_real +
                        args.input_channel *
                            ((args.stride_x * output_x +
                              (filter_w - args.filter_width - filter_width_excess) * args.dilation_w) -
                             args.padding_w_head);
                    args.filter_y_offset =
                        (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                    args.filter_element =
                        filter_ptr_y + (filter_w - args.filter_width - filter_width_excess) * filter_c_n_ptr_offset;
                    c_impl_func(buffer, input_x_real, args);
                    n_wise_tail(output_yx, buffer, args);
                    output_yx += args.output_x_offset;
                }

                input_x_real = input_y_real + args.input_channel * (args.stride_x * n_w_head - args.padding_w_head);
                args.filter_width = filter_w;
                args.filter_y_offset = 0; // ??? c， xtensa， tie 顺序不同
                args.filter_element = filter_ptr_y;
                for (size_t output_x = 0; output_x < n_w_body; output_x++) {
                    c_impl_func(buffer, input_x_real, args);
                    n_wise_tail(output_yx, buffer, args);
                    output_yx += args.output_x_offset;
                    input_x_real += args.input_stride_x_offset;
                }

                for (size_t output_x = 0; output_x < n_w_tail; output_x++) {
                    args.filter_width = (args.padding_w_head + args.input_width -
                                         (n_w_head + n_w_body + output_x) * args.stride_x + args.dilation_w - 1) /
                        args.dilation_w;
                    args.filter_y_offset =
                        (filter_w - args.filter_width) * filter_c_n_offset; // ??? c， xtensa， tie 顺序不同
                    c_impl_func(buffer, input_x_real, args);
                    n_wise_tail(output_yx, buffer, args);
                    output_yx += args.output_x_offset;
                    input_x_real += args.input_stride_x_offset;
                }
                input_y_real += args.input_stride_y_offset;
            }
            heap_caps_free(buffer);
        }
    } else { // padding valid
        if (i_impl_func_sp) {
            if (n_wise_tail) {
                for (size_t output_y = 0; output_y < args.output_height; output_y++) {
                    feature_t *input_syx = input_ptr;
                    feature_t *output_yx = output_ptr;

                    for (size_t output_x = 0; output_x < args.output_width; output_x++) {
                        i_impl_func_sp(output_yx, input_syx, (void *const)&args);
                        n_wise_tail(output_yx, NULL, args);

                        input_syx += args.input_stride_x_offset;
                        output_yx += args.output_x_offset;
                    }
                    input_ptr += args.input_stride_y_offset;
                    output_ptr += args.output_y_offset;
                }
            } else {
                for (size_t output_y = 0; output_y < args.output_height; output_y++) {
                    feature_t *input_syx = input_ptr;
                    feature_t *output_yx = output_ptr;

                    for (size_t output_x = 0; output_x < args.output_width; output_x++) {
                        i_impl_func_sp(output_yx, input_syx, (void *const)&args);

                        input_syx += args.input_stride_x_offset;
                        output_yx += args.output_x_offset;
                    }
                    input_ptr += args.input_stride_y_offset;
                    output_ptr += args.output_y_offset;
                }
            }
        } else // run c_impl_func
        {
            args.filter_y_offset = 0;
            buffer_t *buffer = (buffer_t *)heap_caps_calloc(args.output_channel, sizeof(buffer_t), MALLOC_CAP_DEFAULT);
            for (size_t output_y = 0; output_y < args.output_height; output_y++) {
                feature_t *input_syx = input_ptr;
                feature_t *output_yx = output_ptr;

                for (size_t output_x = 0; output_x < args.output_width; output_x++) {
                    c_impl_func_sp(buffer, input_syx, args);
                    n_wise_tail(output_yx, buffer, args);

                    input_syx += args.input_stride_x_offset;
                    output_yx += args.output_x_offset;
                }
                input_ptr += args.input_stride_y_offset;
                output_ptr += args.output_y_offset;
            }
            heap_caps_free(buffer);
        }
    }

    if (args.mac_shift == INT_MIN) {
        heap_caps_free(args.tie_filter_channel_factor);
        heap_caps_free(args.filter_channel_factor);
    }

    if (args.debug_value) {
        heap_caps_free(args.debug_value);
        args.debug_value = nullptr;
    }

    return;
}

typedef void (*c_impl_acti_s16_t)(int16_t *, int16_t *, const ArgsType<int16_t> &);
typedef void (*c_impl_acti_s8_t)(int8_t *, int8_t *, const ArgsType<int8_t> &);

/**
 * @brief Get the activation args object
 *
 * @tparam feature_t
 * @param output
 * @param input
 * @param activate
 * @param activation_alpha
 * @param runtime_mode
 * @return std::vector<ArgsType<feature_t>>
 */
template <typename feature_t>
std::vector<ArgsType<feature_t>> get_activation_args(TensorBase *output,
                                                     TensorBase *input,
                                                     const activation_type_t activate = Linear,
                                                     TensorBase *activation_alpha = nullptr,
                                                     const runtime_mode_t runtime_mode = RUNTIME_MODE_AUTO)
{
    ArgsType<feature_t> args;
    args.input_element = (feature_t *)input->get_element_ptr();
    args.input_channel = input->shape[3];
    // args.input_stride_y_offset = input->shape[2] * input->shape[3];
    args.input_stride_x_offset = input->shape[3];

    args.output_element = (feature_t *)output->get_element_ptr();
    args.output_height = output->shape[1];
    args.output_width = output->shape[2];
    args.output_channel = output->shape[3];
    // args.output_y_offset = output->shape[2] * output->shape[3];
    args.output_x_offset = output->shape[3];

    args.output_scale = 1;
    if (output->exponent == input->exponent)
        args.output_shift = 0;
    else
        args.output_shift = output->exponent - input->exponent;

    if (args.output_shift < 0) { // ( * output_scale ) >> output_shift
        args.output_scale = 1 << (-args.output_shift);
        args.output_shift = 0;
    }

    switch (activate) {
    case ReLU:
        args.activation_alpha = 0;
        args.activation_shift = 0;
        args.activation_alpha_ptr = NULL;
        break;
    case LeakyReLU:
        args.activation_alpha = *((int *)activation_alpha->get_element_ptr());
        args.activation_shift = -activation_alpha->exponent;
        args.activation_alpha_ptr = NULL;
        break;
    case PReLU:
        args.activation_alpha_ptr = activation_alpha->get_element_ptr();
        args.activation_shift = output->exponent - input->exponent - activation_alpha->exponent;
        break;
    default:
        args.activation_alpha_ptr = NULL;
        args.activation_shift = -1;
        break;
    }
    // for ISA
    int u = 16 / sizeof(feature_t);
    args.c_div_x_1 = input->shape[3] / u - 1;
    args.c_remainder = (args.input_channel % u) * sizeof(feature_t);

    int c_div_x = input->shape[3] / u;
    args.n_div_x = c_div_x;
    args.c_rs1_1 = DL_MAX(c_div_x / 2 - 1, 0);     // actually c / 2u - 1
    args.c_rs2_1 = c_div_x - 2 * args.c_rs1_1 - 1; // actually c left - 1

    // TODO: slice
    std::vector<ArgsType<feature_t>> m_args(1, args);
    if (runtime_mode == RUNTIME_MODE_MULTI_CORE) {
        // TODO:
    }

    return m_args;
}

template <typename feature_t>
void activation_shell(const ArgsType<feature_t> &args,
                      ImplFunc_t<feature_t, feature_t> i_impl_func,
                      void (*c_impl_func)(feature_t *, feature_t *, const ArgsType<feature_t> &))
{
    feature_t *input_ptr = (feature_t *)args.input_element;
    feature_t *output_ptr = (feature_t *)args.output_element;
    size_t loop_size = args.output_height * args.output_width;

    if (i_impl_func) {
        for (size_t i = 0; i < loop_size; i++) {
            i_impl_func(output_ptr, input_ptr, (void *const)&args);
            input_ptr += args.input_stride_x_offset;
            output_ptr += args.output_x_offset;
        }
    } else // run c_impl_func
    {
        for (size_t i = 0; i < loop_size; i++) {
            c_impl_func(output_ptr, input_ptr, args);
            input_ptr += args.input_stride_x_offset;
            output_ptr += args.output_x_offset;
        }
    }
    return;
}

// For Arithmetic: Add, Sub, Mul etc
template <typename feature_t>
struct arithArgsType {
    feature_t *input0_element;         /*!< 0 */
    int input0_y_offset;               /*!< 1 */
    int input0_x_offset;               /*!< 2 */
                                       //
    feature_t *input1_element;         /*!< 3 */
    int input1_y_offset;               /*!< 4 */
    int input1_x_offset;               /*!< 5 */
                                       //
    feature_t *output_element;         /*!< 6 */
    int output_y_offset;               /*!< 7 */
    int output_x_offset;               /*!< 8 */
                                       //
    int width;                         /*!< 9 */
    int height;                        /*!< 10 */
    int channel;                       /*!< 11 */
                                       //
    activation_type_t activation_type; /*!< 12 */
    int activation_alpha;              /*!< 13 */
    const void *activation_alpha_ptr;  /*!< 14 */
    int activation_shift;              /*!< 15 */
                                       //
    int c_div_x_1;                     /*!< 16 */
    int c_div_2x_1;                    /*!< 17 */
    int c_left_x_1;                    /*!< 18 */
    int c_remainder;                   /*!< 19 */
                                       //
    int rescale_input;                 /*!< 20 */
    int rescale_output;                /*!< 21 */
                                       //
    int input_shift;                   /*!< 22 */
    int output_shift;                  /*!< 23 */
    int output_scale;                  /*!< 24 */
    int mul_shift;                     /*!< 25 */
    int neg_output_scale;              /*!< 26 */

    int input0_b; /*!< 27 */
    int input0_c; /*!< 28 */
    int input0_h; /*!< 29 */
    int input0_w; /*!< 30 */

    int input1_b; /*!< 31 */
    int input1_c; /*!< 32 */
    int input1_h; /*!< 33 */
    int input1_w; /*!< 34 */

    int output_b; /*!< 35 */
    int output_c; /*!< 36 */
    int output_h; /*!< 37 */
    int output_w; /*!< 38 */

    int output_max_dims; /*!< 39 */

    int input0_b_same; /*!< 40 */
    int input0_c_same; /*!< 41 */
    int input0_h_same; /*!< 42 */
    int input0_w_same; /*!< 43 */

    int input1_b_same; /*!< 44 */
    int input1_c_same; /*!< 45 */
    int input1_h_same; /*!< 46 */
    int input1_w_same; /*!< 47 */
};

/**
 * @brief Get the arith operation args object
 *
 * @tparam feature_t
 * @param output
 * @param input0
 * @param input1
 * @param activate
 * @param activation_alpha
 * @param runtime_mode
 * @return std::vector<arithArgsType<feature_t>>
 */
template <typename feature_t>
std::vector<arithArgsType<feature_t>> get_arith_operation_args(TensorBase *output,
                                                               TensorBase *input0,
                                                               TensorBase *input1,
                                                               const activation_type_t activate = Linear,
                                                               TensorBase *activation_alpha = nullptr,
                                                               const runtime_mode_t runtime_mode = RUNTIME_MODE_AUTO)
{
    arithArgsType<feature_t> args;
    // op between (h w c) and (1 1 c) is allowed.
    bool is_hw_input0_11 = (input0->shape[1] == 1) && (input0->shape[2] == 1);
    bool is_hw_input1_11 = (input1->shape[1] == 1) && (input1->shape[2] == 1);
    bool is_same_channel_num = input0->shape[3] == input1->shape[3];
    bool is_11c_and_hwc = is_same_channel_num && (is_hw_input0_11 || is_hw_input1_11);
    bool is_same_shape = input0->shape == input1->shape;
    // assert(is_same_shape || is_11c_and_hwc);
    if (is_same_shape) {
        args.height = input0->shape[1]; // inputs and output are the same shape
        args.width = input0->shape[2];
        args.channel = input0->shape[3];
        args.input0_x_offset = input0->shape[3];
        args.input1_x_offset = input1->shape[3];
    } else {
        if (is_hw_input0_11) {
            args.height = input1->shape[1];
            args.width = input1->shape[2];
            args.channel = input1->shape[3];
            args.input0_x_offset = 0;
            args.input1_x_offset = input1->shape[3];
        } else {
            args.height = input0->shape[1];
            args.width = input0->shape[2];
            args.channel = input0->shape[3];
            args.input0_x_offset = input0->shape[3];
            args.input1_x_offset = 0;
        }
    }

    args.input0_element = (feature_t *)input0->get_element_ptr();
    // args.input0_y_offset = input0->shape[2] * input0->shape[3];

    args.input1_element = (feature_t *)input1->get_element_ptr();
    // args.input1_y_offset = input1->shape[2] * input1->shape[3];

    args.output_element = (feature_t *)output->get_element_ptr(); // output
    // args.output_y_offset = output->shape[1] * output->shape[2];
    args.output_x_offset = output->shape[3];

    args.rescale_input = 1;
    args.rescale_output = 1;
    args.output_scale = 1;
    args.input_shift = 0;
    args.output_shift = 0;

    args.mul_shift = output->exponent - input0->exponent - input1->exponent;

    if (input0->exponent == input1->exponent) {
        args.rescale_input = 0;
        if (input0->exponent == output->exponent) {
            args.rescale_output = 0;
            args.input_shift = -1; // do not need to rescale
        } else {
            args.output_shift = output->exponent - input0->exponent; // right shift
        }
    } else {
        if (input0->exponent < input1->exponent) {
            args.rescale_input = 2;
            args.input_shift = input1->exponent - input0->exponent; // input0 * 2^(-input_shift)
            args.output_shift = output->exponent - input1->exponent;
        } else {
            // default: args.rescale_input = 1;
            args.input_shift = input0->exponent - input1->exponent; // input1 * 2^(-input_shift)
            args.output_shift = output->exponent - input0->exponent;
        }
    }
    if (args.output_shift < 0) { // ( * output_scale ) >> output_shift
        args.output_scale = 1 << (-args.output_shift);
        args.output_shift = 0;
    }

    args.neg_output_scale = -args.output_scale;
    args.activation_type = activate;

    switch (args.activation_type) {
    case ReLU:
        args.activation_alpha = 0;
        args.activation_shift = 0;
        args.activation_alpha_ptr = NULL;
        break;
    case LeakyReLU:
        // ESP_LOGE(__FUNCTION__, "Do not support Leaky ReLU");
        //     args.activation_alpha = activation_alpha->get_element_ptr()[0];
        //     args.activation_shift = -activation_alpha->exponent;
        //     args.activation_alpha_ptr = NULL;
        break;
    case PReLU:
        // ESP_LOGE(__FUNCTION__, "Do not support PReLU");
        // args.activation_alpha_ptr = activation_alpha->get_element_ptr(); //TODO: auto_split
        // args.activation_shift = -activation_alpha->exponent;
        break;
    default:
        args.activation_alpha_ptr = NULL;
        args.activation_shift = -1;
        break;
    }

    // for ISA
    int u = 16 / sizeof(feature_t);
    int c_div_x = input0->shape[3] / u;
    args.c_remainder = (args.channel % u) * sizeof(feature_t);
    args.c_div_x_1 = c_div_x - 1;
    args.c_div_2x_1 = DL_MAX(c_div_x / 2 - 1, 0);
    args.c_left_x_1 = c_div_x - 2 * args.c_div_2x_1 - 1;

    // args.rescale_input = 1; // test

    // for multidirectional_broadcasting
    int size = input0->shape.size();
    if (size == 4) {
        args.input0_b = input0->shape[0];
        args.input0_c = input0->shape[1];
        args.input0_h = input0->shape[2];
        args.input0_w = input0->shape[3];
    } else if (size == 3) {
        args.input0_b = 1;
        args.input0_c = input0->shape[0];
        args.input0_h = input0->shape[1];
        args.input0_w = input0->shape[2];
    } else if (size == 2) {
        args.input0_b = 1;
        args.input0_c = 1;
        args.input0_h = input0->shape[0];
        args.input0_w = input0->shape[1];
    } else {
        args.input0_b = 1;
        args.input0_c = 1;
        args.input0_h = 1;
        args.input0_w = input0->shape[0];
    }

    size = input1->shape.size();
    if (size == 4) {
        args.input1_b = input1->shape[0];
        args.input1_c = input1->shape[1];
        args.input1_h = input1->shape[2];
        args.input1_w = input1->shape[3];
    } else if (size == 3) {
        args.input1_b = 1;
        args.input1_c = input1->shape[0];
        args.input1_h = input1->shape[1];
        args.input1_w = input1->shape[2];
    } else if (size == 2) {
        args.input1_b = 1;
        args.input1_c = 1;
        args.input1_h = input1->shape[0];
        args.input1_w = input1->shape[1];
    } else {
        args.input1_b = 1;
        args.input1_c = 1;
        args.input1_h = 1;
        args.input1_w = input1->shape[0];
    }

    size = output->shape.size();
    args.output_max_dims = size;
    if (size == 4) {
        args.output_b = output->shape[0];
        args.output_c = output->shape[1];
        args.output_h = output->shape[2];
        args.output_w = output->shape[3];
    } else if (size == 3) {
        args.output_b = 1;
        args.output_c = output->shape[0];
        args.output_h = output->shape[1];
        args.output_w = output->shape[2];
    } else if (size == 2) {
        args.output_b = 1;
        args.output_c = 1;
        args.output_h = output->shape[0];
        args.output_w = output->shape[1];
    } else {
        args.output_b = 1;
        args.output_c = 1;
        args.output_h = 1;
        args.output_w = output->shape[0];
    }

    if (args.input0_b == args.output_b) {
        args.input0_b_same = 1;
    } else {
        args.input0_b_same = 0;
    }

    if (args.input0_c == args.output_c) {
        args.input0_c_same = 1;
    } else {
        args.input0_c_same = 0;
    }

    if (args.input0_h == args.output_h) {
        args.input0_h_same = 1;
    } else {
        args.input0_h_same = 0;
    }

    if (args.input0_w == args.output_w) {
        args.input0_w_same = 1;
    } else {
        args.input0_w_same = 0;
    }

    if (args.input1_b == args.output_b) {
        args.input1_b_same = 1;
    } else {
        args.input1_b_same = 0;
    }

    if (args.input1_c == args.output_c) {
        args.input1_c_same = 1;
    } else {
        args.input1_c_same = 0;
    }

    if (args.input1_h == args.output_h) {
        args.input1_h_same = 1;
    } else {
        args.input1_h_same = 0;
    }

    if (args.input1_w == args.output_w) {
        args.input1_w_same = 1;
    } else {
        args.input1_w_same = 0;
    }

    if (args.output_max_dims == 4) {
        int u = 16 / sizeof(feature_t);
        int c_div_x = args.output_w / u;
        args.c_remainder = (args.output_w % u) * sizeof(feature_t);
        args.c_div_x_1 = c_div_x - 1;
        args.c_div_2x_1 = DL_MAX(c_div_x / 2 - 1, 0);
        args.c_left_x_1 = c_div_x - 2 * args.c_div_2x_1 - 1;
    }

    // slice
    std::vector<arithArgsType<feature_t>> m_args(1, args);
    if (runtime_mode == RUNTIME_MODE_MULTI_CORE) {
        // TODO:
    }

    return m_args;
}
typedef void (*arith_i_impl_func_s16_t)(int16_t *, int16_t *, int16_t *, void *);
typedef void (*arith_c_impl_func_s16_t)(int16_t *, int16_t *, int16_t *, const arithArgsType<int16_t> &);
typedef void (*arith_n_wise_tail_s16_t)(int16_t *, const arithArgsType<void> &);

typedef void (*arith_i_impl_func_s8_t)(int8_t *, int8_t *, int8_t *, void *);
typedef void (*arith_c_impl_func_s8_t)(int8_t *, int8_t *, int8_t *, const arithArgsType<int8_t> &);
typedef void (*arith_n_wise_tail_s8_t)(int8_t *, const arithArgsType<void> &);

template <typename feature_t>
void arith_operation_shell(
    const arithArgsType<feature_t> &args,
    void (*arith_i_impl_func)(feature_t *, feature_t *, feature_t *, void *),
    void (*arith_c_impl_func)(feature_t *, feature_t *, feature_t *, const arithArgsType<feature_t> &),
    void (*arith_n_wise_tail)(feature_t *, const arithArgsType<void> &))
{
    feature_t *input0_ptr = args.input0_element;
    feature_t *input1_ptr = args.input1_element;
    feature_t *output_ptr = args.output_element;

    arithArgsType<void> activation_args;
    activation_args.channel = args.channel;
    activation_args.activation_type = args.activation_type;
    activation_args.activation_alpha = args.activation_alpha;
    activation_args.activation_shift = args.activation_shift;
    activation_args.activation_alpha_ptr = args.activation_alpha_ptr;
    size_t loop_size = args.height * args.width;

    if (arith_i_impl_func) {
        if (arith_n_wise_tail) {
            if (args.rescale_input < 2) {
                for (size_t i = 0; i < loop_size; i++) {
                    arith_i_impl_func(output_ptr, input0_ptr, input1_ptr, (void *const)&args);
                    arith_n_wise_tail(output_ptr, activation_args);
                    input0_ptr += args.input0_x_offset;
                    input1_ptr += args.input1_x_offset;
                    output_ptr += args.output_x_offset;
                }
            } else {
                for (size_t i = 0; i < loop_size; i++) {
                    arith_i_impl_func(output_ptr, input1_ptr, input0_ptr, (void *const)&args);
                    arith_n_wise_tail(output_ptr, activation_args);
                    input0_ptr += args.input0_x_offset;
                    input1_ptr += args.input1_x_offset;
                    output_ptr += args.output_x_offset;
                }
            }
        } else {
            if (args.rescale_input < 2) {
                for (size_t i = 0; i < loop_size; i++) {
                    arith_i_impl_func(output_ptr, input0_ptr, input1_ptr, (void *const)&args);
                    input0_ptr += args.input0_x_offset;
                    input1_ptr += args.input1_x_offset;
                    output_ptr += args.output_x_offset;
                }
            } else {
                for (size_t i = 0; i < loop_size; i++) {
                    arith_i_impl_func(output_ptr, input1_ptr, input0_ptr, (void *const)&args);
                    input0_ptr += args.input0_x_offset;
                    input1_ptr += args.input1_x_offset;
                    output_ptr += args.output_x_offset;
                }
            }
        }
    } else // run c_impl_func
    {
        if (arith_n_wise_tail) {
            if (args.rescale_input < 2) {
                for (size_t i = 0; i < loop_size; i++) {
                    arith_c_impl_func(output_ptr, input0_ptr, input1_ptr, args);
                    arith_n_wise_tail(output_ptr, activation_args);
                    input0_ptr += args.input0_x_offset;
                    input1_ptr += args.input1_x_offset;
                    output_ptr += args.output_x_offset;
                }
            } else {
                for (size_t i = 0; i < loop_size; i++) {
                    arith_c_impl_func(output_ptr, input1_ptr, input0_ptr, args);
                    arith_n_wise_tail(output_ptr, activation_args);
                    input0_ptr += args.input0_x_offset;
                    input1_ptr += args.input1_x_offset;
                    output_ptr += args.output_x_offset;
                }
            }
        } else {
            if (args.rescale_input < 2) {
                for (size_t i = 0; i < loop_size; i++) {
                    arith_c_impl_func(output_ptr, input0_ptr, input1_ptr, args);
                    input0_ptr += args.input0_x_offset;
                    input1_ptr += args.input1_x_offset;
                    output_ptr += args.output_x_offset;
                }
            } else {
                for (size_t i = 0; i < loop_size; i++) {
                    arith_c_impl_func(output_ptr, input1_ptr, input0_ptr, args);
                    input0_ptr += args.input0_x_offset;
                    input1_ptr += args.input1_x_offset;
                    output_ptr += args.output_x_offset;
                }
            }
        }
    }

    return;
}

template <typename feature_t>
void arith_operation_shell_(
    const arithArgsType<feature_t> &args,
    void (*arith_i_impl_func)(feature_t *, feature_t *, feature_t *, void *),
    void (*arith_c_impl_func)(feature_t *, feature_t *, feature_t *, const arithArgsType<feature_t> &),
    void (*arith_n_wise_tail)(feature_t *, const arithArgsType<void> &))
{
    feature_t *input0_ptr = args.input0_element;
    feature_t *input1_ptr = args.input1_element;
    feature_t *output_ptr = args.output_element;

    arithArgsType<void> activation_args;
    activation_args.channel = args.channel;
    activation_args.activation_type = args.activation_type;
    activation_args.activation_alpha = args.activation_alpha;
    activation_args.activation_shift = args.activation_shift;
    activation_args.activation_alpha_ptr = args.activation_alpha_ptr;
    size_t loop_size = 1;
    // args.rescale_input = 1;

    if (arith_i_impl_func) {
        if (arith_n_wise_tail) {
            if (args.rescale_input < 2) {
                for (size_t i = 0; i < loop_size; i++) {
                    arith_i_impl_func(output_ptr, input0_ptr, input1_ptr, (void *const)&args);
                    arith_n_wise_tail(output_ptr, activation_args);
                    input0_ptr += args.input0_x_offset;
                    input1_ptr += args.input1_x_offset;
                    output_ptr += args.output_x_offset;
                }
            } else {
                for (size_t i = 0; i < loop_size; i++) {
                    arith_i_impl_func(output_ptr, input1_ptr, input0_ptr, (void *const)&args);
                    arith_n_wise_tail(output_ptr, activation_args);
                    input0_ptr += args.input0_x_offset;
                    input1_ptr += args.input1_x_offset;
                    output_ptr += args.output_x_offset;
                }
            }
        } else {
            if (args.rescale_input < 2) {
                for (size_t i = 0; i < loop_size; i++) {
                    arith_i_impl_func(output_ptr, input0_ptr, input1_ptr, (void *const)&args);
                    input0_ptr += args.input0_x_offset;
                    input1_ptr += args.input1_x_offset;
                    output_ptr += args.output_x_offset;
                }
            } else {
                for (size_t i = 0; i < loop_size; i++) {
                    arith_i_impl_func(output_ptr, input1_ptr, input0_ptr, (void *const)&args);
                    input0_ptr += args.input0_x_offset;
                    input1_ptr += args.input1_x_offset;
                    output_ptr += args.output_x_offset;
                }
            }
        }
    } else // run c_impl_func
    {
        if (arith_n_wise_tail) {
            if (args.rescale_input < 2) {
                for (size_t i = 0; i < loop_size; i++) {
                    arith_c_impl_func(output_ptr, input0_ptr, input1_ptr, args);
                    arith_n_wise_tail(output_ptr, activation_args);
                    input0_ptr += args.input0_x_offset;
                    input1_ptr += args.input1_x_offset;
                    output_ptr += args.output_x_offset;
                }
            } else {
                for (size_t i = 0; i < loop_size; i++) {
                    arith_c_impl_func(output_ptr, input1_ptr, input0_ptr, args);
                    arith_n_wise_tail(output_ptr, activation_args);
                    input0_ptr += args.input0_x_offset;
                    input1_ptr += args.input1_x_offset;
                    output_ptr += args.output_x_offset;
                }
            }
        } else {
            if (args.rescale_input < 2) {
                for (size_t i = 0; i < loop_size; i++) {
                    arith_c_impl_func(output_ptr, input0_ptr, input1_ptr, args);
                    input0_ptr += args.input0_x_offset;
                    input1_ptr += args.input1_x_offset;
                    output_ptr += args.output_x_offset;
                }
            } else {
                for (size_t i = 0; i < loop_size; i++) {
                    arith_c_impl_func(output_ptr, input1_ptr, input0_ptr, args);
                    input0_ptr += args.input0_x_offset;
                    input1_ptr += args.input1_x_offset;
                    output_ptr += args.output_x_offset;
                }
            }
        }
    }

    return;
}

template <typename feature_t>
struct resizeArgsType {
    feature_t *input_element; /*!<  0 */
    int input_height;         /*!<  1 */
    int input_width;          /*!<  2 */
    int input_channel;        /*!<  3 */

    feature_t *output_element; /*!<  4 */
    int output_x_offset;       /*!<  5 */
    int output_y_offset;       /*!<  6 */

    resize_mode_t resize_mode; /*!<  7 */

    float scale_h; /*!<  8 */
    float scale_w; /*!<  9 */

    int c_div_x;     /*!< 10 */
    int c_remainder; /*!< 11 */

    int output_shift;    /*!< 12 */
    int output_scale;    /*!< 13 */
    int dims;            /*!< 14 */
    int output_height;   /*!< 15 */
    int output_width;    /*!< 16 */
    int align_corners;   /*!< 17 */
    float scale_h_inv;   /*!< 18 */
    float scale_w_inv;   /*!< 19 */
    float *cache;        /*!< 20 */
    int input_exponent;  /*!< 21 */
    int output_exponent; /*!< 22 */
};

struct requantizeArgsType {
    void *input_element;  /*!< 0 */
    void *output_element; /*!< 1 */

    int size_div_x;         /*!< 2 */
    int in_size_remainder;  /*!< 3 */
    int out_size_remainder; /*!< 4 */
    int output_shift;       /*!< 5 */
    int output_scale;       /*!< 6 */
};

} // namespace base
} // namespace dl
