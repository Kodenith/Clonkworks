/*-- Neues Script --*/

#strict 2
#include CXEC
#include BAS1
local Force;
local PlayingSound;

public func GetRotorForce(){
	return(Force);
}

func Initialize() {
	SetAction("Movement");
  return(1);
}

func ControlLeft(pClonk){
	[$TxtLeft$]
	for(var i in FindObjects(Find_ID(GERL))){
		if(GetActionTarget(1,i) != this()) continue;
		var Rotor = GetActionTarget(0,i);
		Rotor->~ControlLeft(pClonk);
	}
	Sound("SignalClick");
}

func ControlRight(pClonk){
	[$TxtRight$]
	for(var i in FindObjects(Find_ID(GERL))){
		if(GetActionTarget(1,i) != this()) continue;
		var Rotor = GetActionTarget(0,i);
		Rotor->~ControlRight(pClonk);
	}
	Sound("SignalClick");
}

func ControlUp(pClonk){
	[$TxtLeft$]
	for(var i in FindObjects(Find_ID(GERL))){
		if(GetActionTarget(1,i) != this()) continue;
		var Rotor = GetActionTarget(0,i);
		Rotor->~ControlUp(pClonk);
		Force=0;
	}
	Sound("SignalClick");
}

//timers
func ThawOut(){
	if(Force != 0){
	if(Force > 0) Force--;
	if(Force < 0) Force++;
	}
	
	if(Force > 0){
		var ph = GetPhase()+1;
		if(ph > 16) ph = 0;
		SetPhase(ph);
	}else if(Force < 0){
		var ph = GetPhase()-1;
		if(ph < 0) ph = 16;
		SetPhase(ph);
	}
	
	if(Abs(Force) > 20 && !PlayingSound){
			PlayingSound = true;
			Sound("Gear",0,this(),0,0,+1);
	}else if(PlayingSound && Abs(Force) <= 20){
			PlayingSound = false;
			Sound("Gear",0,this(),0,0,-1);
	}
	
	//Message("%v",this(),Force);
}


func IsAdvancedProduct(){ return(1); }
func GetResearchBase(){ return(ROTR); }

func SetConveyorSpeed(){
	Force = Par(0);
}

func Incineration(){
		var wood = CreateObject(WOOD);
		Incinerate(wood);
		Fling(wood,RandomX(-3,3),RandomX(-3,3));
	    wood = CreateObject(CPIG);
		Fling(wood,RandomX(-3,3),RandomX(-3,3));
		RemoveObject();
}

public func IsRotorTarget(){ return(1); }
public func IsRotorSource(){ return(1); }

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