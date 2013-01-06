from fileListProducer import *


result=[]
rootDir='../data/traindata/'#root dir for training set

fileReader=FileListProducer()
result=fileReader.ReadDir(rootDir)

outputFileName='../etc/filelist.txt'
fileHandle=open(outputFileName,'w')


for element in result:
	#print element.catName
	#print element.fileName
	#print '_______________________'
	oneLine=element.catName+'\t'+element.fileName+'\n'
	fileHandle.writelines(oneLine)


fileHandle.close()
print 'after writing the filelist.txt under etc/'
