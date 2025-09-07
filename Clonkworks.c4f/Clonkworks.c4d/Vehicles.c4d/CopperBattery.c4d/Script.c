/*-- Neues Script --*/

#strict

local CanExit;
local Charge;

func Construction(){
	Charge = 0;
	CanExit = true;
}

public func GetChargeHold(){ return(20); }

func UpdateLook(){
	var Fixed = false;
	
	var eng = GetEnergy();
	
	if(!Random(GetChargeHold()))
	if(Charge > eng){
		SetBatteryEnergy(Charge-1);
	}else if(Charge < eng){
		SetBatteryEnergy(Charge+1);
	};
	
	if(!Random(GetChargeHold()) && Charge <= 2 && Charge > 0){
		SetBatteryEnergy(Charge-1);
	}
	
	if(CanExit && GetActTime() > 120) CanExit = false;
	
	if(GetAction() ne "Charge") SetAction("Charge");
	SetPhase(Charge-1);
}

func ExitWorkshop(){
	if(CanExit && GetCon() == 100){
		CanExit = false;
		return(1); 
	}else{
		return(0);
	}
}

//useful functions that make working with batteries and their energy a bit easier (i blame myself for my horrid code)

func IsBattery(){ return(1); }

func DoBatteryEnergy(int Amount){
	if(Abs(Amount) > 100) return(0);
	DoEnergy(Amount);
	Charge = GetEnergy();
	return(1);
}

func SetBatteryEnergy(int Amount){
	if(Amount < 0) return(0);
	DoEnergy(-GetEnergy());
	DoEnergy(Amount);
	Charge = Amount;
	return(1);
}