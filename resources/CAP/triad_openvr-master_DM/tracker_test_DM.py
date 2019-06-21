#
#  Created by Dylan Menzies
#  Copyright (c) 2019 ISVR, University of Southampton. All rights reserved.
#

import triad_openvr_DM
import time
import sys

from pythonosc import osc_message_builder
from pythonosc import udp_client


#ip = "192.168.1.83"
#ip = "10.9.129.135"
#ip = "10.9.181.176"
ip = "169.254.217.219"

port = 5005  # 5005

interval = 1/50

v = triad_openvr_DM.triad_openvr()
v.print_discovered_objects()

if len(sys.argv) == 1:
    interval
elif len(sys.argv) == 2:
    interval = 1/float(sys.argv[0])
else:
    print("Invalid number of arguments")
    interval = False

client = udp_client.SimpleUDPClient(ip, port)

    
if interval:
    while(True):
        start = time.time()

        pose = v.devices["tracker_1"].get_pose_euler()
        #print(pose)
        #print("\r")

        client.send_message("/x", (pose[2]+10.0)/20.0 )
        client.send_message("/y", (pose[0]+10.0)/20.0 )
        client.send_message("/z", (pose[1]+10.0)/20.0 )
        client.send_message("/yaw", (pose[3]+180)/360.0)
        client.send_message("/pitch", (pose[4]+90)/180.0)
        client.send_message("/roll", (pose[5]+180)/360.0)

        #client.send_message("/ya", pose[3])
        #client.send_message("/pi", pose[4])
        #client.send_message("/ro", pose[5])


        sleep_time = interval-(time.time()-start)
        if sleep_time>0:
            time.sleep(sleep_time)