# STM32F103VET6开发板测试程序

开发板使用手册请查看 [STM32F103VET6 开发板使用手册](./STM32F103VET6开发板使用手册.md)

本测试程序用于验证开发板（除左右两大排针外的）板载接口功能。各外设的 CubeMX 配置已生成在 `Core/`，测试代码集中在 `User/` 目录。

## 硬件与环境

| 项目 | 说明 |
| --- | --- |
| 主控 | STM32F103VET6（LQFP100，72 MHz） |
| 时钟 | HSE 8 MHz；LSE 32.768 kHz（RTC） |
| 工具链 | Keil MDK-ARM，工程见 `MDK-ARM/` |
| 配置工具 | STM32CubeMX，工程见 `stm32f103vet6_test.ioc` |
| 调试 / 下载 | SWD（CN2，ST-Link / J-Link）或串口 ISP（USB1，板载 CH340） |
| 串口参数 | 115200-8-N-1 |

## 目录结构

```
Core/        CubeMX 生成的外设初始化与主程序
User/        各外设测试代码（本程序重点）
  Inc/       *_test.h
  Src/       *_test.c
Drivers/     HAL 库与 CMSIS
USB_DEVICE/  USB CDC 设备代码
MDK-ARM/     Keil 工程与启动文件
```

## 编译与烧录

1. 用 Keil MDK 打开 `MDK-ARM/stm32f103vet6_test.uvprojx`，编译。
2. 烧录方式二选一：
   - **SWD**：ST-Link / J-Link 接 CN2（`GND / SWCLK / SWDIO / VCC+5V`），在 Keil 中选择 ST-Link、SWD 模式下载。
   - **串口 ISP**：USB 线接 USB1，用 STM32CubeProgrammer / FlyMcu 选择对应串口下载（板载 CH340 自动控制 BOOT0 / RST）。
3. 上电后确认 +5V 与 +3.3V 指示灯点亮。

## 测试项

测试在 `main()` 中初始化，`while(1)` 中周期性执行。当前实现情况：

| 模块 | 引脚 | 测试方式 | 现象 | 状态 |
| --- | --- | --- | --- | --- |
| 按键 + LED | KEY1/2/3 = PE13/PE14/PE15；红=PB6、绿=PB7、蓝=PD10 | 按键触发 EXTI，回调中翻转对应 LED | 按 KEY1/2/3 分别翻转红/绿/蓝灯 | 已实现 |
| 串口回显 | USART2 PA2/PA3、USART3 PD8/PD9、UART4 PC10/PC11 | DMA 接收转空闲（Receive-To-Idle），收到即原样发回 | 串口助手发送的数据被回显 | 已实现 |
| ADC | PA0(IN0)、PA1(IN1)，SMA 座 | ADC1 扫描 2 通道 + DMA 循环，每秒打印原始值与电压 | USART1 每秒输出 `PA0=... (..mV)  PA1=...` | 已实现 |
| RTC | LSE 32.768 kHz + CR1220 | 每秒读取时间 / 日期并打印；用备份寄存器实现掉电保持 | USART1 秒级递增；断电再上电时间续走 | 已实现 |
| DAC | PA4(DAC_OUT1)、PA5(DAC_OUT2)，SMA 座 | 开机固定输出直流，可用万用表量 SMA 座 | PA4 ≈ 2.0 V，PA5 ≈ 1.0 V | 已实现 |
| I2C2 | PB10(SCL)/PB11(SDA)，CN4 | CubeMX 已配置（100 kHz），未写测试代码 | - | 待补充 |
| SPI1 | PA15(CS)/PB3(SCK)/PB4(MISO)/PB5(MOSI)，U9 | CubeMX 已配置（主机模式，已关 JTAG 保留 SWD） | - | 待补充 |
| USB CDC | USB2（PA11/PA12） | 虚拟串口（VCP）回显：PC 发送的数据原样返回 | PC 端虚拟串口回显 | 已实现 |
| SWD | PA13(SWDIO)/PA14(SWCLK)，CN2 | 用调试器连接 | 可下载 / 调试 | 可用 |

