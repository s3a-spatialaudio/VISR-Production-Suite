

#
#  Created by Dylan Menzies
#  Copyright (c) 2019 ISVR, University of Southampton. All rights reserved.
#


Extensions to the Triad OpenVR Python Wrapper


tracker_test_DM_measureArray.py
Orientate array and send calibrated loudspeaker positions over udp to be used to form an array config in the renderer path. The orientation data is stored in a file frameMap.json This is used to transform the Vive coords to VISR coords

tracker_test_DM_head.py
Used for sending real-time head 6dof data to renderer. Cos are transformed using arrayMap.json


listen-osc_writeArrayConfig.py
Receive osc messages sent by tracker_test_DM_measureArray.py, and then writes an array config file, which can be loaded by a renderer. The frame map files will then be in sync with the config file.