from cslope import cslope as c

# named colors
BLACK  = 0
WHITE  = 1
RED    = 2
GREEN  = 3
BLUE   = 4

# symbols to plot data points
LINE      = 0
CIRCLES   = 1
TRIANGLES = 2
SQUARES   = 3
PLUSSES   = 4

class Data(object):
    
    def __init__(self):
        self.cobj = None
    
    def set_name(self, name):
        c.slope_data_set_name(self.cobj, name)
    
    def set_visible(self, on):
        if on:
            c.slope_data_set_visible(self.cobj, 1)
        else:
            c.slope_data_set_visible(self.cobj, 0)

