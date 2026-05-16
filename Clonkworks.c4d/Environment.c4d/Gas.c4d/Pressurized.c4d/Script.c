#include GS__
#strict 2

public func StartCon(){ return(10); } //At what size does this gas start with?
public func EndCon(){ return(100); } //At what size will this gas finally fade out?
public func SkyFade(){ return(3); } //Speed for fading in the Sky material. 1 or 0 is rather quick.
public func TunnelFade(){ return(3); } //Speed for fading in anything other than sky.
public func LiquidFade(){ return(50); } //Speed for fading in liquid.
public func FadeOffset(){ return(30); } //when does the object start fading?
public func GasShiftChance(){ if(GetMaterial() == Material("Sky")) return(5); else return(20);  } //How commonly does the gas change direction?
public func FadeSize(){ return(RandomX(1,3)); } //when the gas fades, how much does it fade?

protected func RandomFloat(){ return(RandomX(100,350)); } //Random Float Stat
protected func AlphaOffset(){ return(RandomX(160,240)); } //Offset in alpha so every bit of smoke doesnt feel the same.
protected func StartSizeOffset(){ return(RandomX(0,50)); } //Differenciate all gasses even more.

public func GasCustomTimer(){
  if(!GBackLiquid()){
  var Victims = FindObjects(Find_Distance(GetCon()/5),Find_NoContainer(),Find_Or(Find_Category(C4D_Living),Find_Category(C4D_Vehicle),Find_Category(C4D_Object)),Find_Not(Find_ID(GEY1)),Find_Not(Find_ID(GEY2)),Sort_Random());
  for(var Victim in Victims){
    //all nearbv objects are to be flung!
    var Power = 5;
    var Ang = Angle(GetX(),GetY(),GetX(Victim),GetY(Victim));
    var Xd = Sin(Ang, Power);
    var Yd = -Cos(Ang, Power);

    Fling(Victim,Xd,Yd,1,1);
  }
  }else{
    var Sawnicks = FindObjects(Find_Distance(GetCon()/7),Find_NoContainer(),Find_OCF(OCF_Alive));
    //help a drowning creature breathe.
    for(var Sawnick in Sawnicks){
      if(GetBreath(Sawnick) > 50) continue;
      if(!Sawnick->GBackLiquid()) continue;
      DoBreath(100,Sawnick);
      if(Sawnick->~IsClonk()) Sound("Breath");
      else Sound("Snuff*");
      RemoveObject();
      break;
    }
  }

  //if(GBackLiquid()) SetObjectBlitMode(2);
  return(1); 
}

public func GasColor(){ return(RGBa(255,255,255)); } //used by pump
//public func GasName(){ return("Toxic Gas"); } //used by pump, used instead of GetName to prevent potential sync errors