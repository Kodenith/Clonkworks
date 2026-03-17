/*-- Neues Script --*/

#strict 3
#include IO__

func Initialize() {
  SetAction("Attached");
  SetComDir(COMD_None);
  return(1);
}

public func OutputList(){
  return(["First Value"]);
}

public func InputList(){
  return(["V1","V2","V3","V4","V5","V6","V7","V8","V9","V10"]);
}

func MustBeOnWall(){ return(1); }

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
  var a = InputActive("V1");
   if(a != nil) return(a);
   a = InputActive("V2");
   if(a != nil) return(a);
   a = InputActive("V3");
   if(a != nil) return(a);
   a = InputActive("V4");
   if(a != nil) return(a);
   a = InputActive("V5");
   if(a != nil) return(a);
   a = InputActive("V6");
   if(a != nil) return(a);
   a = InputActive("V7");
   if(a != nil) return(a);
   a = InputActive("V8");
   if(a != nil) return(a);
   a = InputActive("V9");
   if(a != nil) return(a);
   a = InputActive("V10");
   if(a != nil) return(a);

   return(nil);
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