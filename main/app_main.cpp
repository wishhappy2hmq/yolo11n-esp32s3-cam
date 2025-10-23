#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_system.h"
#include "esp_camera.h"
#include "esp_http_server.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "coco_detect.hpp"
#include "img_converters.h"

static const char *TAG = "YOLO_DETECT_CAM";

// ==================== WiFi 配置 ====================
// WiFi信息 - 保持不变
#define WIFI_SSID "Niuniuniu"
#define WIFI_PASS "niuniuniu666!"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static EventGroupHandle_t s_wifi_event_group;
static int s_retry_num = 0;
#define WIFI_MAXIMUM_RETRY  5

// ==================== 摄像头引脚配置 ====================
#define CAM_PIN_PWDN -1
#define CAM_PIN_RESET -1
#define CAM_PIN_XCLK 15 // 摄像头时钟
#define CAM_PIN_SIOD 4  // I2C 数据线 (SDA)
#define CAM_PIN_SIOC 5  // I2C 时钟线 (SCL)

#define CAM_PIN_D7 16   // 数据线7 (Y9)
#define CAM_PIN_D6 17   // 数据线6 (Y8)
#define CAM_PIN_D5 18   // 数据线5 (Y7)
#define CAM_PIN_D4 12   // 数据线4 (Y6)
#define CAM_PIN_D3 10   // 数据线3 (Y5)
#define CAM_PIN_D2 8    // 数据线2 (Y4)
#define CAM_PIN_D1 9    // 数据线1 (Y3)
#define CAM_PIN_D0 11   // 数据线0 (Y2)
#define CAM_PIN_VSYNC 6 // 垂直同步
#define CAM_PIN_HREF 7  // 水平参考
#define CAM_PIN_PCLK 13 // 像素时钟

static camera_config_t camera_config = {
    .pin_pwdn = CAM_PIN_PWDN,
    .pin_reset = CAM_PIN_RESET,
    .pin_xclk = CAM_PIN_XCLK,
    .pin_sccb_sda = CAM_PIN_SIOD,
    .pin_sccb_scl = CAM_PIN_SIOC,

    .pin_d7 = CAM_PIN_D7,
    .pin_d6 = CAM_PIN_D6,
    .pin_d5 = CAM_PIN_D5,
    .pin_d4 = CAM_PIN_D4,
    .pin_d3 = CAM_PIN_D3,
    .pin_d2 = CAM_PIN_D2,
    .pin_d1 = CAM_PIN_D1,
    .pin_d0 = CAM_PIN_D0,
    .pin_vsync = CAM_PIN_VSYNC,
    .pin_href = CAM_PIN_HREF,
    .pin_pclk = CAM_PIN_PCLK,

    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_RGB565,   // 使用RGB565用于YOLO检测
    .frame_size = FRAMESIZE_QVGA,       // 320x240，适合实时检测
    .jpeg_quality = 12,
    .fb_count = 2,                      // 双缓冲，提升性能
    .fb_location = CAMERA_FB_IN_PSRAM,
    .grab_mode = CAMERA_GRAB_LATEST,    // 获取最新帧
    .sccb_i2c_port = 1
};

// ==================== 全局变量 ====================
static COCODetect *detector = nullptr;

// ==================== WiFi 事件处理 ====================
static void event_handler(void* arg, esp_event_base_t event_base,
                          int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < WIFI_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "重试连接WiFi... (%d/%d)", s_retry_num, WIFI_MAXIMUM_RETRY);
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG, "连接到AP失败");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "获得IP地址: " IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

// ==================== 初始化 WiFi ====================
void wifi_init_sta(void)
{
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    wifi_config_t wifi_config = {};
    strcpy((char*)wifi_config.sta.ssid, WIFI_SSID);
    strcpy((char*)wifi_config.sta.password, WIFI_PASS);
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "WiFi初始化完成");

    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "已连接到WiFi SSID: %s", WIFI_SSID);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "连接到WiFi SSID: %s 失败", WIFI_SSID);
    }
}

// ==================== 初始化摄像头 ====================
esp_err_t init_camera(void)
{
    ESP_LOGI(TAG, "初始化OV2640摄像头...");

    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "摄像头初始化失败: 0x%x", err);
        return err;
    }

    ESP_LOGI(TAG, "摄像头初始化成功！");

    sensor_t *s = esp_camera_sensor_get();
    if (s != NULL) {
        ESP_LOGI(TAG, "传感器ID: 0x%x", s->id.PID);
        // 调整摄像头参数以提升性能
        s->set_vflip(s, 0);        // 垂直翻转
        s->set_hmirror(s, 0);      // 水平镜像
    }

    return ESP_OK;
}

