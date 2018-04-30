#!/usr/bin/env python

import asyncio
import websockets
import json

payload = {
  "method": "echo",
  "params": ["echome!"],
  "jsonrpc": "2.0",
  "id": 0,
}


async def hello():
    async with websockets.connect('ws://localhost:8888') as websocket:
        payloadstr = json.dumps(payload)
        await websocket.send(payloadstr)
        print("> {}".format(payloadstr))

        response = await websocket.recv()
        print("< {}".format(response))

asyncio.get_event_loop().run_until_complete(hello())
