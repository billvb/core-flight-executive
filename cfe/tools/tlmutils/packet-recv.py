#
# Packet Receive program 
#
from socket import *

#
# strToHex routine
#
def strToHex(aString):
    hexStr = ""
    for x in aString:
        hexStr = hexStr + "%02X " % ord(x)
    return hexStr
#-------------------------------------------

#
# Parse packet
#
def parsePacket(packetData):
         appIdString = "%02X" % ord(packetData[0])
         appIdString = appIdString + "%02X" % ord(packetData[1])
         appId = (ord(packetData[0]) << 8) + (ord(packetData[1]))
         print "\n-----------------------------------------------"
         print "\nPacket: App ID = ",  hex(appId)
         if appId == 0x0841:
            print "\nEvent: ", packetData
         elif appId == 0x0815:
            print "\nCI CLCW "
         elif appId == 0x080B:
            print "\nFast HK Packet"
         else:
            print "\nPacket Data: ", strToHex(packetData)

#----------------------------------------------------------------
#
# Set the socket parameters
#
host = ""
port = 1235 
buf = 1024
addr = (host,port)

#
# Create socket and bind to address
#
UDPSock = socket(AF_INET,SOCK_DGRAM)
UDPSock.bind(addr)

#
# Receive messages
#
while 1:
	data,addr = UDPSock.recvfrom(buf)
	if not data:
		print "Client has exited!"
		break
	else:
                parsePacket(data)

#
# Close socket
#
UDPSock.close()

