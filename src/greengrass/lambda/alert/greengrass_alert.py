#
# Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
#

# greengrassHelloWorldCounter.py
# Demonstrates a simple publish to a topic using Greengrass core sdk
# This lambda function will retrieve underlying platform information and send a hello world message along with the
# platform information to the topic 'hello/world/counter' along with a counter to keep track of invocations.
#
# This Lambda function requires the AWS Greengrass SDK to run on Greengrass devices.
# This can be found on the AWS IoT Console.

import json
import logging
import platform
import sys
import time

import greengrasssdk

# Setup logging to stdout
logger = logging.getLogger(__name__)
logging.basicConfig(stream=sys.stdout, level=logging.DEBUG)

# Creating a greengrass core sdk client
client = greengrasssdk.client("iot-data")

# Retrieving platform information to send from Greengrass Core
my_platform = platform.platform()


def function_handler(event, context):
    if event['temperature'] > 37 :
        try:
            client.publish(
                topic="kiosk/alert",
                queueFullPolicy="AllOrException",
                payload=json.dumps(
                    {"alert": {"temperature" : event['temperature']}}
                ),
            )
        except Exception as e:
            logger.error("Failed to publish message: " + repr(e))
    
    return
