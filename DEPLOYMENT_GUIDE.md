# Project Deployment Guide

## Overview
This guide explains which files are included in the build, which can be removed, and how to copy a working version of the project.

---

## Project Structure Analysis

### Files INCLUDED in Build

The ESP-IDF build system **only compiles files referenced in CMakeLists.txt**. Based on the configuration:

#### Root Level
```
yolo11_detect/
├── CMakeLists.txt              ✓ Required - Main project file
├── partitions.csv              ✓ Required - Flash partition table
├── sdkconfig.defaults          ✓ Required - Base configuration
├── sdkconfig.defaults.esp32s3  ✓ Required - ESP32-S3 specific config
├── sdkconfig.camera.esp32s3    ✓ Optional - Alternative camera config
├── main/                       ✓ Required - Application code
│   ├── CMakeLists.txt
│   ├── idf_component.yml
│   ├── app_main.cpp
│   └── bus.jpg                 ✓ Embedded test image
├── coco_detect/                ✓ Required - YOLO detection component
│   ├── CMakeLists.txt
│   ├── Kconfig
│   ├── idf_component.yml
│   ├── coco_detect.cpp
│   ├── coco_detect.hpp
│   └── models/s3/*.espdl       ✓ Model files (only selected ones)
└── esp-dl/                     ✓ Required - ESP-DL library
    ├── CMakeLists.txt
    ├── dl/                     ✓ Core DL library
    ├── vision/                 ✓ Vision algorithms
    ├── audio/                  ✓ Audio features
    └── fbs_loader/             ✓ Model loader
```

#### Managed Components (Auto-downloaded)
```
managed_components/             ✓ Auto-managed by ESP-IDF
├── espressif__button/
├── espressif__esp32-camera/
├── espressif__esp32_s3_eye_noglib/
├── espressif__esp_codec_dev/
├── espressif__esp_jpeg/
├── espressif__esp_new_jpeg/
└── espressif__esp-dsp/
```

---

### Files EXCLUDED from Build

These directories are **NOT compiled** and can be safely removed:

#### Documentation Files
```
❌ Can Remove:
├── README.md                   # Documentation
├── BUILD_GUIDE.md              # Build instructions
├── LICENSE                     # License files
├── *.md                        # All markdown files
└── img/                        # Images for documentation
```

#### Example Projects
```
❌ Can Remove - Examples (34+ directories):
├── esp-dl/examples/            # ESP-DL examples
│   ├── basic_math/
│   ├── conv2d/
│   ├── dotprod/
│   ├── fft/
│   ├── fft4real/
│   ├── fir/
│   ├── iir/
│   ├── kalman/
│   └── matrix/
├── esp-dl/applications/        # ESP-DL application examples
│   ├── lyrat_board_app/
│   ├── spectrum_box_lite/
│   └── m5stack_core_s3/
└── esp-dl/external_examples/   # External examples
```

```
❌ Can Remove - Managed Component Examples:
├── managed_components/espressif__esp32-camera/examples/
├── managed_components/espressif__esp_jpeg/examples/
├── managed_components/espressif__esp_jpeg/test_apps/
├── managed_components/espressif__esp-dsp/examples/
├── managed_components/espressif__esp-dsp/applications/
├── managed_components/espressif__esp-dsp/external_examples/
├── managed_components/espressif__button/examples/
└── managed_components/espressif__button/test_apps/
```

#### Test Files
```
❌ Can Remove - Tests:
├── esp-dl/test/                # Unit tests
└── managed_components/**/test/ # Component tests
```

---

## Essential Files for Deployment

### Minimal Working Project

To create a deployable version, you need:

```
yolo11_detect_minimal/
├── CMakeLists.txt
├── partitions.csv
├── sdkconfig.defaults
├── sdkconfig.defaults.esp32s3
├── main/
│   ├── CMakeLists.txt
│   ├── idf_component.yml
│   ├── app_main.cpp
│   └── bus.jpg
├── coco_detect/
│   ├── CMakeLists.txt
│   ├── Kconfig
│   ├── idf_component.yml
│   ├── coco_detect.cpp
│   ├── coco_detect.hpp
│   └── models/
│       └── s3/
│           └── mymodel.espdl          # Only the model you're using
└── esp-dl/
    ├── CMakeLists.txt
    ├── idf_component.yml
    ├── dl/                             # Core library source
    ├── vision/                         # Vision algorithms
    ├── audio/                          # Audio features
    └── fbs_loader/                     # Model loader
```

**Note**: `managed_components/` will be auto-downloaded when you run `idf.py build`

---

## How to Copy a Working Version

### Method 1: Copy Essential Files (Recommended)

