/**
 ******************************************************************************
 * @file    WriteURIMail.ino
 * @author  wi6labs
 * @version V1.0.0
 * @date    6 July 2017
 * @brief   Arduino test application for the STMicrolectronics
 *          STM32L475V-DISCOVERY-IOT. NFC tag based on M24SR device.
 *          This application makes use of C++ classes obtained from the C
 *          components' drivers.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/**
 ******************************************************************************
* How to use this sketch
*
* This sketch uses the I2C interface to communicate with the NFC device.
* It writes an NFC tag type Mail and reads this same tag.
* Check the pinout and the I2C interface used for communication.
* Choose the recipient, the subject and the body of the mail by changing
* the content of uri_add, uri_subject and uri_body.
*
* When the NFC module is started and ready, the message "Sytstem init done!" is
* displayed on the monitor window. Next, the tag is written, read and printed on
* the monitor window.
*
* You can also use your smartphone to read/write a tag.
* On Android, donwload a NFC Tools. Then start the app, check if NFC is activated
* on your smartphone. Put your smartphone near the tag, you can read it. You can
* write a tag with this app.
 ******************************************************************************
 */

#include <Wire.h>
#include <M24SR.h>

/*  This configuration is dedicated to the board Disco L475VG IoT.
    You should update those parameters according to the board tested.
*/
#define SerialPort      Serial
#define I2C2_SCL        PB10
#define I2C2_SDA        PB11
#define M24SR_ADDR      0xAC
#define GPO_PIN         PE4
#define RF_DISABLE_PIN  PE2

TwoWire dev_i2c(I2C2_SDA, I2C2_SCL);
M24SR nfcTag(M24SR_ADDR, &dev_i2c, NULL, GPO_PIN, RF_DISABLE_PIN);

/**
 * Write a Ndef URI mail message and than read the message from
 * the Nfc tag.
 *
 */
void setup() {
  const char uri_add[]     = "test@test.com"; // recipient of the mail
  const char uri_subject[] = "nfc mail";      // subject of the mail
  const char uri_body[]    = "Hello world";   // body of the mail
  char uri_read_add[255] = {'\0'};            // read address
  char uri_read_subject[255] = {'\0'};        // read subject
  char uri_read_body[255] = {'\0'};           // read body

  // Initialize serial for output.
  SerialPort.begin(9600);

  // Initialize I2C bus.
  dev_i2c.begin();

  // Intialize NFC module
  if(nfcTag.begin(NULL) == 0) {
    SerialPort.println("System Init done!");
  } else {
    SerialPort.println("System Init failed!");
    while(1);
  }

  delay(100);

  if(nfcTag.writeURIMail(uri_add, uri_subject, uri_body) == false) {
    SerialPort.println("Write failed!");
    while(1);
  }

  delay(100);

  //read the txt message and print it
  nfcTag.readURIMail(uri_read_add, uri_read_subject, uri_read_body);
  SerialPort.print("Recipient: ");
  SerialPort.println(uri_read_add);
  SerialPort.print("Subject: ");
  SerialPort.println(uri_read_subject);
  SerialPort.print("Body: ");
  SerialPort.println(uri_read_body);

  if((strcmp(uri_add, uri_read_add) == 0) &&
    (strcmp(uri_subject, uri_read_subject) == 0) &&
    (strcmp(uri_body, uri_read_body) == 0)) {
    SerialPort.println("Successfully written and read!");
  } else {
    SerialPort.println("Read failed!");
  }
}

void loop() {
  //empty loop
}
