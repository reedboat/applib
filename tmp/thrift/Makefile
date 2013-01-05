#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements. See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership. The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License. You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied. See the License for the
# specific language governing permissions and limitations
# under the License.
#

#BOOST_DIR = /usr/local/boost/include/boost-1_33_1/
BOOST_DIR = /usr/local/boost/include/boost-1_44_0/
THRIFT_DIR = /usr/local/include/thrift
LIB_DIR = /usr/local/lib
MY_DIR=./include
SEG_INCLUDE=../../Segment/include
SEG_LIB=../../Segment/lib
SEG_SEG_LIB=-llexicon -ltouni -lfromuni -lcommon -lustr -lpthread -ldl -lstdc++


GEN_SRC = ./gen-cpp/CatText.cpp ./gen-cpp/Cat_types.cpp ./gen-cpp/Cat_constants.cpp

MY_SRC=../src/CatIdMap.cpp  ../src/SuperString.cpp ../src/Config.cpp ../src/KNN.cpp  ../src/SegmentSina.cpp  ../src/FeatureConv.cpp  ../src/Timer.cpp

default: server  

server: CatTextServer.cpp
	g++ -m32 -O2 -o CatServer -I${THRIFT_DIR} -I${BOOST_DIR} -I${MY_DIR} -I${SEG_INCLUDE}  -I../gen-cpp -L${LIB_DIR}  -lthrift -L${SEG_LIB}  CatTextServer.cpp ${GEN_SRC} ${MY_SRC}  ${SEG_SEG_LIB}



