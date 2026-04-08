/*-- Neues Script --*/

#strict 3
#include IO__

local Value;

func Initialize() {
  SetAction("Attached");
  SetComDir(COMD_None);
  return(1);
}

public func OutputList(){
  return(["Output"]);
}

public func WireTo(){ return(0); }

func MustBeOnWall(){ return(1); }

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
    if(Value)
     return(Value);

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

public func ControlUp(pClonk){
  [$TxtModify$]
  //if(Locked) return(0);
  CallMessageBoard(this,0,"INT:",GetController(pClonk));
}

func ResetVal(){
  Value = nil;
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

        Schedule("ResetVal()",38+19,0,this);
}