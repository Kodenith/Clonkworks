#strict 3
#include CANN
#include IO__

local LastFired;

func Initialize(){
  LastFired = 0;
  SetPlrViewRange(200,this);
  return(_inherited());
}

public func OutputList(){
  return(["[Object/Camera] This","Current Angle"]);
}
public func InputList(){
  return(["Move Left","Move Right","Thrust Upwards","Aim Up","Aim Down","Aim to Angle","Fire","Turn"]);
}

public func HasCamera(){ return(1); }

private func RobotLogic(){
  var doSound = false;
  if(InputActive("Move Left")){
    doSound = true;
    if(!Random(5)) CreateParticle("Fire",16,0,RandomX(1,20),RandomX(-2,2),RandomX(30,70),RGBa(255,255,255));
    if(!Random(5)) CreateParticle("Fire2",16,0,RandomX(1,20),RandomX(-2,2),RandomX(30,70),RGBa(255,255,255));
    if(!Random(5)) Smoke(16,0,RandomX(10,20));
    if(GetXDir() > -20) SetXDir(GetXDir()-RandomX(2,5));
  }else if(InputActive("Move Right")){
    doSound = true;
    if(!Random(5)) CreateParticle("Fire",-16,0,-RandomX(1,20),RandomX(-2,2),RandomX(30,70),RGBa(255,255,255));
    if(!Random(5)) CreateParticle("Fire2",-16,0,-RandomX(1,20),RandomX(-2,2),RandomX(30,70),RGBa(255,255,255));
    if(!Random(5)) Smoke(-16,0,RandomX(10,20));
    if(GetXDir() < 20) SetXDir(GetXDir()+RandomX(2,5));
  }

    if(doSound) Sound("Elevator",false,this,50,0,+1);
  else Sound("Elevator",false,this,50,0,-1);

  if(InputActive("Thrust Upwards") && GBackSolid(0,14) && Abs(GetYDir()) < 20){
      Fling(this,0,-5,nil,true);
      Sound("Discharge");
      var am = RandomX(6,15); for(var i = 0; i < am; i++) CreateParticle("Fire",RandomX(-5,5),14,RandomX(-10,10),RandomX(5,20),RandomX(30,70),RGBa(255,255,255));
      am = RandomX(6,15); for(var i = 0; i < am; i++) CreateParticle("Fire2",RandomX(-5,5),14,RandomX(-10,10),RandomX(5,20),RandomX(30,70),RGBa(255,255,255));
      am = RandomX(2,8); Smoke(RandomX(-5,5),12,RandomX(10,20));
  }

  if(FrameCounter()%4 == 0){
  if(InputActive("Aim to Angle") && GetType(InputActive("Aim to Angle")) == C4V_Int){
      WireAimAt(InputActive("Aim to Angle"));
  }
  else
  if(InputActive("Aim Up")){
      if(GetPhase() > 0 && FrameCounter()%4 == 0){
        SetPhase(GetPhase()-1);
        Sound("Command");
      }
  }else if(InputActive("Aim Down")){
      if(GetPhase() < 18 && FrameCounter()%4 == 0){
        SetPhase(GetPhase()+1);
        Sound("Command");
      }
  }
  }

  if(InputActive("Turn")) SetDir(1);
  else if(DoesInputHaveWire("Turn")) SetDir(0);

  if(InputActive("Fire") && LastFired < FrameCounter()-38){
      Fire(true);
      LastFired = FrameCounter();
  }


}

public func OutputActive(string OutputName){
   if(OutputName == "[Object/Camera] This") return(this);
   if(OutputName == "Current Angle"){
    var iAngle = GetPhase() * 90 / 20;
    if(GetDir() == DIR_Left) iAngle = -iAngle;
    return(iAngle);
   }
    return(0);
}
public func GetResearchBase() { return(CANN); }

func WireAimAt(iAngle){
  while(iAngle > 180) iAngle-=360;  
  // Richtung
  //if(iAngle > 0) SetDir(DIR_Right);
  //if(iAngle < 0) SetDir(DIR_Left);
  // Zielrichtung
  var goal = BoundBy( 20*Abs(iAngle)/90, 0,18);
  if(GetPhase() > goal){
          SetPhase(GetPhase()-1);
          Sound("Command");
  }else if(GetPhase() < goal){
          SetPhase(GetPhase()+1);
          Sound("Command");
  }
}

public func ControlThrow(pClonk)    // Feuern / Inhalt
{
  // Der Clonk will doch bestimmt nur etwas nachladen: nicht vorzeitig abfeuern
  var pThing;
  if (pThing = pClonk->Contents())
    if (AllowLoad(GetID(pThing)))
      return(0);
  // Abfeuern wenn m�glich, sonst Objekt herausnehmen
  return(1);
}