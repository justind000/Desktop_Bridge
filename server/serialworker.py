import serial
import time
import multiprocessing

# Change this to match your local settings
SERIAL_BAUDRATE = 9600


class SerialProcess(multiprocessing.Process):

    def __init__(self, input_queue, output_queue, serial_port):
        multiprocessing.Process.__init__(self)
        self.input_queue = input_queue
        self.output_queue = output_queue
        self.sp = serial.Serial(serial_port, SERIAL_BAUDRATE, timeout=1)

    def close(self):
        self.sp.close()

    def writeSerial(self, data):
        data = data + '\n'
        self.sp.write(data.encode('utf-8'))

    def readSerial(self):
        return self.sp.readline()

    def run(self):

        self.sp.flushInput()

        while True:
            # look for incoming tornado request
            if not self.input_queue.empty():
                data = self.input_queue.get()

                # send it to the serial device
                self.writeSerial(data)
                print("writing to serial: " + data)

            # look for incoming serial data
            if (self.sp.inWaiting() > 0):
                data = self.readSerial()
                print(data)
                # send it back to tornado
                self.output_queue.put(data)
