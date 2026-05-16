#include GS__
#strict 2

public func StartCon(){ return(15); } //At what size does this gas start with?
public func EndCon(){ return(200); } //At what size will this gas finally fade out?
  public func SkyFade(){ 
    if(OnFire()) return(8);
    return(4);
  } //Speed for fading in the Sky material. 1 or 0 is rather quick.
  public func TunnelFade(){
    if(OnFire()) return(12);
    return(6);
  } //Speed for fading in anything other than sky.
public func LiquidFade(){ return(4); } //Speed for fading in liquid.
public func FadeOffset(){ return(50); } //when does the object start fading?
public func GasShiftChance(){ return(5); } //How commonly does the gas change direction?
public func FadeSize(){ return(RandomX(1,2)); } //when the gas fades, how much does it fade?

protected func RandomFloat(){ return(RandomX(30,120)); } //Random Float Stat
protected func AlphaOffset(){ return(RandomX(100,240)); } //Offset in alpha so every bit of smoke doesnt feel the same.
protected func StartSizeOffset(){ return(RandomX(-10,20)); } //Differenciate all gasses even more.

public func GasCustomTimer(){
  return(1); 
}

public func GasColor(){ return(RGBa(255,214,0)); } //used by pump