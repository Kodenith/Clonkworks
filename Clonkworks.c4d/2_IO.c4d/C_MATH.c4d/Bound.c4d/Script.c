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
  return(["Number","Min","Max"]);
}

func MustBeOnWall(){ return(1); }

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
   var Number = InputActive("Number");
   var Min = InputActive("Min");
   var Max = InputActive("Max");
   if(GetType(Number) != C4V_Int && Number != nil) return(0);
   if(GetType(Min) != C4V_Int && Min != nil) return(0);
   if(GetType(Max) != C4V_Int && Max != nil) return(0);
   if(Number == nil) Number = 0;
   if(Min == nil) Min = 0;
   if(Max == nil) Max = 0;
   return(BoundBy(Number,Min,Max));
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
