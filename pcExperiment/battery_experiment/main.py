import asyncio

from asyncio import Event, AbstractEventLoop
import websockets
import json

import logging
import signal
import functools

from mouse import GuiRobot


async def execute_operation(window_name: str, click_seq_path: str):

    logging.info('Starting operation')

    gui_robot = GuiRobot(windows_name=window_name, click_seq_path=click_seq_path)
    gui_robot.run()


operations_map = {
    'start_experiment': {
        'window_name': '',
        'click_seq_path': ''
    },
    'stop_experiment': {
        'window_name': '',
        'click_seq_path': ''
    },
    'start_measurements': {
        'window_name': '',
        'click_seq_path': ''
    },
    'stop_measurements': {
        'window_name': '',
        'click_seq_path': ''
    }
}


class ExperimentWebSocket:

    def __init__(self, ip: str, port: str, query: str):

        self.loop = asyncio.get_event_loop()
        self.task = None

        self.uri = f"ws://{ip}:{port}/{query}"

    async def ws_operation_ok(self, websocket: websockets.WebSocketCommonProtocol, operation_requested: str):
        """
        Transmit
        :param websocket: websocket object
        :param operation_requested: which operation was requested from the esp32
        """
        json_msg = {
            'type': 'transmit_message_to',
            'from': 'pc',
            'to': 'microcontroller',
            'content': {
                'operation_requested': operation_requested,
                'status': 'ok'
            }
        }
        msg = json.dumps(json_msg)

        await websocket.send(msg)

    async def ws_operation_failed(self, websocket, operation_requested: str):
        """
        Transmit
        :param websocket: websocket object
        :param operation_requested: which operation was requested from the esp32
        """
        json_msg = {
            'type': 'transmit_message_to',
            'from': 'pc',
            'to': 'microcontroller',
            'content': {
                'operation_requested': operation_requested,
                'status': 'error'
            }
        }
        msg = json.dumps(json_msg)

        await websocket.send(msg)

    async def websocket_serve_forever(self):

        async with websockets.connect(self.uri) as websocket:

            logging.info('Connected')

            # Main webSocket loop. Wait for any incoming message from the server,
            # execute operation requested and notify if succeeded (or failed)
            async for msg in websocket:
                await self.websocket_serve_once(websocket, msg)
                await asyncio.sleep(0.1)

    async def websocket_serve_once(self, websocket, msg):

        logging.debug("Incoming ws message: ")
        logging.debug(msg)

        operation_requested_str = ''

        try:

            json_msg = json.loads(msg)

            # Retrieve which operation has to be executed
            operation_requested_str = json_msg['content']['operation_requested']
            operation = operations_map[operation_requested_str]

            # Execute operation
            await execute_operation(operation['window_name'], operation['click_seq_path'])

            # Notify operation succeeded
            await self.ws_operation_ok(websocket, operation_requested_str)

        except (json.JSONDecodeError, KeyError, asyncio.CancelledError) as e:
            # Notify operation failed
            logging.error(e)
            logging.error(msg)
            await self.ws_operation_failed(websocket, operation_requested_str)

        except Exception as e:
            # Notify operation failed
            logging.error(e)
            logging.error(msg)
            await self.ws_operation_failed(websocket, operation_requested_str)
            raise Exception

    def run(self):
        loop = asyncio.get_event_loop()
        try:
            loop.run_until_complete(self.websocket_serve_forever())
        finally:
            loop.close()


if __name__ == '__main__':

    import os
    logging.basicConfig(level=os.environ.get("LOGLEVEL", "INFO"))

    # Ws parameters
    ip = '34.67.206.162'
    port = '3001'
    query = '?platform=pc'

    # Init ws com
    experiment_ws = ExperimentWebSocket(ip, port, query)
    experiment_ws.run()
