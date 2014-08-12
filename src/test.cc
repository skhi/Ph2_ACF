/*

FileName :                     test.cpp
Content :                      Little test program to see if everything is working fine
Programmer :                   Nicolas PIERRE, Lorenzo Bidegain
Version :                      0.3
Date of creation :             24/06/14
Support :                      mail to : nico.pierre@cern.ch, lorenzo.bidegain@cern.ch

*/

#include "../HWDescription/Cbc.h"
#include "../HWDescription/Module.h"
#include "../HWDescription/BeBoard.h"
#include "../HWInterface/CbcInterface.h"
#include "../HWInterface/BeBoardInterface.h"
#include "../HWDescription/Definition.h"

using namespace Ph2_HwDescription;
using namespace Ph2_HwInterface;

int main()
{
	//std::cout<<std::hex<<106<<std::endl;

	//Two Cbc
	Cbc cCbc_00(0,0,0,0,0,FE0CBC0HOLE);
	Cbc cCbc_01(0,0,0,0,1,FE0CBC1HOLE);

	//One Module
	Module cModule_00;
	cModule_00.fModuleId=0;
	cModule_00.addCbc(cCbc_00);
	cModule_00.addCbc(cCbc_01);

	//One Glib
	BeBoard cGlib_00;
	cGlib_00.addModule(cModule_00);

<<<<<<< HEAD
	GlibInterface cGlibInterface(UHAL_CONNECTION_FILE,8);
	CbcInterface cCbcInterface(UHAL_CONNECTION_FILE);
=======
	BeBoardFWInterface* cBeBoardFW;
	std::map<int8_t,BeBoardFWInterface*> cBeBoardMap;
>>>>>>> origin/Dev

	CbcInterface cCbcInterface(cBeBoardMap);
	BeBoardInterface cBeBoardInterface(cBeBoardMap);
	cBeBoardFW= new GlibFWInterface(UHAL_CONNECTION_FILE,0);
	cBeBoardMap[0]=cBeBoardFW;

<<<<<<< HEAD
=======
	cBeBoardInterface.ConfigureBoard(&cGlib_00);

>>>>>>> origin/Dev
	cCbcInterface.ConfigureCbc(cGlib_00.getModule(0)->getCbc(0));
	cCbcInterface.ConfigureCbc(cGlib_00.getModule(0)->getCbc(1));

	std::cout << "\nChanging Value of VCth...\n" << std::endl;

<<<<<<< HEAD
    cCbcInterface.WriteCbc(cGlib_00.getModule(0)->getCbc(0),"VCth",0x03);
	cCbcInterface.WriteCbc(cGlib_00.getModule(0)->getCbc(1),"VCth",0x10);
	cCbcInterface.UpdateAllCbc(cGlib_00.getModule(0));
=======
    cCbcInterface.WriteCbcReg(cGlib_00.getModule(0)->getCbc(0),"VCth",0x03);
	cCbcInterface.WriteCbcReg(cGlib_00.getModule(0)->getCbc(1),"VCth",0x10);
	cCbcInterface.ReadAllCbc(cGlib_00.getModule(0));
>>>>>>> origin/Dev

    std::cout << "\nValue of VCth Changed !\n" << std::endl;

	cBeBoardInterface.ReadBoardReg(&cGlib_00,EXT_TRG);
	cBeBoardInterface.ReadBoardReg(&cGlib_00,FAKE_DATA);

	uint32_t dump1 = cGlib_00.getReg(EXT_TRG);
	uint32_t dump2 = cGlib_00.getReg(FAKE_DATA);

	std::cout << "\nThe value for EXT_TRG is " << dump1 << std::endl;
	std::cout << "\nThe value for FAKE_DATA is " << dump2 << std::endl;

<<<<<<< HEAD
    cGlib_00.getModule(0)->getCbc(0)->writeRegValues("/afs/cern.ch/user/n/npierre/public/settings/output_00.txt");
	cGlib_00.getModule(0)->getCbc(1)->writeRegValues("/afs/cern.ch/user/n/npierre/public/settings/output_01.txt");
=======
    cGlib_00.getModule(0)->getCbc(0)->saveRegMap("/afs/cern.ch/user/n/npierre/dev/output/output_00.txt");
	cGlib_00.getModule(0)->getCbc(1)->saveRegMap("/afs/cern.ch/user/n/npierre/dev/output/output_01.txt");
>>>>>>> origin/Dev

	std::cout << "\nHurray, it works !!" << std::endl;

	delete cBeBoardFW;

	return 0;
}
