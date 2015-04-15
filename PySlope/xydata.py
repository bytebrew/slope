import data
from cslope import cslope as c
import ctypes

class XyData(data.Data):

    def __init__(self, x, y, name='data',
                 color=data.BLUE, symbol=data.LINE):
        n = len(x)
        self.x_arr = (ctypes.c_double *n)(*x)
        self.y_arr = (ctypes.c_double *n)(*y)
        self.cobj = c.slope_xydata_create_simple(
            self.x_arr,self.y_arr,n,name,color,symbol)

    def set_antialias(self, on):
        if on:
            c.slope_xydata_set_antialias(self.cobj,1)
        else:
            c.slope_xydata_set_antialias(self.cobj,0)

