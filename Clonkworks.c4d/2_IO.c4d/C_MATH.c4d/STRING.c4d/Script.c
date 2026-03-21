/*-- Neues Script --*/

#strict 3
#include IO__

local Value;

func Initialize() {
  SetAction("Attached");
  SetComDir(COMD_None);
  Value="Foo";
  SetColorDw(RGBa(69,69,69));
  return(1);
}

public func OutputList(){
  return(["Value"]);
}

public func InputList(){
  return(["Set"]);
}

public func CheckSet(){
   if(InputActive("Set") && GetType(InputActive("Set")) == C4V_String) Value = InputActive("Set");
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
	return(IB_Grab);
}

public func InfobarMax(){
	return(1);
}

public func InfobarValue(){
	return(1);
}

public func InfobarColor(){
	  return(RGBa(0,255,255));
}

public func InfobarInfo(){
	return(Format("\"%s\"",Value));
}

public func ControlUp(pClonk){
  [$TxtModify$]
    if(Locked) return(0);
  CallMessageBoard(this,0,"STRING:",GetController(pClonk));
}

func InputCallback(string szInput) {
    var Input = szInput;
    for(var i = 0; i < GetLength(Input)-1; i++){
      //Log("Current: %s, Next: %s",Input[i],Input[i+1]);
      if(Input[i] == "%" && Input[i+1] != "v"){
        Sound("Error");
        Message("$TxtPlaceError$",this,"%v");
        return(0);
      }
    }
    Value = Input;
    Sound("Click");
}