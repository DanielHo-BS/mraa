# Neuron Library Examples

Here are some C examples to run Neuron Library.

# GPIO

`./gpio <pin1> <pin2>` can toggle pin1 and pin2 repeatedly.

```bash
# Toggle pin 34 and 35, which is the DO0 and DO1 in ROScube-I
./gpio 34 35
```

# LED

`./led <LED number> <turn on/off>` can toggle the LED.

```bash
# Turn on the LED 0
./led 0 1
# Turn off the LED 0
./led 0 0
```

# UART

`./uart <COM port>` can send data to COM port. Default settings is 9600, 8N1.

```bash
# Connect COM 0 and COM 1. Then open COM 1 with 9600, 8N1.
./uart 0
# You'll see data on COM 1
```

# I2C

`./i2c_example <I2C_BUS> <I2C_DEV_ADDR>` can set the value to the registers of I2C_DEV_ADDR on I2C_BUS.

Note that the I2C_BUS does not match `i2c-n` while using `i2cdetect -l`.
I2C_BUS means the supported I2C bus on ROScube platform, which starts from 0.

```bash
# It'll set all the registers of the device which is located on bus 0 and address 0x50
./i2c_example 0 0x50
```