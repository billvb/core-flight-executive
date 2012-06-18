#
# pyTos ( PieToss?) Ground system command module
#

import gtk
import pygtk
import os

# pygtk.require('2.0')

#
# Command Definitions
# Right now, these are just commands to the command C program to send a UDP packet with a CCSDS command in it.
# 

# TO commands
# Enable tlm output to LE 192.168.0.8..
TO_EnableTLMCmd =       "./sendUdpCmd LE 192.168.0.8 1234 0x1880,0xC000,0x0012,0x06E3,0x3139,0x322E,0x3136,0x382E,0x302E,0x3230,0x3000"
TO_NoOpCmd =            "./sendUdpCmd LE 192.168.0.8 1234 0x1880,0xC000,0x0001,0x0000"

#CI Commands
CI_NoOpCmd =            "./sendUdpCmd LE 192.168.0.8 1234 0x1881,0xC000,0x0001,0x0100"
CI_ResetCountersCmd =   "./sendUdpCmd LE 192.168.0.8 1234 0x1881,0xC000,0x0001,0x0300"

#ES Commands
ES_NoOpCmd =            "./sendUdpCmd LE 192.168.0.8 1234 0x1806,0xC000,0x0001,0x0000"
ES_ResetCountersCmd =   "./sendUdpCmd LE 192.168.0.8 1234 0x1806,0xC000,0x0001,0x0100"
ES_QueryAllCmd =        "./sendUdpCmd LE 192.168.0.8 1234 0x1806,0xC000,0x0001,0x0900"
ES_WriteXLogCmd =       "./sendUdpCmd LE 192.168.0.8 1234 0x1806,0xC000,0x0001,0x0B00"
ES_ClearXLogCmd =       "./sendUdpCmd LE 192.168.0.8 1234 0x1806,0xC000,0x0001,0x0A00"

#EVS Commands
EVS_EnableDebugCmd =    "./sendUdpCmd LE 192.168.0.8 1234 0x1801,0xC000,0x0002,0x0300,0x0001" 
EVS_NoOpCmd =           "./sendUdpCmd LE 192.168.0.8 1234 0x1801,0xC000,0x0001,0x0100"
EVS_ResetCountersCmd =  "./sendUdpCmd LE 192.168.0.8 1234 0x1801,0xC000,0x0001,0x0200" 
EVS_WriteLogCmd =       "./sendUdpCmd LE 192.168.0.8 1234 0x1801,0xC000,0x0001,0x1200"

#SB Commands
SB_NoOpCmd =            "./sendUdpCmd LE 192.168.0.8 1234 0x1803,0xC000,0x0001,0x0000"
SB_ResetCountersCmd =   "./sendUdpCmd LE 192.168.0.8 1234 0x1803,0xC000,0x0001,0x0100"
SB_SendStatsCmd =       "./sendUdpCmd LE 192.168.0.8 1234 0x1803,0xC000,0x0001,0x0200"
SB_SendRoutingInfoCmd = "./sendUdpCmd LE 192.168.0.8 1234 0x1803,0xC000,0x0001,0x0300"

#TIME Commands
TIME_NoOpCmd =          "./sendUdpCmd LE 192.168.0.8 1234 0x1805,0xC000,0x0001,0x0000"
TIME_ResetCountersCmd = "./sendUdpCmd LE 192.168.0.8 1234 0x1805,0xC000,0x0001,0x0100"
TIME_AddAdjustCmd =     "./sendUdpCmd LE 192.168.0.8 1234 0x1805,0xC000,0x0001,0x0A00"
TIME_ReqDiagCmd =     "./sendUdpCmd LE 192.168.0.8 1234 0x1805,0xC000,0x0001,0x0200"

#TBL Commands
TBL_NoOpCmd =          "./sendUdpCmd LE 192.168.0.8 1234 0x1804,0xC000,0x0001,0x0000"
TBL_ResetCountersCmd = "./sendUdpCmd LE 192.168.0.8 1234 0x1804,0xC000,0x0001,0x0100"

