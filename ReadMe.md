# 智能车控制系统项目 (STC32G12K128)

[![Language](https://img.shields.io/badge/Language-C251-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform](https://img.shields.io/badge/Platform-STC32G-orange.svg)](http://www.stcmcudata.com/)
[![MDK](https://img.shields.io/badge/Keil-MDK5.60-green.svg)](https://www.keil.com/)

这是一个基于 **STC32G12K128** 高性能单片机开发的智能车控制系统，面向第十八届全国大学生智能车竞赛(for First Prize of the Competition Area)。项目实现了多传感器融合的姿态感知、精确的 PID 控制算法以及复杂的赛道元素（环岛、坡道、障碍和入库）识别逻辑。

---

## 🌟 核心特性

- **多传感器融合**: 集成电感采集、编码器反馈、TOF 激光测距、陀螺仪 (ICM20602/MPU6050) 等。
- **先进控制算法**:
  - **转向控制**: 基于电感偏差的 PD 控制，引入模糊 PID 逻辑。
  - **速度控制**: 增量式 PID 闭环控制。
- **复杂元素识别**: 完美支持 **直道、弯道、坡道、环岛、障碍物、入库** 等全场景识别。
- **交互系统**: 完整的 UI 菜单系统，支持实时参数调整与 EEPROM 持久化存储。

## 🛠 硬件架构

| 组件 | 型号/描述 |
| :--- | :--- |
| **主控** | STC32G12K128 (35MHz IRC) |
| **测距** | TOF DL1A / DL1B |
| **姿态** | ICM20602 / MPU6050 / IMU660RA |
| **显示** | IPS114 SPI 屏 / 0.96 OLED |

## 🚀 快速开始

### 1. 环境准备
- 安装 **Keil MDK for C251** (推荐 V5.60 及以上版本)。
- 准备 **STC-ISP** 下载工具。

### 2. 编译工程
- 打开 `MDK/SEEKFREE.uvproj`。
- 点击 `Rebuild All` 进行编译。
- 编译产物位于 `MDK/Out_File/STC32.hex`。

### 3. 下载程序
- 打开 STC-ISP，选择芯片型号为 `STC32G12K128`。
- **注意**: 在下载设置中将 `IRC频率` 设置为 **35.000MHz**（需与 `main.c` 中的 `sys_clk` 匹配）。

## 📂 项目结构

```text
.
├── MDK/                # Keil 项目工程与驱动层
│   ├── Out_File/       # 编译输出目录
│   └── element.c       # 赛道元素处理逻辑
├── USER/               # 用户应用层
│   ├── src/            # 核心控制逻辑 (main.c, isr.c)
│   └── inc/            # 头文件定义
└── README.md
```

## 🤝 贡献与反馈
如果你在运行或学习过程中遇到问题，欢迎提交 [Issues](https://github.com/your-username/your-repo/issues) 或通过邮件联系。

---
*© L_37 基于逐飞科技开源库构建。*