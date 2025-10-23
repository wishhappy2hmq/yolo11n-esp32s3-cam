# YOLO11 Model Replacement Guide

## Overview
This guide explains how to replace the YOLO11 detection model in the ESP32-S3 project.

---

## Model Configuration Files

### Key Files to Modify

1. **Model File Location**
   - Path: `coco_detect/models/s3/mymodel.espdl`
   - This is the actual model file used when `CONFIG_FLASH_COCO_DETECT_MYMODEL` is enabled

2. **Kconfig Configuration**
   - Path: `coco_detect/Kconfig`
   - Defines available models and their selection options

3. **CMake Build Configuration**
   - Path: `coco_detect/CMakeLists.txt`
   - Handles model packing and flashing

4. **SDK Config**
   - Path: `sdkconfig.defaults.esp32s3`
   - Default configuration for model selection

---

## Steps to Replace Model

### Option 1: Replace the Current Model (Recommended for Quick Updates)

**Step 1: Prepare Your Model**
- Ensure your new model is in `.espdl` format
- Model should be quantized to INT8 for ESP32-S3
- Recommended input size: 320x320 or 192x192

**Step 2: Replace Model File**
```bash
# Backup the original model
cp coco_detect/models/s3/mymodel.espdl coco_detect/models/s3/mymodel.espdl.bak

# Copy your new model
cp /path/to/your/new_model.espdl coco_detect/models/s3/mymodel.espdl
```

**Step 3: Clean and Rebuild**
```bash
idf.py fullclean
idf.py build
```

---

### Option 2: Add a New Model Configuration

**Step 1: Add Model File**
```bash
# Copy your model to the s3 models directory
cp /path/to/your/custom_model.espdl coco_detect/models/s3/coco_detect_custom.espdl
```

**Step 2: Update Kconfig** (`coco_detect/Kconfig`)

Add the flash option (around line 18):
```kconfig
config FLASH_COCO_DETECT_CUSTOM
    bool "flash custom_model"
    depends on !COCO_DETECT_MODEL_IN_SDCARD
    default n
```

Add the selection option (around line 40):
```kconfig
config COCO_DETECT_CUSTOM
    bool "custom_model"
    depends on COCO_DETECT_MODEL_IN_SDCARD || FLASH_COCO_DETECT_CUSTOM
```

Update the default model index (around line 51):
```kconfig
config DEFAULT_COCO_DETECT_MODEL
    int
    default 0 if COCO_DETECT_YOLO11N_S8_V1
    default 1 if COCO_DETECT_YOLO11N_S8_V2
    default 2 if COCO_DETECT_YOLO11N_S8_V3
    default 3 if COCO_DETECT_YOLO11N_320_S8_V3
    default 4 if COCO_DETECT_MYMODEL
    default 5 if COCO_DETECT_CUSTOM        # Add this line
```

**Step 3: Update CMakeLists.txt** (`coco_detect/CMakeLists.txt`)

Add around line 42:
```cmake
if(CONFIG_FLASH_COCO_DETECT_CUSTOM)
    list(APPEND models ${models_dir}/coco_detect_custom.espdl)
endif()
```

**Step 4: Configure and Build**
```bash
idf.py menuconfig
# Navigate to: Component config -> models: coco_detect
# Enable "flash custom_model"
# Select "custom_model" as default model

idf.py build
```

---

## Model Storage Options

### 1. Flash RODATA (Default - Recommended)
- **Location**: Compiled into firmware as read-only data
- **Config**: `CONFIG_COCO_DETECT_MODEL_IN_FLASH_RODATA=y`
- **Pros**: Fastest access, no external storage needed
- **Cons**: Takes up flash space (2-3MB per model)
- **Max Models**: Limited by flash size (8MB total)

### 2. Flash Partition
- **Location**: Separate flash partition
- **Config**: `CONFIG_COCO_DETECT_MODEL_IN_FLASH_PARTITION=y`
- **Pros**: Easier to update model without reflashing firmware
- **Cons**: Requires partition table modification
- **Note**: Need to update `partitions.csv` to add a `coco_det` partition

