/*-- Neues Script --*/

#strict 2

local Liquid;
local Random1;

local Intensity;

func Initialize() {
  while(GBackSolid()){
    SetY(GetY()-1);
    if(GetY() <= 0) break;
  }
  SetCategory(C4D_Vehicle); 

  SetLiquid("Lava");
  SetRandom(RandomX(120,1500));
  AddEffect("Gli",this,1,0,this);
  return(1);
}

func Damage(){
  if(GetDamage() > 150){
    //Let's make the destruction cool!
    // 1. Dirt - it's made of it.
    CastPXS("Earth",48*3,20);

    // 2. Spew stuff one last time.
    CastPXS(Liquid,RandomX(100,280),RandomX(40,100));

    // 3. Shake the camera.
    ShakeViewPort(15,0,GetX(),GetY());
    Sound("Earthquake");

    // 4. die.
    RemoveObject();
  }
}

public func SetLiquid(string Type){
  /*
  if(Type == "DuroLava") SetName(Format("Lava Geyser",Type));
  else SetName(Format("%s Geyser",Type));
  */
  return(Liquid = Type);
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
        var Amount = RandomX(10,95);
        if(!Random(2)) return(0);
        while(Amount--){
          var Intensity1 = RandomX(10,25);
          var Intensity2 = RandomX(30,120);
          CastPXSX(Liquid,1,RandomX(Intensity1,Intensity),GetVertex(),GetVertex(0,1),-90+RandomX(-20,20));
        }
        if(Intensity < Intensity2) Intensity += RandomX(2,10);
        ShakeViewPort(Intensity/3,this,0,0);
      }
  }
}


/* EFFECT
for displaying data in the description. no more, no less. */

public func FxGliInfo(target,effectnum){
  var cId = RMMG; //random magic id, its a bunch of questionmarks.
  if(GetBarrelType(Material(Liquid))) cId = GetBarrelType(Material(Liquid)); //id fo a barrel if it exists

  return(Format("$IF$",cId,Liquid));
}

//for goal
public func IsOilGeyser(){
  if(Liquid == "Oil") return(1);
}