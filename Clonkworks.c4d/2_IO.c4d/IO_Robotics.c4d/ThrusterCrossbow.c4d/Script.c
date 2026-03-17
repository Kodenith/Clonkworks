#strict 3
#include XBOW
#include IO__

public func OutputList(){
  return(["[Sensor] Nearby Objects","[Object/Camera] This"]);
}

public func InputList(){
  return(["Move Left","Move Right","Thrust Upwards","Aim Up","Aim Down","Fire","Turn"]);
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
     return(0);
}

public func GetResearchBase() { return(XBOW); }