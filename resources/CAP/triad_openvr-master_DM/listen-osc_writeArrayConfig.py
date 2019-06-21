#
#  Created by Dylan Menzies
#  Copyright (c) 2019 ISVR, University of Southampton. All rights reserved.
#


"""Small example OSC server

This program listens to several addresses, and prints some information about
received packets.
"""
import argparse
import math

from pythonosc import dispatcher
from pythonosc import osc_server

from lxml import objectify, etree

#from pybeep.pybeep import PyVibrate, PyBeep


#ip = "192.168.1.83"
#ip = "10.9.183.191"
#ip = "10.9.182.244"
#ip = "192.168.1.149"
ip = "169.254.217.219"


arrayConfigFile = 'CAPstereoLR.xml'
#arrayConfigFile = 'CAP_LCR.xml'
#arrayConfigFile = 'arrayConfig.xml'

yaw = 0
pitch = 0
roll = 0


  
  
def nxyz(unused_addr, n, x, y, z, isVirtual):
    print(n,"%7.4f"%x,"%7.4f"%y,"%7.4f"%z, isVirtual)
    # Load Array Config (AC)
    fp = open(arrayConfigFile)
    # fp = open('stereo.xml')
    # print(fp.read())
    AC = objectify.fromstring(fp.read())
    
    #print(len(AC.virtualspeaker))
    
    if (isVirtual == False):
        if n <= len(AC.loudspeaker):
            AC.loudspeaker[n-1].cart.set("x", "{0:.3f}".format(x))
            AC.loudspeaker[n-1].cart.set("y", "{0:.3f}".format(y))
            AC.loudspeaker[n-1].cart.set("z", "{0:.3f}".format(z))
            print('\a') # beep 
        else:
            print('INVALID')      
             
    elif (isVirtual == True):
        if n <= len(AC.virtualspeaker):
            AC.virtualspeaker[n-1].cart.set("x", "{0:.3f}".format(x))
            AC.virtualspeaker[n-1].cart.set("y", "{0:.3f}".format(y))
            AC.virtualspeaker[n-1].cart.set("z", "{0:.3f}".format(z))
            print('\a')   
        else:
            print('INVALID')
        
             
        # x = AC.loudspeaker[n-1].cart.get("x")
        # print(x)
    # Create xml string
    AC_xml = etree.tostring(AC,
                             pretty_print=True,
                             xml_declaration=False)
    #print(AC_xml)
    fp.close
    # Write edited xml
    fp = open(arrayConfigFile,'wb')
    fp.write(AC_xml)
    fp.close



  

if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument("--ip",
      default=ip, help="The ip to listen on")
  parser.add_argument("--port",
      type=int, default=5005, help="The port to listen on")
  args = parser.parse_args()


  dispatcher = dispatcher.Dispatcher()
  
  dispatcher.map("/nxyz", nxyz)  

  #nxyz("/nxyz", 5, 1.111, 0, 0)
  
  
  

  server = osc_server.ThreadingOSCUDPServer(
      (args.ip, args.port), dispatcher)
  print("Serving on {}".format(server.server_address))
  server.serve_forever()
  
  print('end')
  
  