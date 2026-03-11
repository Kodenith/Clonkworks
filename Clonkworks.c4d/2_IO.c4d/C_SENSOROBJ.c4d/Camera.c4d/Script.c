/*-- Neues Script --*/

#strict 3
#include IO__

func Initialize() {
  SetAction("Track");
  SetComDir(COMD_None);
  return(1);
}

public func OutputList(){
  return(["Camera"]);
}

public func WireTo(){
  return(0);
}

public func HasCamera(){ return(1); }

func MustBeOnWall(){ return(1); }

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
     return(this);
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