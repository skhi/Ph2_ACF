#include "Channel.h"
#include "TMath.h"
#include <cmath>

Double_t MyErf( Double_t *x, Double_t *par ){
	Double_t x0 = par[0];
	Double_t width = par[1];
	Double_t fitval(0);
	if( x[0] < x0 ) fitval = 0.5 * TMath::Erfc( ( x0 - x[0] )/width );
	else fitval = 0.5 + 0.5 * TMath::Erf( ( x[0] - x0 )/width );
	return fitval;
}

Channel::Channel(uint8_t pBeId, uint8_t pFeId, uint8_t pCbcId, uint8_t pChannelId) :
fBeId( pBeId ),
fFeId( pFeId ),
fCbcId( pCbcId ),
fChannelId( pChannelId ){

	fScurve = NULL;
	fFit = NULL;
}


Channel::~Channel(){}

double Channel::getPedestal(){

	if( fFit !=NULL ){
		return fabs(fFit->GetParameter(0));
	}
	else return -1;
}

double Channel::getNoise(){

	if( fFit !=NULL ){
		return fabs(fFit->GetParError(0));
	}
	else return -1;
}

void Channel::setOffset( uint8_t pOffset ){
	fOffset = pOffset;
}

void Channel::initializeHist(uint8_t pValue, bool pVplusScan){

	TString histname;
	TString fitname;

	if(pVplusScan){
		histname = Form("Scurve_Be%d_Fe%d_Cbc%d_Channel%d_Vplus%d", fBeId, fFeId, fCbcId, fChannelId, pValue);
		fitname = Form("Fit_Be%d_Fe%d_Cbc%d_Channel%d_Vplus%d", fBeId, fFeId, fCbcId, fChannelId, pValue);

	}
	else {
		histname = Form("Scurve_Be%d_Fe%d_Cbc%d_Channel%d_Offset%d", fBeId, fFeId, fCbcId, fChannelId, pValue);
		fitname = Form("Fit_Be%d_Fe%d_Cbc%d_Channel%d_Offset%d", fBeId, fFeId, fCbcId, fChannelId, pValue);

	
	fScurve = (TH1F*) gROOT->FindObject(histname);
	if( fScurve ) delete fScurve;
	else fScurve = new TH1F(histname,Form("Scurve_Be%d_Fe%d_Cbc%d_Channel%d", fBeId, fFeId, fCbcId, fChannelId),256, -0.5, 255.5);

	TF1* fFit = (TF1*) gROOT->FindObject(fitname);
	if( fFit ) delete fFit;
	fFit = new TF1(fitname, MyErf, 0x00, 0xFF, 2 );

		}
}

void Channel::fillHist(uint8_t pVcth){
		fScurve->Fill(pVcth);
}

void Channel::fitHist(uint8_t pEventsperVcth, bool pHole, uint8_t pVplus, TFile* pResultfile){

	if ( fFit == NULL ){

		// Normalize first
		fScurve->Sumw2();
		fScurve->Scale(1/pEventsperVcth);

		// Get first non 0 and first 1
		double cFirstNon0(0);
		double cFirst1(0); 

		// Not Hole Mode
		if( !pHole ){
			for( Int_t cBin = 1; cBin <= fScurve->GetNbinsX(); cBin++ ){
				double cContent = fScurve->GetBinContent( cBin );
				if( !cFirstNon0	){
					if( cContent ) cFirstNon0 = fScurve->GetBinCenter(cBin);
				}
				else if( cContent == 1 ) {
					cFirst1 = fScurve->GetBinCenter(cBin); 
					break;
				}
			}
		}
		// Hole mode
		else{
			for( Int_t cBin = fScurve->GetNbinsX(); cBin >=1; cBin-- ){
				double cContent = fScurve->GetBinContent( cBin );
				if( !cFirstNon0	){
					if( cContent ) cFirstNon0 = fScurve->GetBinCenter(cBin);
				}
				else if( cContent == 1 ) {
					cFirst1 = fScurve->GetBinCenter(cBin); 
					break;
				}
			}
		}

		// Get rough midpoint & width
		double cMid = ( cFirst1 + cFirstNon0 ) * 0.5;
		double cWidth = ( cFirst1 - cFirstNon0 ) * 0.5;

		fFit->SetParameters(cMid, cWidth);

		// Fit
		fScurve->Fit(fFit,"RSLQ");

		// Eventually add TFitResultPointer
		// create a Directory in the file for the current Offset and save the channel Data
		TString cDirName = Form("Vplus%d",pVplus);
		TObject* cObj = gROOT->FindObject(cDirName);
		if (!cObj) pResultfile->mkdir(cDirName);
		pResultfile->cd(cDirName);

		fScurve->Write(fScurve->GetName(), TObject::kOverwrite);
		fFit->Write(fFit->GetName(), TObject::kOverwrite);

		pResultfile->cd();
	}
	else std::cout << "Historgram Empty for Fe " << fFeId << " Cbc " << fCbcId << " Channel " << fChannelId << std::endl;


}

void Channel::resetHist(){

	fScurve = NULL;
	fFit = NULL;

}


TestGroup::TestGroup(uint8_t pBeId,uint8_t pFeId,uint8_t pCbcId,uint8_t pGroupId):
fBeId( pBeId ),
fFeId(pFeId),
fCbcId(pCbcId),
fGroupId(pGroupId){
	TString graphname = Form("VplusVcthGraph_Fe%d_Cbc%d_Group%d",fFeId,fCbcId,fGroupId);
	fVplusVcthGraph = (TGraphErrors*) gROOT->FindObject(graphname);
	if (fVplusVcthGraph) delete fVplusVcthGraph;
	fVplusVcthGraph = new TGraphErrors();
	fVplusVcthGraph->SetName(graphname);
}

void TestGroup::FillVplusVcthGraph(uint8_t pVplus, double pPedestal, double pNoise){

	if (fVplusVcthGraph != NULL){
		fVplusVcthGraph->SetPoint(fVplusVcthGraph->GetN(),pPedestal,pVplus);
		fVplusVcthGraph->SetPointError(fVplusVcthGraph->GetN()-1,pNoise,0);
	}
}


	