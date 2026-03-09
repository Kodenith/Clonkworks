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
  return(["Number","Min","Max"]);
}

func MustBeOnWall(){ return(1); }

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
   if(GetType(InputActive("Min")) != C4V_Int) return(0);
   if(GetType(InputActive("Max")) != C4V_Int) return(0);
   if(GetType(InputActive("Number")) != C4V_Int) return(0);
   return(BoundBy(InputActive("Number"),InputActive("Min"),InputActive("Max")));
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
