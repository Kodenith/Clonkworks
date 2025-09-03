/*-- Neues Script --*/

#strict

local CanExit;
local oldeng;
local Timer;

func Construction(){
	Timer = 0;
	CanExit = true;
}

public func GetChargeHold(){ return(3); }

func UpdateLook(){
	var Fixed = false;
	
	var eng = GetEnergy();
	
	if(oldeng < eng-1){
		eng = oldeng+1;
		Fixed = true;
		}
	else if(oldeng > eng+1){
		Fixed = true;
		eng = oldeng-1;
	}
	
	if(oldeng <= eng && oldeng <= 2 && oldeng > 0) oldeng -= 1;
	if(oldeng < 0) oldeng = 0;
	
	if(!Random(GetChargeHold())){
	if(oldeng < eng){
		DoEnergy(-eng+(eng-1));
	}else if(oldeng > eng){
		DoEnergy(-eng-(eng+50));
	}
	
	oldeng = eng;
	}else{
		DoEnergy(-eng);
		DoEnergy(oldeng);
	}

	if(CanExit && GetActTime() > 120) CanExit = false;
	
	if(GetAction() ne "Charge") SetAction("Charge");
	if(!Fixed)
	SetPhase(eng-1);
	else SetPhase(oldeng-1);
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
	oldeng = GetEnergy();
	return(1);
}

func SetBatteryEnergy(int Amount){
	if(Amount < 0) return(0);
	DoEnergy(-GetEnergy());
	DoEnergy(Amount);
	oldeng = Amount;
	return(1);
}