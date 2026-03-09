/*-- Neues Script --*/

#strict 2
#include IO__

func Initialize() {
  SetAction("Attached");
  SetComDir(COMD_None);
  return(1);
}

public func OutputList(){
  return(["Positive","Negative"]);
}

public func InputList(){
  return(["Integer"]);
}

func MustBeOnWall(){ return(1); }

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
   if(GetType(InputActive("Integer")) != C4V_Int) return(0);
   if(OutputName == "Positive") return(Abs(InputActive("Integer")));
   if(OutputName == "Negative") return(-Abs(InputActive("Integer")));
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
