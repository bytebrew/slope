import metrics, data
from cslope import cslope as c

TOP_AXIS    = 0
BOTTOM_AXIS = 1
LEFT_AXIS   = 2
RIGHT_AXIS  = 3

class XyMetrics(metrics.Metrics):

    def __init__(self):
        self.cobj = c.slope_xymetrics_create()

    def add_data(self, data):
        c.slope_metrics_add_data(self.cobj,data.cobj)
    
    def get_axis(self, ax):
        ax_cobj = c.slope_xymetrics_get_axis(self.cobj,ax)
        return XyAxis(ax_cobj)


class XyAxis(data.Data):

    def __init__(self, cobj):
        self.cobj = cobj