// ==================== 在图像上绘制矩形框 ====================
void draw_rectangle(uint8_t* img_data, int img_width, int img_height,
                   int x1, int y1, int x2, int y2, uint16_t color, int thickness)
{
    uint16_t* img = (uint16_t*)img_data;

    // 确保坐标在图像范围内
    x1 = (x1 < 0) ? 0 : ((x1 >= img_width) ? img_width - 1 : x1);
    x2 = (x2 < 0) ? 0 : ((x2 >= img_width) ? img_width - 1 : x2);
    y1 = (y1 < 0) ? 0 : ((y1 >= img_height) ? img_height - 1 : y1);
    y2 = (y2 < 0) ? 0 : ((y2 >= img_height) ? img_height - 1 : y2);

    // 绘制上下边
    for (int t = 0; t < thickness; t++) {
        for (int x = x1; x <= x2; x++) {
            if (y1 + t < img_height) img[(y1 + t) * img_width + x] = color;
            if (y2 - t >= 0) img[(y2 - t) * img_width + x] = color;
        }
    }

    // 绘制左右边
    for (int t = 0; t < thickness; t++) {
        for (int y = y1; y <= y2; y++) {
            if (x1 + t < img_width) img[y * img_width + (x1 + t)] = color;
            if (x2 - t >= 0) img[y * img_width + (x2 - t)] = color;
        }
    }
}

