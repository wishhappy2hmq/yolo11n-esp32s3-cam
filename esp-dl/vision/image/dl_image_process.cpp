#include "dl_image_process.hpp"
#include "dl_image_pixel_cvt_dispatch.hpp"
#include "esp_log.h"

static const char *TAG = "ImageTransformer";

namespace dl {
namespace image {
NormQuantWrapper::NormQuantWrapper() : m_quant_type(UNK), m_chn(0), m_norm_quant(nullptr)
{
}

NormQuantWrapper::NormQuantWrapper(
    const std::vector<float> &mean, const std::vector<float> &std, int exp, quant_type_t quant_type, int chn) :
    m_quant_type(quant_type), m_chn(chn)
{
    if (m_quant_type == INT8_QUANT && m_chn == 3) {
        m_norm_quant = (void *)(new NormQuant<int8_t, 3>(mean, std, exp));
    } else if (m_quant_type == INT8_QUANT && m_chn == 1) {
        m_norm_quant = (void *)(new NormQuant<int8_t, 1>(mean, std, exp));
    } else if (m_quant_type == INT16_QUANT && m_chn == 3) {
        m_norm_quant = (void *)(new NormQuant<int16_t, 3>(mean, std, exp));
    } else if (m_quant_type == INT16_QUANT && m_chn == 1) {
        m_norm_quant = (void *)(new NormQuant<int16_t, 1>(mean, std, exp));
    }
}

NormQuantWrapper::~NormQuantWrapper()
{
    clear();
}

void NormQuantWrapper::clear()
{
    if (m_quant_type == INT8_QUANT && m_chn == 3) {
        delete ((NormQuant<int8_t, 3> *)m_norm_quant);
    } else if (m_quant_type == INT8_QUANT && m_chn == 1) {
        delete ((NormQuant<int8_t, 1> *)m_norm_quant);
    } else if (m_quant_type == INT16_QUANT && m_chn == 3) {
        delete ((NormQuant<int16_t, 3> *)m_norm_quant);
    } else if (m_quant_type == INT16_QUANT && m_chn == 1) {
        delete ((NormQuant<int16_t, 1> *)m_norm_quant);
    }
}

NormQuantWrapper::NormQuantWrapper(NormQuantWrapper &&rhs) noexcept :
    m_quant_type(rhs.m_quant_type), m_chn(rhs.m_chn), m_norm_quant(rhs.m_norm_quant)
{
    rhs.m_chn = 0;
    rhs.m_quant_type = UNK;
    rhs.m_norm_quant = nullptr;
}

NormQuantWrapper &NormQuantWrapper::operator=(NormQuantWrapper &&rhs) noexcept
{
    clear();
    m_chn = rhs.m_chn;
    m_quant_type = rhs.m_quant_type;
    m_norm_quant = rhs.m_norm_quant;
    rhs.m_chn = 0;
    rhs.m_quant_type = UNK;
    rhs.m_norm_quant = nullptr;
    return *this;
}

ImageTransformer::ImageTransformer() :
    m_src_img(),
    m_dst_img(),
    m_scale_x(0),
    m_scale_y(0),
    m_inv_scale_x(0),
    m_inv_scale_y(0),
    m_caps(0),
    m_x(nullptr),
    m_y(nullptr),
    m_x1(nullptr),
    m_x2(nullptr),
    m_y1(nullptr),
    m_y2(nullptr),
    m_gen_xy_map(false),
    m_new_bg_value(false),
    m_bg_src_color_space(false),
    m_bg_value_same(false)
{
}

ImageTransformer::~ImageTransformer()
{
    heap_caps_free(m_x);
    heap_caps_free(m_y);
    heap_caps_free(m_x1);
    heap_caps_free(m_x2);
    heap_caps_free(m_y1);
    heap_caps_free(m_y2);
}

ImageTransformer &ImageTransformer::set_norm_quant_param(const std::vector<float> &mean,
                                                         const std::vector<float> &std,
                                                         int exp,
                                                         NormQuantWrapper::quant_type_t quant_type)
{
    assert(mean.size() == std.size() && (mean.size() == 3 || mean.size() == 1));
    m_norm_quant_wrapper = NormQuantWrapper(mean, std, exp, quant_type, mean.size());
    return *this;
}

ImageTransformer &ImageTransformer::set_src_img_crop_area(const std::vector<int> &crop_area)
{
    assert(((crop_area.size() == 4 && crop_area[0] >= 0 && crop_area[1] >= 0 && crop_area[0] < crop_area[2] &&
             crop_area[1] < crop_area[3])) ||
           crop_area.empty());
    if (m_crop_area != crop_area) {
        m_crop_area = crop_area;
        m_gen_xy_map = true;
    }
    return *this;
}

ImageTransformer &ImageTransformer::set_dst_img_border(const std::vector<int> &border)
{
    assert((border.size() == 4 && std::all_of(border.begin(), border.end(), [](const auto &v) { return v >= 0; })) ||
           border.empty());
    if (m_border != border) {
        m_border = border;
        m_gen_xy_map = true;
    }
    return *this;
}

ImageTransformer &ImageTransformer::set_bg_value(const std::vector<uint8_t> &bg_value, bool src_color_space)
{
    if (src_color_space != m_bg_src_color_space || bg_value != m_bg_value) {
        m_bg_src_color_space = src_color_space;
        m_bg_value = bg_value;
        m_new_bg_value = true;
    }
    return *this;
}

ImageTransformer &ImageTransformer::set_src_img(const img_t &src_img)
{
    if ((src_img.width != m_src_img.width) || (src_img.height != m_src_img.height) ||
        get_pix_byte_size(src_img.pix_type) != get_pix_byte_size(m_src_img.pix_type)) {
        m_gen_xy_map = true;
    }
    m_src_img = src_img;
    return *this;
}

ImageTransformer &ImageTransformer::set_dst_img(const img_t &dst_img)
{
    if ((dst_img.width != m_dst_img.width) || (dst_img.height != m_dst_img.height) ||
        get_pix_byte_size(dst_img.pix_type) != get_pix_byte_size(m_dst_img.pix_type)) {
        m_gen_xy_map = true;
    }
    m_dst_img = dst_img;
    return *this;
}

ImageTransformer &ImageTransformer::set_caps(uint32_t caps)
{
    m_caps = caps;
    return *this;
}

ImageTransformer &ImageTransformer::set_warp_affine_matrix(const math::Matrix<float> &M, bool inv)
{
    auto set_M = [this](const math::Matrix<float> &M) -> ImageTransformer & {
        assert((M.array && M.h == 2 && M.w == 3) || !M.array);
        bool flag = false;
        if ((m_M.array && !M.array) || (!m_M.array && M.array)) {
            flag = true;
        } else if (m_M.array && M.array) {
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 3; j++) {
                    if (m_M.array[i][j] != M.array[i][j]) {
                        flag = true;
                        break;
                    }
                }
            }
        }
        if (flag) {
            m_M = M;
            m_gen_xy_map = true;
        }
        return *this;
    };

