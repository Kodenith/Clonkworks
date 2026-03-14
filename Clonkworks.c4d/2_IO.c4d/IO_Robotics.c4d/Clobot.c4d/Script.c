#strict 3
#include CLNK
#include IO__

func Initialize(){
  SetAction("Walk");
  SetDir(Random(2));
  Resort(this);
}

public func OutputList(){
  return(["[Sensor] Nearby Objects","[Object/Camera] This","[Object] Held item"]);
}

public func InputList(){
  return(["Move Left","Move Right","Move Up","Move Down","Jump","Dig","Activiate Held","Throw","Chop","[Object] Get","[Object] Grab","UnGrab","[Object] Go to","Cancel Command"]);
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

  if(!InputActive("Chop") && !InputActive("[Object] Get") && !InputActive("[Object] Grab") && !InputActive("[Object] Go to")){
    //if(MoveDir != COMD_Stop) FinishCommand();
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
  }

  if(InputActive("Jump")){
    Jump();
  }else if(InputActive("Dig") && GetAction() == "Walk"){
    SetAction("Dig");
  }

  if(Contents()){
    if(InputActive("Activiate Held")) Contents()->~Activate();
    if(InputActive("Throw")){
      SetCommand(this,"Throw");
      return(nil);
    }
  }

  if(InputActive("Chop")) ContextChop();

  if(InputActive("Cancel Command")) return(FinishCommand());
  
  if(InputActive("[Object] Get") && GetType(InputActive("[Object] Get")) == C4V_C4Object && !GetCommand()){
      SetCommand(this,"Get",InputActive("[Object] Get"));
      return(nil);
  }

  if(InputActive("[Object] Grab") && GetType(InputActive("[Object] Grab")) == C4V_C4Object && !GetCommand()){
      SetCommand(this,"Grab",InputActive("[Object] Grab"));
      return(nil);
  }

  if(InputActive("[Object] Go to") && GetType(InputActive("[Object] Go to")) == C4V_C4Object && !GetCommand()){
      SetCommand(this,"MoveTo",InputActive("[Object] Go to"));
      return(nil);
  }

  if(InputActive("UnGrab")){
      SetCommand(this,"UnGrab");
      return(nil);
  }
}

public func OutputActive(string OutputName){
   if(OutputName == "[Sensor] Nearby Objects") return(FindObjects(Find_Distance(350),Sort_Distance()));
   if(OutputName == "[Object/Camera] This") return(this);
   if(OutputName == "[Object] Held item") return(Contents());
     return(0);
}


protected func Hurt()
{
  Sound("ClonkHit*");
  return(1);
}

private func Punching()
{
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