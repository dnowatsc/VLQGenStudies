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

    // std::cout << "Number of particles = " << nup_ << std::endl;

    // if ( evt->pdf() != NULL ) {
    //   std::cout << "PDF scale = " << std::setw(14) << std::fixed << evt->pdf()->scalePDF << std::endl;  
    //   std::cout << "PDF 1 : id = " << std::setw(14) << std::fixed << evt->pdf()->id.first 
    //             << " x = " << std::setw(14) << std::fixed << evt->pdf()->x.first 
    //             << " xPDF = " << std::setw(14) << std::fixed << evt->pdf()->xPDF.first << std::endl;  
    //   std::cout << "PDF 2 : id = " << std::setw(14) << std::fixed << evt->pdf()->id.second 
    //             << " x = " << std::setw(14) << std::fixed << evt->pdf()->x.second 
    //             << " xPDF = " << std::setw(14) << std::fixed << evt->pdf()->xPDF.second << std::endl;  
    // }

//bW checks
//if(!(
//    checkNof(6,idup_)==1 &&
//    checkNof(zprimepdg,idup_)==1 &&
//    checkNof(tprimepdg,idup_)==1 &&
//    checkDau(zprimepdg,tprimepdg,6,idup_,mothup_) &&
//    checkDau(tprimepdg,5,24,idup_,mothup_) &&
//    (checkDau(6,5,24,idup_,mothup_) || checkDau(6,4,24,idup_,mothup_) ) 
//    )
//    ){
// std::cout<<"NO bW\n";
// std::cout<<"checkNof(6,idup_)==1 " << checkNof(6,idup_) <<std::endl;
// std::cout<<"checkNof(zprimepdg,idup_)==1 "<<checkNof(zprimepdg,idup_) <<std::endl;
// std::cout<<"checkNof(tprimepdg,idup_)==1 "<<checkNof(tprimepdg,idup_) <<std::endl;
// std::cout<<"checkDau(zprimepdg,tprimepdg,6,idup_,mothup_) "<<checkDau(zprimepdg,tprimepdg,6,idup_,mothup_) <<std::endl;
// std::cout<<"checkDau(tprimepdg,5,24,idup_,mothup_) "<<checkDau(tprimepdg,5,24,idup_,mothup_) <<std::endl;
// std::cout<<"checkDau(6,5,24,idup_,mothup_) "<<checkDau(6,5,24,idup_,mothup_) <<std::endl;
// std::cout<<"checkDau(6,4,24,idup_,mothup_) "<<checkDau(6,4,24,idup_,mothup_) <<std::endl;
// std::cout<<std::endl;
// for ( unsigned int icount = 0 ; icount < (unsigned int)nup_; icount++ ) {
//       std::cout << "# " << std::setw(14) << std::fixed << icount 
//                 << std::setw(14) << std::fixed << idup_[icount] 
//                 << std::setw(14) << std::fixed << (pup_[icount])[0] 
//                 << std::setw(14) << std::fixed << (pup_[icount])[1] 
//                 << std::setw(14) << std::fixed << (pup_[icount])[2] 
//                 << std::setw(14) << std::fixed << (pup_[icount])[3] 
//                 << std::setw(14) << std::fixed << (pup_[icount])[4]
//                 << std::setw(14) << std::fixed << mothup_[icount].first
//                 << std::setw(14) << std::fixed << mothup_[icount].second
//                 << std::endl;
//               }
//}
//tZ checks
//if(!(
//    checkNof(6,idup_)==2 &&
//    checkNof(zprimepdg,idup_)==1 &&
//    checkNof(tprimepdg,idup_)==1 &&
//    checkNof(23,idup_)==1 && //Z 25 for H
//    checkDau(zprimepdg,tprimepdg,6,idup_,mothup_) &&
//    checkDau(tprimepdg,6,23,idup_,mothup_) && //tZ
//    (checkDau(6,5,24,idup_,mothup_) || checkDau(6,4,24,idup_,mothup_) ) )
//    ){
// std::cout<<"NO tZ\n";
// std::cout<<"checkNof(6,idup_)==2 " << checkNof(6,idup_) <<std::endl;
// std::cout<<"checkNof(zprimepdg,idup_)==1 "<<checkNof(zprimepdg,idup_) <<std::endl;
// std::cout<<"checkNof(tprimepdg,idup_)==1 "<<checkNof(tprimepdg,idup_) <<std::endl;
// std::cout<<"checkNof(23,idup_)==1 "<<checkNof(23,idup_) <<std::endl;
// std::cout<<"checkDau(zprimepdg,tprimepdg,6,idup_,mothup_) "<<checkDau(zprimepdg,tprimepdg,6,idup_,mothup_) <<std::endl;
// std::cout<<"checkDau(tprimepdg,6,23,idup_,mothup_) "<<checkDau(tprimepdg,6,23,idup_,mothup_) <<std::endl;
// std::cout<<"checkDau(6,5,24,idup_,mothup_) "<<checkDau(6,5,24,idup_,mothup_) <<std::endl;
// std::cout<<"checkDau(6,4,24,idup_,mothup_) "<<checkDau(6,4,24,idup_,mothup_) <<std::endl;
// std::cout<<std::endl;
// for ( unsigned int icount = 0 ; icount < (unsigned int)nup_; icount++ ) {
//       std::cout << "# " << std::setw(14) << std::fixed << icount 
//                 << std::setw(14) << std::fixed << idup_[icount] 
//                 << std::setw(14) << std::fixed << (pup_[icount])[0] 
//                 << std::setw(14) << std::fixed << (pup_[icount])[1] 
//                 << std::setw(14) << std::fixed << (pup_[icount])[2] 
//                 << std::setw(14) << std::fixed << (pup_[icount])[3] 
//                 << std::setw(14) << std::fixed << (pup_[icount])[4]
//                 << std::setw(14) << std::fixed << mothup_[icount].first
//                 << std::setw(14) << std::fixed << mothup_[icount].second
//                 << std::endl;
//               }
//} 
//tH checks
//if(!(
//    checkNof(6,idup_)==2 &&
//    checkNof(zprimepdg,idup_)==1 &&
//    checkNof(tprimepdg,idup_)==1 &&
//    checkNof(25,idup_)==1 && //Z 25 for H
//    checkDau(zprimepdg,tprimepdg,6,idup_,mothup_) &&
//    checkDau(tprimepdg,6,25,idup_,mothup_) && //tZ
//    (checkDau(6,5,24,idup_,mothup_) || checkDau(6,4,24,idup_,mothup_) ) )
//    ){
// std::cout<<"NO tH\n";
// std::cout<<"checkNof(6,idup_)==2 " << checkNof(6,idup_) <<std::endl;
// std::cout<<"checkNof(zprimepdg,idup_)==1 "<<checkNof(zprimepdg,idup_) <<std::endl;
// std::cout<<"checkNof(tprimepdg,idup_)==1 "<<checkNof(tprimepdg,idup_) <<std::endl;
// std::cout<<"checkNof(25,idup_)==1 "<<checkNof(25,idup_) <<std::endl;
// std::cout<<"checkDau(zprimepdg,tprimepdg,6,idup_,mothup_) "<<checkDau(zprimepdg,tprimepdg,6,idup_,mothup_) <<std::endl;
// std::cout<<"checkDau(tprimepdg,6,25,idup_,mothup_) "<<checkDau(tprimepdg,6,25,idup_,mothup_) <<std::endl;
// std::cout<<"checkDau(6,5,24,idup_,mothup_) "<<checkDau(6,5,24,idup_,mothup_) <<std::endl;
// std::cout<<"checkDau(6,4,24,idup_,mothup_) "<<checkDau(6,4,24,idup_,mothup_) <<std::endl;
// std::cout<<std::endl;
// for ( unsigned int icount = 0 ; icount < (unsigned int)nup_; icount++ ) {
//       std::cout << "# " << std::setw(14) << std::fixed << icount 
//                 << std::setw(14) << std::fixed << idup_[icount] 
//                 << std::setw(14) << std::fixed << (pup_[icount])[0] 
//                 << std::setw(14) << std::fixed << (pup_[icount])[1] 
//                 << std::setw(14) << std::fixed << (pup_[icount])[2] 
//                 << std::setw(14) << std::fixed << (pup_[icount])[3] 
//                 << std::setw(14) << std::fixed << (pup_[icount])[4]
//                 << std::setw(14) << std::fixed << mothup_[icount].first
//                 << std::setw(14) << std::fixed << mothup_[icount].second
//                 << std::endl;
//               }
//} 



    for ( unsigned int icount = 0 ; icount < (unsigned int)nup_; icount++ ) {

      // std::cout << "# " << std::setw(14) << std::fixed << icount 
      //           << std::setw(14) << std::fixed << idup_[icount] 
      //           << std::setw(14) << std::fixed << (pup_[icount])[0] 
      //           << std::setw(14) << std::fixed << (pup_[icount])[1] 
      //           << std::setw(14) << std::fixed << (pup_[icount])[2] 
      //           << std::setw(14) << std::fixed << (pup_[icount])[3] 
      //           << std::setw(14) << std::fixed << (pup_[icount])[4]
      //           << std::setw(14) << std::fixed << mothup_[icount].first
      //           << std::setw(14) << std::fixed << mothup_[icount].second
      //           << std::endl;

       TLorentzVector part4vec((pup_[icount])[0], (pup_[icount])[1], (pup_[icount])[2], (pup_[icount])[3]);

       if (abs(idup_[icount])==6) histos1D_[ "topPt" ]->Fill(part4vec.Pt());
       if ((abs(idup_[mothup_[icount].first-1])==6) && (abs(idup_[icount])==24)) histos1D_[ "wtPt" ]->Fill(part4vec.Pt());
       if ((abs(idup_[mothup_[icount].first-1])==6) && (abs(idup_[icount])==5)) histos1D_[ "btPt" ]->Fill(part4vec.Pt());
       if (abs(idup_[icount])==tprimepdg) histos1D_[ "tprimePt" ]->Fill(part4vec.Pt());
       if (abs(idup_[icount])==25) histos1D_[ "hPt" ]->Fill(part4vec.Pt());

       if (abs(idup_[icount])==6) histos1D_[ "topEta" ]->Fill(part4vec.Eta());
       if ((abs(idup_[mothup_[icount].first-1])==6) && (abs(idup_[icount])==24)) histos1D_[ "wtEta" ]->Fill(part4vec.Eta());
       if ((abs(idup_[mothup_[icount].first-1])==6) && (abs(idup_[icount])==5)) histos1D_[ "btEta" ]->Fill(part4vec.Eta());
       if (abs(idup_[icount])==tprimepdg) histos1D_[ "tprimeEta" ]->Fill(part4vec.Eta());
       if (abs(idup_[icount])==25) histos1D_[ "hEta" ]->Fill(part4vec.Eta());

       if (abs(idup_[icount])==6) histos1D_[ "topPhi" ]->Fill(part4vec.Phi());
       if ((abs(idup_[mothup_[icount].first-1])==6) && (abs(idup_[icount])==24)) histos1D_[ "wtPhi" ]->Fill(part4vec.Phi());
       if ((abs(idup_[mothup_[icount].first-1])==6) && (abs(idup_[icount])==5)) histos1D_[ "btPhi" ]->Fill(part4vec.Phi());
       if (abs(idup_[icount])==tprimepdg) histos1D_[ "tprimePhi" ]->Fill(part4vec.Phi());
       if (abs(idup_[icount])==25) histos1D_[ "hPhi" ]->Fill(part4vec.Phi());

       int mother1 = idup_[mothup_[icount].first-1];
       int mother2 = idup_[mothup_[icount].second-1];
       mother1 = abs(mother1) != tprimepdg ? mother1 : (mother1 < 0 ? -8 : 8);
       mother2 = abs(mother2) != tprimepdg ? mother2 : (mother2 < 0 ? -8 : 8);

       if (abs(idup_[icount])==6) histos1D_[ "topMothers" ]->Fill(mother1);
       if (abs(idup_[icount])==6) histos1D_[ "topMothers" ]->Fill(mother2);
       if ((abs(idup_[mothup_[icount].first-1])==6) && (abs(idup_[icount])==24)) histos1D_[ "wtMothers" ]->Fill(mother1);
       if ((abs(idup_[mothup_[icount].first-1])==6) && (abs(idup_[icount])==24)) histos1D_[ "wtMothers" ]->Fill(mother2);
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
       if (idup_[icount]==tprimepdg) {
           histos1D_[ "tprimeDaughters" ]->Fill(checkDau(tprimepdg, idup_, mothup_).first);
           histos1D_[ "tprimeDaughters" ]->Fill(checkDau(tprimepdg, idup_, mothup_).second);
       }
       if (idup_[icount]==-6) {
           histos1D_[ "topDaughters" ]->Fill(checkDau(-6, idup_, mothup_).first);
           histos1D_[ "topDaughters" ]->Fill(checkDau(-6, idup_, mothup_).second);
       }
       if (idup_[icount]==-tprimepdg) {
           histos1D_[ "tprimeDaughters" ]->Fill(checkDau(-tprimepdg, idup_, mothup_).first);
           histos1D_[ "tprimeDaughters" ]->Fill(checkDau(-tprimepdg, idup_, mothup_).second);
       }



       //if (abs(idup_[icount])==6) histos1D_[ "topP" ]->Fill(sqrt(pow((pup_[icount])[0],2)+pow((pup_[icount])[1],2)+pow((pup_[icount])[2],2)));
       //if ((abs(idup_[mothup_[icount].first-1])==tprimepdg) && (abs(idup_[icount])==24)) histos1D_[ "wtpP" ]->Fill(sqrt(pow((pup_[icount])[0],2)+pow((pup_[icount])[1],2)+pow((pup_[icount])[2],2)));
       //if ((abs(idup_[mothup_[icount].first-1])==tprimepdg) && (abs(idup_[icount])==5)) histos1D_[ "btpP" ]->Fill(sqrt(pow((pup_[icount])[0],2)+pow((pup_[icount])[1],2)+pow((pup_[icount])[2],2)));
       //if (abs(idup_[icount])==tprimepdg) histos1D_[ "tprimeP" ]->Fill(sqrt(pow((pup_[icount])[0],2)+pow((pup_[icount])[1],2)+pow((pup_[icount])[2],2)));

       if (abs(idup_[icount])==6) histos1D_[ "topM" ]->Fill((pup_[icount])[4]);
       if (abs(idup_[icount])==tprimepdg) histos1D_[ "tprimeM" ]->Fill((pup_[icount])[4]);
       //if (abs(idup_[icount])==zprimepdg) histos1D_[ "zprimeM" ]->Fill((pup_[icount])[4]);

       //tZ specific
       //if ((abs(idup_[mothup_[icount].first-1])==tprimepdg) && (abs(idup_[icount])==6)) histos1D_[ "toptprimePt" ]->Fill(sqrt(pow((pup_[icount])[0],2)+pow((pup_[icount])[1],2)));
       //if ((abs(idup_[mothup_[icount].first-1])==zprimepdg) && (abs(idup_[icount])==6)) histos1D_[ "topzprimePt" ]->Fill(sqrt(pow((pup_[icount])[0],2)+pow((pup_[icount])[1],2)));
       //if (abs(idup_[icount])==23) histos1D_[ "zPt" ]->Fill(sqrt(pow((pup_[icount])[0],2)+pow((pup_[icount])[1],2)));
       //if (abs(idup_[icount])==23) histos1D_[ "zM" ]->Fill((pup_[icount])[4]);
       //tH specific
       if (abs(idup_[icount])==25) histos1D_[ "hM" ]->Fill((pup_[icount])[4]);

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
  histos1D_[ "wtPt" ] = fileService->make< TH1D >( "wtPt", ";W from t p_{T} [GeV];Events", 200, 0., 2000);
  histos1D_[ "btPt" ] = fileService->make< TH1D >( "btPt", ";b from t p_{T} [GeV];Events", 200, 0., 2000);
  histos1D_[ "hPt" ] = fileService->make< TH1D >( "hPt", ";h p_{T} [GeV];Events", 200, 0., 2000);

  histos1D_[ "topEta" ] = fileService->make< TH1D >( "topEta", ";top p [GeV];Events", 140, -7, 7);
  histos1D_[ "tprimeEta" ] = fileService->make< TH1D >( "tprimeEta", ";T' p [GeV];Events", 140, -7, 7);
  histos1D_[ "wtEta" ] = fileService->make< TH1D >( "wtEta", ";W from t p [GeV];Events", 140, -7, 7);
  histos1D_[ "btEta" ] = fileService->make< TH1D >( "btEta", ";b from t p [GeV];Events", 140, -7, 7);
  histos1D_[ "hEta" ] = fileService->make< TH1D >( "hEta", ";h p [GeV];Events", 140, -7, 7);

  histos1D_[ "topPhi" ] = fileService->make< TH1D >( "topPhi", ";top p [GeV];Events", 63, -3.15, 3.15);
  histos1D_[ "tprimePhi" ] = fileService->make< TH1D >( "tprimePhi", ";T' p [GeV];Events", 63, -3.15, 3.15);
  histos1D_[ "wtPhi" ] = fileService->make< TH1D >( "wtPhi", ";W from t p [GeV];Events", 63, -3.15, 3.15);
  histos1D_[ "btPhi" ] = fileService->make< TH1D >( "btPhi", ";b from t p [GeV];Events", 63, -3.15, 3.15);
  histos1D_[ "hPhi" ] = fileService->make< TH1D >( "hPhi", ";h p [GeV];Events", 63, -3.15, 3.15);

  histos1D_[ "topMothers" ] = fileService->make< TH1D >( "topMothers", ";top p [GeV];Events", 61, -30.5, 30.5);
  histos1D_[ "tprimeMothers" ] = fileService->make< TH1D >( "tprimeMothers", ";T' p [GeV];Events", 61, -30.5, 30.5);
  histos1D_[ "wtMothers" ] = fileService->make< TH1D >( "wtMothers", ";W from t p [GeV];Events", 61, -30.5, 30.5);
  histos1D_[ "btMothers" ] = fileService->make< TH1D >( "btMothers", ";b from t p [GeV];Events", 61, -30.5, 30.5);
  histos1D_[ "hMothers" ] = fileService->make< TH1D >( "hMothers", ";h p [GeV];Events", 61, -30.5, 30.5);

  histos1D_[ "topDaughters" ] = fileService->make< TH1D >( "topDaughters", ";top p [GeV];Events", 61, -30.5, 30.5);
  histos1D_[ "tprimeDaughters" ] = fileService->make< TH1D >( "tprimeDaughters", ";T' p [GeV];Events", 61, -30.5, 30.5);

  histos1D_[ "topM" ] = fileService->make< TH1D >( "topM", ";top mass [GeV];Events", 100, 100, 200);
  histos1D_[ "tprimeM" ] = fileService->make< TH1D >( "tprimeM", ";T' mass [GeV];Events", 200, 0, 2000);
  histos1D_[ "hM" ] = fileService->make< TH1D >( "hM", ";H mass [GeV];Events", 100, 80, 180);

    // Handle<LHERunInfoProduct> run;
    // iRun.getByLabel( src_, run );
    
    // const lhef::HEPRUP thisHeprup_ = run->heprup();

    // std::cout << "HEPRUP \n" << std::endl;
    // std::cout << "IDBMUP " << std::setw(14) << std::fixed << thisHeprup_.IDBMUP.first 
    //           << std::setw(14) << std::fixed << thisHeprup_.IDBMUP.second << std::endl; 
    // std::cout << "EBMUP  " << std::setw(14) << std::fixed << thisHeprup_.EBMUP.first 
    //           << std::setw(14) << std::fixed << thisHeprup_.EBMUP.second << std::endl; 
    // std::cout << "PDFGUP " << std::setw(14) << std::fixed << thisHeprup_.PDFGUP.first 
    //           << std::setw(14) << std::fixed << thisHeprup_.PDFGUP.second << std::endl; 
    // std::cout << "PDFSUP " << std::setw(14) << std::fixed << thisHeprup_.PDFSUP.first 
    //           << std::setw(14) << std::fixed << thisHeprup_.PDFSUP.second << std::endl; 
    // std::cout << "IDWTUP " << std::setw(14) << std::fixed << thisHeprup_.IDWTUP << std::endl; 
    // std::cout << "NPRUP  " << std::setw(14) << std::fixed << thisHeprup_.NPRUP << std::endl; 
    // std::cout << "        XSECUP " << std::setw(14) << std::fixed 
    //           << "        XERRUP " << std::setw(14) << std::fixed 
    //           << "        XMAXUP " << std::setw(14) << std::fixed 
    //           << "        LPRUP  " << std::setw(14) << std::fixed << std::endl;
    // for ( unsigned int iSize = 0 ; iSize < thisHeprup_.XSECUP.size() ; iSize++ ) {
    //   std::cout  << std::setw(14) << std::fixed << thisHeprup_.XSECUP[iSize]
    //              << std::setw(14) << std::fixed << thisHeprup_.XERRUP[iSize]
    //              << std::setw(14) << std::fixed << thisHeprup_.XMAXUP[iSize]
    //              << std::setw(14) << std::fixed << thisHeprup_.LPRUP[iSize] 
    //              << std::endl;
    // }
    // std::cout << " " << std::endl;

  }
  

  InputTag src_;
};

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE( DummyLHEAnalyzer );


