# Example of live gesture inference with Particle Photon 2

This Particle Workbench project demonstrates how to perform live inference with an accelerometer using the Particle Photon 2.

## Hardware connections

You will need the following:
 * [ADXL362 accelerometer breakout board](https://www.adafruit.com/product/1018)
 * [Particle Photon 2](https://store.particle.io/products/photon-2)
 * Solderless breadboard
 * Jumper wires
 * USB micro-B cable

Note that you can also purchase the [Particle Edge ML kit](https://docs.particle.io/reference/datasheets/accessories/edge-ml-kit/).

Follow the directions [on this Edge Impulse tutorial](https://docs.edgeimpulse.com/docs/development-platforms/officially-supported-mcu-targets/particle-photon-2) to connect the ADXL362 accelerometer to the P2. The [P2 pinout diagram can be found here](https://docs.particle.io/reference/datasheets/wi-fi/photon-2-datasheet/#pin-markings).

| ADXL362   | Photon 2   |
| ---------- | --------- |
| VIN        | 3V3       |
| GND        | GND       |
| SCL (SCK)  | SCK (D17) |
| SDA (MOSI) | MO (D15)  |
| SDO (MISO) | MI (D16)  |
| CS         | A2 (D13)  |
| INT1       | Not connected |
| INT2       | Not connected |

## Repository contents

The source code for this example can be found in the *src/* directory. You probably want to look at *src/main.cpp*. This contains the application demo for performing live inference by reading from the ADXL362 accelerometer.

Note that this repository has a pre-trained model that has been deployed from Edge Impulse. The model was trained by using a small dataset of continuous gestures. You can clone and deploy the model [from this public Edge Impulse project](https://studio.edgeimpulse.com/public/311665/latest).

There is also a *static buffer* example found in *examples/static_buffer/main.cpp*. Feel free to look at that demo to see how you might test inference without connecting the accelerometer.

## Deploy your own model

If you wish to deploy your own model, deploy with the *Particle* target from the *Deployment* page in Edge Impulse. Alternatively, you can deploy with the *C++ library* option and copy the following folders to the *src/* directory in this project:

 * *edge-impulse-sdk/*
 * *model-parmaeters/*
 * *tflite-model/*

When deployed, these folders contain your trained model and all the necessary code to run inference.

## Build and flash

Install the Particle Workbench by [following these instructions](https://docs.particle.io/workbench/). We recommend simply installing the [VS Code extension](https://docs.particle.io/quickstart/workbench/#workbench-extension-installation).

Once installed, click the Particle Workshop extension on the left-side toolbar of VS Code. Alternatively, you can press *ctrl + shift + p* (Win/Linux) or *cmd + shfit + p* (Mac) to bring up the command palette and enter `>Particle:` to access the various Workbench commands.

1. In Workbench, select **Particle: Import Project** and select the `project.properties` file in the directory that you just downloaded and extracted.

2. Use **Particle: Configure Project for Device** and select **deviceOS@5.3.2** and choose a target. (e.g. **P2** , this option is also used for the Photon 2).

3. Compile with  **Particle: Compile application (local)**

4. Flash with **Particle: Flash application (local)**

5. Open the serial monitor with **Particle: Serial Monitor** to view the inference output

> **Important!** At this time you cannot use the **Particle: Cloud Compile** or **Particle: Cloud Flash** options; local compilation is required.

## License

Unless otherwise noted, all code is released under the Apache 2.0 license.

Copyright (c) 2023 EdgeImpulse Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an "AS
IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
express or implied. See the License for the specific language
governing permissions and limitations under the License.

SPDX-License-Identifier: Apache-2.0