**Step 1: Create Deployment Script**

Save as `copy_deployment.sh` or `copy_deployment.bat`:

```bash
#!/bin/bash
# Copy deployment version

SOURCE_DIR="."
DEST_DIR="../yolo11_detect_deploy"

# Create destination
mkdir -p "$DEST_DIR"

# Copy root files
cp CMakeLists.txt "$DEST_DIR/"
cp partitions.csv "$DEST_DIR/"
cp sdkconfig.defaults "$DEST_DIR/"
cp sdkconfig.defaults.esp32s3 "$DEST_DIR/"

# Copy main application
cp -r main "$DEST_DIR/"

# Copy coco_detect component (only your model)
mkdir -p "$DEST_DIR/coco_detect/models/s3"
cp coco_detect/CMakeLists.txt "$DEST_DIR/coco_detect/"
cp coco_detect/Kconfig "$DEST_DIR/coco_detect/"
cp coco_detect/idf_component.yml "$DEST_DIR/coco_detect/"
cp coco_detect/*.cpp "$DEST_DIR/coco_detect/"
cp coco_detect/*.hpp "$DEST_DIR/coco_detect/"
cp coco_detect/models/s3/mymodel.espdl "$DEST_DIR/coco_detect/models/s3/"

# Copy esp-dl (excluding examples)
rsync -av --exclude='examples' \
          --exclude='applications' \
          --exclude='external_examples' \
          --exclude='test' \
          --exclude='*.md' \
          esp-dl/ "$DEST_DIR/esp-dl/"

echo "Deployment copy created in $DEST_DIR"
```

**Windows PowerShell Version** (`copy_deployment.ps1`):

```powershell
# Copy deployment version
$SOURCE_DIR = "."
$DEST_DIR = "..\yolo11_detect_deploy"

# Create destination
New-Item -ItemType Directory -Force -Path $DEST_DIR

# Copy root files
Copy-Item CMakeLists.txt $DEST_DIR\
Copy-Item partitions.csv $DEST_DIR\
Copy-Item sdkconfig.defaults $DEST_DIR\
Copy-Item sdkconfig.defaults.esp32s3 $DEST_DIR\

# Copy main application
Copy-Item -Recurse main $DEST_DIR\

# Copy coco_detect component
New-Item -ItemType Directory -Force -Path $DEST_DIR\coco_detect\models\s3
Copy-Item coco_detect\CMakeLists.txt $DEST_DIR\coco_detect\
Copy-Item coco_detect\Kconfig $DEST_DIR\coco_detect\
Copy-Item coco_detect\idf_component.yml $DEST_DIR\coco_detect\
Copy-Item coco_detect\*.cpp $DEST_DIR\coco_detect\
Copy-Item coco_detect\*.hpp $DEST_DIR\coco_detect\
Copy-Item coco_detect\models\s3\mymodel.espdl $DEST_DIR\coco_detect\models\s3\

# Copy esp-dl (excluding examples)
robocopy esp-dl $DEST_DIR\esp-dl /E /XD examples applications external_examples test /XF *.md

Write-Host "Deployment copy created in $DEST_DIR"
```

**Step 2: Run the Script**

```bash
# Linux/Mac
bash copy_deployment.sh

# Windows
powershell -ExecutionPolicy Bypass -File copy_deployment.ps1
```

**Step 3: Build in New Location**

```bash
cd ../yolo11_detect_deploy
idf.py set-target esp32s3
idf.py build
```

---

### Method 2: Copy Everything and Clean Later

**Step 1: Full Copy**
```bash
cp -r yolo11_detect yolo11_detect_deploy
cd yolo11_detect_deploy
```

**Step 2: Remove Unnecessary Files**
```bash
# Remove documentation
rm -rf img/ *.md BUILD_GUIDE.md

# Remove examples from esp-dl
rm -rf esp-dl/examples/
rm -rf esp-dl/applications/
rm -rf esp-dl/external_examples/

# Remove managed components (will be re-downloaded)
rm -rf managed_components/

# Remove build artifacts
rm -rf build/
rm -f sdkconfig sdkconfig.old

# Remove unused models (keep only mymodel.espdl)
cd coco_detect/models/s3/
ls | grep -v mymodel.espdl | xargs rm
cd ../../..
```

**Step 3: Rebuild**
```bash
idf.py set-target esp32s3
idf.py build
```

---

## Size Comparison

### Full Project (with examples)
```
Total: ~500-800 MB
├── Source code: ~50 MB
├── Examples: ~100 MB
├── Managed components: ~200 MB
├── Build artifacts: ~150-400 MB
└── Git history: ~50 MB (if cloned)
```

