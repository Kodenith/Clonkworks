#strict 3
#include CLNK
#include IO__

func Initialize(){
  SetAction("Walk");
  SetDir(Random(2));
  Resort(this);
  SetPlrViewRange(500,this);
}

public func OutputList(){
  return(["[Sensor] Nearby Objects","[Sensor] Nearby Objects (Unordered)","[Object/Camera] This","[Object] Held item"]);
}

public func InputList(){
  return(["Move Left","Move Right","Move Up","Move Down","Jump","Dig","Stop Dig","Activiate Held","Throw","Chop","Grab","Ungrab","Follow Object","Acquire Object","Cancel Command"]);
}

public func HasCamera(){ return(1); }

/* Incredible timer function that takes up an entire city */
private func HandleInput(){
  var MoveDir = COMD_Stop;
  if(InputActive("Move Left")) MoveDir = COMD_Left;
  if(InputActive("Move Right")) MoveDir = COMD_Right;
  if(InputActive("Move Up") && InputActive("Move Left")) MoveDir = COMD_UpLeft;
  if(InputActive("Move Up") && InputActive("Move Right")) MoveDir = COMD_UpRight;
  if(InputActive("Move Down") && InputActive("Move Left")) MoveDir = COMD_DownLeft;
  if(InputActive("Move Down") && InputActive("Move Right")) MoveDir = COMD_DownRight;

  if(InputActive("Cancel Command")) FinishCommand();

    SetComDir(MoveDir);
    if(GetContact(this, -1) & CNAT_Right) if(GetAction() == "Walk" && GetDir() == DIR_Right) SetAction("Scale");
    if(GetContact(this, -1) & CNAT_Left) if(GetAction() == "Walk" && GetDir() == DIR_Left) SetAction("Scale");
    if(GetContact(this, -1) & CNAT_Right) if(GetAction() == "Jump" && GetDir() == DIR_Right) SetAction("Scale");
    if(GetContact(this, -1) & CNAT_Left) if(GetAction() == "Jump" && GetDir() == DIR_Left) SetAction("Scale");
    if(GetContact(this, -1) & CNAT_Top) if(GetAction() == "Scale") SetAction("Hangle");
    if(GetContact(this, -1) & CNAT_Right) if(GetAction() == "Scale" && GetDir() == DIR_Right && MoveDir == COMD_Left){
      SetXDir(-5);
      SetAction("Jump");
    }
    if(GetContact(this, -1) & CNAT_Left) if(GetAction() == "Scale" && GetDir() == DIR_Left && MoveDir == COMD_Right){
      SetXDir(5);
      SetAction("Jump");
    }
    if(GetContact(this, -1) & CNAT_Top) if(GetAction() == "Hangle" && MoveDir == COMD_Down){
      SetAction("Jump");
    }

  if(InputActive("Jump")){
    Jump();
  }else if(InputActive("Dig") && GetAction() == "Walk"){
    SetAction("Dig");
  }

  if(GetAction() == "Dig" && !InputActive("Stop Dig")) SetAction("Walk");

  if(Contents()){
    if(InputActive("Activiate Held")) Contents()->~Activate(this);
    if(InputActive("Throw")){
      SetCommand(this,"Throw");
      return(nil);
    }
  }

  if(InputActive("Chop")) ContextChop();

  if(InputActive("Grab") && !GetCommand()){
      var ToGrab = FindObject2(Find_AtPoint(),Find_NoContainer(),Find_OCF(OCF_Grab));
      if(ToGrab)
      SetCommand(this,"Grab",ToGrab);
  }

  if(InputActive("Follow Object") && GetType(InputActive("Follow Object")) == C4V_C4Object && !GetCommand()){
      SetCommand(this,"MoveTo",InputActive("Follow Object"));
  }

  if(InputActive("Acquire Object") && GetType(InputActive("Acquire Object")) == C4V_C4Object && !GetCommand() && (GetOCF(InputActive("Acquire Object")) & OCF_Collectible)){
      SetCommand(this,"Get",InputActive("Acquire Object"));
  }

  if(InputActive("Ungrab")){
      SetCommand(this,"UnGrab");
  }

  if(InputActive("Cancel Command")) FinishCommand();
}

public func OutputActive(string OutputName){
   if(OutputName == "[Sensor] Nearby Objects") return(FindObjects(Find_Distance(500),Sort_Distance()));
   if(OutputName == "[Sensor] Nearby Objects (Unordered)") return(FindObjects(Find_Distance(500)));
   if(OutputName == "[Object/Camera] This") return(this);
   if(OutputName == "[Object] Held item") return(Contents());
     return(0);
}


protected func Hurt()
{
  Sound("ClonkHit*");
  CastParticles("PxSpark",RandomX(2,6),RandomX(5,50),0,5,20,50,RGBa(255,255,0),RGBa(255,150,0));
  return(1);
}

private func Punching()
{
  if (!Random(3)) Sound("ClonkHit*");
  if (!Random(5)) Sound("MetalHit*");
  if (!Random(2)) return(1);
  Punch(GetActionTarget());
  return(1);
}

protected func Death(int iKilledBy)
{
  // Der Broadcast k�nnte seltsame Dinge gemacht haben: Clonk ist noch tot?
  if (GetAlive()) return(nil);
  
  // den Beutel fallenlassen
  if(GetAlchemBag()) GetAlchemBag()->~Loose();

  Sound("ClobotDeath");
  CastObjects(CLSC,RandomX(3,7),RandomX(20,45));
  CastParticles("PxSpark",RandomX(2,6),RandomX(30,70),0,5,20,50,RGBa(255,255,0),RGBa(255,150,0));
  MakeCorpse(OnFire());
  return(1);
}
protected func Destruction()
{
  return(1);
}

private func MakeCorpse(burn){
  var corpse = CreateObject(SCLB,0,10);
  SetXDir(GetXDir(),corpse);
  SetYDir(GetYDir(),corpse);
  SetDir(GetDir(),corpse);
  if(burn) Incinerate(corpse); 
  return(RemoveObject());
}

public func isClobot(){ return(1); }