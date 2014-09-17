in .bashrc do: export CMSSW_GIT_REFERENCE=/nfs_scratch/$USER/.cmsgit-cache

scram project CMSSW_5_3_14
cd CMSSW_5_3_14/src
cmsenv

git clone https://github.com/zaixingmao/patTupleProduction.git

git cms-init
git clone --recursive https://github.com/uwcms/FinalStateAnalysis.git

cd FinalStateAnalysis
cd recipe/
PATPROD=1 LUMI=1 LIMITS=0 ./recipe.sh
cd ../../

cp patTupleProduction/uwSkims_cfi.py FinalStateAnalysis/RecoTools/python

export USER_CXXFLAGS="-Wno-delete-non-virtual-dtor -Wno-error=unused-but-set-variable -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=reorder"
scram b -j 8

source ./FinalStateAnalysis/environment.sh