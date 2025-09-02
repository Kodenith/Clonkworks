/*-- Neues Script --*/

#strict 2
#include CPBT
#include BAS2

public func LightningStrike() 
{
  if (GetCon() < 100) return(0);
    Sound("Electric");
	SetBatteryEnergy(100);
	previous = 100;
	CastParticles("PxSpark", 10,100, 0,-30, 25, 80, RGBa(255,255,0,0), RGBa(255,149,0,127));
}

func UpdateLook(){
	if(Timer < 9999) Timer++;
	var chng;
	var pLine = FindObject(0, 0,0,0,0, 0, "Connect", this());
	if(previous != GetEnergy()){
		if(previous < GetEnergy()){
			previous += 1;
			chng = -1;
		}else{ 
			if(!pLine) return(0);
			previous -= 1;
			chng = 1;
		}
		
		DoEnergy(-GetEnergy());
		if(Timer <= 13){
			DoEnergy(previous);
			CanExit = false;
		}
		else{ DoEnergy(previous+chng); }
		
		if(Timer >= 99) Timer = 0;
	}
	
	if(GetAction() != "Charge") SetAction("Charge");
	SetPhase(GetEnergy()-1);
}