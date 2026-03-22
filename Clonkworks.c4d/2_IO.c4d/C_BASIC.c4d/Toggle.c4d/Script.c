/*-- Neues Script --*/

#strict 3
#include IO__

func Initialize() {
  SetAction("Off");
  SetComDir(COMD_None);
  return(1);
}

public func InputList(){
  return(["Toggle"]);
}

public func OutputList(){
  return(["On"]);
}

local Armed;

private func ProcessInput(){
  var Act = InputActive("Toggle");
  if(!Act) Armed = true;
  else if(Armed && Act){
    if(GetAction() == "Off") SetAction("On");
    else SetAction("Off");
    if(!Locked) Sound("Command");
    Armed = false;
  }
}

func MustBeOnWall(){ return(1); }

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
   if(OutputName == "On" && GetAction() == "On") return(1);
     return(0);
}

func Malfunction(){ OnDetach(); }
func Damage(){
  if(GetDamage() > 50 && !Locked) OnDetach();
}
func CanBeDetached(){ return(!Locked); }
func OnDetach(pClonk){
  if(pClonk && Hostile(GetOwner(),GetController(pClonk)) && !FindObject(WSAB)) return(0);
  if(Locked) return(0);
  Sound("Connect");
  Split2Components();
}