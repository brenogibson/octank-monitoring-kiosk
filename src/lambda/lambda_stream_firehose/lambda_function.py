
import json
import boto3
from base64 import b64encode
from dynamodb_json import json_util as djson
from boto3.dynamodb.types import TypeDeserializer

deser = TypeDeserializer()
print('- Begin function -')

serializer = TypeDeserializer()


def deserialize(data):
    if isinstance(data, list):
       return [deserialize(v) for v in data]

    if isinstance(data, dict):
        try: 
            return serializer.deserialize(data)
        except TypeError:
            return { k : deserialize(v) for k, v in data.items() }
    else:
        return data

def lambda_handler(event, context):
    fh_client = boto3.client('firehose')
    payload = {'Data':''}
    
    response = ''
    for record in event['Records']:
    	print(record)
    	if record['eventName'] != 'INSERT':
    		continue
    	payload_record = djson.loads(json.dumps(record['dynamodb']['NewImage']))

    	payload['Data'] = (json.dumps(payload_record))
    	response = fh_client.put_record(DeliveryStreamName='---BUCKET_NAME---',Record=payload)
    print(response)
    return response

