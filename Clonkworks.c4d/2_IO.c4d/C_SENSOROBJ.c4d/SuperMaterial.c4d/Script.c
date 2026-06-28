/* Super Sensor */
//it's logic is a lot different than for most sensors.

#strict 3
#include IO__

public func OutputList(){
  return(["Found Solid","Found Liquid","Found Background","Found Semi-Solid"]);
}

public func InputList(){
  return(["Angle","Start Radius","End Radius","Related Object"]);
}

func OutputActive(iName){
    var iAng = InputActive("Angle");
    if(GetType(iAng) != C4V_Int) iAng = 0;

    var iX1 = InputActive("Start Radius");
    if(GetType(iX1) != C4V_Int) iX1 = 0;

    var iX2 = InputActive("End Radius");
    if(GetType(iX2) != C4V_Int) iX2 = 1;

    var parent = InputActive("Related Object");
    if(GetType(parent) != C4V_C4Object) parent = this;

    for(var x = iX1; x < iX2; x++){
      var iX = Sin(iAng,x);
      var iY = -Cos(iAng,x);

      if(iName == "Found Solid" && parent->GBackSolid(iX,iY)) return(MaterialName(parent->GetMaterial(iX,iY)));
      if(iName == "Found Liquid" && parent->GBackLiquid(iX,iY)) return(MaterialName(parent->GetMaterial(iX,iY)));
      if(iName == "Found Background" && !parent->GBackSolid(iX,iY) && !parent->GBackLiquid(iX,iY) && parent->GetMaterial(iX,iY) != Material("Sky")) return(MaterialName(parent->GetMaterial(iX,iY)));
      if(iName == "Found Semi-Solid" && parent->GBackSemiSolid(iX,iY)) return(MaterialName(parent->GetMaterial(iX,iY)));
    }
}

func MustBeOnWall(){ return(1); }

func FoundSomething(){
  var IL = OutputList();
  for(var x in IL){
    if(OutputActive(x) && DoesOutputHaveWire(x)) return(1);
  }
}

func Detect(){
  //i only ever use this for state checks
  if(FoundSomething() && GetAction() == "Attached"){
    SetAction("Active");
    if(!Locked) Sound("Command");
  }else if(!FoundSomething() && GetAction() != "Attached"){
    SetAction("Attached");
    if(!Locked) Sound("Command");
  }

  if(FindObject2(Find_ActionTarget(this),Find_Action("Push")) && FrameCounter()%15 == 0)
    DrawRangeCircle();
}

private func DrawRangeCircle(){
    var iAng = InputActive("Angle");
    if(GetType(iAng) != C4V_Int) iAng = 0;

    var iX1 = InputActive("Start Radius");
    if(GetType(iX1) != C4V_Int) iX1 = 0;

    var iX2 = InputActive("End Radius");
    if(GetType(iX2) != C4V_Int) iX2 = 1;

    var Parent = InputActive("Related Object");
    if(GetType(Parent) != C4V_C4Object) Parent = this;

    for(var x = iX1; x < iX2; x++){
      var iX = Sin(iAng,x);
      var iY = -Cos(iAng,x);

      if(!Random(2)) Parent->CreateParticle("NoGravSpark", iX, iY , 0, 0, 25, RGBa(0,255,0));
    }
  //Parent->CreateParticle("NoGravSpark", Sin(x, Range, 1)+XOff, Cos(x, Range, 1)+YOff , Sin(x, 0, 1), Cos(x, 0, 1), 25, RGBa(0,255,0));
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