### 3. SD Card
- **Location**: External SD card
- **Config**: `CONFIG_COCO_DETECT_MODEL_IN_SDCARD=y`
- **Pros**: Easy to swap models, unlimited storage
- **Cons**: Slower access, requires SD card hardware
- **Path**: Set via `CONFIG_COCO_DETECT_MODEL_SDCARD_DIR`

---

## Available Pre-built Models

The project includes 5 pre-built YOLO11n models:

| Model Name | File Size | Input Size | Description |
|------------|-----------|------------|-------------|
| yolo11n_s8_v1 | ~2.8 MB | 192x192 | Version 1 (INT8) |
| yolo11n_s8_v2 | ~2.9 MB | 192x192 | Version 2 (INT8) |
| yolo11n_s8_v3 | ~2.8 MB | 192x192 | Version 3 (INT8) |
| yolo11n_320_s8_v3 | ~2.8 MB | 320x320 | Version 3 (320px, INT8) |
| mymodel | ~2.8 MB | custom | Your custom model |

---

## Model Format Requirements

### ESP-DL Model Format (.espdl)

Your model must be converted to ESP-DL's `.espdl` format:

1. **Training**: Train your YOLO11 model using PyTorch/Ultralytics
2. **Export**: Export to ONNX format
3. **Quantization**: Quantize to INT8 using ESP-DL tools
4. **Conversion**: Convert to `.espdl` format using ESP-DL's model converter

**Reference**:
- ESP-DL Model Zoo: https://github.com/espressif/esp-dl
- Model Conversion Tools: `esp-dl/tools/`

---

## Verification After Replacement

1. **Check Build Output**
   ```bash
   idf.py build
   # Look for: "Move and Pack models..." in the output
   # Verify model size in build logs
   ```

2. **Check Flash Size**
   ```bash
   # Ensure your model fits in the partition
   # Check: build/yolo11_detect.bin size < 8MB
   ```

3. **Test Detection**
   - Flash to device: `idf.py flash monitor`
   - Check HTTP endpoint: `http://<device-ip>/`
   - Verify detection results match your model's classes

---

## Troubleshooting

### Model Too Large
- **Problem**: `region 'irom0_0_seg' overflowed`
- **Solution**:
  - Reduce model size or use Flash Partition/SD Card storage
  - Increase flash size in `sdkconfig.defaults.esp32s3`:
    ```
    CONFIG_ESPTOOLPY_FLASHSIZE_16MB=y  # Instead of 8MB
    ```

### Model Not Loading
- **Problem**: Device crashes or fails to detect
- **Solution**:
  - Verify `.espdl` format is correct
  - Check PSRAM is enabled (`CONFIG_SPIRAM=y`)
  - Ensure model input size matches preprocessing code

### Wrong Model Loaded
- **Problem**: Old model still being used
- **Solution**:
  ```bash
  idf.py fullclean
  rm -rf build/
  idf.py build
  ```

---

## Model Configuration Summary

| Configuration Item | Location | Purpose |
|-------------------|----------|---------|
| Model binary file | `coco_detect/models/s3/*.espdl` | Actual model data |
| Model selection | `coco_detect/Kconfig` | Available model options |
| Build integration | `coco_detect/CMakeLists.txt` | Model packing logic |
| Default config | `sdkconfig.defaults.esp32s3` | Default model selection |
| Detection code | `coco_detect/coco_detect.cpp` | Model loading & inference |
| Application code | `main/app_main.cpp` | Detection pipeline |

---

## Quick Reference Commands

```bash
# View current model configuration
idf.py menuconfig
# Navigate to: Component config -> models: coco_detect

# Clean build
idf.py fullclean

# Build with verbose output
idf.py -v build

# Flash and monitor
idf.py -p COM3 flash monitor

# Check model size
ls -lh coco_detect/models/s3/*.espdl
```

---

## Notes

1. **Performance**: 320x320 models provide better accuracy but slower inference than 192x192
2. **Memory**: Each model requires ~3MB flash + ~2MB RAM during inference
3. **Classes**: COCO dataset has 80 classes. If using custom classes, update detection labels in code
4. **Multiple Models**: You can flash multiple models and switch between them via menuconfig
