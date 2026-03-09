/*-- Neues Script --*/

#strict 3
#include IO__

func Initialize() {
  SetAction("Attached");
  SetComDir(COMD_None);
  return(1);
}

public func OutputList(){
  return(["Formatted"]);
}

public func InputList(){
  return(["String","F1","F2","F3","F4","F5"]);
}

func MustBeOnWall(){ return(1); }

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
   if(GetType(InputActive("String")) != C4V_String) return(0);
   return(Format(InputActive("String"),InputActive("F1"),InputActive("F2"),InputActive("F3"),InputActive("F4"),InputActive("F5")));
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
