import os
import ctypes

CLIB_POSSIBLE_PATHS = [
    '/usr/lib/libslope.so',
    '/usr/lib/libslope0.1.so',
    '/usr/local/lib/libslope.so',
    '/usr/local/lib/libslope0.1.so',
    '../build/libslope.so']

class SlopeNotFound(Exception):
    def __str__(self):
        return 'PySlope couldn\'t find slope shared library'

class CSlopeLibObj(object):
    def __init__(self):
        self.clib_path = None
        self.clib = None
        try:
            for path in CLIB_POSSIBLE_PATHS:
                if os.path.exists(path):
                        self.clib_path = path
            if self.clib_path == None:
                raise SlopeNotFound()
        except SlopeNotFound as e:
            print e
        # if everything is ok, load slope
        self.clib = ctypes.cdll.LoadLibrary(self.clib_path)

cslope_obj = CSlopeLibObj()
cslope = cslope_obj.clib
cslope_path = cslope_obj.clib_path

