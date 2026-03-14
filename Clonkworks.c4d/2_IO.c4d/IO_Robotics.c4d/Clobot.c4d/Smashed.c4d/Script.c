/*-- Neues Script --*/

#strict

func Initialize() {
  SetAction("Die");
  return(1);
}

private func Sparking(){
  if(GetActTime() > 38*120 || OnFire()) return(SetAction("Deteriorate"));
  Smoke(0,0,RandomX(1,15));
  if(Random(7)) return(0);
  if(!Random(100)) Incinerate();
  CastParticles("PxSpark",RandomX(2,6),RandomX(5,50),0,5,20,50,RGBa(255,255,0),RGBa(255,150,0));
  Sound("Spark*");
}

func Rem(){
  if(OnFire()) RemoveObject();
  else Split2Components();
}

func Incineration(){
  SetClrModulation(RGBa(255,233,233));
}

func ControlThrow(pClonk){
  [$TxtRevive$|Image=ADM3]
  if(GetID(Contents(,pClonk)) == ADM3){
      RemoveObject(Contents(,pClonk));
      if(!Random(10)) Explode(15);
      else Revive();
  }
}

public func Revive(){
  var newClobot = CreateObject(CLBT,0,10);
  Sound("Electric");
  newClobot->SetAction("FlatUp");
  RemoveObject();
}