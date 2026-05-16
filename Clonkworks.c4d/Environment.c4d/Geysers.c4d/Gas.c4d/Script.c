/*-- Neues Script --*/

#strict 2

local Gas;
local Random1;

local Intensity;

func Initialize() {
  while(GBackSolid()){
    SetY(GetY()-1);
    if(GetY() <= 0) break;
  }
  SetCategory(C4D_Vehicle); 

  SetGas(GS_1);
  SetRandom(RandomX(100,500));
  AddEffect("Gli",this,1,0,this);
  return(1);
}

func Damage(){
  if(GetDamage() > 150){
    //Let's make the destruction cool!
    // 1. Dirt - it's made of it.
    CastPXS("Earth",48*3,20);

    // 2. Spew stuff one last time.
    CastObjects(Gas,RandomX(100,200));

    // 3. Shake the camera.
    ShakeViewPort(15,0,GetX(),GetY());
    Sound("Earthquake");

    // 4. die.
    RemoveObject();
  }
}

public func SetGas(Type){
  /*
  if(Type == "DuroLava") SetName(Format("Lava Geyser",Type));
  else SetName(Format("%s Geyser",Type));
  */
  return(Gas = Type);
}

public func SetRandom(int Am){
  return(Random1 = Am);
}

private func Update(){
  if(GetAction() == "Idle"){
    if(!Random(Random1)){
      if(!GBackSolid(GetVertex(0,0),GetVertex(0,1))){
        Intensity = RandomX(10,25);
        SetAction("Spew");
      }
    }
  }else{
      if(!GBackSolid(GetVertex(0,0),GetVertex(0,1))){
        var Amount = 1;
        if(!Random(4)) return(0);
        while(Amount--){
          var Intensity1 = RandomX(10,25);
          var Intensity2 = RandomX(30,120);
          var Gas1 = CreateObject(Gas,0,RandomX(0,-10));
          Gas1->~SetComDir(COMD_Up);
        }
        if(Intensity < Intensity2) Intensity += RandomX(2,10);
        ShakeViewPort(Intensity/10,this,0,0);
      }
  }
}


/* EFFECT
for displaying data in the description. no more, no less. */

public func FxGliInfo(target,effectnum){
  return(Format("$IF$",Gas,GetName(,Gas)));
}