/*
Copyright 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.

 Licensed under the Apache License, Version 2.0 (the "License").
 You may not use this file except in compliance with the License.
 A copy of the License is located at

     http://www.apache.org/licenses/LICENSE-2.0

 or in the "license" file accompanying this file. This file is distributed
 on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 express or implied. See the License for the specific language governing
 permissions and limitations under the License.
*/

#include <WiFi.h>
#include <AWSGreenGrassIoT.h>

#define  PIN_R 4
#define  PIN_G 13
#define  PIN_B 15


/*
 * PEM-encoded client certificate.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----\n"\
 * "...base64 data...\n"\
 * "-----END CERTIFICATE-----"
 */
#define keyCLIENT_CERTIFICATE_PEM \
"-----BEGIN CERTIFICATE-----\n"\
"MIIDWTCCAkGgA---------------------------------------------------\n"\
"BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n"\
"SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIwMDYxOTE1MDky\n"\
"MloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n"\
"ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAOcp6waMzHs0i31SMNUF\n"\
"ztRurYtAsGQ2BOgb8Kq5tNnGbvyh/0Em5eMfn9nVvHE8at/yRvpIuMTrahW1uB6D\n"\
"FbChZdhmi5Iud5cXXs0c3jgj6zcynZT3kmDMCS1feOAt3kl4D5rYxPQCDdTlB9Iv\n"\
"VR0OBBYEFIThhGI5e61eC7zyN4yV8U/N2SSaMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n"\
"AQH/BAQDAgeAMA0GCSqGSIb3DQEBsadsadasdsadasdsadase2KHs8eU3iftSPML\n"\
"YLJTUvwFURSS/Q2o88xpZ/kYnG5zsAL1sJ2llGCD3N1QPyFUfo3pYXyKWIEiZmiQ\n"\
"y/hUDoH2ZuKHeMCvv+TNKVvsCPfbZHYDkAXL1KdDcRtpA9AqD9LsZBStG4ZhhczV\n"\
"MLjkkjQgQfA7YN0t+NVBRc8TdcKsOr4x2YyG/TyMLoJCCP9KelW6Q5AIac0XywR/\n"\
"Di9Z664bZ60WfhBmuYDHlM6hA5EK4f4kcr96RQKlqHBUdlxgAC+PyUHiARUR0Mdt\n"\
"RTbu7PDcdmZl+u9dRj3IvBL8i+Wu1i1JwbQjlox4cV835SXaJuf+J9u3Ocp3\n"\
"-----END CERTIFICATE-----"


/*
 * PEM-encoded client private key.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN RSA PRIVATE KEY-----\n"\
 * "...base64 data...\n"\
 * "-----END RSA PRIVATE KEY-----"
 */
#define keyCLIENT_PRIVATE_KEY_PEM \
"-----BEGIN RSA PRIVATE KEY-----\n"\
"MIIEpQIBA-------------------------------------------------------\n"\
"QSbl4x+f2dW8cTxq3/JG+ki4xOtqFbW4HoMVsKFl2GaLki53lxdezRzeOCPrNzKd\n"\
"lPeSYMwJLV944C3eSXgPmtjE9AIN1OUH0i/q4Cgvp9wyKYWkhMb5w6cxuWfqaAgA\n"\
"yGUIiKfgFc3IiazEWbsd1sR6RPDhBOLvqgrfmSwt2KrO7vK+E6rDxi1rh2aAoKQN\n"\
"p2Rus91uTZcimysJa15F6DJVGYqgItsaVo8ZPFjxYqvRvRXtgAZWJz0xSppN5TD7\n"\
"SGJVCG+9VVEygtQwKFGWBg9l8b+EfVnC0SKs4XFrzP1r578wPhbK33hkIQhdEtBm\n"\
"yEimgnpSqwQKJnbotNKETfhaoZEKfuvZ4r4WcTn29nJ2UVtKu3b8l/SNAoGBAPCj\n"\
"0Jo6r0p9CHExxVUsgfxDd3rhxDnbN0uPkE2UtGOkUqB+kVYce5i3XHnq9AQGFY95\n"\
"Ltb8P4xziW3OvbkIaKmlOoGU+vzZvVIw+tlnK2Giv2FTyAXLYiORIa82iZCM94WK\n"\
"z7njq0ccQFcTtgZPHeb5dHQZkpbr1T5TJAOJHkqfAoGAbwGeCoP8lhWpYjp8HNL4\n"\
"9ciCIJg3Pw9EOXJCgpgpeF8tckXZwrdNW03E/CN78qYZVkzStllBLQw4ftoAd1cw\n"\
"LAyOHSOzrmLNaZnJrCezkECUu/TsTXE619A9QvAtPRobFaz+Vxl/+wpWnReoL0pA\n"\
"YVEuT/fT5XQs7CEIalxQ1Z9QUUX76ANLcxj4L1SNXeQoVp0pK7s3RWo0RlvnqK1A\n"\
"yWalYJECgYEAqTKfiFpuG82JrrnZe8dpkPasdsaGzwYODm8u3TN/jXw6bH7/3FXw\n"\
"BQXxDUQELTbbKa56D9rYjwiBTjKkqjZb/gFP0BaFzTjvgKcsKx4prwtfl9XbmvGQ\n"\
"IzMlLc5ZeTFeEUBtWZQtH+XOJY+wbmgLjRjdQXq0R1NAotQh3/P5Omk=\n"\
"-----END RSA PRIVATE KEY-----"



enum command {
  CMD_OPEN,
  CMD_CLOSE,
  CMD_UNKNOWN
};

extern const char aws_root_ca[];
//extern const char thingCA[];
//extern const char thingKey[];

char WIFI_SSID[]="_SSID_";
char WIFI_PASSWORD[]="_PASSWORD_";
char AWSIOTURL[]="_AWS_IOT_ENDPOINT_";
char THING[]= "_THING_NAME_";
char TOPIC_NAME[]= "MB3/kiosk/alert";

AWSGreenGrassIoT  * greengrass;

int status = WL_IDLE_STATUS;
int msgReceived = 0;
char payload[512];
bool alert_state = false;

static void subscribeCallback (int topicNameLen, char *topicName, int payloadLen, char *payLoad)
{

    Serial.println("Alert received");
    alert_state = true;
}


void setup() {
    Serial.begin(115200);
    pinMode(PIN_R, OUTPUT);
    pinMode(PIN_G, OUTPUT);
    pinMode(PIN_B, OUTPUT);
    
    while (status != WL_CONNECTED)
    {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(WIFI_SSID);
        // Connect to WPA/WPA2 network. 
        status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

        // wait 3 seconds for connection:
        delay(3000);

    }

    Serial.println("Connected to wifi");

    greengrass = new AWSGreenGrassIoT(AWSIOTURL, THING, aws_root_ca, keyCLIENT_CERTIFICATE_PEM, keyCLIENT_PRIVATE_KEY_PEM);

   if(greengrass->connectToGG() == true)
    {
        Serial.println("Connected to AWS GreenGrass");
        delay(2000);

       if( true == greengrass->subscribe(TOPIC_NAME,subscribeCallback)) {
            Serial.println("Subscribe to topic successful ");
       }
       else {
            Serial.println("Subscribe to Window/# Failed, Check the Thing Name and Certificates");
            while(1);
       }

     }
    else
    {
        Serial.println("Connection to Greengrass failed, check if Greengrass is on and connected to the WiFi");
        while(1);
    }

}



void loop() {
    if(alert_state) {
        digitalWrite(PIN_R, HIGH);
        alert_state = false;
        delay(3000);
        digitalWrite(PIN_R, LOW);
    }
    
}
