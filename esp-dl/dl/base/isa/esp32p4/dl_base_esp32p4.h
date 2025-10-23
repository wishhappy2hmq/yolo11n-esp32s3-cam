#pragma once

extern "C" {

/* Int16 API */
void dl_esp32p4_s16_conv2d_11cn_bias(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_conv2d_11cn_bias_relu(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_conv2d_11cn(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_conv2d_11cn_relu(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);

void dl_esp32p4_s16_unaligned_conv2d_11cn_bias(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_unaligned_conv2d_11cn_bias_relu(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_unaligned_conv2d_11cn(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_unaligned_conv2d_11cn_relu(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);

void dl_esp32p4_s16_conv2d_33cn_bias(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_conv2d_33cn_bias_relu(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_conv2d_33cn(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_conv2d_33cn_relu(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);

void dl_esp32p4_s16_unaligned_conv2d_33cn_bias(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_unaligned_conv2d_33cn_bias_relu(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_unaligned_conv2d_33cn(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_unaligned_conv2d_33cn_relu(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);

void dl_esp32p4_s16_conv2d_hwcn_bias(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_conv2d_hwcn_bias_relu(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_conv2d_hwcn(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_conv2d_hwcn_relu(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);

void dl_esp32p4_s16_unaligned_conv2d_hwcn_bias(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_unaligned_conv2d_hwcn_bias_relu(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_unaligned_conv2d_hwcn(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_unaligned_conv2d_hwcn_relu(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);

void dl_esp32p4_s16_depthwise_conv2d_33c1_bias(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_depthwise_conv2d_33c1_bias_relu(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_depthwise_conv2d_33c1(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_depthwise_conv2d_33c1_relu(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);

void dl_esp32p4_s16_unaligned_depthwise_conv2d_33c1_bias(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_unaligned_depthwise_conv2d_33c1_bias_relu(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_unaligned_depthwise_conv2d_33c1(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_unaligned_depthwise_conv2d_33c1_relu(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);

void dl_esp32p4_s16_depthwise_conv2d_hwc1_bias(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_depthwise_conv2d_hwc1_bias_relu(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_depthwise_conv2d_hwc1(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_depthwise_conv2d_hwc1_relu(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);

void dl_esp32p4_s16_unaligned_depthwise_conv2d_hwc1_bias(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_unaligned_depthwise_conv2d_hwc1_bias_relu(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_unaligned_depthwise_conv2d_hwc1(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_unaligned_depthwise_conv2d_hwc1_relu(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);

void dl_esp32p4_s16_add2d_11c(int16_t *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_add2d_11c_relu(int16_t *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_add2d_11c_prelu(int16_t *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_rescale_add2d_11c(int16_t *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_rescale_add2d_11c_relu(int16_t *output_ptr,
                                           int16_t *input0_ptr,
                                           int16_t *input1_ptr,
                                           void *args_ptr);
void dl_esp32p4_s16_rescale_add2d_11c_prelu(int16_t *output_ptr,
                                            int16_t *input0_ptr,
                                            int16_t *input1_ptr,
                                            void *args_ptr);
void dl_esp32p4_s16_unaligned_add2d_11c(int16_t *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_unaligned_add2d_11c_relu(int16_t *output_ptr,
                                             int16_t *input0_ptr,
                                             int16_t *input1_ptr,
                                             void *args_ptr);
void dl_esp32p4_s16_unaligned_add2d_11c_prelu(int16_t *output_ptr,
                                              int16_t *input0_ptr,
                                              int16_t *input1_ptr,
                                              void *args_ptr);

void dl_esp32p4_s16_mul2d_11c(int16_t *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_mul2d_11c_relu(int16_t *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_mul2d_11c_prelu(int16_t *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_unaligned_mul2d_11c(int16_t *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_unaligned_mul2d_11c_relu(int16_t *output_ptr,
                                             int16_t *input0_ptr,
                                             int16_t *input1_ptr,
                                             void *args_ptr);
void dl_esp32p4_s16_unaligned_mul2d_11c_prelu(int16_t *output_ptr,
                                              int16_t *input0_ptr,
                                              int16_t *input1_ptr,
                                              void *args_ptr);

/* Int8 API */
void dl_esp32p4_s8_conv2d_11cn_bias(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_conv2d_11cn_bias_relu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_conv2d_11cn_bias_prelu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_conv2d_11cn(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_conv2d_11cn_relu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_conv2d_11cn_prelu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);

void dl_esp32p4_s8_unaligned_conv2d_11cn_bias(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_conv2d_11cn_bias_leakyrelu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_conv2d_11cn_bias_prelu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_conv2d_11cn(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_conv2d_11cn_leakyrelu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_conv2d_11cn_prelu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);

void dl_esp32p4_s8_conv2d_33cn_bias(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_conv2d_33cn_bias_relu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_conv2d_33cn_bias_prelu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_conv2d_33cn(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_conv2d_33cn_relu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_conv2d_33cn_prelu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);

void dl_esp32p4_s8_unaligned_conv2d_33cn_bias(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_conv2d_33cn_bias_leakyrelu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_conv2d_33cn_bias_prelu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_conv2d_33cn(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_conv2d_33cn_leakyrelu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_conv2d_33cn_prelu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);

void dl_esp32p4_s8_conv2d_hwcn_bias(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_conv2d_hwcn_bias_relu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_conv2d_hwcn_bias_prelu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_conv2d_hwcn(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_conv2d_hwcn_relu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_conv2d_hwcn_prelu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);

void dl_esp32p4_s8_unaligned_conv2d_hwcn_bias(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_conv2d_hwcn_bias_leakyrelu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_conv2d_hwcn_bias_prelu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_conv2d_hwcn(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_conv2d_hwcn_leakyrelu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_conv2d_hwcn_prelu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);

void dl_esp32p4_s8_depthwise_conv2d_33c1_bias(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_depthwise_conv2d_33c1_bias_relu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_depthwise_conv2d_33c1_bias_prelu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_depthwise_conv2d_33c1(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_depthwise_conv2d_33c1_relu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_depthwise_conv2d_33c1_prelu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);

void dl_esp32p4_s8_unaligned_depthwise_conv2d_33c1_bias(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_depthwise_conv2d_33c1_bias_relu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_depthwise_conv2d_33c1(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_depthwise_conv2d_33c1_relu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);

void dl_esp32p4_s8_depthwise_conv2d_hwc1_bias(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_depthwise_conv2d_hwc1_bias_relu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_depthwise_conv2d_hwc1_bias_prelu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_depthwise_conv2d_hwc1(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_depthwise_conv2d_hwc1_relu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_depthwise_conv2d_hwc1_prelu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);

void dl_esp32p4_s8_unaligned_depthwise_conv2d_hwc1_bias(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_depthwise_conv2d_hwc1_bias_relu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_depthwise_conv2d_hwc1(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_depthwise_conv2d_hwc1_relu(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);

void dl_esp32p4_s8_mul2d_11c(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_mul2d_11c_relu(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_mul2d_11c_prelu(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_mul2d_11c(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_mul2d_11c_relu(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_mul2d_11c_prelu(int8_t *output_ptr,
                                             int8_t *input0_ptr,
                                             int8_t *input1_ptr,
                                             void *args_ptr);

void dl_esp32p4_s8_add2d_11c(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_add2d_11c_relu(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_add2d_11c_prelu(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_rescale_add2d_11c(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_rescale_add2d_11c_relu(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_rescale_add2d_11c_prelu(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_add2d_11c(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_add2d_11c_relu(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_add2d_11c_prelu(int8_t *output_ptr,
                                             int8_t *input0_ptr,
                                             int8_t *input1_ptr,
                                             void *args_ptr);

void dl_esp32p4_s8_max_pool2d_22c1(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_max_pool2d_22c1(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_max_pool2d_hwc1(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_max_pool2d_hwc1(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);

void dl_esp32p4_s8_avg_pool2d_22c1(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_avg_pool2d_22c1(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_avg_pool2d_hwc1(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_avg_pool2d_hwc1(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);

void dl_esp32p4_s8_resize_nearest_2x2_c1(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_resize_nearest_c1(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_resize_nearest_2x2_c1(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_resize_nearest_c1(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);

void dl_esp32p4_s8_prelu_11c(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_unaligned_prelu_11c(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);

void dl_esp32p4_s8_add_w1_16_w2_16(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_add_w1_16_w2_1(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_add_w1_1_w2_16(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_add_w1_16_w2_16_unaligned(int8_t *output_ptr,
                                             int8_t *input0_ptr,
                                             int8_t *input1_ptr,
                                             void *args_ptr);
void dl_esp32p4_s8_add_w1_16_w2_1_unaligned(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_add_w1_1_w2_16_unaligned(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);

void dl_esp32p4_s16_add_w1_8_w2_8(int16_t *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_add_w1_8_w2_1(int16_t *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_add_w1_1_w2_8(int16_t *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_add_w1_8_w2_8_unaligned(int16_t *output_ptr,
                                            int16_t *input0_ptr,
                                            int16_t *input1_ptr,
                                            void *args_ptr);
void dl_esp32p4_s16_add_w1_8_w2_1_unaligned(int16_t *output_ptr,
                                            int16_t *input0_ptr,
                                            int16_t *input1_ptr,
                                            void *args_ptr);
void dl_esp32p4_s16_add_w1_1_w2_8_unaligned(int16_t *output_ptr,
                                            int16_t *input0_ptr,
                                            int16_t *input1_ptr,
                                            void *args_ptr);

void dl_esp32p4_s8_sub_w1_16_w2_16(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_sub_w1_16_w2_1(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_sub_w1_1_w2_16(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_sub_w1_16_w2_16_unaligned(int8_t *output_ptr,
                                             int8_t *input0_ptr,
                                             int8_t *input1_ptr,
                                             void *args_ptr);
void dl_esp32p4_s8_sub_w1_16_w2_1_unaligned(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_sub_w1_1_w2_16_unaligned(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);

void dl_esp32p4_s16_sub_w1_8_w2_8(int16_t *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_sub_w1_8_w2_1(int16_t *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_sub_w1_1_w2_8(int16_t *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_sub_w1_8_w2_8_unaligned(int16_t *output_ptr,
                                            int16_t *input0_ptr,
                                            int16_t *input1_ptr,
                                            void *args_ptr);
void dl_esp32p4_s16_sub_w1_8_w2_1_unaligned(int16_t *output_ptr,
                                            int16_t *input0_ptr,
                                            int16_t *input1_ptr,
                                            void *args_ptr);
void dl_esp32p4_s16_sub_w1_1_w2_8_unaligned(int16_t *output_ptr,
                                            int16_t *input0_ptr,
                                            int16_t *input1_ptr,
                                            void *args_ptr);

void dl_esp32p4_s8_mul_w1_16_w2_16(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_mul_w1_16_w2_1(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_mul_w1_1_w2_16(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_mul_w1_16_w2_16_unaligned(int8_t *output_ptr,
                                             int8_t *input0_ptr,
                                             int8_t *input1_ptr,
                                             void *args_ptr);
void dl_esp32p4_s8_mul_w1_16_w2_1_unaligned(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_mul_w1_1_w2_16_unaligned(int8_t *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);

void dl_esp32p4_s16_mul_w1_8_w2_8(int16_t *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_mul_w1_8_w2_1(int16_t *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_mul_w1_1_w2_8(int16_t *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_mul_w1_8_w2_8_unaligned(int16_t *output_ptr,
                                            int16_t *input0_ptr,
                                            int16_t *input1_ptr,
                                            void *args_ptr);
void dl_esp32p4_s16_mul_w1_8_w2_1_unaligned(int16_t *output_ptr,
                                            int16_t *input0_ptr,
                                            int16_t *input1_ptr,
                                            void *args_ptr);
void dl_esp32p4_s16_mul_w1_1_w2_8_unaligned(int16_t *output_ptr,
                                            int16_t *input0_ptr,
                                            int16_t *input1_ptr,
                                            void *args_ptr);

void dl_esp32p4_s8_equal_w1_16_w2_16(bool *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_equal_w1_16_w2_1(bool *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_equal_w1_1_w2_16(bool *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_equal_w1_16_w2_16_unaligned(bool *output_ptr,
                                               int8_t *input0_ptr,
                                               int8_t *input1_ptr,
                                               void *args_ptr);
void dl_esp32p4_s8_equal_w1_16_w2_1_unaligned(bool *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_equal_w1_1_w2_16_unaligned(bool *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);

void dl_esp32p4_s16_equal_w1_8_w2_8(bool *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_equal_w1_8_w2_1(bool *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_equal_w1_1_w2_8(bool *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_equal_w1_8_w2_8_unaligned(bool *output_ptr,
                                              int16_t *input0_ptr,
                                              int16_t *input1_ptr,
                                              void *args_ptr);
void dl_esp32p4_s16_equal_w1_8_w2_1_unaligned(bool *output_ptr,
                                              int16_t *input0_ptr,
                                              int16_t *input1_ptr,
                                              void *args_ptr);
void dl_esp32p4_s16_equal_w1_1_w2_8_unaligned(bool *output_ptr,
                                              int16_t *input0_ptr,
                                              int16_t *input1_ptr,
                                              void *args_ptr);

void dl_esp32p4_s8_greater_w1_16_w2_16(bool *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_greater_w1_16_w2_1(bool *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_greater_w1_1_w2_16(bool *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);

void dl_esp32p4_s8_greater_w1_16_w2_16_unaligned(bool *output_ptr,
                                                 int8_t *input0_ptr,
                                                 int8_t *input1_ptr,
                                                 void *args_ptr);
void dl_esp32p4_s8_greater_w1_16_w2_1_unaligned(bool *output_ptr,
                                                int8_t *input0_ptr,
                                                int8_t *input1_ptr,
                                                void *args_ptr);
void dl_esp32p4_s8_greater_w1_1_w2_16_unaligned(bool *output_ptr,
                                                int8_t *input0_ptr,
                                                int8_t *input1_ptr,
                                                void *args_ptr);

void dl_esp32p4_s16_greater_w1_8_w2_8(bool *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_greater_w1_8_w2_1(bool *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_greater_w1_1_w2_8(bool *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);

void dl_esp32p4_s16_greater_w1_8_w2_8_unaligned(bool *output_ptr,
                                                int16_t *input0_ptr,
                                                int16_t *input1_ptr,
                                                void *args_ptr);
void dl_esp32p4_s16_greater_w1_8_w2_1_unaligned(bool *output_ptr,
                                                int16_t *input0_ptr,
                                                int16_t *input1_ptr,
                                                void *args_ptr);
void dl_esp32p4_s16_greater_w1_1_w2_8_unaligned(bool *output_ptr,
                                                int16_t *input0_ptr,
                                                int16_t *input1_ptr,
                                                void *args_ptr);

void dl_esp32p4_s8_less_w1_16_w2_16(bool *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_less_w1_16_w2_1(bool *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_less_w1_1_w2_16(bool *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_less_w1_16_w2_16_unaligned(bool *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_less_w1_16_w2_1_unaligned(bool *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_less_w1_1_w2_16_unaligned(bool *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);

void dl_esp32p4_s16_less_w1_8_w2_8(bool *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_less_w1_8_w2_1(bool *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_less_w1_1_w2_8(bool *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_less_w1_8_w2_8_unaligned(bool *output_ptr,
                                             int16_t *input0_ptr,
                                             int16_t *input1_ptr,
                                             void *args_ptr);
void dl_esp32p4_s16_less_w1_8_w2_1_unaligned(bool *output_ptr,
                                             int16_t *input0_ptr,
                                             int16_t *input1_ptr,
                                             void *args_ptr);
void dl_esp32p4_s16_less_w1_1_w2_8_unaligned(bool *output_ptr,
                                             int16_t *input0_ptr,
                                             int16_t *input1_ptr,
                                             void *args_ptr);

void dl_esp32p4_s8_lessorequal_w1_16_w2_16(bool *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_lessorequal_w1_16_w2_1(bool *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_lessorequal_w1_1_w2_16(bool *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_lessorequal_w1_16_w2_16_unaligned(bool *output_ptr,
                                                     int8_t *input0_ptr,
                                                     int8_t *input1_ptr,
                                                     void *args_ptr);
void dl_esp32p4_s8_lessorequal_w1_16_w2_1_unaligned(bool *output_ptr,
                                                    int8_t *input0_ptr,
                                                    int8_t *input1_ptr,
                                                    void *args_ptr);
void dl_esp32p4_s8_lessorequal_w1_1_w2_16_unaligned(bool *output_ptr,
                                                    int8_t *input0_ptr,
                                                    int8_t *input1_ptr,
                                                    void *args_ptr);

void dl_esp32p4_s16_lessorequal_w1_8_w2_8(bool *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_lessorequal_w1_8_w2_1(bool *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_lessorequal_w1_1_w2_8(bool *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s16_lessorequal_w1_8_w2_8_unaligned(bool *output_ptr,
                                                    int16_t *input0_ptr,
                                                    int16_t *input1_ptr,
                                                    void *args_ptr);
void dl_esp32p4_s16_lessorequal_w1_8_w2_1_unaligned(bool *output_ptr,
                                                    int16_t *input0_ptr,
                                                    int16_t *input1_ptr,
                                                    void *args_ptr);
void dl_esp32p4_s16_lessorequal_w1_1_w2_8_unaligned(bool *output_ptr,
                                                    int16_t *input0_ptr,
                                                    int16_t *input1_ptr,
                                                    void *args_ptr);

void dl_esp32p4_s8_greaterorequal_w1_16_w2_16(bool *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_greaterorequal_w1_16_w2_1(bool *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_greaterorequal_w1_1_w2_16(bool *output_ptr, int8_t *input0_ptr, int8_t *input1_ptr, void *args_ptr);
void dl_esp32p4_s8_greaterorequal_w1_16_w2_16_unaligned(bool *output_ptr,
                                                        int8_t *input0_ptr,
                                                        int8_t *input1_ptr,
                                                        void *args_ptr);
void dl_esp32p4_s8_greaterorequal_w1_16_w2_1_unaligned(bool *output_ptr,
                                                       int8_t *input0_ptr,
                                                       int8_t *input1_ptr,
                                                       void *args_ptr);
void dl_esp32p4_s8_greaterorequal_w1_1_w2_16_unaligned(bool *output_ptr,
                                                       int8_t *input0_ptr,
                                                       int8_t *input1_ptr,
                                                       void *args_ptr);

void dl_esp32p4_s16_greaterorequal_w1_8_w2_8(bool *output_ptr,
                                             int16_t *input0_ptr,
                                             int16_t *input1_ptr,
                                             void *args_ptr);
void dl_esp32p4_s16_greaterorequal_w1_8_w2_1(bool *output_ptr,
                                             int16_t *input0_ptr,
                                             int16_t *input1_ptr,
                                             void *args_ptr);
void dl_esp32p4_s16_greaterorequal_w1_1_w2_8(bool *output_ptr,
                                             int16_t *input0_ptr,
                                             int16_t *input1_ptr,
                                             void *args_ptr);
void dl_esp32p4_s16_greaterorequal_w1_8_w2_8_unaligned(bool *output_ptr,
                                                       int16_t *input0_ptr,
                                                       int16_t *input1_ptr,
                                                       void *args_ptr);
void dl_esp32p4_s16_greaterorequal_w1_8_w2_1_unaligned(bool *output_ptr,
                                                       int16_t *input0_ptr,
                                                       int16_t *input1_ptr,
                                                       void *args_ptr);
void dl_esp32p4_s16_greaterorequal_w1_1_w2_8_unaligned(bool *output_ptr,
                                                       int16_t *input0_ptr,
                                                       int16_t *input1_ptr,
                                                       void *args_ptr);

void dl_esp32p4_s8_and4d_bchw_w1_16_w2_16_simdand(int8_t *output_ptr,
                                                  int8_t *input0_ptr,
                                                  int8_t *input1_ptr,
                                                  void *args_ptr);
void dl_esp32p4_s8_and4d_bchw_w1_16_w2_1_simdand(int8_t *output_ptr,
                                                 int8_t *input0_ptr,
                                                 int8_t *input1_ptr,
                                                 void *args_ptr);
void dl_esp32p4_s8_and4d_bchw_w1_1_w2_16_simdand(int8_t *output_ptr,
                                                 int8_t *input0_ptr,
                                                 int8_t *input1_ptr,
                                                 void *args_ptr);
void dl_esp32p4_s16_and4d_bchw_w1_8_w2_8_simdand(int16_t *output_ptr,
                                                 int16_t *input0_ptr,
                                                 int16_t *input1_ptr,
                                                 void *args_ptr);
void dl_esp32p4_s16_and4d_bchw_w1_8_w2_1_simdand(int16_t *output_ptr,
                                                 int16_t *input0_ptr,
                                                 int16_t *input1_ptr,
                                                 void *args_ptr);
void dl_esp32p4_s16_and4d_bchw_w1_1_w2_8_simdand(int16_t *output_ptr,
                                                 int16_t *input0_ptr,
                                                 int16_t *input1_ptr,
                                                 void *args_ptr);
void dl_esp32p4_s8_and4d_bchw_w1_16_w2_16_simdand_unaligned(int8_t *output_ptr,
                                                            int8_t *input0_ptr,
                                                            int8_t *input1_ptr,
                                                            void *args_ptr);
void dl_esp32p4_s8_and4d_bchw_w1_16_w2_1_simdand_unaligned(int8_t *output_ptr,
                                                           int8_t *input0_ptr,
                                                           int8_t *input1_ptr,
                                                           void *args_ptr);
void dl_esp32p4_s8_and4d_bchw_w1_1_w2_16_simdand_unaligned(int8_t *output_ptr,
                                                           int8_t *input0_ptr,
                                                           int8_t *input1_ptr,
                                                           void *args_ptr);
void dl_esp32p4_s16_and4d_bchw_w1_8_w2_8_simdand_unaligned(int16_t *output_ptr,
                                                           int16_t *input0_ptr,
                                                           int16_t *input1_ptr,
                                                           void *args_ptr);
void dl_esp32p4_s16_and4d_bchw_w1_8_w2_1_simdand_unaligned(int16_t *output_ptr,
                                                           int16_t *input0_ptr,
                                                           int16_t *input1_ptr,
                                                           void *args_ptr);
void dl_esp32p4_s16_and4d_bchw_w1_1_w2_8_simdand_unaligned(int16_t *output_ptr,
                                                           int16_t *input0_ptr,
                                                           int16_t *input1_ptr,
                                                           void *args_ptr);

void dl_esp32p4_s8_xor4d_bchw_w1_16_w2_16_simdxor(int8_t *output_ptr,
                                                  int8_t *input0_ptr,
                                                  int8_t *input1_ptr,
                                                  void *args_ptr);
void dl_esp32p4_s8_xor4d_bchw_w1_16_w2_1_simdxor(int8_t *output_ptr,
                                                 int8_t *input0_ptr,
                                                 int8_t *input1_ptr,
                                                 void *args_ptr);
void dl_esp32p4_s8_xor4d_bchw_w1_1_w2_16_simdxor(int8_t *output_ptr,
                                                 int8_t *input0_ptr,
                                                 int8_t *input1_ptr,
                                                 void *args_ptr);
void dl_esp32p4_s16_xor4d_bchw_w1_8_w2_8_simdxor(int16_t *output_ptr,
                                                 int16_t *input0_ptr,
                                                 int16_t *input1_ptr,
                                                 void *args_ptr);
void dl_esp32p4_s16_xor4d_bchw_w1_8_w2_1_simdxor(int16_t *output_ptr,
                                                 int16_t *input0_ptr,
                                                 int16_t *input1_ptr,
                                                 void *args_ptr);
void dl_esp32p4_s16_xor4d_bchw_w1_1_w2_8_simdxor(int16_t *output_ptr,
                                                 int16_t *input0_ptr,
                                                 int16_t *input1_ptr,
                                                 void *args_ptr);
void dl_esp32p4_s8_xor4d_bchw_w1_16_w2_16_simdxor_unaligned(int8_t *output_ptr,
                                                            int8_t *input0_ptr,
                                                            int8_t *input1_ptr,
                                                            void *args_ptr);
void dl_esp32p4_s8_xor4d_bchw_w1_16_w2_1_simdxor_unaligned(int8_t *output_ptr,
                                                           int8_t *input0_ptr,
                                                           int8_t *input1_ptr,
                                                           void *args_ptr);
void dl_esp32p4_s8_xor4d_bchw_w1_1_w2_16_simdxor_unaligned(int8_t *output_ptr,
                                                           int8_t *input0_ptr,
                                                           int8_t *input1_ptr,
                                                           void *args_ptr);
void dl_esp32p4_s16_xor4d_bchw_w1_8_w2_8_simdxor_unaligned(int16_t *output_ptr,
                                                           int16_t *input0_ptr,
                                                           int16_t *input1_ptr,
                                                           void *args_ptr);
void dl_esp32p4_s16_xor4d_bchw_w1_8_w2_1_simdxor_unaligned(int16_t *output_ptr,
                                                           int16_t *input0_ptr,
                                                           int16_t *input1_ptr,
                                                           void *args_ptr);
void dl_esp32p4_s16_xor4d_bchw_w1_1_w2_8_simdxor_unaligned(int16_t *output_ptr,
                                                           int16_t *input0_ptr,
                                                           int16_t *input1_ptr,
                                                           void *args_ptr);

void dl_esp32p4_s8_or4d_bchw_w1_16_w2_16_simdor(int8_t *output_ptr,
                                                int8_t *input0_ptr,
                                                int8_t *input1_ptr,
                                                void *args_ptr);
void dl_esp32p4_s8_or4d_bchw_w1_16_w2_1_simdor(int8_t *output_ptr,
                                               int8_t *input0_ptr,
                                               int8_t *input1_ptr,
                                               void *args_ptr);
void dl_esp32p4_s8_or4d_bchw_w1_1_w2_16_simdor(int8_t *output_ptr,
                                               int8_t *input0_ptr,
                                               int8_t *input1_ptr,
                                               void *args_ptr);
void dl_esp32p4_s16_or4d_bchw_w1_8_w2_8_simdor(int16_t *output_ptr,
                                               int16_t *input0_ptr,
                                               int16_t *input1_ptr,
                                               void *args_ptr);
void dl_esp32p4_s16_or4d_bchw_w1_8_w2_1_simdor(int16_t *output_ptr,
                                               int16_t *input0_ptr,
                                               int16_t *input1_ptr,
                                               void *args_ptr);
void dl_esp32p4_s16_or4d_bchw_w1_1_w2_8_simdor(int16_t *output_ptr,
                                               int16_t *input0_ptr,
                                               int16_t *input1_ptr,
                                               void *args_ptr);
void dl_esp32p4_s8_or4d_bchw_w1_16_w2_16_simdor_unaligned(int8_t *output_ptr,
                                                          int8_t *input0_ptr,
                                                          int8_t *input1_ptr,
                                                          void *args_ptr);
void dl_esp32p4_s8_or4d_bchw_w1_16_w2_1_simdor_unaligned(int8_t *output_ptr,
                                                         int8_t *input0_ptr,
                                                         int8_t *input1_ptr,
                                                         void *args_ptr);
void dl_esp32p4_s8_or4d_bchw_w1_1_w2_16_simdor_unaligned(int8_t *output_ptr,
                                                         int8_t *input0_ptr,
                                                         int8_t *input1_ptr,
                                                         void *args_ptr);
void dl_esp32p4_s16_or4d_bchw_w1_8_w2_8_simdor_unaligned(int16_t *output_ptr,
                                                         int16_t *input0_ptr,
                                                         int16_t *input1_ptr,
                                                         void *args_ptr);
void dl_esp32p4_s16_or4d_bchw_w1_8_w2_1_simdor_unaligned(int16_t *output_ptr,
                                                         int16_t *input0_ptr,
                                                         int16_t *input1_ptr,
                                                         void *args_ptr);
void dl_esp32p4_s16_or4d_bchw_w1_1_w2_8_simdor_unaligned(int16_t *output_ptr,
                                                         int16_t *input0_ptr,
                                                         int16_t *input1_ptr,
                                                         void *args_ptr);

void dl_esp32p4_s8_min4d_bchw_w1_16_w2_16_simdmin(int8_t *output_ptr,
                                                  int8_t *input0_ptr,
                                                  int8_t *input1_ptr,
                                                  void *args_ptr);
void dl_esp32p4_s8_min4d_bchw_w1_16_w2_1_simdmin(int8_t *output_ptr,
                                                 int8_t *input0_ptr,
                                                 int8_t *input1_ptr,
                                                 void *args_ptr);
void dl_esp32p4_s8_min4d_bchw_w1_1_w2_16_simdmin(int8_t *output_ptr,
                                                 int8_t *input0_ptr,
                                                 int8_t *input1_ptr,
                                                 void *args_ptr);
void dl_esp32p4_s16_min4d_bchw_w1_8_w2_8_simdmin(int16_t *output_ptr,
                                                 int16_t *input0_ptr,
                                                 int16_t *input1_ptr,
                                                 void *args_ptr);
void dl_esp32p4_s16_min4d_bchw_w1_8_w2_1_simdmin(int16_t *output_ptr,
                                                 int16_t *input0_ptr,
                                                 int16_t *input1_ptr,
                                                 void *args_ptr);
void dl_esp32p4_s16_min4d_bchw_w1_1_w2_8_simdmin(int16_t *output_ptr,
                                                 int16_t *input0_ptr,
                                                 int16_t *input1_ptr,
                                                 void *args_ptr);
void dl_esp32p4_s8_min4d_bchw_w1_16_w2_16_simdmin_unaligned(int8_t *output_ptr,
                                                            int8_t *input0_ptr,
                                                            int8_t *input1_ptr,
                                                            void *args_ptr);
void dl_esp32p4_s8_min4d_bchw_w1_16_w2_1_simdmin_unaligned(int8_t *output_ptr,
                                                           int8_t *input0_ptr,
                                                           int8_t *input1_ptr,
                                                           void *args_ptr);
void dl_esp32p4_s8_min4d_bchw_w1_1_w2_16_simdmin_unaligned(int8_t *output_ptr,
                                                           int8_t *input0_ptr,
                                                           int8_t *input1_ptr,
                                                           void *args_ptr);
void dl_esp32p4_s16_min4d_bchw_w1_8_w2_8_simdmin_unaligned(int16_t *output_ptr,
                                                           int16_t *input0_ptr,
                                                           int16_t *input1_ptr,
                                                           void *args_ptr);
void dl_esp32p4_s16_min4d_bchw_w1_8_w2_1_simdmin_unaligned(int16_t *output_ptr,
                                                           int16_t *input0_ptr,
                                                           int16_t *input1_ptr,
                                                           void *args_ptr);
void dl_esp32p4_s16_min4d_bchw_w1_1_w2_8_simdmin_unaligned(int16_t *output_ptr,
                                                           int16_t *input0_ptr,
                                                           int16_t *input1_ptr,
                                                           void *args_ptr);

void dl_esp32p4_s8_max4d_bchw_w1_16_w2_16_simdmax(int8_t *output_ptr,
                                                  int8_t *input0_ptr,
                                                  int8_t *input1_ptr,
                                                  void *args_ptr);
void dl_esp32p4_s8_max4d_bchw_w1_16_w2_1_simdmax(int8_t *output_ptr,
                                                 int8_t *input0_ptr,
                                                 int8_t *input1_ptr,
                                                 void *args_ptr);
void dl_esp32p4_s8_max4d_bchw_w1_1_w2_16_simdmax(int8_t *output_ptr,
                                                 int8_t *input0_ptr,
                                                 int8_t *input1_ptr,
                                                 void *args_ptr);
void dl_esp32p4_s16_max4d_bchw_w1_8_w2_8_simdmax(int16_t *output_ptr,
                                                 int16_t *input0_ptr,
                                                 int16_t *input1_ptr,
                                                 void *args_ptr);
void dl_esp32p4_s16_max4d_bchw_w1_8_w2_1_simdmax(int16_t *output_ptr,
                                                 int16_t *input0_ptr,
                                                 int16_t *input1_ptr,
                                                 void *args_ptr);
void dl_esp32p4_s16_max4d_bchw_w1_1_w2_8_simdmax(int16_t *output_ptr,
                                                 int16_t *input0_ptr,
                                                 int16_t *input1_ptr,
                                                 void *args_ptr);
void dl_esp32p4_s8_max4d_bchw_w1_16_w2_16_simdmax_unaligned(int8_t *output_ptr,
                                                            int8_t *input0_ptr,
                                                            int8_t *input1_ptr,
                                                            void *args_ptr);
void dl_esp32p4_s8_max4d_bchw_w1_16_w2_1_simdmax_unaligned(int8_t *output_ptr,
                                                           int8_t *input0_ptr,
                                                           int8_t *input1_ptr,
                                                           void *args_ptr);
void dl_esp32p4_s8_max4d_bchw_w1_1_w2_16_simdmax_unaligned(int8_t *output_ptr,
                                                           int8_t *input0_ptr,
                                                           int8_t *input1_ptr,
                                                           void *args_ptr);
void dl_esp32p4_s16_max4d_bchw_w1_8_w2_8_simdmax_unaligned(int16_t *output_ptr,
                                                           int16_t *input0_ptr,
                                                           int16_t *input1_ptr,
                                                           void *args_ptr);
void dl_esp32p4_s16_max4d_bchw_w1_8_w2_1_simdmax_unaligned(int16_t *output_ptr,
                                                           int16_t *input0_ptr,
                                                           int16_t *input1_ptr,
                                                           void *args_ptr);
void dl_esp32p4_s16_max4d_bchw_w1_1_w2_8_simdmax_unaligned(int16_t *output_ptr,
                                                           int16_t *input0_ptr,
                                                           int16_t *input1_ptr,
                                                           void *args_ptr);

void dl_esp32p4_s8_s8_requantize_linear(int8_t *output_ptr, int8_t *input_ptr, void *args_ptr);
void dl_esp32p4_s8_s16_requantize_linear(int8_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_s16_requantize_linear(int16_t *output_ptr, int16_t *input_ptr, void *args_ptr);
void dl_esp32p4_s16_s8_requantize_linear(int16_t *output_ptr, int8_t *input_ptr, void *args_ptr);

void dl_esp32p4_dotprod_i16k16o16(int16_t *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, int shift, int n);
void dl_esp32p4_dotprod_i16k16o32(int32_t *output_ptr, int16_t *input0_ptr, int16_t *input1_ptr, int shift, int n);
void dl_esp32p4_dotprod_i16k8o16(int16_t *output_ptr, int8_t *input0_ptr, int16_t *input1_ptr, int shift, int n);
void dl_esp32p4_dotprod_i8k8o16(int16_t *out2_int8, int8_t *in1_int8, int8_t *in2_int8, int shift, int n);
}
