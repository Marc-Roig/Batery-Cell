import aiofirebase
import asyncio


class FirebaseOperations:

    def __init__(self, base_url, auth, loop=None):

        self.loop = loop or asyncio.get_event_loop()

        self.firebase = aiofirebase.FirebaseHTTP(base_url, auth, loop=self.loop)

    async def get_experiment_parameters(self, firebase_id: str) -> None:

        path = f"{firebase_id}/content/parameters"
        experiment_parameters = await self.firebase.get(path=path)
        print(experiment_parameters)

    async def record_cell_time_usage(self, firebase_id: str, time_s: int) -> None:

        path = f"{firebase_id}/content"

        # Get previous time
        previous_time = await self.firebase.get(path=f"{path}/time_cell_usages")
        if previous_time:
            time_s += previous_time

        await self.firebase.put(path=f"{path}/time_cell_usages", value=time_s)


    async def stop(self) -> None:
        await self.firebase.close()


    # result = await firebase.patch(path='/hardware/content', value={'eppendorf_idx': 0})

async def main_coro(firebase):
    await asyncio.gather(*[
        firebase.get_experiment_parameters("-LxVpoNVlj3UhVthtbVf"),
        firebase.record_cell_time_usage("-LxVpoNVlj3UhVthtbVf", 100)
    ])

def main():

    loop = asyncio.get_event_loop()
    firebase = FirebaseOperations("https://eurebattautoamation.firebaseio.com",
                                  auth="ysXfTdlCjMLPD5WGQE2WPqGCrAqZYohOu6Qwh5OM", loop=loop)
    try:
        loop.run_until_complete(main_coro(firebase))
    finally:
        loop.run_until_complete(firebase.stop())
        loop.close()


if __name__ == '__main__':
    main()