// ==================== HTTP 主页处理 ====================
static esp_err_t index_handler(httpd_req_t *req)
{
    const char* html =
        "<!DOCTYPE html>"
        "<html>"
        "<head>"
        "<meta charset='UTF-8'>"
        "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
        "<title>ESP32 YOLO11 单分类检测</title>"
        "<style>"
        "body { font-family: 'Segoe UI', Arial, sans-serif; text-align: center; margin: 0; padding: 20px; "
        "background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); }"
        "h1 { color: white; text-shadow: 2px 2px 4px rgba(0,0,0,0.3); margin-bottom: 10px; }"
        ".subtitle { color: #f0f0f0; font-size: 14px; margin-bottom: 20px; }"
        "img { max-width: 100%; height: auto; border: 4px solid white; "
        "box-shadow: 0 8px 16px rgba(0,0,0,0.3); border-radius: 8px; }"
        ".container { max-width: 1200px; margin: 0 auto; background: white; "
        "padding: 30px; border-radius: 15px; box-shadow: 0 10px 30px rgba(0,0,0,0.3); }"
        ".info { margin: 20px 0; color: #666; font-size: 14px; }"
        ".status { display: inline-block; padding: 8px 16px; background: #4CAF50; "
        "color: white; border-radius: 20px; margin: 10px 0; font-weight: bold; }"
        ".feature { display: inline-block; margin: 5px 10px; padding: 5px 12px; "
        "background: #f0f0f0; border-radius: 15px; font-size: 12px; color: #555; }"
        "</style>"
        "</head>"
        "<body>"
        "<div class='container'>"
        "<h1>ESP32-S3 YOLO11 检测系统</h1>"
        "<div class='subtitle'>基于 YOLO11n 单分类检测模型</div>"
        "<div class='status'>检测系统运行中</div>"
        "<br>"
        "<img src='/stream' alt='视频流加载中...' />"
        "<div class='info'>"
        "<span class='feature'>YOLO11n-int8</span>"
        "<span class='feature'>实时检测</span>"
        "<span class='feature'>320x240</span>"
        "<span class='feature'>单分类检测</span>"
        "</div>"
        "<div class='info' style='color: #999; font-size: 12px;'>刷新页面可重新连接 | Powered by ESP-DL</div>"
        "</div>"
        "</body>"
        "</html>";

    httpd_resp_send(req, html, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// ==================== HTTP MJPEG 流处理（带检测）====================
static esp_err_t stream_handler(httpd_req_t *req)
{
    camera_fb_t *fb = NULL;
    esp_err_t res = ESP_OK;
    size_t jpg_buf_len = 0;
    uint8_t *jpg_buf = NULL;
    char part_buf[64];

    res = httpd_resp_set_type(req, "multipart/x-mixed-replace; boundary=frame");
    if (res != ESP_OK) {
        return res;
    }

    ESP_LOGI(TAG, "流媒体客户端已连接");

    while (true) {
        // 获取摄像头帧
        fb = esp_camera_fb_get();
        if (!fb) {
            ESP_LOGE(TAG, "摄像头捕获失败");
            res = ESP_FAIL;
            break;
        }

        // 准备检测的图像格式
        dl::image::img_t img = {
            .data = fb->buf,
            .width = static_cast<uint16_t>(fb->width),
            .height = static_cast<uint16_t>(fb->height),
            .pix_type = dl::image::DL_IMAGE_PIX_TYPE_RGB565
        };

        // 运行检测
        int64_t detect_start = esp_timer_get_time();
        auto &detect_results = detector->run(img);
        int64_t detect_time = (esp_timer_get_time() - detect_start) / 1000;

        // 在图像上绘制检测结果
        int obj_count = 0;
        for (const auto &res : detect_results) {
            obj_count++;

            // 绘制绿色矩形框 (RGB565: 0x07E0 = 绿色)
            draw_rectangle(fb->buf, fb->width, fb->height,
                         res.box[0], res.box[1], res.box[2], res.box[3],
                         0x07E0, 2);

            ESP_LOGI(TAG, "目标 #%d: [分类: %d, 置信度: %.2f, 坐标: (%d,%d)-(%d,%d)]",
                     obj_count, res.category, res.score,
                     res.box[0], res.box[1], res.box[2], res.box[3]);
        }

        if (obj_count > 0) {
            ESP_LOGI(TAG, "检测到 %d 个目标，耗时: %lld ms", obj_count, detect_time);
        }

        // 将RGB565转换为JPEG
        bool jpeg_converted = frame2jpg(fb, 80, &jpg_buf, &jpg_buf_len);

        if (!jpeg_converted) {
            ESP_LOGE(TAG, "JPEG转换失败");
            esp_camera_fb_return(fb);
            res = ESP_FAIL;
            break;
        }

        // 发送MJPEG帧头
        if (res == ESP_OK) {
            size_t hlen = snprintf(part_buf, 64,
                "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n", jpg_buf_len);
            res = httpd_resp_send_chunk(req, part_buf, hlen);
        }

        // 发送JPEG数据
        if (res == ESP_OK) {
            res = httpd_resp_send_chunk(req, (const char *)jpg_buf, jpg_buf_len);
        }

        // 发送帧分隔符
        if (res == ESP_OK) {
            res = httpd_resp_send_chunk(req, "\r\n--frame\r\n", 13);
        }

        // 释放资源
        if (jpg_buf) {
            free(jpg_buf);
            jpg_buf = NULL;
        }
        esp_camera_fb_return(fb);

        if (res != ESP_OK) {
            ESP_LOGI(TAG, "客户端断开连接");
            break;
        }
    }

    return res;
}

// ==================== 启动 HTTP 服务器 ====================
httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;
    config.ctrl_port = 32768;
    config.max_open_sockets = 7;
    config.max_uri_handlers = 8;
    config.stack_size = 10240;  // 增加栈大小以支持YOLO检测

    ESP_LOGI(TAG, "启动HTTP服务器，端口: %d", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        ESP_LOGI(TAG, "注册URI处理器");

        httpd_uri_t index_uri = {
            .uri       = "/",
            .method    = HTTP_GET,
            .handler   = index_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &index_uri);

        httpd_uri_t stream_uri = {
            .uri       = "/stream",
            .method    = HTTP_GET,
            .handler   = stream_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &stream_uri);

        return server;
    }

    ESP_LOGE(TAG, "启动服务器失败");
    return NULL;
}

// ==================== 主函数 ====================
extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "   ESP32-S3 YOLO11 检测 Web 服务器");
    ESP_LOGI(TAG, "========================================");

    // 初始化 NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // 初始化摄像头
    ESP_LOGI(TAG, "步骤 1/3: 初始化摄像头...");
    if (init_camera() != ESP_OK) {
        ESP_LOGE(TAG, "摄像头初始化失败！");
        return;
    }
    ESP_LOGI(TAG, "✓ 摄像头初始化成功");

    // 初始化检测器
    ESP_LOGI(TAG, "步骤 2/3: 加载检测模型...");
    detector = new COCODetect();
    ESP_LOGI(TAG, "✓ 检测模型加载成功 (YOLO11n 单分类)");

    // 连接 WiFi
    ESP_LOGI(TAG, "步骤 3/3: 连接WiFi...");
    wifi_init_sta();
    ESP_LOGI(TAG, "✓ WiFi连接成功");

    // 启动 Web 服务器
    httpd_handle_t server = start_webserver();
    if (server == NULL) {
        ESP_LOGE(TAG, "Web服务器启动失败！");
        return;
    }

    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "   服务器已启动！");
    ESP_LOGI(TAG, "   请在浏览器中访问 ESP32 的 IP 地址");
    ESP_LOGI(TAG, "   检测已启用 (YOLO11n 单分类)");
    ESP_LOGI(TAG, "========================================");

    // 保持运行
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
