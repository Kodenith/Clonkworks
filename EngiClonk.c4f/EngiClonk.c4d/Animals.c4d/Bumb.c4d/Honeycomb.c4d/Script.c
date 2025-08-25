/*-- Neues Script --*/

#strict

local PollenAmount;
local DesignatedBumb;

func Initialize() {
  SetAction("Fill");
  return(1);
}

func Update(){
	if(PollenAmount >= 30) PollenAmount = 30;
	SetPhase((PollenAmount / 3) - 1);
	if(!DesignatedBumb){
		DoCon(-1);
	}else{
		SetCon(100);
	}
}
