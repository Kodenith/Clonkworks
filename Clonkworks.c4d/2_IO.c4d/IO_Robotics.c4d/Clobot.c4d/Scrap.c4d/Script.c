/*-- Neues Script --*/

#strict 3

local Fader;

func Initialize() {
  SetAction("Scrap");
  SetPhase(RandomX(0,8));
  SetRDir(RandomX(-50,50));
  SetCon(RandomX(80,120));
  Fader = 0;
  return(1);
}

func Fade(){
  DoCon(RandomX(-1,0));
  Fader++;
  if(Fader == 255) RemoveObject();
  else{
    SetClrModulation(RGBa(255,255,255,Fader));
  }
}

func Hit(){ SetRDir(0); }