#
#  Created by Dylan Menzies
#  Copyright (c) 2019 ISVR, University of Southampton. All rights reserved.
#

import triad_openvr_DM
import time
import sys
import numpy as np
import json

from msvcrt import getch

#cccfrom pythonosc import osc_message_builder
from pythonosc import udp_client

keyDelay = 0 # 5 time delay (s) from key hit to measurement

#ip = "192.168.1.83"
#ip = "10.9.129.135"
#ip = "10.9.181.176"
ip = "169.254.217.219"

port = 5005  # 5005

v = triad_openvr_DM.triad_openvr()
v.print_discovered_objects()

client = udp_client.SimpleUDPClient(ip, port)

key2n = {b'1': 1, b'2': 2, b'3': 3, b'4': 4, b'5': 5, b'6' : 6, b'7' : 7, b'8' : 8, b'9' : 9}
    
isVirtual = False

C = np.matrix([ 0.0, 0.0, 0.0]).T
Cauto = True # Define centre automatically based on Y axis
Y = np.matrix([ 1.0, 0.0, 0.0]).T
X = np.matrix([ 0.0, 0.0, 0.0]).T
t = np.matrix([ 0.0, 0.0, 0.0]).T

# Load array coordinate map
fp = open('frameMap.json')
obj = json.load(fp)
Y = np.matrix(obj["Y"])
C = np.matrix(obj["C"])
fp.close()


def defineLoudspeaker(n, t):
        # Transform tip using Y axis / Centre calibration
        R = np.identity(3)
        #print(Y)
        R[0,0] = Y[0]
        R[0,2] = Y[2]
        R[2,0] = -Y[2]
        R[2,2] = Y[0]

        t = R * (t - C)
        
        # Convert to VISR coordinates, with index, and send
        VISRnxyz = [ n, t.item(2), t.item(0), t.item(1), isVirtual ]
        print(VISRnxyz)
        client.send_message("/nxyz", VISRnxyz )
            
            

while(True):
    print('')
    print('c - define centre with tracker (default is Y mark centre (with optional shift on x-axis) ')
    print('y - (twice) define +Y axis, and write speaker 1,2')
    print('1,2,3.. - mark loudspeaker / virtualspeaker position')
    print('l / v - edit loudspeakers / virtualspeakers')
    print('e - exit')
    key = getch() 
    #print(key)
    print('\a')

    if (key == b'e'):
        exit()

    elif (key == b'v'):
        isVirtual = True
        print('Virtualspeaker')

    elif (key == b'l'):
        print('Loudspeaker')
        isVirtual = False

    else:
        time.sleep(keyDelay)  # Delay for when making measurements solo
        euler = v.devices["tracker_1"].get_pose_euler()
        #print(pose[3])
        M = v.devices["tracker_1"].get_pose()
        M = [ [M[0][0], M[0][1], M[0][2], M[0][3]],
              [M[1][0], M[1][1], M[1][2], M[1][3]],
              [M[2][0], M[2][1], M[2][2], M[2][3]] ]
        R = np.matrix(M)
        p = R[0:3,3] # tracker position
        n = R[0:3,2] # normal vector pointing down from tracker 
    
        t1 = t # store last
        t = p + n * 0.14 # find position pf the end of pen 
                         # 0.036 position at end of round head bolt
    
        print("position", "%7.4f"%p.item(2), "%7.4f"%p.item(0), "%7.4f"%p.item(1))
        #print("normal", "%7.4f"%n.item(2), "%7.4f"%n.item(0), "%7.4f"%n.item(1))
        print("tip", "%7.4f"%t.item(2), "%7.4f"%t.item(0), "%7.4f"%t.item(1))


        if (key == b'c'):
            print('Define centre')
            print('\a')
            C = t
            Cauto = False
 #           print(C.tolist())
            print( [C.item(2), C.item(0), C.item(1)] ) # VISR order
            fp = open('frameMap.json', 'w')
            obj = {"Y" : Y.tolist(), "C" : C.tolist()}
            json.dump(obj, fp)
            fp.close()


        elif (key == b'y'):
            print('Define Y axis')
            print('\a')
            Y = t - t1
            Y[1] = 0   # z axis unchanged, axis horizontal
            Y = Y / np.linalg.norm(Y)
            X[2] = Y[0]
            X[0] = -Y[2]
            if (Cauto == True): # C not yet set by user
                C = (t + t1)/2
                print( [C.item(2), C.item(0), C.item(1)] ) 
                C = C - X *1.6  # pull centre back along x axis
            # print(Y.tolist())
            print( [Y.item(2), Y.item(0), Y.item(1)] ) # VISR order
            fp = open('frameMap.json', 'w')
            obj = {"Y" : Y.tolist(), "C" : C.tolist()}
            json.dump(obj, fp)
            fp.close()
            
            # Write initial speaker coords, useful for Stereo-CAP  t->0 t1->1
            defineLoudspeaker(1, t)
            time.sleep(0.1)
            defineLoudspeaker(2, t1)            
            
            
                
    
        elif key in key2n : #(key == b'p'):
            print('Define loudspeaker')   
            print('\a')
            n = key2n[key]
            defineLoudspeaker(n, t)
    

        else:
            print('INVALID')

   