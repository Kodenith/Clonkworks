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

//what advanced lines can be connected?
public func ALKConnectType(){
	if(Par(0) == 1) return(0);
	//if(FindObject2(Find_ID(GERL),Find_ActionTarget(this()))) return(0);
	return([GERL]);
}

func IsAdvancedProduct(){ return(1); }

public func GetResearchBase(){
	return(CNVY);
}

public func Malfunction(){
	ControlUp();
}