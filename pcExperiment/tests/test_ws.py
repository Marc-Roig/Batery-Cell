import json
import asynctest

from battery_experiment import main


class WebsocketProtocol(asynctest.MagicMock):

    def push_message(self, msg: str):

        if hasattr(self, 'frames'):
            self.frames.append(msg)
        else:
            self.frames = [msg]

    def __await__(self):
        return self
        # noinspection PyUnreachableCode
        yield None

    async def __aenter__(self):
        return await self

    async def __aexit__(self, exc_type, exc_value, traceback):
        return


class TestWs(asynctest.TestCase):

    async def setUp(self) -> None:
        self.protocol = WebsocketProtocol()
        self.protocol.send = asynctest.CoroutineMock(return_value=True)
        self.protocol.recv = asynctest.CoroutineMock(return_value='{"type": "transmit_message_to"}')

        self.experiment_ws = main.ExperimentWebSocket(ip='34.67.206.162', port='3001', query='?platform=pc')
        self.experiment_ws.ws_operation_ok = asynctest.CoroutineMock(return_value=None)
        self.experiment_ws.ws_operation_failed = asynctest.CoroutineMock(return_value=None)

    # async def tearDown(self) -> None:
    #     await self.experiment_ws.stop()

    async def test_ws_serve_once(self):

        # with asynctest.patch('websockets.connect', return_value=self.protocol) as connect_mock:

        msg = {'type': 'transmit_message_to', 'content': {'operation_requested': 'start_experiment'}}

        # Mock start_experiment method
        with asynctest.patch('battery_experiment.main.start_experiment') as start_experiment_mock:
            await self.experiment_ws.websocket_serve_once(self.protocol, json.dumps(msg))
            # Assert start_experiment method has been called
            self.assertEqual(start_experiment_mock.call_count, 1)

        # Assert notification of operation succeeded
        self.assertEqual(self.experiment_ws.ws_operation_ok.call_count, 1)
        self.assertEqual(self.experiment_ws.ws_operation_failed.call_count, 0)

    # TODO: Assert exceptions
