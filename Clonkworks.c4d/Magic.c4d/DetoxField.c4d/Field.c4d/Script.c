/*-- Neues Objekt --*/

#strict 2

local Size;
local Initialized;

func Initialize() {
  if(Initialized) return(0);
  Sound("Magic1");
  Sound("Energy",0,this,0,0,+1);
  Initialized=1;
  DoEnergy(100);
  Size = 80;
  SetAlive(1,this);
  SetCon(1);
  return(1);
}

protected func Destruction(){
    Sound("DeEnergize");
}

public func GetParticleColor(){
  var List = [RGBa(0,255,51),RGBa(61,196,34),RGBa(146,253,129)];
  return(List[RandomX(0,2)]);
}

protected func Logic(){
  //grow according to size.
  if(GetCon() < Size) DoCon(1);
  if(GetCon() > Size) DoCon(-1);

  //deteriorate every second
  if(FrameCounter() % 38 == 0){
      DoEnergy(-1);
  }

  //Message(Format("%d",GetEnergy()));

  //if dead, shrink every second.
  if(!GetAlive(this) && FrameCounter() % 38 == 0){
    Size--;
  }

  //draw a circle around this
  for(var x = 180; x >= -180; x-=2){
    if(!Random(30)) CreateParticle("NoGravSpark", Sin(x, GetCon(), 1)+RandomX(-3,3), Cos(x, GetCon(), 1)+RandomX(-3,3) , Sin(x, 0, 1), Cos(x, 0, 1), RandomX(10,40), GetParticleColor());
  }

  //Push out gasses
  for(var Gas in FindObjects(Find_Distance(GetCon()),Find_Func("isGas"),Find_NoContainer())){
    //Cool particles
    Gas->CastParticles("PxSpark",3,13,0,0,10,40,GetParticleColor(),GetParticleColor());
    //Move out of the way.
    var RotRef = Angle(GetX(Gas),GetY(Gas),GetX(),GetY());
		RotRef= (RotRef + 180) % 360 - 180;
    var iXDir = Sin(RotRef, 1);
		var iYDir = -Cos(RotRef, 1);
    SetX(GetX(Gas)-iXDir,Gas);
		SetY(GetY(Gas)-iYDir,Gas);
  }
}