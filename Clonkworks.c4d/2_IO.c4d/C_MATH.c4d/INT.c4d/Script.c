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
  return(["Value"]);
}

public func InputList(){
  return(["Increase","Decrease","Set"]);
}

func FastHandle(){
  if(InputActive("Increase")) Value++;
  if(InputActive("Decrease")) Value--;
  if(InputActive("Set") != nil) Value = InputActive("Set");
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
	  return(RGBa(0,255,0));
}

public func InfobarInfo(){
	return(Format("%d",Value));
}

public func ControlUp(pClonk){
  [$TxtModify$]
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
}