/*-- Neues Script --*/

#strict

local CanExit;
local previous;

func Construction(){
	previous = 1;
	CanExit = true;
}

func UpdateLook(){
	var chng;
	if(previous != GetEnergy()){
		if(previous < GetEnergy()){
			previous += 1;
			chng = -1;
		}else{ 
			previous -= 1;
			chng = 1;
		}
		
		
		
		
		DoEnergy(-GetEnergy());
		if(RandomX(0,5) == 1)
			DoEnergy(previous);
		else DoEnergy(previous+chng);
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