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
   var Rand = RandomX(A,B);
   if(Rand == nil) Rand = 0;
   return(Rand);
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
