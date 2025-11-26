/*-- Neues Script --*/

#strict 2

func Move(){
	if(GetAction() == "Idle"){
		RemoveObject();
		return(0);
	}
	if(!GetActionTarget(0)){
		RemoveObject();
		return(0);
	}
	if(GetActTime() > 36*2){
		RemoveObject();
		return(0);
	}
	
	var act = GetActTime();
	while(act > 18) act -= 18;
	if(act > 9) SetClrModulation(RGBa(0,0,1,255));
	else SetClrModulation(RGBa(255,255,255,0));
	
	//get height
	var Target = GetActionTarget(0);
	var TargetHeight = GetDefCoreVal("Height", "DefCore", GetID(Target));
	
	SetXDir(0);
	SetYDir(0);
	SetPosition(GetX(Target),GetY(Target)-(TargetHeight+5));
}

//warn about fuel, optionally display a message to a player.
global func FuelWarn(pObject,FuelType,iPlr){
	//Creating the icon.
	var Icn = CreateObject(WR_F);
	Icn->SetAction("Warn",this());
	
	//if a player is set, warn the player.
	if(iPlr){
		if(GetType(FuelType) == C4V_C4ID){
			PlayerMessage(iPlr-1,"$TxtFuelWarning$",,GetName(,FuelType));
		}else if(GetType(FuelType) == C4V_Array){
			var nameList = StringPieceTogetherNames(FuelType,", ");
			PlayerMessage(iPlr-1,"$TxtFuelWarning$",,nameList);
		}
	}
}

global func StringPieceTogetherNames(list,devider){
	var str = "";
	for(var i in list){
		str = Concat2(str,GetName(,i));
		str = Concat2(str,devider);
	}
	return(str);
}