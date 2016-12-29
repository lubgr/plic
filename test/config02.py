
import logging

formatter = logging.Formatter('%(name)s - %(levelname)s - %(message)s')

fileHandler = logging.FileHandler('misc/output/test02.log', 'w')
fileHandler.setFormatter(formatter)

logger01 = logging.getLogger('test')
logger01.addHandler(fileHandler)
logger01.setLevel(logging.WARNING)
