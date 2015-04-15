#!/usr/bin/env python

import simple_charts as sc
import data

x = [ 0, 1, 2, 3, 4, 5]
y = [ 0, 1, 2, 3, 4, 5]
ysqr = [ 0, 1, 4, 9, 16, 25]

chart = sc.XyChart(
    'Compare functions',
    'X values',
    'Identity and Squares')

chart.new_graph(x, y, 'ident', data.RED, data.CIRCLES)
chart.new_graph(x, ysqr, 'sqr', data.BLUE)

chart.save()

