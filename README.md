# Gluttonous-Snake on MSPM0G3519

A classic Snake game implemented on the Texas Instruments **MSPM0G3519** microcontroller, featuring an OLED display, a 4×4 matrix keypad for local control, and **UART remote control** via VOFA+ for simultaneous operation, developed as a training project for university electronics competition.

基于德州仪器 **MSPM0G3519** 微控制器实现的经典贪吃蛇游戏，配备 OLED 显示屏、4×4 矩阵键盘本地控制，并通过 **UART** 与 VOFA+ 实现远程控制，支持双控制方式同时操作，为大学生电子设计竞赛培训项目。

---

## 🎮 Features | 功能特性

- **Full Snake Game Logic**  
  Snake movement, food consumption, growing body, boundary collision, and scoring.  
  **完整的贪吃蛇游戏逻辑**：蛇的移动、食物消耗、身体增长、边界碰撞及计分。

- **OLED Graphics**  
  Real‑time display of the snake, food, game field, and UI screens using a OLED .  
  **OLED 图形显示**：通过 OLED 屏幕实时显示蛇、食物、游戏区域及界面。

- **Dual Control Interface**  
  - **Matrix keypad** for local control: direction keys, pause/resume, quit.  
  - **UART command interface** (VOFA+) for remote control: start, pause, direction commands.  
  Both can be used simultaneously – commands from either source are handled in the main loop.  

  **双控制方式**：  
  - **矩阵键盘**本地控制：方向键、暂停/继续、退出。  
  - **UART 串口命令**（VOFA+）远程控制：开始、暂停、方向等命令。  
  两种方式可同时使用，主循环统一处理命令源。

- **Food Mechanics**  
  Food appears at random positions. If not eaten within a configurable timeout, it respawns to prevent stalemate.  
  **食物机制**：食物随机生成；若在设定时间内未被吃掉，则重新生成，防止死局。

- **Scoring & Penalty**  
  Score increases with each food eaten. Hitting the field boundary deducts points (configurable).  
  **计分与惩罚**：每吃一个食物增加分数；碰到边界扣分（可配置）。

- **Pause / Resume**  
  Game can be paused and resumed via keypad or serial command.  
  **暂停/继续**：通过键盘或串口命令均可暂停/恢复游戏。

- **Startup & Menu Screens**  
  Boot screen and a simple choice screen to start the game.  
  **启动与菜单界面**：开机画面及开始游戏的简单选择界面。

- **Hardware Debouncing**  
  Keypad inputs are debounced in software to ensure reliable detection.  
  **硬件消抖**：软件对键盘输入进行消抖处理，保证可靠检测。

---

## 🛠 Hardware | 硬件

| Component  | Details | 说明 |
|------------|---------|------|
| **MCU**    | Texas Instruments MSPM0G3519 | 德州仪器 MSPM0G3519 |
| **Display** | 128×64 OLED  | 128×64 OLED 屏幕|
| **Input**  | 4×4 matrix keypad (8 I/O pins) | 4×4 矩阵键盘，占用 8 个 I/O 引脚 |
| **Remote Control** | UART for communication with VOFA+ or any serial tool | UART，用于与 VOFA+ 或串口工具通信 |

---

## 💻 Software Architecture | 软件架构

The firmware is written in C using the **TI DriverLib** and SysConfig.  
固件使用 **TI DriverLib** 和 SysConfig 编写，采用 C 语言。

### Main Components | 主要模块

| Module            | Description | 说明 |
|-------------------|-------------|------|
| `main.c`          | Initialisation, main loop, keyboard + UART command handling, screen state machine. | 初始化、主循环、键盘+UART 命令处理、屏幕状态机。 |
| `Keyboard.c/h`    | Matrix keypad scanning with software debounce. Returns ASCII characters for pressed keys. | 矩阵键盘扫描及软件消抖，返回按下按键的 ASCII 字符。 |
| `oled_spi_V0.2`   | Low‑level OLED driver: SPI communication, drawing pixels, fonts, and screen refresh. | OLED 底层驱动：SPI 通信、像素绘制、字体、屏幕刷新。 |
| `function.c/h`    | Core game logic: snake movement, collision detection, scoring, food generation, timing. | 游戏核心逻辑：蛇移动、碰撞检测、计分、食物生成、定时。 |
| `interface.c/h`   | UI screens (boot, pause, game over) and menu drawing. | UI 界面（开机、暂停、结束画面）及菜单绘制。 |

### UART Remote Control | UART 远程控制

- **Baud rate**: 115200 (configurable) | **波特率**：115200（可配置）  
- **Data format**: 8N1 | **数据格式**：8N1  
- **Protocol**: Simple single‑byte commands (ASCII). | **协议**：简单的单字节命令（ASCII 字符）。  
- **VOFA+ setup**: Create buttons with the following “Send Content” (plain text, no extra headers).  
  **VOFA+ 配置**：创建按钮，设置“发送内容”为对应 ASCII 字符（纯文本，无需额外帧头）。

