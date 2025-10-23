#include "coco_detect.hpp"
#include "esp_log.h"
#include <filesystem>

#if CONFIG_COCO_DETECT_MODEL_IN_FLASH_RODATA
extern const uint8_t coco_detect_espdl[] asm("_binary_coco_detect_espdl_start");
static const char *path = (const char *)coco_detect_espdl;
#elif CONFIG_COCO_DETECT_MODEL_IN_FLASH_PARTITION
static const char *path = "coco_det";
#else
#if !defined(CONFIG_BSP_SD_MOUNT_POINT)
#define CONFIG_BSP_SD_MOUNT_POINT "/sdcard"
#endif
#endif
namespace coco_detect {
Yolo11n::Yolo11n(const char *model_name, float score_thr, float nms_thr)
{
#if !CONFIG_COCO_DETECT_MODEL_IN_SDCARD
    bool param_copy = true;
    if (heap_caps_get_total_size(MALLOC_CAP_SPIRAM) < 1024 * 1024 * 9) {
        param_copy = false;
    }
    m_model = new dl::Model(path,
                            model_name,
                            static_cast<fbs::model_location_type_t>(CONFIG_COCO_DETECT_MODEL_LOCATION),
                            0,
                            dl::MEMORY_MANAGER_GREEDY,
                            nullptr,
                            param_copy);
#else
    auto sd_path = std::filesystem::path(CONFIG_BSP_SD_MOUNT_POINT) / CONFIG_COCO_DETECT_MODEL_SDCARD_DIR / model_name;
    m_model = new dl::Model(sd_path.c_str(), fbs::MODEL_LOCATION_IN_SDCARD);
#endif
    m_model->minimize();
#if CONFIG_IDF_TARGET_ESP32P4
    m_image_preprocessor = new dl::image::ImagePreprocessor(m_model, {0, 0, 0}, {255, 255, 255});
#else
    m_image_preprocessor = new dl::image::ImagePreprocessor(
        m_model, {0, 0, 0}, {255, 255, 255}, dl::image::DL_IMAGE_CAP_RGB565_BIG_ENDIAN);
#endif
    m_image_preprocessor->enable_letterbox({114, 114, 114});
    m_postprocessor = new dl::detect::yolo11PostProcessor(
        m_model, m_image_preprocessor, 0.25, 0.7, 10, {{8, 8, 4, 4}, {16, 16, 8, 8}, {32, 32, 16, 16}});
}

} // namespace coco_detect

COCODetect::COCODetect(model_type_t model_type, bool lazy_load) : m_model_type(model_type)
{
    switch (model_type) {
    case model_type_t::YOLO11N_S8_V1:
    case model_type_t::YOLO11N_S8_V2:
    case model_type_t::YOLO11N_S8_V3:
    case model_type_t::YOLO11N_320_S8_V3:
    case model_type_t::MYMODEL:
        m_score_thr[0] = coco_detect::Yolo11n::default_score_thr;
        m_nms_thr[0] = coco_detect::Yolo11n::default_nms_thr;
        break;
    }
    if (lazy_load) {
        m_model = nullptr;
    } else {
        load_model();
    }
}

void COCODetect::load_model()
{
    switch (m_model_type) {
    case model_type_t::YOLO11N_S8_V1:
#if CONFIG_FLASH_COCO_DETECT_YOLO11N_S8_V1 || CONFIG_COCO_DETECT_MODEL_IN_SDCARD
        m_model = new coco_detect::Yolo11n("coco_detect_yolo11n_s8_v1.espdl", m_score_thr[0], m_nms_thr[0]);
#else
        ESP_LOGE("coco_detect", "coco_detect_yolo11n_s8_v1 is not selected in menuconfig.");
#endif
        break;
    case model_type_t::YOLO11N_S8_V2:
#if CONFIG_FLASH_COCO_DETECT_YOLO11N_S8_V2 || CONFIG_COCO_DETECT_MODEL_IN_SDCARD
        m_model = new coco_detect::Yolo11n("coco_detect_yolo11n_s8_v2.espdl", m_score_thr[0], m_nms_thr[0]);
#else
        ESP_LOGE("coco_detect", "coco_detect_yolo11n_s8_v2 is not selected in menuconfig.");
#endif
        break;
    case model_type_t::YOLO11N_S8_V3:
#if CONFIG_FLASH_COCO_DETECT_YOLO11N_S8_V3 || CONFIG_COCO_DETECT_MODEL_IN_SDCARD
        m_model = new coco_detect::Yolo11n("coco_detect_yolo11n_s8_v3.espdl", m_score_thr[0], m_nms_thr[0]);
#else
        ESP_LOGE("coco_detect", "coco_detect_yolo11n_s8_v3 is not selected in menuconfig.");
#endif
        break;
    case model_type_t::YOLO11N_320_S8_V3:
#if CONFIG_FLASH_COCO_DETECT_YOLO11N_320_S8_V3 || CONFIG_COCO_DETECT_MODEL_IN_SDCARD
        m_model = new coco_detect::Yolo11n("coco_detect_yolo11n_320_s8_v3.espdl", m_score_thr[0], m_nms_thr[0]);
#else
        ESP_LOGE("coco_detect", "coco_detect_yolo11n_320_s8_v3 is not selected in menuconfig.");
#endif
        break;
    case model_type_t::MYMODEL:
#if CONFIG_FLASH_COCO_DETECT_MYMODEL || CONFIG_COCO_DETECT_MODEL_IN_SDCARD
        m_model = new coco_detect::Yolo11n("mymodel.espdl", m_score_thr[0], m_nms_thr[0]);
#else
        ESP_LOGE("coco_detect", "mymodel is not selected in menuconfig.");
#endif
        break;
    }
}
