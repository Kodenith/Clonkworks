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
		if(RandomX(0,20) == 1)
			DoEnergy(previous);
		else DoEnergy(previous+chng);
	}
	
	if(GetAction() ne "Charge") SetAction("Charge");
	SetPhase(GetEnergy()-1);
}

func ExitWorkshop(){ return(1); }
