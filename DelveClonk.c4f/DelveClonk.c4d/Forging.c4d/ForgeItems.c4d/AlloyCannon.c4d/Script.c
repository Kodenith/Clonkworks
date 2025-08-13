/*-- Haubitze --*/

#strict
#include CANN

local Color;
local Mass;
local Speed;
local Power;

protected func Initialize()
{
  SetAction("Ready");
  SetPhase(10);
  
  	Color = RGBa(177, 171, 170, 0);
	Mass = 3;
	Speed = 4;
	Power = 1;
}

func FRGUpdate(){
	//SetColorDw(Color, this());
	SetMass(35*Mass, this());
	SetClrModulation(Color, this());
}

public func ControlUp(object clonk)     // Zielen: hoch (klassisch)
{
  [$TxtAimup$|Image=CAN1:2]
  AimUp(clonk, Speed, "ControlConf");
}

public func ControlDig(object clonk)    // Zielen: runter (klassisch)
{
  [$TxtAimdown$|Method=Classic|Image=CAN1:0]
  AimDown(clonk, Speed, "ControlConf");
}

public func Fire(bool fAuto)
{
  // Schießpulver und Projektil suchen
  var pGunpowder=FindContents(GUNP), pProjectile=FindOtherContents(GUNP);
  // Schießpulver oder Projektil fehlt
  if (!pGunpowder || !pProjectile) 
  {
    Sound("Click");
    return(0);
  }
  // Schießpulver verbrauchen
  RemoveObject(pGunpowder);

  // Austritt berechnen
  var iX = (GetPhase()+3)*(GetDir()*2-1);
  var iY = GetPhase()-14;
  var iAngle = BoundBy(GetPhase()*5-5,0,90);
  var iXDir = Sin(iAngle,32*(GetDir()*2-1));
  var iYDir = Cos(iAngle,-18);
  
  // Fuer Killverfolgung
  if (GetOCF(pProjectile) & OCF_Alive) pProjectile->SetKiller(GetController());
  pProjectile->SetController(GetController());

  // Projektil abfeuern
  Exit(pProjectile,iX,iY,Random(360),iXDir * Power,iYDir * Power,+30);

  // Sicht verfolgen (wenn kein automatischer Schuss)
  if(!fAuto)
    if(GetPlrViewMode(GetController())!=2)
      SetPlrView(GetController(),pProjectile);
  // Sound
  Sound("Blast2");
  // Rauch
  for (var i = 0; i < 6; ++i) 
    Smoke(iX+RandomX(-5,+5),iY+RandomX(-5,+5),RandomX(5,12));
  // Erfolgreich geschossen
  return(1);
}
