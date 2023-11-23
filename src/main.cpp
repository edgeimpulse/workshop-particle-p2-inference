/*
 * Copyright (c) 2023 EdgeImpulse Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an "AS
 * IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Include ----------------------------------------------------------------- */
#include "Particle.h"
#include "ADXL362DMA.h"

// This must come after Particle.h
#include <particle-p2-gestures_inferencing.h>

SYSTEM_THREAD(ENABLED);

SerialLogHandler logHandler(LOG_LEVEL_ERROR);

/* Forward declerations ---------------------------------------------------- */
int raw_feature_get_data(size_t offset, size_t length, float *out_ptr);
void setup();
void loop();

// Global variables
// static const float features[] = {   
//     // Expected: [0, 0.35, 0.61, 0.04]
//     -0.9194, -0.0383, 16.7690, -1.3503, -0.0958, 18.7610, -1.0151, 0.2298, 15.2463, -2.1835, 1.4365, 19.6037, -1.0439, -1.0918, 17.1712, -1.7047, 7.7764, 66.4630, -2.0590, 1.3120, 17.5447, -2.1548, 0.9481, 18.0044, -1.6472, -0.0766, 15.2846, -1.4653, -0.9194, 13.9726, -1.6568, -1.4557, 13.7044, -1.8100, -1.3503, 13.8576, -1.5706, -1.1588, 12.1434, -3.6679, -2.4612, 21.1264, -1.1396, -0.8619, 10.1993, -1.1684, -0.5459, 10.1514, -1.0151, 0.1628, 8.9447, -0.8619, 0.6033, 7.7189, -0.5842, 0.8045, 6.6942, -2.8826, 3.1412, 22.1799, -2.6719, 0.0575, 10.8409, 0.0766, 0.0575, 9.9599, 0.0287, -0.0383, 4.5586, -0.1245, -0.1628, 4.2138, -0.6033, -0.0670, 5.0566, -1.1492, 0.0575, 5.9376, -1.5227, 0.1437, 11.6071, -4.7405, 2.5666, 12.0093, -1.6089, -0.2777, 7.1635, -1.5993, -0.6129, 8.0349, -1.2641, -0.9577, 8.1786, -1.3503, -0.6512, 22.1224, -1.4557, -0.1532, 11.2911, -1.2450, -0.2490, 11.5592, -1.2258, -0.4980, 23.0418, -1.1684, -0.4310, 12.4211, -1.3024, -0.2011, 13.2543, -1.1588, 0.1437, 14.2407, -1.1971, -0.2203, 14.7387, -1.1684, 0.1341, 30.9139, -1.0151, 0.2969, 15.1314, 1.0726, 12.0285, 254.8197, -1.8483, 0.6991, 17.6979, -2.3176, 2.0207, 19.6037, -2.1835, 1.9058, 19.0291, -1.9154, 1.5897, 18.2917, -1.7526, 1.6185, 17.0371, -1.3982, 0.9002, 30.1478, -1.6568, 0.1532, 16.0028, -1.9154, -0.0766, 16.4242, -1.8292, -0.5459, 15.5432, -1.5323, -0.7374, 25.0338, -1.1684, -0.6129, 9.6055, -1.1205, 0.0000, 6.5122, -0.8428, -0.4693, 4.2808, -0.2969, -2.2984, 1.1492, 0.2969, -3.0358, -1.6568, -0.2394, -1.1205, -1.7717, -1.2929, 0.6800, 1.5131, -1.6951, 0.3065, 3.8595, -1.9154, -0.0766, 13.5991, -1.9441, -2.4804, 3.6775
// };
static float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
static ADXL362DMA accel(SPI, D13); // A2 is D13, but is undefined in <project>_inferencing.h

/**
 * @brief      Copy raw feature data in out_ptr
 *             Function called by inference library
 *
 * @param[in]  offset   The offset
 * @param[in]  length   The length
 * @param      out_ptr  The out pointer
 *
 * @return     0
 */
int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, features + offset, length * sizeof(float));
    return 0;
}

void print_inference_result(ei_impulse_result_t result);


/**
 * @brief      Particle setup function
 */
void setup()
{
    // put your setup code here, to run once:

    // Wait for serial to make it easier to see the serial logs at startup.
    waitFor(Serial.isConnected, 15000);
    delay(2000);

    ei_printf("Edge Impulse standalone inferencing (P2) example\n");

    // Init the accelerometer
    accel.softReset();
    while(accel.readStatus() == 0) {
        ei_printf("Waiting for accelerometer...\r\n");
        delay(1000);
    }

    // Set accelerometer range and measure mode
    accel.writeFilterControl(ADXL362DMA::RANGE_2G, false, false, ADXL362DMA::ODR_200);
    accel.setMeasureMode(true);
}

/**
 * @brief      Particle main function
 */
void loop()
{
    // Read the accelerometer
    ei_printf("Sampling...\r\n");
    for (int i = 0; i < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; i += 3) {

        // Read the accelerometer
        int16_t x, y, z;
        accel.readXYZ(x, y, z);

        // Convert Gs to M/s^2 and save to feature array
        features[i] = (((float)x * 2) / 2048.0) * 9.80665;
        features[i + 1] = (((float)y * 2) / 2048.0) * 9.80665;
        features[i + 2] = (((float)z * 2) / 2048.0) * 9.80665;

        // Wait for 16 ms (62.5 Hz) before reading again
        delay(16);
    }

    ei_printf("Doing inference...\r\n");

    if (sizeof(features) / sizeof(float) != EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
        ei_printf("The size of your 'features' array is not correct. Expected %lu items, but had %lu\n",
            EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, sizeof(features) / sizeof(float));
        delay(1000);
        return;
    }

    ei_impulse_result_t result = { 0 };

    // the features are stored into flash, and we don't want to load everything into RAM
    signal_t features_signal;
    features_signal.total_length = sizeof(features) / sizeof(features[0]);
    features_signal.get_data = &raw_feature_get_data;

    // invoke the impulse
    EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, false);
    if (res != EI_IMPULSE_OK) {
        ei_printf("ERR: Failed to run classifier (%d)\n", res);
        return;
    }

    // print inference return code
    ei_printf("run_classifier returned: %d\r\n", res);
    print_inference_result(result);

    delay(1000);
}

void print_inference_result(ei_impulse_result_t result) {

    // Print how long it took to perform inference
    ei_printf("Timing: DSP %d ms, inference %d ms, anomaly %d ms\r\n",
            result.timing.dsp,
            result.timing.classification,
            result.timing.anomaly);

    // Print the prediction results (object detection)
#if EI_CLASSIFIER_OBJECT_DETECTION == 1
    ei_printf("Object detection bounding boxes:\r\n");
    for (uint32_t i = 0; i < result.bounding_boxes_count; i++) {
        ei_impulse_result_bounding_box_t bb = result.bounding_boxes[i];
        if (bb.value == 0) {
            continue;
        }
        ei_printf("  %s (%f) [ x: %u, y: %u, width: %u, height: %u ]\r\n",
                bb.label,
                bb.value,
                bb.x,
                bb.y,
                bb.width,
                bb.height);
    }

    // Print the prediction results (classification)
#else
    ei_printf("Predictions:\r\n");
    for (uint16_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        ei_printf("  %s: ", ei_classifier_inferencing_categories[i]);
        ei_printf("%.5f\r\n", result.classification[i].value);
    }
#endif

    // Print anomaly result (if it exists)
#if EI_CLASSIFIER_HAS_ANOMALY == 1
    ei_printf("Anomaly prediction: %.3f\r\n", result.anomaly);
#endif

}
