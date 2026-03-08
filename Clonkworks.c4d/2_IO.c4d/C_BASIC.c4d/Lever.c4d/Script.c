/*-- Neues Script --*/

#strict 2
#include BAS1
#include IO__

public func ControlLeft(){
  [$TxtLeft$|Image=I1_L]
  if(GetAction() != "Idle"){
    Sound("SignalClick");
    SetAction("Idle");
  }
}

public func ControlRight(){
  [$TxtRight$|Image=I1_L:1]
  if(GetAction() != "Right"){
    Sound("SignalClick");
    SetAction("Right");
  }
}

public func ControlUp(){
  [$TxtUp$|Image=I1_L:2]
  if(GetAction() != "Middle"){
    Sound("SignalClick");
    SetAction("Middle");
  }
}

public func WireTo(){ return(0); }

//this function returns a list of string that can be output.
//default is a single output.
public func OutputList(){
  return(["Left","Right","Up"]);
}

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
   if(OutputName == "Left" && GetAction() == "Idle") return(1);
   if(OutputName == "Right" && GetAction() == "Right") return(1);
   if(OutputName == "Up" && GetAction() == "Middle") return(1);
}

func CanBeDetached(){ return(1); }
func OnDetach(){
  Sound("Connect");
  Split2Components();
}