# -*- coding: iso-8859-1 -*-

import logging

formatter = logging.Formatter('%(name)s : %(message)s')

fileHandler = logging.FileHandler('misc/output/test04.log', 'w')
fileHandler.setFormatter(formatter)

logger01 = logging.getLogger('test.test-äüöß')
logger01.addHandler(fileHandler)
logger01.setLevel(logging.INFO)
