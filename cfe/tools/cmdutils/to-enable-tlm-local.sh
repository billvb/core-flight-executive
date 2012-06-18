# to-enable-tlm.sh : This script will call the sendUdpCmd program with a command packet to the
#                    TO cFE application. It will tell TO to start sending telemetry to 
#                    IP address 192.168.0.200 . In this TO command the host IP that TO will 
#                    send data to is encoded in ASCII.
./sendUdpCmd BE localhost 1234 0x1880,0xC000,0x0012,0x06E3,0x3231,0x2E37,0x3030,0x2E30,0x2E30,0x3030,0x0031
