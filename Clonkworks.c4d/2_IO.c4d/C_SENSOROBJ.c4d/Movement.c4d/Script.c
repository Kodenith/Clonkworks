/*-- Neues Script --*/

#strict 3
#include IO__

local Range;
local Detected;
local Dx,Dy;

func Initialize() {
  Range=80;
  SetAction("Attached");
  SetComDir(COMD_None);
  return(1);
}

public func OutputList(){
  return(["Detected","Detected Object","Detected X","Detected Y"]);
}

public func InputList(){
  return(["Range","Except"]);
}

func MustBeOnWall(){ return(1); }


//reusable function for all sensors :D
public func GetDetection(){
  var Objects = FindObjects(Find_NoContainer(),Find_Distance(Range),Find_Or(Find_Category(C4D_Living),Find_Category(C4D_Object),Find_Category(C4D_Vehicle)),Find_Exclude(this),Find_Exclude(InputActive("Except")),Sort_Distance());
  for(var i in Objects){
    if(GetSpeed(i) > 5) return(i);
  }
}

private func Detect(){
    if(InputActive("Range") && GetType(InputActive("Range")) == C4V_Int) Range = BoundBy(InputActive("Range"),20,500);

    var Found = GetDetection();
    if(!Found && GetAction() == "Active"){
      SetAction("Attached");
      Sound("Command");
    }

    if(Found){
      Detected = Found;
      Dx = GetX(Detected);
      Dy = GetY(Found);
      if(GetAction() == "Attached"){
        SetAction("Active");
        Sound("Command");
      }
    }
}

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
    if(GetAction() == "Active"){
        if(OutputName == "Detected") return(1);
        if(OutputName == "Detected Object") return(Detected);
        if(OutputName == "Detected X") return(Dx);
        if(OutputName == "Detected Y") return(Dy);
    }
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

//Setting Range.
public func InfobarTrigger(){
	return(IB_Grab);
}

public func InfobarMax(){
	return(500);
}

public func InfobarValue(){
	return(Range);
}

public func InfobarColor(){
  return(RGBa(0,255,0));
}

public func InfobarInfo(){
	return(Format("$TxtRange$",Range));
}

func ControlLeft(){
  [$TxtLeft$]
  Range -= 10;
  Range = BoundBy(Range,20,500);
  DrawRangeCircle();
  Sound("Click");
}

func ControlRight(){
  [$TxtRight$]
  Range+=10;
  Range = BoundBy(Range,20,500);
  DrawRangeCircle();
  Sound("Click");
}

private func DrawRangeCircle(){
  for(var x = 180; x >= -180; x-=2){
    if(!Random(3)) CreateParticle("NoGravSpark", Sin(x, Range, 1), Cos(x, Range, 1) , Sin(x, 0, 1), Cos(x, 0, 1), 25, RGBa(0,255,0));
  }
}