# YOLOv11 ESP32-S3 目标检测项目

基于 ESP32-S3 和 YOLOv11 的实时目标检测系统。

## 模型说明

本项目使用 **YOLOv11 单分类检测模型**（.espdl 格式）。模型文件不包含在仓库中,需要自行添加。

### 模型文件位置
- 路径: `coco_detect/models/s3/mymodel.espdl`
- 格式: ESP-DL 格式 (.espdl)
- 类型: YOLOv11 INT8 量化模型

## 多分类支持修改指南

如果需要使用**自定义的多分类 YOLOv11 模型**,需要修改以下位置:

### 1. 类别标签定义
**文件**: `esp-dl/vision/detect/dl_detect_yolo11_postprocessor.hpp` 或 `coco_detect/coco_detect.cpp`

修改检测类别标签数组,例如:
```cpp
// 默认 COCO 80 类
const char* class_names[] = {"person", "bicycle", "car", ...}; // 80 classes

// 自定义 N 类
const char* class_names[] = {"class1", "class2", "class3", ...}; // N classes
```

### 2. 后处理器配置
**文件**: `coco_detect/coco_detect.cpp:42-43`

调整后处理器参数以匹配模型输出:
```cpp
m_postprocessor = new dl::detect::yolo11PostProcessor(
    m_model,
    m_image_preprocessor,
    0.25,  // score_threshold - 调整置信度阈值
    0.7,   // nms_threshold - 调整NMS阈值
    10,    // max_boxes - 最大检测框数量
    {{8, 8, 4, 4}, {16, 16, 8, 8}, {32, 32, 16, 16}}  // anchor配置,需匹配模型
);
```

### 3. 检测结果处理
**文件**: `main/app_main.cpp`

根据新的类别更新结果显示逻辑:
- 修改类别 ID 到名称的映射
- 调整边界框颜色方案
- 更新检测结果的 JSON 输出格式

### 4. 模型输入尺寸
**文件**: `coco_detect/coco_detect.cpp:36-41`

如果模型输入尺寸不是默认的 192x192 或 320x320,需要确保:
- 图像预处理器配置正确的输入尺寸
- Letterbox 填充参数适配新尺寸

## 项目结构

```
yolo_esp32/
├── coco_detect/          # YOLO 检测组件
│   ├── models/s3/        # 模型文件目录 (*.espdl)
│   ├── coco_detect.cpp   # 检测实现
│   └── coco_detect.hpp   # 接口定义
├── esp-dl/               # ESP 深度学习库
├── main/                 # 主应用程序
│   └── app_main.cpp      # 入口文件
├── MODEL_REPLACEMENT_GUIDE.md   # 模型替换详细指南
├── DEPLOYMENT_GUIDE.md          # 部署指南
└── CMakeLists.txt
```

## 快速开始

1. 将 YOLOv11 模型 (.espdl) 放置到 `coco_detect/models/s3/mymodel.espdl`
2. 配置项目: `idf.py menuconfig`
3. 编译: `idf.py build`
4. 烧录: `idf.py flash monitor`

详细文档请参考:
- [MODEL_REPLACEMENT_GUIDE.md](MODEL_REPLACEMENT_GUIDE.md) - 模型替换详细指南
- [DEPLOYMENT_GUIDE.md](DEPLOYMENT_GUIDE.md) - 部署和配置指南

## 技术栈

- ESP-IDF 5.x
- ESP-DL (ESP32 深度学习库)
- YOLOv11 (INT8 量化)
- ESP32-S3 (带 PSRAM)

## 许可证

参见各组件的 LICENSE 文件。
