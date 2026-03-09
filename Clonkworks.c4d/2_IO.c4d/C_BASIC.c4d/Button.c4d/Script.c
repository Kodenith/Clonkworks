/*-- Neues Script --*/

#strict 3
#include IO__

func Initialize() {
  SetAction("Unclicked");
  SetComDir(COMD_None);
  UpdateTransferZone();
  return(1);
}

func UpdateTransferZone(){
  SetShape(-6,-4,12,8);
}

func ControlThrow(){
  [$TxtClick$]
  if(GetAction() == "Unclicked"){
    SetAction("Clicked");
    Sound("Click");
  }
}

public func WireTo(){ return(0); }

//this function returns a list of string that can be output.
//default is a single output.
public func OutputList(){
  return(["Pressed"]);
}

func MustBeOnWall(){ return(1); }

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
   if(OutputName == "Pressed" && GetAction() == "Clicked") return(1);
     return(0);
}

func Malfunction(){ OnDetach(); }
func Damage(){
  if(GetDamage() > 50) OnDetach();
}

func CanBeDetached(){ return(1); }
func OnDetach(){
  Sound("Connect");
  Split2Components();
}