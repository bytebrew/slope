import scene
import metrics, xymetrics
import data, xydata


class XyChart(object):

    def __init__(self, title='SLOPE CHART',
                 xlabel='X', ylabel='Y', rightlabel=''):
        self._title = title
        self._scene = scene.Scene()
        self._metrics = xymetrics.XyMetrics()
        self._data = {}
        self._metrics.get_axis(xymetrics.TOP_AXIS).set_name(title)
        self._metrics.get_axis(xymetrics.BOTTOM_AXIS).set_name(xlabel)
        self._metrics.get_axis(xymetrics.LEFT_AXIS).set_name(ylabel)
        self._metrics.get_axis(xymetrics.RIGHT_AXIS).set_name(rightlabel)
        self._scene.add_metrics(self._metrics)


    def new_graph(self, x, y, name, color=data.BLUE,
                  symbol=data.LINE):
        data = xydata.XyData(x,y,name,color,symbol)
        self._metrics.add_data(data)
        self._data[name] = data
        return self

    def graph(self, name):
        return self._data[name]

    def save(self, filename=None, width=600, height=450):
        if not filename:
            filename = self._title + '.png'
        self._scene.write_to_png(filename,width,height)
        
    def axis(self, name):
        if name == 'top':
            return self._metrics.get_axis(xymetrics.TOP_AXIS)
        elif name == 'left':
            return self._metrics.get_axis(xymetrics.LEFT_AXIS)
        elif name == 'bottom':
            return self._metrics.get_axis(xymetrics.BOTTOM_AXIS)
        elif name == 'right':
            return self._metrics.get_axis(xymetrics.RIGHT_AXIS)
        else:
            raise Exception('axis not valid')

