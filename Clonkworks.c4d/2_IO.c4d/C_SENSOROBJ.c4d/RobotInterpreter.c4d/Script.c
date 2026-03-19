/*-- Neues Script --*/

#strict 3
#include IO__

local Detected;
local Dx,Dy;

func Initialize() {
  SetAction("Attached");
  SetComDir(COMD_None);
  return(1);
}

public func OutputList(){
  return(["Detected","Detected Object","Iterated Object"]);
}

public func InputList(){
  return(["Robot Sensor Results","ID String","Except","Ignore Contained","Ignore Dead"]);
}

func MustBeOnWall(){ return(1); }


//reusable function for all sensors :D
public func GetDetection(){
  var DetectList = InputActive("Robot Sensor Results");
  if(!DetectList || GetType(DetectList) != C4V_Array) return(nil);
  var pId = C4Id(InputActive("ID String"));
  //if(!pId) return(nil);

  for(var i in DetectList){
    if(i == InputActive("Except")) continue;
    if(GetCategory(i) & C4D_Living)
      if(GetAlive(i) != 1 && InputActive("Ignore Dead")) continue;
    if(Contained(i) && InputActive("Ignore Contained")) continue;
    if(pId && GetID(i) == pId) return(i);
    else if(!pId) return(i);
  }
}

private func Detect(){
  var Type;
  if(InputActive("ID String") && GetType(InputActive("ID String")) ==  C4V_String) Type = C4Id(InputActive("ID String"));
  else Type = RMMG;
  SetGraphics(nil,this(),Type,1,4);

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

    if(OutputName == "Iterated Object"){
      var DetectList = InputActive("Robot Sensor Results");
      if(!DetectList || GetType(DetectList) != C4V_Array) return(nil);
      return(DetectList[Abs(FrameCounter())%GetLength(DetectList)]);
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