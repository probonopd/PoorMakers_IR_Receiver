Poor Maker's Infrared receiver
==============================

One of the most common problems encountered when trying to decode IR signals is that makers don’t always have the appropriate IR receiver for the job in hand or have to wait for one to be delivered by mail. The makers of AnalysIR describe an affordable method to allow you to use any IR emitter (LED) as a receiver, 2 resistors and any standard Arduino. 

**Note:** Since the receiver LED is not really designed for receiving, you must minimize the distance between sender LED and receiver LED. Make them so close that they actually touch each other and make sure they are precisely aligned.

I added a Python script to automatically query http://irdb.tk in order to interpret the captured signals.

![poormansirreceiver2_cc](https://cloud.githubusercontent.com/assets/2480569/3489938/520688d4-0550-11e4-90f3-e67f98acfce9.png)

Licenses
--------

**PoorMakers_IR_Receiver.ino**

Author: AnalysIR, Copyright: AnalysIR

Free to use, provided AnalysIR is credited and a link to http://www.AnalysIR.com/blog is included in the source and any related article or publication.

Note: I (probonopd) am not involved with or related to AnalysIR in any way, but I assume that the license above allows me to mirror this project here.

**analyze_using_irdb.py**

Author: probonopd, Copyright: probonopd

Free to use, provided irdb.tk is credited and a link to http://irdb.tk is included in the source and any related article or publication.
