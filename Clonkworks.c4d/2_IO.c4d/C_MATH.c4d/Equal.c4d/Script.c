/*-- Neues Script --*/

#strict 3
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
   var A = InputActive("A");
   var B = InputActive("B");
   if(GetType(A) != C4V_Int && A != nil) return(0);
   if(GetType(B) != C4V_Int && B != nil) return(0);
   if(A == nil) A = 0;
   if(B == nil) B = 0;
   return(A==B);
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