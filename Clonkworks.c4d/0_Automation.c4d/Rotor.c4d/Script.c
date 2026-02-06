/*-- Neues Script --*/

#strict 2
#include CXEC
#include BAS3
local Force;

public func GetRotorForce(){
	return(Force);
}

func Initialize() {
  return(1);
}

func ControlLeft(pClonk){
	[$TxtLeft$]
	if(!EnergyCheck(1)) return(0);
	if(GetAction() != "RotateLeft"){
		SetAction("RotateLeft");
		Sound("SignalClick");
	}
}

func ControlRight(pClonk){
	[$TxtRight$]
	if(!EnergyCheck(1)) return(0);
	if(GetAction() != "RotateRight"){
		SetAction("RotateRight");
		Sound("SignalClick");
	}
}

func ControlUp(pClonk){
	[$TxtLeft$]
	if(GetAction() != "Idle"){
		SetAction("Idle");
		Sound("SignalClick");
		Force=0;
	}
}

//timers
func DieOut(){
	if(Force > 0) Force--;
	if(Force < 0) Force++;
	if(Force == 0 && GetAction() != "Idle") SetAction("Idle");
}

func UpdateLeft(){
	Force = -61;
}

func UpdateRight(){
	Force = 61;
}

public func IsRotorSource(){ return(1); }
public func IsPrimaryRotor(){ return(1); }

func IsAdvancedProduct(){ return(1); }

public func GetResearchBase(){
	return(CNVY);
}

public func Malfunction(){
	ControlUp();
}