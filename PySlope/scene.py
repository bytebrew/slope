from cslope import cslope as c

class Scene(object):
    def __init__(self):
        self.cobj = c.slope_scene_create()

    def add_metrics(self, metrics):
        c.slope_scene_add_metrics(self.cobj,metrics.cobj)

    def write_to_png(self, filename, width=600, height=450):
        c.slope_scene_write_to_png(self.cobj,filename,width,height)

