/*-- Neues Script --*/

#strict

local CanExit;
local previous;
local Timer;

func Construction(){
	Timer = 0;
	previous = 1;
	CanExit = true;
}

func UpdateLook(){
	if(Timer < 13) Timer++;
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
		if(Timer >= 13){
			Timer = 0;
			DoEnergy(previous);
			CanExit = false;
		}
		else{ DoEnergy(previous+chng); }
	}
	
	if(GetAction() ne "Charge") SetAction("Charge");
	SetPhase(GetEnergy()-1);
}

func ExitWorkshop(){
	if(CanExit && GetCon() == 100){
		CanExit = false;
		return(1); 
	}else{
		return(0);
	}
}

//useful functions that make working witb batteries and their energy a bit easier (i blame myself for my horrid code)

func IsBattery(){ return(1); }

func DoBatteryEnergy(int Amount){
	if(Abs(Amount) > 100) return(0);
	DoEnergy(Amount);
	previous = GetEnergy();
	return(1);
}

func SetBatteryEnergy(int Amount){
	if(Amount < 0) return(0);
	DoEnergy(-GetEnergy());
	DoEnergy(Amount);
	previous = GetEnergy();
	return(1);
}