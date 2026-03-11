/*-- Neues Script --*/

#strict 3
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
  return(["Check 1","Check 2","Check 3","Check 4","Check 5","Check 6"]);
}

func MustBeOnWall(){ return(1); }

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
  if(InputActive("Check 1") != 0 && InputActive("Check 1") != nil) return(InputActive("Check 1"));
  if(InputActive("Check 2") != 0 && InputActive("Check 2") != nil) return(InputActive("Check 2"));
  if(InputActive("Check 3") != 0 && InputActive("Check 3") != nil) return(InputActive("Check 3"));
  if(InputActive("Check 4") != 0 && InputActive("Check 4") != nil) return(InputActive("Check 4"));
  if(InputActive("Check 5") != 0 && InputActive("Check 5") != nil) return(InputActive("Check 5"));
  if(InputActive("Check 6") != 0 && InputActive("Check 6") != nil) return(InputActive("Check 6"));
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