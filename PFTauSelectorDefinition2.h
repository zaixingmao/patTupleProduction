#ifndef RecoTauTag_TauTagTools_PFTauSelectorDefinition2
#define RecoTauTag_TauTagTools_PFTauSelectorDefinition2

#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include <memory>
#include <boost/foreach.hpp>

#include <iostream>

struct PFTauSelectorDefinition2 {

  typedef reco::PFTauCollection collection;
  typedef edm::Handle< collection > HandleToCollection;
  typedef std::vector< const reco::PFTau *> container;
  typedef container::const_iterator const_iterator;

  struct DiscCutPair {
    edm::Handle<reco::PFTauDiscriminator> handle;
    edm::InputTag inputTag;
    double cut;
  };
  typedef std::vector<DiscCutPair> DiscCutPairVec;

  PFTauSelectorDefinition2 (const edm::ParameterSet &cfg) {
    std::vector<edm::ParameterSet> discriminators =
      cfg.getParameter<std::vector<edm::ParameterSet> >("discriminators");
    // Build each of our cuts
    BOOST_FOREACH(const edm::ParameterSet &pset, discriminators) {
      DiscCutPair newCut;
      newCut.inputTag = pset.getParameter<edm::InputTag>("discriminator");
      newCut.cut = pset.getParameter<double>("selectionCut");
      discriminators_.push_back(newCut);
    }

    // Build a string cut if desired
    if (cfg.exists("cut")) {
      cut_.reset(new StringCutObjectSelector<reco::PFTau>(
            cfg.getParameter<std::string>( "cut" )));
    }
  }

  const_iterator begin() const { return selected_.begin(); }
  const_iterator end() const { return selected_.end(); }

  void select(const HandleToCollection & hc, const edm::Event & e,
      const edm::EventSetup& s) {
    selected_.clear();

    if (!hc.isValid()) {
      throw cms::Exception("PFTauSelectorBadHandle")
        << "an invalid PFTau handle with ProductID"
        << hc.id() << " passed to PFTauSelector.";
    }

    // Load each discriminator
    BOOST_FOREACH(DiscCutPair &disc, discriminators_) {
      //std::cout << disc.inputTag << std::endl;
      e.getByLabel(disc.inputTag, disc.handle);
    }

    //std::cout << "\n\n\n" << e.id() << std::endl;
    const size_t nTaus = hc->size();
    for (size_t iTau = 0; iTau < nTaus; ++iTau) {
      bool passed = true;
      reco::PFTauRef tau(hc, iTau);

      if (cut_.get() && !(*cut_)(*tau)) {
	continue;
      }

      //std::cout << "iTau " << iTau << "   " << (*tau) << std::endl;
      // Check if it passed all the discrimiantors
      BOOST_FOREACH(const DiscCutPair &disc, discriminators_) {
	//std::cout << disc.inputTag << ", "  << (*disc.handle)[tau] << " ";
        // Check this discriminator passes
	if (disc.cut < (*disc.handle)[tau]) {
          passed = false;
          break;
        }
      }

      //std::cout << "overall passed?: " << passed << std::endl << std::endl;
      if (passed)
        selected_.push_back(tau.get());
    }
  } // end select()

  size_t size() const { return selected_.size(); }

 private:
  container selected_;
  DiscCutPairVec discriminators_;
  std::auto_ptr<StringCutObjectSelector<reco::PFTau> > cut_;

};

#endif
