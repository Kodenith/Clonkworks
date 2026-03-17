/*-- Neues Script --*/

#strict 2
#include CXEC
#include BAS3
#include IO__
local Force;

public func GetRotorForce(){
	return(Force);
}

func Initialize() {
  return(1);
}

public func ControlLeft(pClonk){
	[$TxtLeft$]
	if(!EnergyCheck(1)) return(0);
	if(GetAction() != "RotateLeft"){
		SetAction("RotateLeft");
		Sound("SignalClick");
	}
}

public func ControlRight(pClonk){
	[$TxtRight$]
	if(!EnergyCheck(1)) return(0);
	if(GetAction() != "RotateRight"){
		SetAction("RotateRight");
		Sound("SignalClick");
	}
}

public func ControlUp(pClonk){
	[$TxtLeft$]
	if(GetAction() != "Idle"){
		SetAction("Idle");
		Sound("SignalClick");
		Force=0;
	}
}

//timers
func DieOut(){
	if(InputActive("Left")) ControlLeft();
	else if(InputActive("Right")) ControlRight();
	else if(InputActive("Stop")) ControlUp();
	
	if(Force > 0 && Abs(FrameCounter()) % 2 == 0) Force--;
	if(Force < 0 && Abs(FrameCounter()) % 2 == 0) Force++;
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

func AutoOrderComp(){ return(GetCon() > 99 && EnergyCheck(1)); }

func OrderMenu(Caller){
	CreateMenu(GetID(),Caller,this,0,GetName(),,1);
	AddMenuItem("$TxtLeft$","ControlLeft",,Caller);
	AddMenuItem("$TxtStop$","ControlUp",,Caller);
	AddMenuItem("$TxtRight$","ControlRight",,Caller);
}

//particles :)
func DoConnectParticles(Own){
	var Amount = RandomX(8,15);
	while(Amount--) CreateParticle("MSpark",RandomX(-GetDefWidth(GetID())/2,GetDefWidth(GetID())/2),(GetDefHeight(GetID())/2)-RandomX(0,7),0,RandomX(-10,-40),RandomX(45,75),GetPlrColorDw(Own));
}

func WireFrom(){ return(0); }
func InputList(){ return(["Left","Right","Stop"]); }