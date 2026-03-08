/*-- Neues Script --*/

#strict 2
#include IO__
#include BAS1
local ActivateWeight;

func Initialize() {
  SetAction("Off");
  ActivateWeight = 50;
  return(1);
}

func CheckPressure(){
  if(GetAction() == "Off" && GetHeldWeight()>=ActivateWeight){
    Sound("Click");
    SetAction("On");
  }

  if(GetAction() == "On" && GetHeldWeight()<ActivateWeight){
    Sound("Click");
    SetAction("Off");
  }
}

public func WireTo(){ return(0); }

//this function returns a list of string that can be output.
//default is a single output.
public func OutputList(){
  return(["Activated","Weight Value"]);
}

public func GetHeldWeight(){
  var Objects = FindObjects(Find_OnLine(GetVertex(0,0),GetVertex(0,1),GetVertex(1,0),GetVertex(1,1)),Find_NoContainer(),Find_Or(Find_Category(C4D_Living),Find_Category(C4D_Object),Find_Category(C4D_Vehicle)));
  var Weight = 0;
  for(var i in Objects){
      Weight += GetMass(i);
  }
  return(Weight);
}

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
  if(OutputName == "Activated" && GetAction() == "On") return(1);
  if(OutputName == "Weight Value") return(GetHeldWeight());
}

func CanBeDetached(){ return(1); }
func OnDetach(){
  Sound("Connect");
  Split2Components();
}

//INFOBAR
public func InfobarTrigger(){
	return(IB_Grab);
}

public func InfobarMax(){
	return(500);
}

public func InfobarValue(){
	return(ActivateWeight);
}

public func InfobarColor(){
  if(GetAction() == "On")
	  return(RGBa(0,255,0));
  return(RGBa(255,0,0));
}

public func InfobarInfo(){
	return(Format("$TxtVal$",GetHeldWeight(),ActivateWeight));
}

func ControlLeft(){
  [$TxtDecrease$]
  ActivateWeight-=10;
  if(ActivateWeight <= 0) ActivateWeight = 10;
  Sound("Click");
}

func ControlRight(){
  [$TxtIncrease$]
  ActivateWeight+=10;
  if(ActivateWeight > 500) ActivateWeight = 500;
  Sound("Click");
}