    if (!inv && M.array) {
        math::Matrix<float> inv_M(2, 3);
        float **M_ = M.array;
        float **inv_M_ = inv_M.array;
        float M0 = M_[0][0], M1 = M_[0][1], M2 = M_[0][2], M3 = M_[1][0], M4 = M_[1][1], M5 = M_[1][2];
        float D = M0 * M4 - M1 * M3;
        D = D != 0 ? 1. / D : 0;
        inv_M_[0][0] = M4 * D;
        inv_M_[0][1] = M1 * -D;
        inv_M_[1][0] = M3 * -D;
        inv_M_[1][1] = M0 * D;
        inv_M_[0][2] = -M0 * M2 - M1 * M5;
        inv_M_[1][2] = -M3 * M2 - M4 * M5;
        return set_M(inv_M);
    } else {
        return set_M(M);
    }
}

std::vector<int> &ImageTransformer::get_src_img_crop_area()
{
    return m_crop_area;
}

std::vector<int> &ImageTransformer::get_dst_img_border()
{
    return m_border;
}

NormQuantWrapper &ImageTransformer::get_norm_quant_wrapper()
{
    return m_norm_quant_wrapper;
}

const img_t &ImageTransformer::get_src_img()
{
    return m_src_img;
}

const img_t &ImageTransformer::get_dst_img()
{
    return m_dst_img;
}

