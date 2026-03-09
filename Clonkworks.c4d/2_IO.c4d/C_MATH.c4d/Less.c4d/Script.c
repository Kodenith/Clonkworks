/*-- Neues Script --*/

#strict 2
#include IO__

func Initialize() {
  SetAction("Attached");
  SetComDir(COMD_None);
  return(1);
}

public func OutputList(){
  return(["Result"]);
}

public func InputList(){
  return(["A","B"]);
}

func MustBeOnWall(){ return(1); }

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
   if(GetType(InputActive("A")) != C4V_Int && GetType(InputActive("A")) != 0) return(0);
   if(GetType(InputActive("B")) != C4V_Int && GetType(InputActive("B")) != 0) return(0);
   return(InputActive("A")<InputActive("B") == 1);
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
