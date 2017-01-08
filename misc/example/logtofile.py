
import logging

formatter = logging.Formatter('%(asctime)s %(levelname)s: %(message)s', datefmt = '%b %d, %Y %Hh%M')

fileHandler = logging.FileHandler('messages.log', 'w')
fileHandler.setFormatter(formatter)
fileHandler.setLevel(logging.WARNING)

logger = logging.getLogger()
logger.addHandler(fileHandler)
