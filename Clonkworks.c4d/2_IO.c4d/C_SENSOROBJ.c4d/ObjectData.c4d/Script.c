/*-- Neues Script --*/

#strict 3
#include IO__

func Initialize() {
  SetAction("Attached");
  SetComDir(COMD_None);
  return(1);
}

public func OutputList(){
  return(["Name","ID String","Original Name","Action","Procedure","Energy","Mana","Breath","Owner Name","X Coordinate","Y Coordinate","Mass","Value"]);
}

public func InputList(){
  return(["Object"]);
}

func MustBeOnWall(){ return(1); }

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
   var A = InputActive("Object");
   if(GetType(A) != C4V_C4Object) return(0);
   //DebugLog("%v",A);
   if(OutputName == "Name") return(GetName(A));
   if(OutputName == "ID String") return(Format("%i",GetID(A)));
   if(OutputName == "Original Name") return(GetName(,GetID(A)));
   if(OutputName == "Action") return(GetAction(A));
   if(OutputName == "Procedure") return(GetProcedure(A));
   if(OutputName == "Energy") return(GetEnergy(A));
   if(OutputName == "Mana") return(GetMagicEnergy(A));
   if(OutputName == "Breath") return(GetBreath(A));
   if(OutputName == "Owner Name") return(GetPlayerName(GetOwner(A)));
   if(OutputName == "X Coordinate") return(GetX(A));
   if(OutputName == "Y Coordinate") return(GetY(A));
   if(OutputName == "Mass") return(GetMass(A));
   if(OutputName == "Value") return(GetValue(A));
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