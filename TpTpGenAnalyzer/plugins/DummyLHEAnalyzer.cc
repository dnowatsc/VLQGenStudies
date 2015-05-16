#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include <iomanip>
#include <iostream>
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "TH2.h"
#include "TH1.h"
#include "TLorentzVector.h"

using namespace std;
using namespace edm;
using namespace lhef;

class DummyLHEAnalyzer : public EDAnalyzer {
private: 
  bool dumpLHE_;
  bool checkPDG_;
public:
  explicit DummyLHEAnalyzer( const ParameterSet & cfg ) : 
    src_( cfg.getParameter<InputTag>( "src" ) )
  {
  }
private:
  std::map< std::string, TH1D* > histos1D_;

  int checkNof(int pdgid, const std::vector<int> idup_)
  {
    int n=0;
    for(auto id : idup_)
    {
      if (abs(id)==pdgid) n++;
    }
    return n;
  }

  std::pair<int, int> checkDau(int mpdg, const std::vector<int> idup_, const std::vector< std::pair< int, int > > mothup_)
  {
    for (unsigned int m_pos=0; m_pos<idup_.size(); m_pos++)
    {
      if (idup_[m_pos]==mpdg)
      {
        vector<int> dau;
        for(unsigned int i=0; i<idup_.size(); i++)
        {
          if (((unsigned int) (mothup_[i].first-1)==m_pos) || (((unsigned int)(mothup_[i].second-1)==m_pos))) dau.push_back(idup_[i]);
        }
        if (dau.size() == 2)
            return std::make_pair(dau[0], dau[1]);
        else
            return std::make_pair(-1, -1);
      }
    }
    return std::make_pair(-1,-1);
  }

