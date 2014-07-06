#! /usr/bin/env python

#
# probonopd 7/2014
# Communicate with Arduino to capture IR signal
# and then decode it using the irdb.tk web service
#

import sys, serial, select, urllib, urllib2

def decode(line):
    line = line.replace(" ","").replace(",","").replace("+","%20+").replace("-","%20-")[3:]
    url = 'http://irdb.tk/api/decode/' + line
    try:
        req = urllib2.Request(url)
        rsp = urllib2.urlopen(req)
        content = rsp.read()
        # print(content)
        for line in content.split("\n"):
            if "protocol" in line or "device" in line or "subdevice" in line or "function" in line:
                print line.strip().replace('"',"").replace(',',"")
    except:
        print("The server returned an error")

class Arduino() :
    def run(self, baud=115200):
        try:
            port = "/dev/cu.SLAB_USBtoUART"
            self.ser = serial.Serial(port, baud, timeout=1)
            print "Opened", port
        except:
            self.ser = None

        if not self.ser:
            print "Couldn't open a serial port"
            sys.exit(1)

        self.ser.flushInput()
        while True:
            inp, outp, err = select.select([sys.stdin, self.ser], [], [], .2)
            if self.ser in inp:
                line = self.ser.readline().strip()
                if ", +" in line and len(line) > 50:
                    print("")
                    decode(line)

arduino = Arduino()
try:
    arduino.run()
except serial.SerialException:
    print "Disconnected (Serial exception)"
except KeyboardInterrupt:
    print "Interrupt"
