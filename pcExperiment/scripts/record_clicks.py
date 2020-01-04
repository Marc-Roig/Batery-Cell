from battery_experiment.mouse import ClickRecorder

FILE_NAME = "test"

if __name__ == '__main__':
    recorder = ClickRecorder(file_name=FILE_NAME)
    recorder.start_recording()
