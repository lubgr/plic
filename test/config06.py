
import logging

formatter = logging.Formatter('%(clineno)d: %(message)s')

fileHandler = logging.FileHandler('misc/output/test06.log', 'w')
fileHandler.setFormatter(formatter)

logger01 = logging.getLogger('test')
logger01.addHandler(fileHandler)
logger01.setLevel(logging.DEBUG)
