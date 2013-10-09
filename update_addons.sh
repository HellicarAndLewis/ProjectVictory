#!/bin/bash

# addons directory
ad=${PWD}/addons/  
ofad=${PWD}/../../addons/

if [ ! -d ${ad} ] ; then 
    mkdir ${ad}
fi

# 1 = git url
# 2 = sub directory
# 3 = revision to use
# 4 = branch to use

function clone {
    url=${1}
    dir=${2}
    rev=${3}
    branch=${4}
    if [ ! -d ${ad}/${dir} ] ; then 
        mkdir ${ad}/${dir}
        cd ${ad}/${dir}
        git clone ${url} .
    fi

    if [ -d ${ofad}/${dir} ] ; then 
        echo "********************************************************************************************************************************************************************"
        echo "It looks like you cloned the ${url} already in the addons directory of OF. This might cause compile errors because XCode gets confused about what headers to include!"
        echo "********************************************************************************************************************************************************************"
    fi

    cd ${ad}/${dir}
    git checkout ${branch}
    git reset --hard ${rev}
}

clone git@github.com:labatrockwell/ofxLibwebsockets.git ofxLibwebsockets d6e8bdaaab766887d256e52e76390c856f65b1de master
clone git@github.com:kylemcdonald/ofxCv.git ofxCv ab8dfe5ccdd792c2ca50952ebee42bf4d60b5734 master
clone git@github.com:patriciogonzalezvivo/ofxFluid.git ofxFluid 90d7b025988a85f3c3f338cf3fbab3cfd98cdebc master
clone git@github.com:patriciogonzalezvivo/ofxFX.git ofxFX ecf974c87b3c0cf2cd15c4b21f98fa8afe83f559 master
clone git@github.com:memo/ofxMSAObjCPointer.git ofxMSAObjCPointer 55cf29ec8e7ee964afc7db50d6e976c98584924e master
clone git@github.com:memo/ofxMSACore.git ofxMSACore e5c3b08e4a2a4d3aa63bb336a4329abf1e1dfe8c master
clone git@github.com:memo/ofxMSAPhysics.git ofxMSAPhysics df0ee5fd0cfcaaafe0c26d923ba7d454d535ca31 master
clone git@github.com:vanderlin/ofxBox2d.git ofxBox2d 170ea0e1d60705c651b009f4645a58d89a832f29 master
clone git@github.com:rezaali/ofxUI.git ofxUI f422be7a4fd7ddacf57511a0deadb6ddca7cf671 develop

