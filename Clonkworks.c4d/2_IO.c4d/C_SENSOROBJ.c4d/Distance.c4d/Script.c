/*-- Neues Script --*/

#strict 3
#include IO__

func Initialize() {
  SetAction("Attached");
  SetComDir(COMD_None);
  return(1);
}


public func InputList(){
  return(["Obj 1","Obj 2"]);
}

public func OutputList(){
  return(["Distance"]);
}

public func OutputActive(OutputName){
  var Obj1 = InputActive("Obj 1");
  var Obj2 = InputActive("Obj 2");
  if(GetType(Obj1) != C4V_C4Object) return(nil);
  if(GetType(Obj2) != C4V_C4Object) return(nil);
  return(ObjectDistance(Obj1,Obj2));
}

func MustBeOnWall(){ return(1); }

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