> 目前**按键 / LED、串口、ADC、RTC、DAC、USB(CDC)** 有测试代码；**I2C2 / SPI1** 已完成 CubeMX 配置，测试代码待补充。

## 使用方法

### 按键 / LED

无需额外初始化，上电即生效：

- KEY1 → 红 LED，KEY2 → 绿 LED，KEY3 → 蓝 LED；
- 用户 LED 为低电平点亮。

### 串口回显

- **USART2 / USART3 / UART4**：为外接排针（CN6 / CN3 / CN5），需自备 USB-TTL 模块。
  - 注意 **USART3 使用完全重映射 PD8/PD9**。
- 打开串口（115200-8-N-1），发送任意数据即被原样返回。
- **USART1**（板载 CH340，USB1）已**关闭回显**，专门用于 ADC / RTC 的日志输出。
  如需恢复 USART1 回显，取消 `User/Src/uart_test.c` 中 `UartTest_Init()` 里那行的注释即可。

### ADC

把信号源（0 ~ VREF+）接到 **PA0 / PA1 的 SMA 座**（U5 / U6）。串口每秒打印一次原始值与电压值。

> 电压换算基于 `VREF+`，本板 VREF+ 由 TL431 精密基准提供。**使用前请实测 VREF+ 电压，并修改**
> `User/Src/adc_test.c` 中的 `ADC_TEST_VREF_MV`，否则电压值不准。

### DAC

上电后在 `Dac_Init()` 中启动 DAC 两个通道并输出固定直流，用万用表量 **PA4 / PA5 的 SMA 座**（U7 / U8）：

- DAC_OUT1(PA4) ≈ 2.0 V，DAC_OUT2(PA5) ≈ 1.0 V。

> 输出电压同样与 `VREF+` 有关（`dac_test.c` 按 3.3 V 计算）；VREF+ 变了实际电压会等比例变化。

### RTC

串口每秒打印日期时间。若已装 CR1220，断电后时间继续走（靠备份寄存器 `RTC_BKP_DR1` 判断是否首次初始化）。

### USB CDC

- 用 USB 线把 **USB2**（MCU 直连口，PA11/PA12）接到电脑；板子供电仍走 DC 或 USB1。
- PC 端会枚举出一个虚拟串口（VID/PID `0483:5740`）；Windows 未自动识别时需装 **ST VCP 驱动**。
- 打开该串口（波特率任意，CDC 不看波特率），发送任意数据即被原样返回。
- 注意：**USB1 是板载 CH340，USB CDC 测试必须用 USB2**。

### 打开 / 关闭某个测试

在 `Core/Src/main.c` 中：

- `USER CODE BEGIN 2` 段：注释 / 放开对应的 `XxxTest_Init()`；
- `while(1)` 中：注释 / 放开对应的 `XxxTest_Print()`。

## 注意事项

1. **USART1 专用于日志**：USART1 的回显已关闭，仅用于 ADC / RTC 的打印输出；回显测试改在 USART2/3/4 上进行，二者互不干扰。打印函数内部仍会判断 TX 是否忙，占用时返回 `HAL_BUSY` 并跳过，不会阻塞。
2. **按键触发边沿**：`Core/Src/gpio.c` 当前配置为 `GPIO_MODE_IT_RISING`（上升沿）。按键为内部上拉、按下为低电平，因此目前是**松开时触发**；如需“按下触发”，应改为 `GPIO_MODE_IT_FALLING`。
3. **ADC 参考电压**：务必按实测 VREF+ 修改，否则电压换算不准。
4. **RTC 首次初始化**：备份域无电时，时间从 2000-01-01 00:00:00 开始。
5. **掉电保持**：`rtc.c` 中备份寄存器的判断位于 USER CODE 段，重新生成 CubeMX 代码不会丢失。

## 后续计划

- 补充 I2C2、SPI1 的测试代码。

## 许可证

本项目基于 [GNU General Public License v3.0](./LICENSE) 发布。