| Command | Key Press Equivalent | Action | 对应按键 | 动作 |
|---------|----------------------|--------|----------|------|
| `A`     | `A` (keypad)         | Start game | `A`（键盘） | 开始游戏 |
| `#`     | `#` (keypad)         | Pause/Resume | `#`（键盘） | 暂停/继续 |
| `*`     | `*` (keypad)         | Quit game | `*`（键盘） | 退出游戏 |
| `2`     | `2` (keypad)         | Move up | `2`（键盘） | 向上移动 |
| `8`     | `8` (keypad)         | Move down | `8`（键盘） | 向下移动 |
| `4`     | `4` (keypad)         | Move left | `4`（键盘） | 向左移动 |
| `6`     | `6` (keypad)         | Move right | `6`（键盘） | 向右移动 |

The UART receive interrupt stores the byte in a `volatile` variable. The main loop checks this variable alongside the keypad input, ensuring both control sources work simultaneously.  
UART 接收中断将接收到的字节存入 `volatile` 变量，主循环同时检查该变量和键盘输入，确保两种控制方式同时有效。

---

## 🕹 How to Use | 使用方法

### Local Control (Matrix Keypad) | 本地控制（矩阵键盘）

- **Start game**: Press `A` on the keypad (or `A` button if you have one).  
  **开始游戏**：按下键盘上的 `A` 键。
- **Direction**: `2` = up, `8` = down, `4` = left, `6` = right.  
  **方向**：`2` 上、`8` 下、`4` 左、`6` 右。
- **Pause / Resume**: `#`  
  **暂停/继续**：`#`
- **Quit game**: `*`  
  **退出游戏**：`*`

### Remote Control (VOFA+) | 远程控制（VOFA+）

1. Connect the USB‑TTL adapter to your PC and the board.  
   将 USB‑TTL 适配器连接到电脑和开发板。
2. Open **VOFA+**, select the correct COM port, set baud rate to 115200.  
   打开 **VOFA+**，选择正确的 COM 口，设置波特率为 115200。
3. Create buttons in VOFA+ with the **Send Content** set to the ASCII character corresponding to the desired action (e.g., `A` for start).  
   在 VOFA+ 中创建按钮，将“发送内容”设置为对应动作的 ASCII 字符（例如开始为 `A`）。
4. Click the button to send the command. The game will react instantly.  
   点击按钮发送命令，游戏将立即响应。

> 💡 **Note**: The same command can be sent from either the keypad or VOFA+. The game handles them identically.  
> **注意**：键盘和 VOFA+ 发送的命令格式相同，游戏统一处理。

---

## 📁 File Structure | 文件结构
```
📁 项目根目录
|     
├── Application/                     # 应用层：游戏逻辑与界面
│   ├── function.c                    # 游戏核心逻辑（蛇移动、碰撞检测、计分、食物生成）
│   ├── function.h                    # 游戏核心逻辑函数声明
│   ├── interface.c                   # 用户界面绘制（启动画面、暂停画面、游戏结束画面）
│   └── interface.h                   # 界面函数声明
│
├── 📁 BSP                           # 板级支持包 (Board Support Package)
│   ├── 📄 bsp.h                      # 板级支持头文件，包含全局变量声明等
│   ├── 📄 Keyboard.c                 # 矩阵键盘驱动及扫描实现（含软件消抖）
│   ├── 📄 Keyboard.h                 # 键盘驱动函数声明
│   ├── 📄 oled_spi_V0.2.c            # OLED SPI 驱动底层实现（像素绘制、刷新、字体显示）
│   ├── 📄 oled_spi_V0.2.h            # OLED 驱动函数声明
│   ├── 📄 oledfont.h                 # OLED 字库文件（点阵字体数据）
│   └── 📄 oledpicture_V0.2.h         # OLED 图片数据
│
├── 📁 Project                       # 工程配置文件（IDE 项目文件）
│
├── Source/                   
│   ├── ti/                           # TI SDK (BSD‑3‑Clause) / TI 官方库
│   ├── third_party/                  # Third‑party libraries / 第三方库
|
├── 📁 User                          # 用户代码目录
│   ├── 📄 config.syscfg              # SysConfig 图形化配置文件
│   ├── 📄 Event.dot                  
│   ├── 📄 main.c                     # 主程序入口，初始化、主循环、命令处理
│   ├── 📄 ti_msp_dl_config.c         # SysConfig 生成的驱动库配置文件
│   └── 📄 ti_msp_dl_config.h         # SysConfig 生成的驱动库配置头文件
│
└── 📄 README.md                     
```
## Acknowledgments | 致谢

- Texas Instruments for MSPM0 SDK and SysConfig.
- Keil for MDK‑ARM development environment.
- This project was developed as a training exercise for university electronics competition.  
  本项目为大学生电子设计竞赛培训练习而开发。