float ImageTransformer::get_scale_x(bool inv)
{
    return inv ? m_inv_scale_x : m_scale_x;
}

float ImageTransformer::get_scale_y(bool inv)
{
    return inv ? m_inv_scale_y : m_scale_y;
}

void ImageTransformer::reset()
{
    set_src_img_crop_area({})
        .set_dst_img_border({})
        .set_bg_value({}, false)
        .set_src_img({})
        .set_dst_img({})
        .set_caps(0)
        .set_warp_affine_matrix({});
    m_norm_quant_wrapper = {};
}

template <bool SIMD>
esp_err_t ImageTransformer::transform()
{
    if (!m_src_img.data || !m_src_img.height || !m_src_img.width) {
        ESP_LOGE(TAG, "Invalid src img, call set_src_img().");
        return ESP_FAIL;
    }
    if (m_src_img.pix_type != DL_IMAGE_PIX_TYPE_RGB888 && m_src_img.pix_type != DL_IMAGE_PIX_TYPE_RGB565 &&
        m_src_img.pix_type != DL_IMAGE_PIX_TYPE_GRAY) {
        ESP_LOGE(TAG, "Unsupported src img pix_type.");
        return ESP_FAIL;
    }
    if (!m_dst_img.data || !m_dst_img.height || !m_dst_img.width) {
        ESP_LOGE(TAG, "Invalid dst img, call set_dst_img().");
        return ESP_FAIL;
    }
    if (is_pix_type_quant(m_dst_img.pix_type)) {
        bool flag = false;
        if (!m_norm_quant_wrapper.m_norm_quant ||
            get_pix_channel_num(m_dst_img.pix_type) != m_norm_quant_wrapper.m_chn) {
            flag = true;
        }
        if ((m_dst_img.pix_type == DL_IMAGE_PIX_TYPE_GRAY_QINT8 ||
             m_dst_img.pix_type == DL_IMAGE_PIX_TYPE_RGB888_QINT8) &&
            m_norm_quant_wrapper.m_quant_type != NormQuantWrapper::INT8_QUANT) {
            flag = true;
        }
        if ((m_dst_img.pix_type == DL_IMAGE_PIX_TYPE_GRAY_QINT16 ||
             m_dst_img.pix_type == DL_IMAGE_PIX_TYPE_RGB888_QINT16) &&
            m_norm_quant_wrapper.m_quant_type != NormQuantWrapper::INT16_QUANT) {
            flag = true;
        }
        if (flag) {
            ESP_LOGE(TAG, "Invalid norm quant param.");
            return ESP_FAIL;
        }
    }
    if (m_new_bg_value && !m_bg_value.empty()) {
        if (m_bg_src_color_space) {
            if (get_pix_byte_size(m_src_img.pix_type) != m_bg_value.size()) {
                ESP_LOGE(TAG, "Const value byte size does not match src img pixel byte size.");
                return ESP_FAIL;
            }
        } else {
            if (get_pix_byte_size(m_dst_img.pix_type) != m_bg_value.size()) {
                ESP_LOGE(TAG, "Const value byte size does not match dst img pixel byte size.");
                return ESP_FAIL;
            }
        }
    }
    if (!m_crop_area.empty() && (m_crop_area[2] > m_src_img.width || m_crop_area[3] > m_src_img.height)) {
        ESP_LOGE(TAG, "Invalid crop area.");
        return ESP_FAIL;
    }
    if (!m_border.empty() &&
        (m_border[0] + m_border[1] > m_dst_img.height - 1 || m_border[2] + m_border[3] > m_dst_img.width - 1)) {
        ESP_LOGE(TAG, "Invalid border.");
        return ESP_FAIL;
    }
    if (m_gen_xy_map) {
        gen_xy_map();
        m_gen_xy_map = false;
    }
    TransformNNFunctor<SIMD> fn{this};
    return pixel_cvt_dispatch(fn, m_src_img.pix_type, m_dst_img.pix_type, m_caps, m_norm_quant_wrapper.m_norm_quant);
}

