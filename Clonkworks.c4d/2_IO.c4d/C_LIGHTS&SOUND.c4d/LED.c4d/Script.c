/*-- Neues Script --*/

#strict 2
#include IO__

func Initialize() {
  SetAction("Off");
  SetComDir(COMD_None);
  return(1);
}

public func WireFrom(){ return(0); }

func MustBeOnWall(){ return(1); }

func CanBeDetached(){ return(1); }
func OnDetach(){
  Sound("Connect");
  Split2Components();
}

//this function returns a list of string that can be input.
public func InputList(){
  return(["Display"]);
}

func Check(){
  if(InputActive("Display") && GetAction() == "Off") SetAction("On");
  if(!InputActive("Display") && GetAction() != "Off") SetAction("Off");
}

func Malfunction(){ OnDetach(); }
func Damage(){
  if(GetDamage() > 50) OnDetach();
}