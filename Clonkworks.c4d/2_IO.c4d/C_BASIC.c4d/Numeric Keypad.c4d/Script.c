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
    if(Value != nil)
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

func InputCallback(string szInput) {
    var vl = 0;
    var len = GetLength(szInput);
    var start = 0;
    var sign = 1;

    if (len == 0) return;

    if (GetChar(szInput, 0) == 45) {
        sign = -1;
        start = 1;
    }

    if (len > 8 + start) len = 8 + start;

    for (var c = start; c < len; c++) {
        var char = GetChar(szInput, c);
        var digit = 0;

        if(char >= 48 && char <= 57) {
            digit = char - 48;
        }

        vl = (vl * 10) + digit;
    }

    Value = vl * sign;
    Sound("Click");

    Schedule("ResetVal()",38+19,0,this);
}

func ResetVal(){
  Value = nil;
}