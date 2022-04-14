# Neuron Library Examples

Here are some Python examples to run Neuron Library.

# GPIO

`python3 gpio.py <pin1> <pin2>` can toggle pin1 and pin2 repeatedly.

```bash
# Toggle pin 34 and 35, which is the DO0 and DO1 in ROScube-I
python3 gpio.py 34 35
```

# LED

`python3 led.py <LED number> <turn on/off>` can toggle the LED.

```bash
# Turn on the LED 0
python3 led.py 0 1
# Turn off the LED 0
python3 led.py 0 0
```

# UART

`python3 uart.py <COM port>` can send data to COM port. Default settings is 9600, 8N1.

```bash
# Connect COM 0 and COM 1. Then open COM 1 with 9600, 8N1.
python3 uart.py 0
# You'll see data on COM 1
```

# I2C

`python3 i2c.py <I2C_BUS> <I2C_DEV_ADDR>` can set the value to the registers of I2C_DEV_ADDR on I2C_BUS.

Note that the I2C_BUS does not match `i2c-n` while using `i2cdetect -l`.
I2C_BUS means the supported I2C bus on ROScube platform, which starts from 0.

```bash
# It'll set all the registers of the device which is located on bus 0 and address 0x50
python3 i2c.py 0 0x50
```

# PWM

`python3 pwm.py <PWM_PIN> <PWM_Period>` can set the value to period of pwm.

```bash
# For example, your pin of pwm is 22, and you want to set the period of pwm to 200us
python3 pwm.py 22 200
# To exit, using ctrl + c
```

# SPI

`python3 spi.py <SPI_Num> <SPI_Speed>` can set the value to the SPI.

Take MAX7219 chip with LED Matrix as example.  
Display set of patterns on MAX7219 repeately.

Note that we are using `/dev/spidev0.0`.

```bash
# For example, your spi_device is 0, and you want to set the speed to 100000 hz.
python3 spi.py 0 100000
# Press Ctrl+C to exit
```

# GPIO ISR
`python3 spi.py <GPIO_PIN>` can triggers ISR upon GPIO state change.

```bash
# For example, configure Pin 5 for interruption.
python3 gpio_advanced.py 5.
# Press ENTER to stop
```