#
# This Lambda function requires the AWS Greengrass SDK to run on Greengrass devices.
# This can be found on the AWS IoT Console:
# https://docs.aws.amazon.com/greengrass/latest/developerguide/package.html
#
# The function is made to run in a Raspberry Pi with Greengrass Core Software
# It is made to receive a JSON message as input,
# than it gets the sensor's reading from RPi's I2C pins and publishes
# the message in a MQTT topic
#
#

import json
import logging
import platform
import sys
import time
import greengrasssdk

import board
import busio
import adafruit_mlx90614


# Setup logging to stdout
logger = logging.getLogger(__name__)
logging.basicConfig(stream=sys.stdout, level=logging.DEBUG)

# Creating a greengrass core sdk client
client = greengrasssdk.client("iot-data")

# Retrieving platform information to send from Greengrass Core
my_platform = platform.platform()


def function_handler(event, context):
    i2c = busio.I2C(board.SCL,board.SDA, frequency = 100000)
    mlx = adafruit_mlx90614.MLX90614(i2c)
    
    event['temp'] = mlx.object_temperature
    

    try:
        client.publish(
            topic="kiosk/pub",
            queueFullPolicy="AllOrException",
            payload=json.dumps(
                event
            ),
        )
        
    
    except Exception as e:
        logger.error("Failed to publish message: " + repr(e))
    time.sleep(20)
    return
