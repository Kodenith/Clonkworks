/*-- Neues Script --*/

#strict 3
#include IO__

func Initialize() {
  SetAction("Attached");
  SetComDir(COMD_None);
  return(1);
}

public func OutputList(){
  return(["Selected Value"]);
}

public func InputList(){
  return(["V1","V2","V3","V4","V5","V6","V7","V8","V9","V10"]);
}

func MustBeOnWall(){ return(1); }

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
   var Values = [];
   for(var x = 1; x != 11; x++){
      if(DoesInputHaveWire(Format("V%d",x))) ArrayAdd(Values,InputActive(Format("V%d",x)));
   }

   if(GetLength(Values) != 0) return(Values[RandomX(0,GetLength(Values)-1)]);

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