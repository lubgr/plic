
import logging

formatter = logging.Formatter('%(name)s : %(message)s')

fileHandler = logging.FileHandler('misc/output/testfromc.log', 'w')
fileHandler.setFormatter(formatter)

logger01 = logging.getLogger('c.sublogger')
logger01.addHandler(fileHandler)
logger01.setLevel(logging.INFO)
