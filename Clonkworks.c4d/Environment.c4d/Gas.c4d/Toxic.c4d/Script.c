#include GS__
#strict 2

public func StartCon(){ return(50); } //At what size does this gas start with?
public func EndCon(){ return(145); } //At what size will this gas finally fade out?
public func SkyFade(){ return(4); } //Speed for fading in the Sky material. 1 or 0 is rather quick.
public func TunnelFade(){ return(28); } //Speed for fading in anything other than sky.
public func LiquidFade(){ return(5); } //Speed for fading in liquid.
public func FadeOffset(){ return(90); } //when does the object start fading?
public func GasShiftChance(){ return(18); } //How commonly does the gas change direction?
public func FadeSize(){ return(RandomX(1,2)); } //when the gas fades, how much does it fade?

protected func RandomFloat(){ return(RandomX(10,80)); } //Random Float Stat
protected func AlphaOffset(){ return(RandomX(200,240)); } //Offset in alpha so every bit of smoke doesnt feel the same.
protected func StartSizeOffset(){ return(RandomX(-20,10)); } //Differenciate all gasses even more.

public func GasCustomTimer(){
  var Victim = FindObject2(Find_Distance(GetCon()/7),Find_NoContainer(),Find_OCF(OCF_Alive),Find_Not(Find_Func("GasResistant")),Sort_Random());
  if(Victim && !Random(8)) DoEnergy(-1,Victim);
  return(1); 
}

public func GasColor(){ return(RGBa(0,200,0)); } //used by pump
//public func GasName(){ return("Toxic Gas"); } //used by pump, used instead of GetName to prevent potential sync errors