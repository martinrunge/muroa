#!/usr/bin/env python

import asyncio
import websockets
import json

payload = {
  "method": "playctrl",
  "params": { "file" : "/home/martin/tmp/Klangkarussell_-_Sonnentanz_(Original_Version).mp3", "report_progress" : True },
  "jsonrpc": "2.0",
  "id": 2,
}


async def playfile():
    async with websockets.connect('ws://localhost:8888') as websocket:
        payloadstr = json.dumps(payload)
        await websocket.send(payloadstr)
        print("> {}".format(payloadstr))

        while True:
            response = await websocket.recv()
            print("< {}".format(response))
            if ("end_of_stream" in response):
                break


asyncio.get_event_loop().run_until_complete(playfile())
