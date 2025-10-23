# YOLOv11n ESP32-S3 Camera Object Detection / YOLOv11n ESP32-S3 摄像头目标检测

[English](#english) | [中文](#中文)

---

<a name="english"></a>
## 🚀 English

### Overview

A real-time object detection system based on **YOLOv11n** running on **ESP32-S3** with camera support. This project demonstrates how to deploy YOLOv11 models on resource-constrained edge devices using ESP-DL (Espressif Deep Learning Library).

### ✨ Key Features

- 🎯 **YOLOv11n Model**: INT8 quantized YOLOv11n model optimized for ESP32-S3
- 📷 **OV2640 Camera**: Real-time video streaming with object detection
- 🌐 **Web Interface**: MJPEG streaming with detection visualization
- ⚡ **High Performance**: Optimized inference with ESP-DL framework
- 🔧 **Easy Configuration**: Simple WiFi and camera pin configuration
- 📦 **Complete Framework**: Including ESP-DL library and model management

### 📋 Technical Specifications

| Component | Specification |
|-----------|--------------|
| **MCU** | ESP32-S3 (with PSRAM) |
| **Model** | YOLOv11n INT8 quantized |
| **Input Size** | 320×320 pixels |
| **Camera** | OV2640 (320×240 QVGA) |
| **Framework** | ESP-IDF 5.x + ESP-DL |
| **Detection** | Single-class detection (configurable for multi-class) |
| **Interface** | HTTP Web Server + MJPEG Streaming |

### 🔌 Camera Pin Configuration

The default camera pin configuration for ESP32-S3:

```c
// Power & Clock Pins
PWDN   : -1 (Not used)
RESET  : -1 (Not used)
XCLK   : GPIO 15  // Camera clock

// I2C Interface (SCCB)
SIOD   : GPIO 4   // I2C Data (SDA)
SIOC   : GPIO 5   // I2C Clock (SCL)

// Parallel Data Interface (8-bit)
D0 (Y2): GPIO 11
D1 (Y3): GPIO 9
D2 (Y4): GPIO 8
D3 (Y5): GPIO 10
D4 (Y6): GPIO 12
D5 (Y7): GPIO 18
D6 (Y8): GPIO 17
D7 (Y9): GPIO 16

// Sync Signals
VSYNC  : GPIO 6   // Vertical sync
HREF   : GPIO 7   // Horizontal reference
PCLK   : GPIO 13  // Pixel clock
```

**Location in code**: [`main/app_main.cpp:30-47`](main/app_main.cpp#L30-L47)

> 💡 **Note**: Modify these pins according to your hardware connections. Make sure your ESP32-S3 board has PSRAM enabled.

### 📦 Hardware Requirements

- **ESP32-S3 Development Board** (with minimum 8MB Flash + 8MB PSRAM)
- **OV2640 Camera Module** (or compatible camera)
- **USB-C Cable** for programming and power
- **WiFi Network** for web interface access

### 🛠️ Quick Start

#### 1. Clone the Repository

```bash
git clone https://github.com/wishhappy2hmq/yolo11n-esp32s3-cam.git
cd yolo11n-esp32s3-cam
```

#### 2. Add Your Model

Place your YOLOv11 model file (`.espdl` format) in the models directory:

```bash
# Copy your custom model to:
coco_detect/models/s3/mymodel.espdl
```

> ⚠️ **Important**: The `.espdl` model file is NOT included in this repository. You need to convert your YOLOv11 model to ESP-DL format. See [Model Conversion Guide](#model-conversion).

#### 3. Configure WiFi

Edit WiFi credentials in [`main/app_main.cpp`](main/app_main.cpp#L20-L21):

```c
#define WIFI_SSID "YourWiFiSSID"
#define WIFI_PASS "YourWiFiPassword"
```

#### 4. Build and Flash

```bash
# Set ESP-IDF environment (if not already set)
. $HOME/esp/esp-idf/export.sh

# Configure project
idf.py menuconfig

# Build the project
idf.py build

# Flash to ESP32-S3
idf.py -p /dev/ttyUSB0 flash monitor
```

#### 5. Access Web Interface

After successful boot, the device will print its IP address. Open your browser and navigate to:

```
http://<ESP32-IP-ADDRESS>/
```

You should see the live camera feed with real-time object detection!

### 🔄 Model Replacement Guide

#### For Single-Class Models

Simply replace the model file:

```bash
cp your_new_model.espdl coco_detect/models/s3/mymodel.espdl
idf.py fullclean
idf.py build
```

#### For Multi-Class YOLOv11 Models

If you want to use a custom **multi-class YOLOv11 model**, you need to modify the following files:

##### 1. Update Class Labels

**File**: `esp-dl/vision/detect/dl_detect_yolo11_postprocessor.hpp` or `coco_detect/coco_detect.cpp`

Modify the class names array:

```cpp
// Default COCO 80 classes
const char* class_names[] = {"person", "bicycle", "car", ...}; // 80 classes

// Your custom N classes
const char* class_names[] = {"class1", "class2", "class3", ...}; // N classes
```

##### 2. Adjust Post-Processor Configuration

**File**: [`coco_detect/coco_detect.cpp:42-43`](coco_detect/coco_detect.cpp#L42-L43)

Update anchor configuration to match your model:

```cpp
m_postprocessor = new dl::detect::yolo11PostProcessor(
    m_model,
    m_image_preprocessor,
    0.25,  // score_threshold - adjust confidence threshold
    0.7,   // nms_threshold - adjust NMS threshold
    10,    // max_boxes - maximum detection boxes
    {{8, 8, 4, 4}, {16, 16, 8, 8}, {32, 32, 16, 16}}  // anchor config
);
```

##### 3. Update Detection Result Handling

**File**: [`main/app_main.cpp`](main/app_main.cpp#L295-L307)

Modify result display logic:
- Update category ID to name mapping
- Adjust bounding box colors
- Update JSON output format

##### 4. Adjust Input Size (if different from 320×320)

**File**: [`coco_detect/coco_detect.cpp:36-41`](coco_detect/coco_detect.cpp#L36-L41)

Ensure image preprocessor matches your model input size:

```cpp
m_image_preprocessor = new dl::image::ImagePreprocessor(
    m_model, {0, 0, 0}, {255, 255, 255},
    dl::image::DL_IMAGE_CAP_RGB565_BIG_ENDIAN
);
m_image_preprocessor->enable_letterbox({114, 114, 114});
```

### 📐 Model Conversion

<a name="model-conversion"></a>

To convert your YOLOv11 model to ESP-DL format (`.espdl`):

1. **Train** your YOLOv11 model using PyTorch/Ultralytics
2. **Export** to ONNX format
3. **Quantize** to INT8 using ESP-DL tools
4. **Convert** to `.espdl` format

For detailed conversion steps, refer to:
- [MODEL_REPLACEMENT_GUIDE.md](MODEL_REPLACEMENT_GUIDE.md)
- [ESP-DL Documentation](https://github.com/espressif/esp-dl)

### 📚 Project Structure

```
yolo11n-esp32s3-cam/
├── coco_detect/              # YOLO detection component
│   ├── models/s3/            # Model files directory (*.espdl)
│   ├── coco_detect.cpp       # Detection implementation
│   └── coco_detect.hpp       # Detection interface
├── esp-dl/                   # ESP Deep Learning library
│   ├── vision/detect/        # Detection modules (YOLO11 post-processor)
│   └── ...                   # Other ESP-DL components
├── main/                     # Main application
│   ├── app_main.cpp          # Entry point & web server
│   └── idf_component.yml     # Component dependencies
├── MODEL_REPLACEMENT_GUIDE.md # Detailed model replacement guide
├── DEPLOYMENT_GUIDE.md        # Deployment and configuration guide
├── .gitignore                 # Git ignore rules (excludes .espdl files)
└── README.md                  # This file
```

### 🔧 Advanced Configuration

#### Memory Configuration

The project requires **PSRAM** enabled. Key configurations in `sdkconfig.defaults.esp32s3`:

```ini
CONFIG_SPIRAM=y
CONFIG_SPIRAM_MODE_OCT=y
CONFIG_SPIRAM_SPEED_80M=y
CONFIG_ESP32S3_DATA_CACHE_64KB=y
```

#### Performance Tuning

```ini
CONFIG_COMPILER_OPTIMIZATION_PERF=y
CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ_240=y
```

#### Flash Size

Minimum 8MB flash recommended:

```ini
CONFIG_ESPTOOLPY_FLASHSIZE_8MB=y
```

### 🐛 Troubleshooting

| Issue | Solution |
|-------|----------|
| **Camera init failed** | Check pin connections and camera module |
| **Out of memory** | Ensure PSRAM is enabled and working |
| **Model not loading** | Verify `.espdl` file exists and is valid |
| **WiFi connection failed** | Check SSID/password and network availability |
| **Slow detection** | Reduce camera resolution or model input size |

### 📄 License

This project includes multiple components with different licenses:
- ESP-DL: Apache License 2.0
- COCO Detection Component: See [coco_detect/LICENSE](coco_detect/LICENSE)

### 🙏 Acknowledgments

- [Espressif ESP-DL](https://github.com/espressif/esp-dl) - Deep learning library for ESP32
- [Ultralytics YOLOv11](https://github.com/ultralytics/ultralytics) - YOLO model framework

---

<a name="中文"></a>
## 🚀 中文

### 项目简介

这是一个基于 **YOLOv11n** 模型在 **ESP32-S3** 上实现的实时目标检测系统。项目展示了如何在资源受限的边缘设备上部署 YOLOv11 模型，使用 ESP-DL（乐鑫深度学习库）实现高效推理。

### ✨ 核心特性

- 🎯 **YOLOv11n 模型**: 针对 ESP32-S3 优化的 INT8 量化模型
- 📷 **OV2640 摄像头**: 实时视频流与目标检测
- 🌐 **Web 界面**: MJPEG 流式传输与检测可视化
- ⚡ **高性能**: ESP-DL 框架优化推理
- 🔧 **易于配置**: 简单的 WiFi 和摄像头引脚配置
- 📦 **完整框架**: 包含 ESP-DL 库和模型管理

### 📋 技术规格

| 组件 | 规格 |
|------|------|
| **MCU** | ESP32-S3 (带 PSRAM) |
| **模型** | YOLOv11n INT8 量化 |
| **输入尺寸** | 320×320 像素 |
| **摄像头** | OV2640 (320×240 QVGA) |
| **框架** | ESP-IDF 5.x + ESP-DL |
| **检测类型** | 单分类检测（可配置为多分类） |
| **交互界面** | HTTP Web 服务器 + MJPEG 流 |

### 🔌 摄像头引脚配置

ESP32-S3 的默认摄像头引脚配置：

```c
// 电源和时钟引脚
PWDN   : -1 (未使用)
RESET  : -1 (未使用)
XCLK   : GPIO 15  // 摄像头时钟

// I2C 接口 (SCCB)
SIOD   : GPIO 4   // I2C 数据线 (SDA)
SIOC   : GPIO 5   // I2C 时钟线 (SCL)

// 并行数据接口 (8位)
D0 (Y2): GPIO 11
D1 (Y3): GPIO 9
D2 (Y4): GPIO 8
D3 (Y5): GPIO 10
D4 (Y6): GPIO 12
D5 (Y7): GPIO 18
D6 (Y8): GPIO 17
D7 (Y9): GPIO 16

// 同步信号
VSYNC  : GPIO 6   // 垂直同步
HREF   : GPIO 7   // 水平参考
PCLK   : GPIO 13  // 像素时钟
```

**代码位置**: [`main/app_main.cpp:30-47`](main/app_main.cpp#L30-L47)

> 💡 **注意**: 请根据你的硬件连接修改这些引脚。确保你的 ESP32-S3 开发板已启用 PSRAM。

### 📦 硬件需求

- **ESP32-S3 开发板**（至少 8MB Flash + 8MB PSRAM）
- **OV2640 摄像头模块**（或兼容摄像头）
- **USB-C 数据线**（用于编程和供电）
- **WiFi 网络**（用于访问 Web 界面）

### 🛠️ 快速开始

#### 1. 克隆仓库

```bash
git clone https://github.com/wishhappy2hmq/yolo11n-esp32s3-cam.git
cd yolo11n-esp32s3-cam
```

#### 2. 添加模型文件

将你的 YOLOv11 模型文件（`.espdl` 格式）放置到模型目录：

```bash
# 将自定义模型复制到:
coco_detect/models/s3/mymodel.espdl
```

> ⚠️ **重要**: `.espdl` 模型文件未包含在此仓库中。你需要将 YOLOv11 模型转换为 ESP-DL 格式。参见[模型转换指南](#模型转换)。

#### 3. 配置 WiFi

编辑 [`main/app_main.cpp`](main/app_main.cpp#L20-L21) 中的 WiFi 凭据：

```c
#define WIFI_SSID "你的WiFi名称"
#define WIFI_PASS "你的WiFi密码"
```

#### 4. 编译和烧录

```bash
# 设置 ESP-IDF 环境（如果尚未设置）
. $HOME/esp/esp-idf/export.sh

# 配置项目
idf.py menuconfig

# 编译项目
idf.py build

# 烧录到 ESP32-S3
idf.py -p /dev/ttyUSB0 flash monitor
```

#### 5. 访问 Web 界面

成功启动后，设备会打印其 IP 地址。在浏览器中访问：

```
http://<ESP32-IP地址>/
```

你将看到带有实时目标检测的摄像头画面！

### 🔄 模型替换指南

#### 单分类模型

直接替换模型文件：

```bash
cp 你的新模型.espdl coco_detect/models/s3/mymodel.espdl
idf.py fullclean
idf.py build
```

#### 多分类 YOLOv11 模型

如果要使用自定义的**多分类 YOLOv11 模型**，需要修改以下文件：

##### 1. 更新类别标签

**文件**: `esp-dl/vision/detect/dl_detect_yolo11_postprocessor.hpp` 或 `coco_detect/coco_detect.cpp`

修改类别名称数组：

```cpp
// 默认 COCO 80 类
const char* class_names[] = {"person", "bicycle", "car", ...}; // 80 类

// 自定义 N 类
const char* class_names[] = {"类别1", "类别2", "类别3", ...}; // N 类
```

##### 2. 调整后处理器配置

**文件**: [`coco_detect/coco_detect.cpp:42-43`](coco_detect/coco_detect.cpp#L42-L43)

更新 anchor 配置以匹配模型：

```cpp
m_postprocessor = new dl::detect::yolo11PostProcessor(
    m_model,
    m_image_preprocessor,
    0.25,  // score_threshold - 调整置信度阈值
    0.7,   // nms_threshold - 调整 NMS 阈值
    10,    // max_boxes - 最大检测框数量
    {{8, 8, 4, 4}, {16, 16, 8, 8}, {32, 32, 16, 16}}  // anchor 配置
);
```

##### 3. 更新检测结果处理

**文件**: [`main/app_main.cpp`](main/app_main.cpp#L295-L307)

修改结果显示逻辑：
- 更新类别 ID 到名称的映射
- 调整边界框颜色
- 更新 JSON 输出格式

##### 4. 调整输入尺寸（如果与 320×320 不同）

**文件**: [`coco_detect/coco_detect.cpp:36-41`](coco_detect/coco_detect.cpp#L36-L41)

确保图像预处理器匹配模型输入尺寸：

```cpp
m_image_preprocessor = new dl::image::ImagePreprocessor(
    m_model, {0, 0, 0}, {255, 255, 255},
    dl::image::DL_IMAGE_CAP_RGB565_BIG_ENDIAN
);
m_image_preprocessor->enable_letterbox({114, 114, 114});
```

### 📐 模型转换

<a name="模型转换"></a>

将 YOLOv11 模型转换为 ESP-DL 格式（`.espdl`）：

1. **训练** YOLOv11 模型（使用 PyTorch/Ultralytics）
2. **导出** 为 ONNX 格式
3. **量化** 为 INT8（使用 ESP-DL 工具）
4. **转换** 为 `.espdl` 格式

详细转换步骤请参考：
- [MODEL_REPLACEMENT_GUIDE.md](MODEL_REPLACEMENT_GUIDE.md)
- [ESP-DL 文档](https://github.com/espressif/esp-dl)

### 📚 项目结构

```
yolo11n-esp32s3-cam/
├── coco_detect/              # YOLO 检测组件
│   ├── models/s3/            # 模型文件目录 (*.espdl)
│   ├── coco_detect.cpp       # 检测实现
│   └── coco_detect.hpp       # 检测接口
├── esp-dl/                   # ESP 深度学习库
│   ├── vision/detect/        # 检测模块 (YOLO11 后处理器)
│   └── ...                   # 其他 ESP-DL 组件
├── main/                     # 主应用程序
│   ├── app_main.cpp          # 入口点 & Web 服务器
│   └── idf_component.yml     # 组件依赖
├── MODEL_REPLACEMENT_GUIDE.md # 详细模型替换指南
├── DEPLOYMENT_GUIDE.md        # 部署和配置指南
├── .gitignore                 # Git 忽略规则（排除 .espdl 文件）
└── README.md                  # 本文件
```

### 🔧 高级配置

#### 内存配置

项目需要启用 **PSRAM**。关键配置在 `sdkconfig.defaults.esp32s3`：

```ini
CONFIG_SPIRAM=y
CONFIG_SPIRAM_MODE_OCT=y
CONFIG_SPIRAM_SPEED_80M=y
CONFIG_ESP32S3_DATA_CACHE_64KB=y
```

#### 性能调优

```ini
CONFIG_COMPILER_OPTIMIZATION_PERF=y
CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ_240=y
```

#### Flash 大小

建议最小 8MB Flash：

```ini
CONFIG_ESPTOOLPY_FLASHSIZE_8MB=y
```

### 🐛 故障排除

| 问题 | 解决方案 |
|------|----------|
| **摄像头初始化失败** | 检查引脚连接和摄像头模块 |
| **内存不足** | 确保 PSRAM 已启用且正常工作 |
| **模型加载失败** | 验证 `.espdl` 文件存在且有效 |
| **WiFi 连接失败** | 检查 SSID/密码和网络可用性 |
| **检测速度慢** | 降低摄像头分辨率或模型输入尺寸 |

### 📄 许可证

本项目包含多个具有不同许可证的组件：
- ESP-DL: Apache License 2.0
- COCO 检测组件: 见 [coco_detect/LICENSE](coco_detect/LICENSE)

### 🙏 致谢

- [Espressif ESP-DL](https://github.com/espressif/esp-dl) - ESP32 深度学习库
- [Ultralytics YOLOv11](https://github.com/ultralytics/ultralytics) - YOLO 模型框架

---

## 📸 Screenshots / 截图

### Web Interface / Web 界面
![Web Interface](docs/images/web_interface.png)

### Detection Example / 检测示例
![Detection](docs/images/detection_example.png)

---

**Made with ❤️ for Edge AI Development**

**为边缘 AI 开发而制作 ❤️**
