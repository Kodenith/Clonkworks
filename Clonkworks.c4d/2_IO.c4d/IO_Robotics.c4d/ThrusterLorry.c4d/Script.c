/*-- Lore --*/

#strict 3
#include LORY
#include IO__

public func OutputList(){
  return(["[Sensor] Nearby Objects","[Object/Camera] This"]);
}

public func InputList(){
  return(["Move Left","Move Right","Thrust Upwards","Dump Out","Collect"]);
}

public func HasCamera(){ return(1); }

private func RobotLogic(){
  var doSound = false;
  if(InputActive("Move Left")){
    doSound = true;
    if(!Random(5)) CreateParticle("Fire",12,0,RandomX(1,20),RandomX(-2,2),RandomX(30,70),RGBa(255,255,255));
    if(!Random(5)) CreateParticle("Fire2",12,0,RandomX(1,20),RandomX(-2,2),RandomX(30,70),RGBa(255,255,255));
    if(!Random(5)) Smoke(12,0,RandomX(10,20));
    if(GetXDir() > -20) SetXDir(GetXDir()-RandomX(2,5));
  }else if(InputActive("Move Right")){
    doSound = true;
    if(!Random(5)) CreateParticle("Fire",-12,0,-RandomX(1,20),RandomX(-2,2),RandomX(30,70),RGBa(255,255,255));
    if(!Random(5)) CreateParticle("Fire2",-12,0,-RandomX(1,20),RandomX(-2,2),RandomX(30,70),RGBa(255,255,255));
    if(!Random(5)) Smoke(-12,0,RandomX(10,20));
    if(GetXDir() < 20) SetXDir(GetXDir()+RandomX(2,5));
  }

  if(doSound) Sound("Elevator",false,this,50,0,+1);
  else Sound("Elevator",false,this,50,0,-1);

  if(InputActive("Thrust Upwards") && GBackSolid(0,12) && Abs(GetYDir()) < 20){
      Fling(this,0,-5,nil,true);
      Sound("Discharge");
      var am = RandomX(6,15); for(var i = 0; i < am; i++) CreateParticle("Fire",RandomX(-5,5),12,RandomX(-10,10),RandomX(5,20),RandomX(30,70),RGBa(255,255,255));
      am = RandomX(6,15); for(var i = 0; i < am; i++) CreateParticle("Fire2",RandomX(-5,5),12,RandomX(-10,10),RandomX(5,20),RandomX(30,70),RGBa(255,255,255));
      am = RandomX(2,8); Smoke(RandomX(-5,5),12,RandomX(10,20));
  }

  if(InputActive("Dump Out")) while(Contents()) Exit(Contents(),0,10);
  if(InputActive("Collect")){
    for(var col in FindObjects(Find_NoContainer(),Find_OCF(OCF_Collectible),Find_OnLine(-12,10,12,10))){
        Enter(this,col);
        Sound("Grapple");
    }
  }
}

public func OutputActive(string OutputName){
   if(OutputName == "[Sensor] Nearby Objects") return(FindObjects(Find_Distance(100)));
   if(OutputName == "[Object/Camera] This") return(this);
     return(0);
}

public func GetResearchBase() { return(LORY); }