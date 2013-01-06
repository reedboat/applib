from fileListProducer import *


trainingFiles=[]#traing set
testingFiles=[] #test set
#rootDir='../data/traindata/'#root dir for training set
rootDir='../data/traindata/'#root dir for training set

trainingSetRatio=0.8#80% training set used for training


fileReader=FileListProducer()
fileReader.ReadDirSplit(rootDir,trainingSetRatio)

trainingFiles=fileReader.GetTrainingSet()
testingFiles=fileReader.GetTestSet()


outputTrainFileName='../etc/filelist.txt'
outputTestFileName='../etc/testlist.txt'

fileHandle=open(outputTrainFileName,'w')
fileHandle2=open(outputTestFileName,'w')

#outputTestFileName='../etc/testlist.txt'


for element in trainingFiles:
	#print element.catName
	#print element.fileName
	#print '_______________________'
	oneLine=element.catName+'\t'+element.fileName+'\n'
	fileHandle.writelines(oneLine)


fileHandle.close()
print 'after writing the filelist.txt under etc/'


for element in testingFiles:
	 oneLine=element.catName+'\t'+element.fileName+'\n'
	 fileHandle2.writelines(oneLine)

fileHandle2.close()                                                                
print 'after writing the testlist.txt under etc/' 
