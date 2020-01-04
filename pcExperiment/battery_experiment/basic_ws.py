import asyncio
import websockets
import json

import logging

async def hello(uri):
    async with websockets.connect(uri) as websocket:
        await websocket.send("Hello world!")
        result = await websocket.recv()
        print(result)

def func():
    asyncio.get_event_loop().run_until_complete(hello('ws://localhost:8765'))