### Minimal Deployment
```
Total: ~50-100 MB
├── Source code: ~30 MB
├── Models: ~15 MB
└── Build artifacts: ~50 MB (after build)
```

### Savings
- **Removing examples**: Saves ~100 MB
- **Removing docs/images**: Saves ~20 MB
- **Single model only**: Saves ~12 MB (4 unused models × 3MB each)

---

## What Gets Compiled

### During Build Process

1. **CMake Scans**:
   - Root `CMakeLists.txt`
   - Component `CMakeLists.txt` (main, coco_detect, esp-dl)
   - Dependency `idf_component.yml` files

2. **Source Files Compiled**:
   - `main/app_main.cpp`
   - `coco_detect/*.cpp`
   - `esp-dl/dl/**/*.c` and `esp-dl/dl/**/*.cpp`
   - `esp-dl/vision/**/*.c`
   - `esp-dl/audio/**/*.c`
   - Managed component sources (auto-downloaded)

3. **Data Embedded**:
   - `main/bus.jpg` (embedded as binary)
   - `coco_detect/models/s3/mymodel.espdl` (selected model only)

4. **NOT Compiled**:
   - Any files in `examples/` directories
   - Any files in `test/` or `test_apps/` directories
   - Markdown files (*.md)
   - Documentation images

---

## Dependency Management

### Component Dependencies

**Main Application** (`main/idf_component.yml`):
```yaml
dependencies:
  espressif/coco_detect: "*"              # Local override
  espressif/esp32-camera: "^2.0.0"        # Auto-download
  espressif/esp32_s3_eye_noglib: "^3.1.0~1"  # Auto-download
```

**COCO Detect** (`coco_detect/idf_component.yml`):
```yaml
dependencies:
  espressif/esp-dl: "~3.2.0"              # Local override
```

### Managed Components

These are **automatically downloaded** during build:
- `espressif__button`
- `espressif__cmake_utilities`
- `espressif__dl_fft`
- `espressif__esp-dsp`
- `espressif__esp32-camera`
- `espressif__esp_codec_dev`
- `espressif__esp_jpeg`
- `espressif__esp_new_jpeg`

**Important**: You can delete `managed_components/` folder. It will be recreated automatically on next build.

---

## Checklist for Deployment Copy

### Before Copying
- [ ] Verify project builds successfully
- [ ] Test on hardware to confirm functionality
- [ ] Note which model you're using (`mymodel.espdl`)
- [ ] Identify any custom modifications

### Essential Files to Copy
- [ ] Root CMakeLists.txt
- [ ] partitions.csv
- [ ] sdkconfig.defaults*
- [ ] main/ folder (complete)
- [ ] coco_detect/ folder (with at least one model)
- [ ] esp-dl/ folder (source code, no examples)

### Optional to Copy
- [ ] README.md (if you want documentation)
- [ ] BUILD_GUIDE.md
- [ ] Model documentation
- [ ] Additional model files (.espdl)

### After Copying
- [ ] Delete `build/` folder
- [ ] Delete `managed_components/` folder
- [ ] Delete `sdkconfig` and `sdkconfig.old`
- [ ] Run `idf.py set-target esp32s3`
- [ ] Run `idf.py build`
- [ ] Verify build succeeds

---

## Quick Reference

### Files That Must Be Copied
```
✓ CMakeLists.txt (root)
✓ partitions.csv
✓ sdkconfig.defaults*
✓ main/ (all files)
✓ coco_detect/ (source + at least 1 model)
✓ esp-dl/ (source only, no examples)
```

### Files That Can Be Deleted
```
❌ */examples/
❌ */test/
❌ */test_apps/
❌ *.md (except critical docs)
❌ img/
❌ managed_components/ (auto-regenerated)
❌ build/ (build artifacts)
```

### Rebuild Commands
```bash
# Clean everything
idf.py fullclean
rm -rf build managed_components sdkconfig*

# Rebuild from scratch
idf.py set-target esp32s3
idf.py build
```

---

## Troubleshooting

### Missing Dependencies
- **Problem**: Build fails with "component not found"
- **Solution**: Delete `managed_components/` and `dependencies.lock`, then rebuild

### Model Not Found
- **Problem**: Build fails with "model file not found"
- **Solution**: Ensure selected model exists in `coco_detect/models/s3/`

### Permission Errors (Windows)
- **Problem**: Cannot copy files
- **Solution**: Use robocopy or PowerShell with admin rights

### Size Too Large
- **Problem**: Deployment copy still very large
- **Solution**: Run `idf.py fullclean` before copying, remove `build/` folder