#CAMI Commands
CAMI_NoOpCmd =          "./sendUdpCmd LE 192.168.0.8 1234 0x1883,0xC000,0x0001,0x00AF"
CAMI_FetchCmd =         "./sendUdpCmd LE 192.168.0.8 1234 0x1883,0xC000,0x0001,0x02AD" 
CAMI_ResetCountersCmd = "./sendUdpCmd LE 192.168.0.8 1234 0x1883,0xC000,0x0001,0x01AE"

#CAMO Commands
CAMO_NoOpCmd =          "./sendUdpCmd LE 192.168.0.8 1234 0x1884,0xC000,0x0001,0x00AF"
CAMO_SendCmd =         "./sendUdpCmd LE 192.168.0.8 1234 0x1884,0xC000,0x0001,0x02AD" 
CAMO_ResetCountersCmd = "./sendUdpCmd LE 192.168.0.8 1234 0x1884,0xC000,0x0001,0x01AE"

#CFDP Commands
CFDP_EnableCmd =          "./sendUdpCmd LE 192.168.0.8 1234 0x1882,0xC000,0x0001,0x0300"

BorderWidth     = 010
FrameSizeWidth  = 530
FrameSizeHeight = 100
BBoxBorderWidth = 001

#
# Code 
#
class CommandModule:
    # This method rotates the position of the tabs
    def rotate_book(self, button, notebook):
        notebook.set_tab_pos((notebook.get_tab_pos()+1) %4)

    def send_cmd(self, button, data):
        os.system(data)
        
    # Add/Remove the page tabs and the borders
    def tabsborder_book(self, button, notebook):
        tval = gtk.False
        bval = gtk.False
        if self.show_tabs == gtk.False:
	        tval = gtk.TRUE 
        if self.show_border == gtk.False:
	        bval = gtk.TRUE

        notebook.set_show_tabs(tval)
        self.show_tabs = tval
        notebook.set_show_border(bval)
        self.show_border = bval

    def delete(self, widget, event=None):
        gtk.main_quit()
        return False

    def __init__(self):
        window = gtk.Window(gtk.WINDOW_TOPLEVEL)
        window.connect("delete_event", self.delete)
        window.set_border_width(10)
        

        table = gtk.Table(3,6,False)
        window.add(table)

        # Create a new notebook, place the position of the tabs
        notebook = gtk.Notebook()
        notebook.set_tab_pos(gtk.POS_TOP)
        table.attach(notebook, 0,6,0,1)
        notebook.show()
        self.show_tabs = gtk.TRUE
        self.show_border = gtk.TRUE

        #
        # Define command Tabs
        #--------------------------------------------------------

        #
        # TO Command Page
        #
        frame = gtk.Frame("TO Commands")
        frame.set_border_width(BorderWidth)
        frame.set_size_request(FrameSizeWidth, FrameSizeHeight)
        frame.show()

        bbox = gtk.HButtonBox()
        bbox.set_border_width(BBoxBorderWidth)
        frame.add(bbox)
        
        button = gtk.Button("Enable TLM")
        button.connect("clicked",self.send_cmd, TO_EnableTLMCmd)
        bbox.add(button)
        button.show()

        button = gtk.Button("No Op")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, TO_NoOpCmd)
        button.show()

        bbox.show()

        label = gtk.Label("TO CMDS")
        notebook.append_page(frame, label)

        #
        # End defintion for TO command page
        #
        
        #
        # CAMI Command Page
        #
        frame = gtk.Frame("Camera In Commands")
        frame.set_border_width(BorderWidth)
        frame.set_size_request(FrameSizeWidth, FrameSizeHeight)
        frame.show()

        bbox = gtk.HButtonBox()
        bbox.set_border_width(BBoxBorderWidth)
        frame.add(bbox)

        button = gtk.Button("Fetch Image")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, CAMI_FetchCmd)
        button.show()
  
        button = gtk.Button("Reset")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, CAMI_ResetCountersCmd)
        button.show()      

        button = gtk.Button("No Op")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, CAMI_NoOpCmd)
        button.show()


        bbox.show()

        label = gtk.Label("CAMI CMDS")
        notebook.append_page(frame, label)

        #
        # End defintion for CAMI command page
        #

        #
        # CAMO Command Page
        #
        frame = gtk.Frame("Camera Out Commands")
        frame.set_border_width(BorderWidth)
        frame.set_size_request(FrameSizeWidth, FrameSizeHeight)
        frame.show()

        bbox = gtk.HButtonBox()
        bbox.set_border_width(BBoxBorderWidth)
        frame.add(bbox)

        button = gtk.Button("Send Image")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, CAMO_SendCmd)
        button.show()
  
        button = gtk.Button("Reset")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, CAMO_ResetCountersCmd)
        button.show()      

        button = gtk.Button("No Op")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, CAMO_NoOpCmd)
        button.show()


        bbox.show()

        label = gtk.Label("CAMO CMDS")
        notebook.append_page(frame, label)

        #
        # End defintion for CAMO command page
        #

        #
        # CFDP Command Page
        #
        frame = gtk.Frame("CFDP Commands")
        frame.set_border_width(BorderWidth)
        frame.set_size_request(FrameSizeWidth, FrameSizeHeight)
        frame.show()

        bbox = gtk.HButtonBox()
        bbox.set_border_width(BBoxBorderWidth)
        frame.add(bbox)

        button = gtk.Button("Enable CFDP")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, CFDP_EnableCmd)
        button.show()
  
        bbox.show()

        label = gtk.Label("CFDP CMDS")
        notebook.append_page(frame, label)

        #
        # End defintion for CFDP command page
        #

        #
        # CI Command Page
        #
        frame = gtk.Frame("Commands")
        frame.set_border_width(BorderWidth)
        frame.set_size_request(FrameSizeWidth, FrameSizeHeight)
        frame.show()

        bbox = gtk.HButtonBox()
        bbox.set_border_width(BBoxBorderWidth)
        frame.add(bbox)
  
        button = gtk.Button("Reset")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, CI_ResetCountersCmd)
        button.show()      

        button = gtk.Button("No Op")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, CI_NoOpCmd)
        button.show()

        bbox.show()

        label = gtk.Label("CI CMDS")
        notebook.append_page(frame, label)

        #
        # End defintion for CI command page
        #


        #
        # ES Command Page
        #
        frame = gtk.Frame("ES Commands")
        frame.set_border_width(BorderWidth)
        frame.set_size_request(FrameSizeWidth, FrameSizeHeight)
        frame.show()

        bbox = gtk.HButtonBox()
        bbox.set_border_width(BBoxBorderWidth)
        frame.add(bbox)
        
  
        button = gtk.Button("Reset")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, ES_ResetCountersCmd)
        button.show()  

        button = gtk.Button("Query All")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, ES_QueryAllCmd)
        button.show()  

        button = gtk.Button("Write Log")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, ES_WriteXLogCmd)
        button.show()  

        button = gtk.Button("Clear Log")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, ES_ClearXLogCmd)
        button.show() 
        button = gtk.Button("No Op")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, ES_NoOpCmd)
        button.show()

        bbox.show()

        label = gtk.Label("ES CMDS")
        notebook.append_page(frame, label)

        #
        # End defintion for ES command page
        #



        #
        # EVS Command Page
        #
        frame = gtk.Frame("EVS Commands")
        frame.set_border_width(BorderWidth)
        frame.set_size_request(FrameSizeWidth, FrameSizeHeight)
        frame.show()

        bbox = gtk.HButtonBox()
        bbox.set_border_width(BBoxBorderWidth)
        frame.add(bbox)
        
  
        button = gtk.Button("Reset")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, EVS_ResetCountersCmd)
        button.show()  

        button = gtk.Button("Enable Debug")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, EVS_EnableDebugCmd)
        button.show()  


        button = gtk.Button("Write Log")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, EVS_WriteLogCmd)
        button.show()  

        button = gtk.Button("No Op")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, EVS_NoOpCmd)
        button.show()

        bbox.show()

        label = gtk.Label("EVS CMDS")
        notebook.append_page(frame, label)

        #
        # End defintion for EVS command page
        #


        #
        # SB Command Page
        #
        frame = gtk.Frame("SB Commands")
        frame.set_border_width(BorderWidth)
        frame.set_size_request(FrameSizeWidth, FrameSizeHeight)
        frame.show()

        bbox = gtk.HButtonBox()
        bbox.set_border_width(BBoxBorderWidth)
        frame.add(bbox)
        
  
        button = gtk.Button("Reset")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, SB_ResetCountersCmd)
        button.show()  

        button = gtk.Button("Send Stats")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, SB_SendStatsCmd)
        button.show()  

        button = gtk.Button("Send Routing")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, SB_SendRoutingInfoCmd)
        button.show() 

        button = gtk.Button("No Op")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, SB_NoOpCmd)
        button.show()

        bbox.show()

        label = gtk.Label("SB CMDS")
        notebook.append_page(frame, label)

        #
        # End defintion for SB command page
        #

        #
        # TIME Command Page
        #
        frame = gtk.Frame("TIME Commands")
        frame.set_border_width(BorderWidth)
        frame.set_size_request(FrameSizeWidth, FrameSizeHeight)
        frame.show()

        bbox = gtk.HButtonBox()
        bbox.set_border_width(BBoxBorderWidth)
        frame.add(bbox)
        
  
        button = gtk.Button("Reset")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, TIME_ResetCountersCmd)
        button.show()  

        button = gtk.Button("Add Adj.")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, TIME_AddAdjustCmd)
        button.show()  

        button = gtk.Button("Req. Diag.")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, TIME_ReqDiagCmd)
        button.show() 

        button = gtk.Button("No Op")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, TIME_NoOpCmd)
        button.show()

        bbox.show()

        label = gtk.Label("TIME CMDS")
        notebook.append_page(frame, label)

        #
        # End defintion for TIME command page
        #

        #
        # TBL Command Page
        #
        frame = gtk.Frame("TBL Commands")
        frame.set_border_width(BorderWidth)
        frame.set_size_request(FrameSizeWidth, FrameSizeHeight)
        frame.show()

        bbox = gtk.HButtonBox()
        bbox.set_border_width(BBoxBorderWidth)
        frame.add(bbox)
        
  
        button = gtk.Button("Reset")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, TBL_ResetCountersCmd)
        button.show()  


        button = gtk.Button("No Op")
        bbox.add(button)
        button.connect("clicked", self.send_cmd, TBL_NoOpCmd)
        button.show()

        bbox.show()

        label = gtk.Label("TBL CMDS")
        notebook.append_page(frame, label)

        #
        # End defintion for TBL command page
        #

       
        #---------------------------------------------------------
        # Buttons on the bottom of the window
        #
        button = gtk.Button("close")
        button.connect("clicked", self.delete)
        table.attach(button, 0,1,1,2)
        button.show()

        #button = gtk.Button("next page")
        #button.connect("clicked", notebook.next_page)
        #table.attach(button, 1,2,1,2)
        #button.show()

        #button = gtk.Button("prev page")
        #button.connect("clicked", notebook.prev_page)
        #table.attach(button, 2,3,1,2)
        #button.show()

        table.show()
        window.show()

def main():
    gtk.main()
    return 0

if __name__ == "__main__":
    CommandModule()
    main()

