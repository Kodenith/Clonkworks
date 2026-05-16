/* GAS SYSTEM by Kodenith */
#strict 2

public func StartCon(){ return(15); } //At what size does this gas start with?
public func EndCon(){ return(150); } //At what size will this gas finally fade out?
public func SkyFade(){ return(1); } //Speed for fading in the Sky material. 1 or 0 is rather quick.
public func TunnelFade(){ return(15); } //Speed for fading in anything other than sky.
public func LiquidFade(){ return(4); } //Speed for fading in liquid.
public func FadeOffset(){ return(50); } //when does the object start fading?
public func GasShiftChance(){ return(10); } //How commonly does the gas change direction?
public func FadeSize(){ return(RandomX(1,2)); } //when the gas fades, how much does it fade?

protected func RandomFloat(){ return(RandomX(30,110)); } //Random Float Stat
protected func AlphaOffset(){ return(RandomX(-45,200)); } //Offset in alpha so every bit of smoke doesnt feel the same.
protected func StartSizeOffset(){ return(RandomX(-10,10)); } //Differenciate all gasses even more.

local Alpha; //Transparency.
local Initialized; //To Avoid accidentally initializing again.
local AlphaOff;

//When this gas is made, set default values and actions.
func Initialize(){
  if(GetID() == GS__) return(RemoveObject());
  if(Initialized) return(0);
  Initialized = true;
  SetCon(StartCon()+StartSizeOffset());
  Alpha = 0;
  SetAction("Smoke");

  SetPhysical("Float",RandomFloat(),2,this);
  AlphaOff = AlphaOffset();
}

//random smoke sprite selection.
private func RandomSmoke(){
  SetPhase(RandomX(0,14));
  if(!Random(100)) SetPhase(15);
}

//random bubble sprite selection
private func RandomBubble(){
  SetPhase(RandomX(0,3));
}

//Go in a random direction. Used for caves. Also global.
global func RandomComDir(){
    var Comdirs = [COMD_Down, COMD_DownLeft, COMD_DownRight, COMD_Left, COMD_Right, COMD_Up,COMD_UpLeft,COMD_UpRight];
    var Com = Comdirs[RandomX(0,7)];
    SetComDir(Com);
    return(Com);
}

//Same as above, but it only goes up. Not global
protected func RandomComDirSky(){
    var Comdirs = [COMD_Up,COMD_UpLeft,COMD_UpRight];
    var Com = Comdirs[RandomX(0,2)];
    SetComDir(Com);
    return(Com);
}

//fades the gas based on the functions at the very top. Offset is kept in mind as well as StartCon and EndCon
func UpdateAlphaByCon() {
    //i dont want to gnaw my eyes out do i? i put all the data into a function.
    var CurrentCon = this->GetCon();
    var LStartCon = this->StartCon();
    var LEndCon = this->EndCon();
    var Offset = this->FadeOffset();

    if (LEndCon <= LStartCon) return(-1); 

    var CurrentPct = ((CurrentCon - LStartCon) * 100) / (LEndCon - LStartCon);
    var alph = 0;

    if (CurrentPct >= Offset) {
        var FadeSize = 100 - Offset;
        if (FadeSize > 0) {
            var Conversion = ((CurrentPct - Offset) * 100) / FadeSize;
            alph = (Conversion * 255) / 100;
        }
    }

    alph += AlphaOff;
    alph = BoundBy(alph, 0, 255);
    return(alph);
}

//Gas Fading Logic.
protected func SmokeUpdate(){

  //Fading
  if(!Contained()){
  if(GetMaterial() == Material("Sky")){
    if(!Random(SkyFade())) DoCon(FadeSize());
  }else if(GBackLiquid()){
    if(!Random(LiquidFade())) DoCon(FadeSize());
  }else if(GBackSolid()){
    for(var i = 0; i < 5; i++){
      SetY(GetY()+1);
      if(!GBackSolid()) break;
    }
    if(GBackSolid());
    return(RemoveObject()); //no gas in the ground.
  }else{
    if(!Random(TunnelFade())) DoCon(FadeSize());
  }
  }

  Alpha = UpdateAlphaByCon();
  SetClrModulation(RGBa(255,255,255,Alpha),this);
  if(Alpha == 255) return(RemoveObject());

  //Spriting and color modulation for being outside and in liquid.
  if(!GBackLiquid() && GetAction() == "Bubble"){
    SetAction("Smoke");
    SetObjectBlitMode(0,this);
  }else if(GBackLiquid() && GetAction() == "Smoke"){
    SetAction("Bubble");
    SetObjectBlitMode(1,this);
  }

  //Direction Choice
  if(!Random(GasShiftChance())){
    if(GetMaterial() == Material("Sky") || GBackLiquid()) RandomComDirSky();
    else RandomComDir();
  }

  //Custom Timer Stuff :)
  if(!Contained())
  GasCustomTimer();
}

public func GasCustomTimer(){
  return(1); //play around with this on your own gas object!
}

public func GasColor(){ return(RGBa(255,255,255)); } //used by pump
//public func GasName(){ return("Gas"); } //used by pump, used instead of GetName to prevent potential sync errors