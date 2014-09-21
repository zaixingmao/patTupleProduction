#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "RecoTauTag/TauTagTools/plugins/PFTauSelectorDefinition2.h"

typedef ObjectSelector<PFTauSelectorDefinition2> PFTauSelector2;

DEFINE_FWK_MODULE(PFTauSelector2);
