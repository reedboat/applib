#!/bin/bash

cmd=g++-4.1
segment_dir=../Segment

build_feature(){
    $cmd -m32 -o FeatureSelector src/ProgSelectFeat.cpp src/Filter.cpp src/Config.cpp src/CatIdMap.cpp src/ChiSquare.cpp  src/FeatureConv.cpp src/FeatureSelection.cpp src/SegmentSina.cpp src/SuperString.cpp src/TrainingDocParser.cpp src/TrainingSetManager.cpp libs/libjson.a -I $segment_dir/include/ -L $segment_dir/lib -llexicon -ltouni -lfromuni -lcommon -lustr -lpthread -ldl -lstdc++
}

build_pushdrag(){
    $cmd -m32 -o pushdrag src/ProgPushDragModel.cpp src/Filter.cpp src/Config.cpp src/CatIdMap.cpp src/ChiSquare.cpp  src/FeatureConv.cpp src/FeatureSelection.cpp src/SegmentSina.cpp src/SuperString.cpp src/TrainingDocParser.cpp src/TrainingSetManager.cpp libs/libjson.a -I $segment_dir/include/ -L$segment_dir/lib -llexicon -ltouni -lfromuni -lcommon -lustr -lpthread -ldl -lstdc++
}

clean(){
    rm FeatureSelector -f
    rm pushdrag -f
}

case $1 in
FeatureSelector)
    build_feature
    ;;
pushdrag)
    build_pushdrag
    ;;
*)
    clean
    build_feature
    build_pushdrag
    ;;
esac
