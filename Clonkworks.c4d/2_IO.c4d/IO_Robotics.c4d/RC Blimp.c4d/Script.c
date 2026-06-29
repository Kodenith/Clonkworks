/*-- RC Blimp--*/

#strict 2
#include IO__

public func OutputList(){
  return(["[Object/Camera] This"]);
}

public func InputList(){
  return(["Move Left","Move Right","Move Up","Move Down","Drop Item","Stop Flight","Start Flight (Alternative)","Move To Object"]);
}

public func HasCamera(){ return(1); }

public func OutputActive(string OutputName){
   if(OutputName == "[Object/Camera] This") return(this);
  return(0);
}

func Initialize(){
  SetAction("OnGround");
}

func Logic(){
  var HorD, VerD;
  if(InputActive("Move Left")) HorD = COMD_Left;
  else if(InputActive("Move Right")) HorD = COMD_Right;
  else HorD = 0;

  if(InputActive("Move Up")) VerD = COMD_Up;
  else if(InputActive("Move Down")) VerD = COMD_Down;
  else VerD = 0;

  var FinD = 0;
  if(HorD == COMD_Left && VerD == COMD_Up) FinD = COMD_UpLeft;
  else if(HorD == COMD_Right && VerD == COMD_Up) FinD = COMD_UpRight;
  else if(HorD == COMD_Left && VerD == COMD_Down) FinD = COMD_DownLeft;
  else if(HorD == COMD_Right && VerD == COMD_Down) FinD = COMD_DownRight;
  else if(HorD) FinD = HorD;
  else FinD = VerD;

  if(InputActive("Move To Object")){
    var ToObj = InputActive("Move To Object");
    if(GetType(ToObj) == C4V_C4Object && !GetCommand()){
      SetCommand(this,"MoveTo",ToObj);
    }
  }

  if(FinD) FinishCommand();

  if(HorD == COMD_Right && GetDir() == DIR_Left) SetDir(DIR_Right);
  if(HorD == COMD_Left && GetDir() == DIR_Right) SetDir(DIR_Left);
  
  if( (FinD && GetAction() == "OnGround") || InputActive("Start Flight (Alternative)")) SetAction("Fly");
  if(!GetCommand()){
    if(GetProcedure() == "FLOAT") SetComDir(FinD);
    if(!HorD && GetProcedure() == "FLOAT") SetXDir(0);
    if(!VerD && GetProcedure() == "FLOAT") SetYDir(0);
  }else{
    if(GetXDir() > 0) SetDir(DIR_Right);
    else if(GetXDir() <= 0) SetDir(DIR_Left);
  }


  if(InputActive("Drop Item") && Contents()){
      Contents()->Exit(0,0,15);
  }

  if(InputActive("Stop Flight") && GetProcedure() == "FLOAT") SetAction("OnGround");

  if(GetDir() == DIR_Left && FrameCounter()%15 == 0 && GetAction() == "Fly") CreateParticle("NoGravSpark",12,5,10,RandomX(-2,2),RandomX(10,25),RGBa(255,255,255));
  if(GetDir() == DIR_Right && FrameCounter()%15 == 0 && GetAction() == "Fly") CreateParticle("NoGravSpark",-12,5,-10,RandomX(-2,2),RandomX(10,25),RGBa(255,255,255));
}

func Incineration(){
  SetAction("Destroyed");
}
func IncinerationEx(){
  SetAction("Destroyed");
}

func Grabbed(pBy,fGrab){
  if(fGrab) SetAction("OnGround");
}

func Collection(pObj,fPut){
  if(!fPut) Sound("Grapple");
}

public func GetResearchBase() { return(BALN); }