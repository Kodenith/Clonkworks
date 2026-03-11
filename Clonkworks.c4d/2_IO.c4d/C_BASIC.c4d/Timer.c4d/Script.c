/*-- Neues Script --*/

#strict 3
#include IO__

local Ticks;

func Initialize() {
  Ticks = 38;
  SetAction("Attached");
  SetComDir(COMD_None);
  return(1);
}

public func OutputList(){
  return(["Timeout","Set Ticks"]);
}

public func InputList(){
  return(["Start"]);
}

private func TryTimer(){
  if(InputActive("Set Ticks")) Ticks = InputActive("Set Ticks");
      if(Ticks <= 0) Ticks = 1;
  if(InputActive("Start")) SetAction("Timing");
}

private func CheckEnd(){
  if(!Locked) Sound("Command");
  if(GetActTime() >= Ticks){
    SetAction("Finished");
    if(!Locked) Sound("Click");
  }
}

func MustBeOnWall(){ return(1); }

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
   if(GetAction() == "Finished") return(1);
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
	return(Format("%d Tick",Ticks));
}

public func ControlUp(pClonk){
  [$TxtModify$]
  CallMessageBoard(this,0,"TICK:",GetController(pClonk));
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

    Ticks = vl * sign;
    if(Ticks <= 0) Ticks = 1;
    Sound("Click");
}