  void analyze( const Event & iEvent, const EventSetup & iSetup ) override {

    Handle<LHEEventProduct> evt;
    iEvent.getByLabel( src_, evt );

    //int zprimepdg=9900113;
    int tprimepdg=8000001;

    const lhef::HEPEUP hepeup_ = evt->hepeup();

    const int nup_ = hepeup_.NUP; 
    const std::vector<int> idup_ = hepeup_.IDUP;
    const std::vector<lhef::HEPEUP::FiveVector> pup_ = hepeup_.PUP;
    const std::vector< std::pair< int, int > > mothup_ = hepeup_.MOTHUP;

    float ld_mu_pt = 0.f;
    float ld_mu_eta = 0.f;
    float ld_mu_phi = 0.f;

    for ( unsigned int icount = 0 ; icount < (unsigned int)nup_; icount++ ) {

       TLorentzVector part4vec((pup_[icount])[0], (pup_[icount])[1], (pup_[icount])[2], (pup_[icount])[3]);
       
       if (abs(idup_[icount])==13) {
           if (!ld_mu_pt || part4vec.Pt() > ld_mu_pt) {
               ld_mu_pt = part4vec.Pt();
               ld_mu_eta = part4vec.Eta();
               ld_mu_phi = part4vec.Phi();
           }
       }

       if (abs(idup_[icount])==6) histos1D_[ "topPt" ]->Fill(part4vec.Pt());
       if (abs(idup_[icount])==24) histos1D_[ "wPt" ]->Fill(part4vec.Pt());
       if (abs(idup_[icount])==23) histos1D_[ "zPt" ]->Fill(part4vec.Pt());
       if ((abs(idup_[mothup_[icount].first-1])==6) && (abs(idup_[icount])==5)) histos1D_[ "btPt" ]->Fill(part4vec.Pt());
       if (abs(idup_[icount])==tprimepdg) histos1D_[ "tprimePt" ]->Fill(part4vec.Pt());
       if (abs(idup_[icount])==25) histos1D_[ "hPt" ]->Fill(part4vec.Pt());

       if (abs(idup_[icount])==6) histos1D_[ "topEta" ]->Fill(part4vec.Eta());
       if (abs(idup_[icount])==24) histos1D_[ "wEta" ]->Fill(part4vec.Eta());
       if (abs(idup_[icount])==23) histos1D_[ "zEta" ]->Fill(part4vec.Eta());
       if ((abs(idup_[mothup_[icount].first-1])==6) && (abs(idup_[icount])==5)) histos1D_[ "btEta" ]->Fill(part4vec.Eta());
       if (abs(idup_[icount])==tprimepdg) histos1D_[ "tprimeEta" ]->Fill(part4vec.Eta());
       if (abs(idup_[icount])==25) histos1D_[ "hEta" ]->Fill(part4vec.Eta());

       if (abs(idup_[icount])==6) histos1D_[ "topPhi" ]->Fill(part4vec.Phi());
       if (abs(idup_[icount])==24) histos1D_[ "wPhi" ]->Fill(part4vec.Phi());
       if (abs(idup_[icount])==23) histos1D_[ "zPhi" ]->Fill(part4vec.Phi());
       if ((abs(idup_[mothup_[icount].first-1])==6) && (abs(idup_[icount])==5)) histos1D_[ "btPhi" ]->Fill(part4vec.Phi());
       if (abs(idup_[icount])==tprimepdg) histos1D_[ "tprimePhi" ]->Fill(part4vec.Phi());
       if (abs(idup_[icount])==25) histos1D_[ "hPhi" ]->Fill(part4vec.Phi());

       int mother1 = idup_[mothup_[icount].first-1];
       int mother2 = idup_[mothup_[icount].second-1];
       mother1 = abs(mother1) != tprimepdg ? mother1 : (mother1 < 0 ? -8 : 8);
       mother2 = abs(mother2) != tprimepdg ? mother2 : (mother2 < 0 ? -8 : 8);

       if (abs(idup_[icount])==6) histos1D_[ "topMothers" ]->Fill(mother1);
       if (abs(idup_[icount])==6) histos1D_[ "topMothers" ]->Fill(mother2);
       if (abs(idup_[icount])==24) histos1D_[ "wMothers" ]->Fill(mother1);
       if (abs(idup_[icount])==24) histos1D_[ "wMothers" ]->Fill(mother2);
       if (abs(idup_[icount])==23) histos1D_[ "zMothers" ]->Fill(mother1);
       if (abs(idup_[icount])==23) histos1D_[ "zMothers" ]->Fill(mother2);
       if ((abs(idup_[mothup_[icount].first-1])==6) && (abs(idup_[icount])==5)) histos1D_[ "btMothers" ]->Fill(mother1);
       if ((abs(idup_[mothup_[icount].first-1])==6) && (abs(idup_[icount])==5)) histos1D_[ "btMothers" ]->Fill(mother2);
       if (abs(idup_[icount])==tprimepdg) histos1D_[ "tprimeMothers" ]->Fill(mother1);
       if (abs(idup_[icount])==tprimepdg) histos1D_[ "tprimeMothers" ]->Fill(mother2);
       if (abs(idup_[icount])==25) histos1D_[ "hMothers" ]->Fill(mother1);
       if (abs(idup_[icount])==25) histos1D_[ "hMothers" ]->Fill(mother2);

       if (idup_[icount]==6) {
           histos1D_[ "topDaughters" ]->Fill(checkDau(6, idup_, mothup_).first);
           histos1D_[ "topDaughters" ]->Fill(checkDau(6, idup_, mothup_).second);
       }
       if (idup_[icount]==-6) {
           histos1D_[ "topDaughters" ]->Fill(checkDau(-6, idup_, mothup_).first);
           histos1D_[ "topDaughters" ]->Fill(checkDau(-6, idup_, mothup_).second);
       }
       if (idup_[icount]==tprimepdg) {
           histos1D_[ "tprimeDaughters" ]->Fill(checkDau(tprimepdg, idup_, mothup_).first);
           histos1D_[ "tprimeDaughters" ]->Fill(checkDau(tprimepdg, idup_, mothup_).second);
       }
       if (idup_[icount]==-tprimepdg) {
           histos1D_[ "tprimeDaughters" ]->Fill(checkDau(-tprimepdg, idup_, mothup_).first);
           histos1D_[ "tprimeDaughters" ]->Fill(checkDau(-tprimepdg, idup_, mothup_).second);
       }
       if (idup_[icount]==24) {
           int decay_mode = abs(checkDau(24, idup_, mothup_).first)+abs(checkDau(24, idup_, mothup_).second);
           if (decay_mode < 20) histos1D_[ "wHadToNonHad" ]->Fill(1);
           else histos1D_[ "wHadToNonHad" ]->Fill(0);
           histos1D_[ "wDecayModes" ]->Fill(decay_mode);
           histos1D_[ "wDaughters" ]->Fill(checkDau(24, idup_, mothup_).first);
           histos1D_[ "wDaughters" ]->Fill(checkDau(24, idup_, mothup_).second);
       }
       if (idup_[icount]==-24) {
           int decay_mode = abs(checkDau(-24, idup_, mothup_).first)+abs(checkDau(-24, idup_, mothup_).second);
           histos1D_[ "wDecayModes" ]->Fill(decay_mode);
           if (decay_mode < 20) histos1D_[ "wHadToNonHad" ]->Fill(1);
           else histos1D_[ "wHadToNonHad" ]->Fill(0);
           histos1D_[ "wDaughters" ]->Fill(checkDau(-24, idup_, mothup_).first);
           histos1D_[ "wDaughters" ]->Fill(checkDau(-24, idup_, mothup_).second);
       }

       // ONLY FOR VARIAL WEBCREATOR TO ALSO CREATE Z PT PLOT
       if (abs(idup_[icount])==23) histos1D_[ "zzz" ]->Fill(1);

       if (abs(idup_[icount])==6) histos1D_[ "topM" ]->Fill((pup_[icount])[4]);
       if (abs(idup_[icount])==tprimepdg) histos1D_[ "tprimeM" ]->Fill((pup_[icount])[4]);

       if (abs(idup_[icount])==25) histos1D_[ "hM" ]->Fill((pup_[icount])[4]);

    }

    if (ld_mu_pt) {
        histos1D_[ "wMuPt" ]->Fill(ld_mu_pt);
        histos1D_[ "wMuEta" ]->Fill(ld_mu_eta);
        histos1D_[ "wMuPhi" ]->Fill(ld_mu_phi);
    }

    if( evt->weights().size() ) {
      //std::cout << "weights:" << std::endl;
      for ( size_t iwgt = 0; iwgt < evt->weights().size(); ++iwgt ) {
	const LHEEventProduct::WGT& wgt = evt->weights().at(iwgt);
	//std::cout << "\t" << wgt.id << ' ' 
		//  << std::scientific << wgt.wgt << std::endl;
      }
    }

  }

  
  void beginRun(edm::Run const& iRun, edm::EventSetup const& es) override {
edm::Service< TFileService > fileService;
//bW and generic
  histos1D_[ "topPt" ] = fileService->make< TH1D >( "topPt", ";top p_{T} [GeV];Events", 200, 0., 2000);
  histos1D_[ "tprimePt" ] = fileService->make< TH1D >( "tprimePt", ";T' p_{T} [GeV];Events", 200, 0., 2000);
  histos1D_[ "wPt" ] = fileService->make< TH1D >( "wPt", ";W p_{T} [GeV];Events", 200, 0., 2000);
  histos1D_[ "zPt" ] = fileService->make< TH1D >( "zPt", ";Z p_{T} [GeV];Events", 200, 0., 2000);
  histos1D_[ "btPt" ] = fileService->make< TH1D >( "btPt", ";b from t p_{T} [GeV];Events", 200, 0., 2000);
  histos1D_[ "hPt" ] = fileService->make< TH1D >( "hPt", ";h p_{T} [GeV];Events", 200, 0., 2000);
  histos1D_[ "wMuPt" ] = fileService->make< TH1D >( "wMuPt", ";W Leading Muon p_{T} [GeV];Events", 200, 0., 2000);

  histos1D_[ "topEta" ] = fileService->make< TH1D >( "topEta", ";top Eta;Events", 140, -7, 7);
  histos1D_[ "tprimeEta" ] = fileService->make< TH1D >( "tprimeEta", ";T' Eta;Events", 140, -7, 7);
  histos1D_[ "wEta" ] = fileService->make< TH1D >( "wEta", ";W Eta;Events", 140, -7, 7);
  histos1D_[ "zEta" ] = fileService->make< TH1D >( "zEta", ";Z Eta;Events", 140, -7, 7);
  histos1D_[ "btEta" ] = fileService->make< TH1D >( "btEta", ";b from t Eta;Events", 140, -7, 7);
  histos1D_[ "hEta" ] = fileService->make< TH1D >( "hEta", ";h Eta;Events", 140, -7, 7);
  histos1D_[ "wMuEta" ] = fileService->make< TH1D >( "wMuEta", ";W Leading Muon Eta;Events", 140, -7, 7);

  histos1D_[ "topPhi" ] = fileService->make< TH1D >( "topPhi", ";top Phi;Events", 63, -3.15, 3.15);
  histos1D_[ "tprimePhi" ] = fileService->make< TH1D >( "tprimePhi", ";T' Phi;Events", 63, -3.15, 3.15);
  histos1D_[ "wPhi" ] = fileService->make< TH1D >( "wPhi", ";W Phi;Events", 63, -3.15, 3.15);
  histos1D_[ "zPhi" ] = fileService->make< TH1D >( "zPhi", ";Z Phi;Events", 63, -3.15, 3.15);
  histos1D_[ "btPhi" ] = fileService->make< TH1D >( "btPhi", ";b from t Phi;Events", 63, -3.15, 3.15);
  histos1D_[ "hPhi" ] = fileService->make< TH1D >( "hPhi", ";h Phi;Events", 63, -3.15, 3.15);
  histos1D_[ "wMuPhi" ] = fileService->make< TH1D >( "wMuPhi", ";W Leading Muon Phi;Events", 63, -3.15, 3.15);

  histos1D_[ "topMothers" ] = fileService->make< TH1D >( "topMothers", ";top Mothers;Events", 61, -30.5, 30.5);
  histos1D_[ "tprimeMothers" ] = fileService->make< TH1D >( "tprimeMothers", ";T' Mothers;Events", 61, -30.5, 30.5);
  histos1D_[ "wMothers" ] = fileService->make< TH1D >( "wMothers", ";W Mothers;Events", 61, -30.5, 30.5);
  histos1D_[ "zMothers" ] = fileService->make< TH1D >( "zMothers", ";Z Mothers;Events", 61, -30.5, 30.5);
  histos1D_[ "btMothers" ] = fileService->make< TH1D >( "btMothers", ";b from t Mothers;Events", 61, -30.5, 30.5);
  histos1D_[ "hMothers" ] = fileService->make< TH1D >( "hMothers", ";h Mothers;Events", 61, -30.5, 30.5);

  histos1D_[ "topDaughters" ] = fileService->make< TH1D >( "topDaughters", ";top Daughters;Events", 61, -30.5, 30.5);
  histos1D_[ "tprimeDaughters" ] = fileService->make< TH1D >( "tprimeDaughters", ";T' Daughters;Events", 61, -30.5, 30.5);
  histos1D_[ "wDaughters" ] = fileService->make< TH1D >( "wDaughters", ";W Daughters;Events", 61, -30.5, 30.5);
  histos1D_[ "zDaughters" ] = fileService->make< TH1D >( "zDaughters", ";Z Daughters;Events", 61, -30.5, 30.5);
  histos1D_[ "wDecayModes" ] = fileService->make< TH1D >( "wDecayModes", ";W Decay Modes (sum of abs(pdgid));Events", 34, -0.5, 33.5);
  histos1D_[ "wHadToNonHad" ] = fileService->make< TH1D >( "wHadToNonHad", ";W Decay hadronic fraction (1=hadronic);Events", 2, -0.5, 1.5);

  histos1D_[ "topM" ] = fileService->make< TH1D >( "topM", ";top mass [GeV];Events", 100, 100, 200);
  histos1D_[ "tprimeM" ] = fileService->make< TH1D >( "tprimeM", ";T' mass [GeV];Events", 200, 0, 2000);
  histos1D_[ "hM" ] = fileService->make< TH1D >( "hM", ";H mass [GeV];Events", 100, 80, 180);

  histos1D_[ "zzz" ] = fileService->make< TH1D >( "zzz", "zzz", 100, 80, 180);
  }
  

  InputTag src_;
};

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE( DummyLHEAnalyzer );


