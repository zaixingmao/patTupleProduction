####in .bashrc
```bash
export CMSSW_GIT_REFERENCE=/nfs_scratch/$USER/.cmsgit-cache
```
####cmssw
```bash
scram project CMSSW_5_3_14
cd CMSSW_5_3_14/src
cmsenv

git cms-init --https
git clone https://github.com/zaixingmao/patTupleProduction.git
```

####choose an ID
######old
```bash
git clone --recursive https://github.com/elaird/FinalStateAnalysis.git
cd FinalStateAnalysis
git checkout dev
cd recipeGIT/
PATPROD=1 LUMI=1 LIMITS=0 ./recipe.sh
cd ../../
```

######new
```bash
git clone --recursive https://github.com/uwcms/FinalStateAnalysis.git
cd FinalStateAnalysis
cd recipe/
PATPROD=1 LUMI=1 LIMITS=0 ./recipe.sh
cd ../../
```

####build
```bash
cp patTupleProduction/uwSkims_cfi.py FinalStateAnalysis/RecoTools/python
cp patTupleProduction/PFTauSelectorDefinition2.h RecoTauTag/TauTagTools/plugins/
cp patTupleProduction/PFTauSelector2.cc RecoTauTag/TauTagTools/plugins/

export USER_CXXFLAGS="-Wno-delete-non-virtual-dtor -Wno-error=unused-but-set-variable -Wno-error=unused-variable -Wno-error=sign-compare -Wno-error=reorder"
scram b -j 8

source ./FinalStateAnalysis/environment.sh
```

####local jobs
```bash
cd FinalStateAnalysis/PatTools/test
./patTuple_cfg.py isMC=0 globalTag=$datagt inputFiles=file:/hdfs/store/data/Run2012A/Tau/AOD/22Jan2013-v1/20000/7CD7448E-9E79-E211-B184-00266CF9B254.root maxEvents=1000 outputFile=myTestFile.root
```

####to submit jobs
```bash
# edit MetaData/python/data8TeVNew.py to include relevant samples
# NOTE! data samples must start with 'data_'
cd FinalStateAnalysis/PatTools/test/
python submit_tuplization.py arbitrary_name_for_subdirectory --samples "W*JetsToLNu*" > WJets
# check that isMC and globalTag are appropriate
source WJets
```