#if CONFIG_IDF_TARGET_ESP32P4
template esp_err_t ImageTransformer::transform<true>();
#endif
template esp_err_t ImageTransformer::transform<false>();

void ImageTransformer::gen_xy_map()
{
    int dst_width = m_border.empty() ? m_dst_img.width : (m_dst_img.width - m_border[2] - m_border[3]);
    int dst_height = m_border.empty() ? m_dst_img.height : (m_dst_img.height - m_border[0] - m_border[1]);

    int col_step = get_pix_byte_size(m_src_img.pix_type);
    int row_step = m_src_img.width * get_pix_byte_size(m_src_img.pix_type);

    heap_caps_free(m_x);
    heap_caps_free(m_y);
    heap_caps_free(m_x1);
    heap_caps_free(m_x2);
    heap_caps_free(m_y1);
    heap_caps_free(m_y2);
    m_x = nullptr;
    m_y = nullptr;
    m_x1 = nullptr;
    m_x2 = nullptr;
    m_y1 = nullptr;
    m_y2 = nullptr;

    if (!m_M.array) {
        int src_width = m_crop_area.empty() ? m_src_img.width : (m_crop_area[2] - m_crop_area[0]);
        int src_height = m_crop_area.empty() ? m_src_img.height : (m_crop_area[3] - m_crop_area[1]);
        if (src_width == dst_width && src_height == dst_height) {
            m_scale_x = m_scale_y = m_inv_scale_x = m_inv_scale_y = 1;
            return;
        }
        m_x = (int *)heap_caps_malloc(dst_width * sizeof(int), MALLOC_CAP_DEFAULT | MALLOC_CAP_SIMD);
        m_y = (int *)heap_caps_malloc(dst_height * sizeof(int), MALLOC_CAP_DEFAULT | MALLOC_CAP_SIMD);
        m_inv_scale_x = static_cast<float>(src_width) / dst_width;
        m_inv_scale_y = static_cast<float>(src_height) / dst_height;
        m_scale_x = 1.f / m_inv_scale_x;
        m_scale_y = 1.f / m_inv_scale_y;
        if (m_crop_area.empty()) {
            for (int i = 0; i < dst_width; i++) {
                m_x[i] = std::min(static_cast<int>(i * m_inv_scale_x), src_width - 1) * col_step;
            }
            for (int i = 0; i < dst_height; i++) {
                m_y[i] = std::min(static_cast<int>(i * m_inv_scale_y), src_height - 1) * row_step;
            }
        } else {
            for (int i = 0; i < dst_width; i++) {
                m_x[i] = std::min(static_cast<int>(i * m_inv_scale_x) + m_crop_area[0], m_crop_area[2] - 1) * col_step;
            }
            for (int i = 0; i < dst_height; i++) {
                m_y[i] = std::min(static_cast<int>(i * m_inv_scale_y) + m_crop_area[1], m_crop_area[3] - 1) * row_step;
            }
        }
    } else {
        m_x1 = (int *)heap_caps_malloc(dst_width * sizeof(int), MALLOC_CAP_DEFAULT);
        m_x2 = (int *)heap_caps_malloc(dst_width * sizeof(int), MALLOC_CAP_DEFAULT);
        m_y1 = (int *)heap_caps_malloc(dst_height * sizeof(int), MALLOC_CAP_DEFAULT);
        m_y2 = (int *)heap_caps_malloc(dst_height * sizeof(int), MALLOC_CAP_DEFAULT);
        float **M = m_M.array;
        float M0 = M[0][0], M1 = M[0][1], M2 = M[0][2], M3 = M[1][0], M4 = M[1][1], M5 = M[1][2];
        constexpr int scale = 1 << warp_affine_shift;
        for (int i = 0; i < dst_width; i++) {
            m_x1[i] = static_cast<int>(__builtin_lrintf(M0 * i * scale));
            m_y1[i] = static_cast<int>(__builtin_lrintf(M3 * i * scale));
        }
        for (int i = 0; i < dst_height; i++) {
            m_x2[i] = static_cast<int>(__builtin_lrintf((M1 * i + M2) * scale));
            m_y2[i] = static_cast<int>(__builtin_lrintf((M4 * i + M5) * scale));
        }
    }
}
} // namespace image
} // namespace dl
