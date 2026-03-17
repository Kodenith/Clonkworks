/*-- Neues Script --*/

#strict 3
#include IO__

local Value;

func Initialize() {
  SetAction("Attached");
  SetComDir(COMD_None);
  Value=0;
  SetColorDw(RGBa(69,69,69));
  return(1);
}

public func OutputList(){
  return(["Object"]);
}

public func InputList(){
  return(["Set"]);
}

func FastHandle(){
  if(InputActive("Set") && GetType(InputActive("Set")) == C4V_C4Object) Value = InputActive("Set");
}

func MustBeOnWall(){ return(1); }

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
   return(Value);
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


//INFOBAR
public func InfobarTrigger(){
  if(GetType(Value) == C4V_C4Object)
	return(IB_Grab);
}

public func InfobarMax(){
	return(1);
}

public func InfobarValue(){
	return(1);
}

public func InfobarColor(){
	  return(RGBa(255,255,0));
}

public func InfobarInfo(){
	return(Format("{{%i}} %s",GetID(Value),GetName(Value)));
}