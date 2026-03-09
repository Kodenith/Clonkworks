/*-- Neues Script --*/

#strict 2
#include IO__

func Initialize() {
  SetAction("Attached");
  SetComDir(COMD_None);
  return(1);
}

public func OutputList(){
  return(["Output"]);
}

public func InputList(){
  return(["Input","Key"]);
}

func MustBeOnWall(){ return(1); }

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
   if(InputActive("Key")) return(InputActive("Input"));
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