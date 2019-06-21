#
#  Created by Dylan Menzies
#  Copyright (c) 2019 ISVR, University of Southampton. All rights reserved.
#

#import keypoller as kp

import triad_openvr_DM
import time
import sys
import numpy as np
import json
import math

from msvcrt import getch
from msvcrt import kbhit

#cccfrom pythonosc import osc_message_builder
from pythonosc import udp_client



d2 = 0.14 # distance from sensor to head centre in n2 direction
#d = 0.05 # distance from sensor to head centre in n direction 


frameMap = 'frameMap.json'

port = 5005  # 5005

ip = "169.254.217.219"  # "169.254.217.219 ethernet connection with manual address
#ip = "10.9.181.37"


#ip = "192.168.1.45"
#ip = "192.168.1.83"
#ip = "10.9.129.135"
#ip = "10.9.181.176"



v = triad_openvr_DM.triad_openvr()
v.print_discovered_objects()

if len(sys.argv) == 1:
    interval = 1/200
elif len(sys.argv) == 2:
    interval = 1/float(sys.argv[0])
else:
    print("Invalid number of arguments")
    interval = False

client = udp_client.SimpleUDPClient(ip, port)
print("udp client = ", client)


print("frame map = ", frameMap)

# Load array coordinate map
fp = open(frameMap)
obj = json.load(fp)
Y = np.matrix(obj["Y"])
C = np.matrix(obj["C"])
fp.close()

isVerbose = False
isTracking = True

#print(Y)
#print(Y[2])

print("\a")

appStart = time.time()

print('')
print('v - toggle verbose mode')
print('t - toggle tracking')
print('e - exit')
print('MUST EXIT BEFORE SLEEP OR ETHERNET CABLE UNPLUG!!')

if interval:
    while(True):     # time.time() - appStart < 600):	# 10 minutes running time
        start = time.time()
        
#        with kp.KeyPoller() as input:
#            c = input.poll()
#            if not c is None:
#                print(c)

        if kbhit():
            k = getch()
            if k == b'e':
                break
            if k == b'v':
                isVerbose = not isVerbose
                print('')
                print('v - toggle verbose mode')
                print('t - toggle tracking')
                print('e - exit')
                print('MUST EXIT BEFORE SLEEP OR ETHERNET CABLE UNPLUG!!')
            if k == b't':
                isTracking = not isTracking
                if isTracking:
                    print("Tracking ON")
                else:
                    print("Tracking OFF")
            
        #euler = v.devices["tracker_1"].get_pose_euler()
        #print(euler)

        M = v.devices["tracker_1"].get_pose()
        M = [ [M[0][0], M[0][1], M[0][2], M[0][3]],
              [M[1][0], M[1][1], M[1][2], M[1][3]],
              [M[2][0], M[2][1], M[2][2], M[2][3]] ]
        R = np.matrix(M)
        p = R[0:3,3] # tracker position
        n2 = R[0:3,2] # normal vector pointing down from tracker 
        n = R[0:3,1] 
        c = p + n2 * d2  # + n * d
        #R = R[0:3,0:3] # keep rotation part only

        #print("position", "%7.4f"%p.item(2), "%7.4f"%p.item(0), "%7.4f"%p.item(1))
        #print("normal", "%7.4f"%n.item(2), "%7.4f"%n.item(0), "%7.4f"%n.item(1))
        #print("centre", "%7.4f"%c.item(2), "%7.4f"%c.item(0), "%7.4f"%c.item(1))

        # Calc matrix that rotates to VISR frame
        O = np.identity(3)
        O[0,0] = Y[0]
        O[0,2] = Y[2]
        O[2,0] = -Y[2]
        O[2,2] = Y[0]


        # Transform head centre to VISR frame (C is origin of VISR in sensor frame)
        c = O * (c - C)                

        # Transform head orientation, R, to VISR frame
        # and calc new YPR

        R = O * R

        # Permute R to VISR frame
        R[:,[0,1,2]] = R[:,[1,0,2]]
        R[[0,1,2],:] = - R[[2,0,1],:]
        
        # Standard y p r  with -p -r  for VISR signature
        yaw = 180 / math.pi * math.atan2(R[1,0], R[0,0])
        pitch = -180 / math.pi * math.atan2(-1 * R[2,0], math.sqrt(pow(R[2,1], 2) + math.pow(R[2,2], 2)))
        roll = -180 / math.pi * math.atan2(R[2,1], R[2,2])


        # Rotation axes in VISR frame
        ax = R[0:3, 0]
        ay = R[0:3, 1] # axis-0, should be aligned with inter-aural axis
        az = R[0:3, 2]    
        

        if isVerbose:
            print(R)
            # print("rot x axis", "%7.4f"%ax.item(0), "%7.4f"%ax.item(1), "%7.4f"%ax.item(2) )
            # print("rot z axis", "%7.4f"%az.item(0), "%7.4f"%az.item(1), "%7.4f"%az.item(2) )
            print("rot y axis,  position", "%7.4f"%ay.item(0), "%7.4f"%ay.item(1), "%7.4f"%ay.item(2), "   %7.4f"%c.item(2), "%7.4f"%c.item(0), "%7.4f"%c.item(1))

            # print("yaw pitch roll   ", "%7.4f"%yaw, "%7.4f"%pitch, "%7.4f"%roll )
            # isVerbose = False  # for single sample
        

        # Override with default listener
#        c = c * 0
#        yaw = 0
#        pitch = 0
#        roll = 0

        # Send ith VISR xyz axes definition
        # and with normalisation for Reaper
        if isTracking:
            client.send_message("/x", (c.item(2)+10.0)/20.0 ) #include permute
            client.send_message("/y", (c.item(0)+10.0)/20.0 )
            client.send_message("/z", (c.item(1)+10.0)/20.0 )
            client.send_message("/yaw", (yaw+180)/360.0)
            client.send_message("/pitch", (pitch+90)/180.0)
            client.send_message("/roll", (roll+180)/360.0)
        
        # client.send_message("/track/2/volume", (np.sign(yaw)+1)/2 )  # latency test


        sleep_time = interval-(time.time()-start)
        if sleep_time>0:
            # print("..")
            time.sleep(sleep_time)

print("\a")
