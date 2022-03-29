Neuron Library - Peripheral API Library for ROScube
===================================================

Neuron Library is the API library for ADLINK products, such as ROScube-I and ROScube-X.
Neuron Library provides a common API of C++ bindings to Python to interface with the peripheral IO of the controller. 
With a structured and same API where port index matches the platform that you are on,
it's easier for developers and sensor manufacturers to map their sensors & actuators on top of supported hardware
and to allow control of low level communication protocol by high level languages & constructs.

Supported Hardware
==================

X86
---
* [ADLINK ROScube-I](../roscube_series/docs/adlink_roscube_i.md)

ARM
---
* [ADLINK ROScube-X](../roscube_series/docs/adlink_roscube_x.md)
* [ADLINK ROScube PICO Nano](../roscube_series/docs/adlink_roscube_npn1.md)
* [ADLINK ROScube PICO NX](../roscube_series/docs/adlink_roscube_npn2.md)

Examples
========

See the [examples](../../tree/master/examples) available for supported languages, include C++ and Python.

To use API of Neuron library with ROS 2, please check examples from [here](https://github.com/Adlink-ROS/neuron_library_example)

Build
=====

* Install necessary packages

```bash
# Ubuntu 18.04
sudo apt-get install git build-essential swig3.0 python3-dev nodejs-dev cmake libjson-c-dev
# Ubuntu 20.04
sudo apt-get install git build-essential swig3.0 python3-dev libnode-dev cmake libjson-c-dev
```

* Get the code

```bash
git clone https://github.com/Adlink-ROS/mraa.git
cd mraa
```

* Build the Debian package

```bash
cmake -Bbuild -H. -DBUILDSWIGNODE=OFF -DCMAKE_INSTALL_PREFIX=/opt/adlink/neuron-sdk/neuron-library/
cmake --build build --target package
```

The deb package will be under /build.

API Documentation
=================

<a href="http://c.mraa.io"><img src="http://iotdk.intel.com/misc/logos/c++.png"/></a>
<a href="http://py.mraa.io"><img src="http://iotdk.intel.com/misc/logos/python.png"/></a>

Contact Us
==========

To ask questions either file issues in github or send emails to Adlink service@adlinktech.com . 

Changelog
=========

Version changelog [here](ADLINK_CHANGELOG).
