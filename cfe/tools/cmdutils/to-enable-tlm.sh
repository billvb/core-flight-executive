# to-enable-tlm.sh : This script will call the sendUdpCmd program with a command packet to the
#                    TO cFE application. It will tell TO to start sending telemetry to 
#                    IP address 127.0.0.1 ( localhost ). In this TO command the host IP that TO will 
#                    send data to is encoded in ASCII.
./sendUdpCmd BE localhost 1234 0x1880,0xC000,0x0011,0x06E3,0x3132,0x372E,0x3030,0x302E,0x302E,0x3031,0x0000
