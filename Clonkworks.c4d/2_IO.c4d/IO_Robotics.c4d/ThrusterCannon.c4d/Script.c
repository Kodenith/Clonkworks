#strict 3
#include CANN
#include IO__

func Initialize(){
  SetPlrViewRange(200,this);
  return(_inherited());
}

public func OutputList(){
  return(["[Sensor] Nearby Objects","[Object/Camera] This","[Sensor] Nearby Objects (Unordered)","[Sensor] Nearby Objects (Object)","[Sensor] Nearby Objects (Vehicle)","[Sensor] Nearby Objects (Living)","[Sensor] Nearby Objects (Structure)"]);
}
public func InputList(){
  return(["Move Left","Move Right","Thrust Upwards","Aim Up","Aim Down","Fire","Turn","Aim At"]);
}

public func HasCamera(){ return(1); }

private func RobotLogic(){
  var doSound = false;
  if(FrameCounter()%4 == 0){
  if(InputActive("Aim At") && GetType(InputActive("Aim At")) & C4V_C4Object){
    WireAimAt(InputActive("Aim At"));
  }else
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

  if(InputActive("Turn")) SetDir(1);
  else if(DoesInputHaveWire("Turn")) SetDir(0);

  if(InputActive("Fire")) Fire(true);


}

public func OutputActive(string OutputName){
   if(OutputName == "[Sensor] Nearby Objects") return(FindObjects(Find_Distance(200),Sort_Distance()));
   if(OutputName == "[Object/Camera] This") return(this);
    if(OutputName == "[Sensor] Nearby Objects (Unordered)") return(FindObjects(Find_Distance(200)));
   if(OutputName == "[Sensor] Nearby Objects (Object)") return(FindObjects(Find_Distance(200),Find_Category(C4D_Object),Sort_Distance()));
   if(OutputName == "[Sensor] Nearby Objects (Vehicle)") return(FindObjects(Find_Distance(200),Find_Category(C4D_Vehicle),Sort_Distance()));
   if(OutputName == "[Sensor] Nearby Objects (Living)") return(FindObjects(Find_Distance(200),Find_Category(C4D_Living),Sort_Distance()));
   if(OutputName == "[Sensor] Nearby Objects (Structure)") return(FindObjects(Find_Distance(200),Find_Category(C4D_Structure),Sort_Distance()));
     return(0);
}
public func GetResearchBase() { return(CANN); }

func WireAimAt(pObj){
  var iX = GetX(pObj);
  var iY = GetY(pObj);
  var iAngle;
  // Zielwinkel
  iAngle = Angle(GetX(),GetY(),iX,iY);
  // Bei gr��erer Distanz h�her zielen
  if(Inside(iX-GetX(),+1,+300))
     iAngle -= Abs(iX-GetX())/12;
  if(Inside(iX-GetX(),-300,-1))
     iAngle += Abs(iX-GetX())/12;

  while(iAngle > 180) iAngle-=360;
  // Richtung
  if(iAngle > 0) SetDir(DIR_Right);
  if(iAngle < 0) SetDir(DIR_Left);
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