#!/usr/bin/env python

import asyncio
import websockets
import json
import argparse


parser = argparse.ArgumentParser()
parser.add_argument("-s", "--server", action="store", dest="serverurl", default='ws://localhost:8888', help="URL of the muroa server, e.g. ws://localhost:8888")
parser.add_argument("-q", "--quiet", action="store_false", dest="verbose", default=True, help="don't print status messages to stdout")
parser.add_argument("filename", help="file to play")

args = parser.parse_args()

print("serverurl: %s"%args.serverurl)
print("file to play: %s"%args.filename)

playid = 2

payload = {
  "method": "playctrl",
  "params": { "file" : args.filename, "report_progress" : True },
  "jsonrpc": "2.0",
  "id": playid,
}

def parse_response(jsonmsg):
    cont = True
    try:
        obj = json.loads(jsonmsg)
        print(obj)
        if(obj["jsonrpc"] != "2.0"):
            cont = False
        if "id" in obj:
            #  a response
            respid = obj["id"]
            if(playid == respid):
                success = obj["result"]["success"]
                if(success == False):
                    cont = False
        else:
            # a notification
            method = obj["method"]
            if(method == "progress"):
                # params need to include 'duration_in_secs' and 'pos_in_secs' 
                duration_in_secs = obj["params"]["duration_in_secs"]
                pos_in_secs = obj["params"]["pos_in_secs"]
            if(method == "end_of_stream"):
                cont = False
        
        
    except json.JSONDecodeError as de:
        print(de)
        cont = False
    
    return cont
    



async def playfile():
    async with websockets.connect(args.serverurl) as websocket:
        payloadstr = json.dumps(payload)
        await websocket.send(payloadstr)
        print("> {}".format(payloadstr))

        while True:
            response = await websocket.recv()
            cont = parse_response(response)
            if(cont == False):
                break


asyncio.get_event_loop().run_until_complete(playfile())
