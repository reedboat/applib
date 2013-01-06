import string
import os

class CatNameFilePair:
	def __Init__(self):
		self.catName=''
		self.fileName=''


class FileListProducer:
	catNameFileList=[] #to store the:"catname:filename"pair
	RootDir=''

	TrainingSetList=[]#for training set
	TestSetList=[]#for test set

	def ReadDir(self,rootDir):
		del self.catNameFileList[:]

		self.SetRoot(rootDir)
		for dirItem in os.listdir(self.RootDir):
			#print dirItem
			#print self.RootDir+dirItem
			if os.path.isdir(self.RootDir+dirItem):
				fileList=self.ReadFiles(self.RootDir+dirItem)
				#print len(fileList)

				for element in fileList:
					nfPair=CatNameFilePair()
					nfPair.catName=dirItem
					nfPair.fileName=self.RootDir+dirItem+'/'+element
					if  nfPair.fileName.find('../')==0:
						size=len(nfPair.fileName)
						nfPair.fileName=nfPair.fileName[3:size]
					
					#print 'nfPair:'+nfPair.fileName

					self.catNameFileList.append(nfPair)

		return self.catNameFileList



	def ReadFiles(self,dirItem):
		resultList=[]
		
		for fileItem in os.listdir(dirItem):
			#print fileItem
			#print dirItem+fileItem
			if os.path.isfile(dirItem+'/'+fileItem):
				#print 'appending'+fileItem
				resultList.append(fileItem)

		return resultList

	def ReadDirSplit(self,rootDir,ratio):
		self.ReadDir(rootDir)
		self.SplitTrainingSet(ratio)


	def SplitTrainingSet(self,ratio):
		eachK=int(1.0/(1.0-ratio))
		counter=0
		for element in self.catNameFileList:
			if counter%eachK==0:#for test set
				self.TestSetList.append(element)
			else:#for training set
				self.TrainingSetList.append(element)

			counter+=1


	def GetTrainingSet(self):
		return self.TrainingSetList

	def GetTestSet(self):
		return self.TestSetList


	
	def SetRoot(self,dir):
		self.RootDir=dir

	
	def GetRoot(self):
		return self.RootDir




