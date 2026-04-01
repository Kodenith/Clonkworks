/*--- Gesch�tzturm ---*/

#strict

local cannon;

#include CXEC
#include BAS2
#include DOOR
#include IO__

public func InputList(){
  return(["Turn Left","Turn Right","Fire"]);
}

public func OutputList(){
  return(["Cannon Angle"]);
}

public func OutputActive(){
  if(cannon) return(GetR(cannon));
}


local LastWireFired;

public func WireAim(){
  if(DoesInputHaveWire("Turn Left") || DoesInputHaveWire("Turn Right")){
    if(!cannon) return(0);
    if(InputActive("Turn Left")){
      cannon->ComLeft(0);
    }else if(InputActive("Turn Right")){
      cannon->ComRight(0);
    }else{
      cannon->ComStop(0);
    }
  }

  if(cannon && InputActive("Fire") && LastWireFired+19 <= FrameCounter()){
    LastWireFired = FrameCounter();
    cannon->ComFire(0,true);
  }
}

/* Lore suchen und Inhalt nehmen */

private func CloseEntrance()
{
  GrabContents(FindContents(LORY));
  return(_inherited() );
}

/* Kanonensteuerung */

protected func ContainedLeft(object pObj)
{
  [$TxtRotatecannontothelef$|Image=CT01:0]
  if(!cannon) return(0);
  Sound("Click");
  return(cannon->ComLeft(pObj) );
}

protected func ContainedLeftReleased(object pObj)
{
  if(!cannon) return(0);
  Sound("CannonStop");
  return(cannon->ComStop(pObj));
}

protected func ContainedRight(object pObj)
{
  [$TxtRotatecannontotherig$|Image=CT01:2]
  if(!cannon) return(0);
  Sound("Click");
  return(cannon->ComRight(pObj) );
}

protected func ContainedRightReleased(object pObj)
{
  if(!cannon) return(0);
  Sound("CannonStop");
  return(cannon->ComStop(pObj));
}

protected func ContainedUp(object pObj)
{
  [$TxtStopcannonrotation$|Method=Classic]
  if(!cannon) return(0);

  if(!GetPlrJumpAndRunControl(pObj->GetController()))
  {
    Sound("Click");
    Sound("CannonStop");
    return(cannon->ComStop(pObj) );
  }
}

protected func ContainedUpDouble(object pObj)
{
  [$TxtDisconnectcannon$|Image=CT01:3]
  if(!cannon) return(0);
  return(ReleaseCannon());
}

protected func ContainedDig(object pObj)
{
  [$TxtFire$|Image=CT01:1]
  if (!cannon) return(0);
  return (cannon->ComFire(pObj));
}

protected func ContainedDown(object pObj)
{
  [Method=Classic]
  /* Kanone anhalten und den Clonk noch rauslassen */
  ContainedUp(pObj);
  return(0);
}

/* Kontext */

public func ContextReleaseCannon(pCaller)
{
  [$TxtDisconnectcannon$|Image=CT01:3|Condition=HasCannon]
  // Der Clonk soll erst in Gesch�tzturm laufen
  if (pCaller->Contained() != this())
  {
    SetCommand(pCaller, "None");
    AddCommand(pCaller, "Call", this(), 0,0,0,0, "ReleaseCannon");
    AddCommand(pCaller, "Enter", this());
    return(0);
  }
  // Der Clonk kann die Kanone direkt abschrauben
  return(ReleaseCannon());  
}

/* Kanone anschrauben */

protected func Collection2(object pObj)
{
  if(pObj->~IsCannon() )
  {
    if(!cannon)
      ConnectCannon(pObj);
  }
  else
  {
    if(cannon)
      cannon->~ComEnter(pObj);
  }
}

protected func Ejection(object pObj)
{
  if(cannon)
    cannon->~ComLeave(pObj);
}

public func ConnectCannon(object pCannon)
{
  // Wir rufen die Connect-Funktion der Kanone auf. Somit kann die Kanone
  // eigene Dinge tun um sich anzubauen.
  pCannon->Connect(this() );
  SetR(45, pCannon);
  if(GetX() > LandscapeWidth() / 2) SetR(-45, pCannon);
  
  // neue Kategorie f�r die Kanone um sie in den Hintergrund zu kriegen
  SetCategory(2,pCannon);
  SetObjectOrder(pCannon);

  // Die neue Kanone �ber enthaltene Objekte informieren
  var i = ContentsCount() - 1, obj;
  while(obj = Contents(--i) )
    if(!obj->~IsCannon() )
      pCannon->~ComEnter(obj);
  
  cannon = pCannon;
  Sound("Connect");
}

// Diese Funktion k�nnen die Kanonen aufrufen, um sich an den Gesch�tzturm anzuschrauben,
// wenn es darum geht, ihre Definition zu �ndern.
public func ConnectCannonDef(object pCannon, id defChg, string szAction)
{
  if(!szAction) szAction = "Attaching";
  ChangeDef(defChg, pCannon);
  ObjectSetAction(pCannon, szAction, this() ); 
}

// L�st die Kanone vom Gesch�tzturm
public func ReleaseCannon()
{
  if(!cannon) return(0);
  Sound("Connect");
  cannon->ComStopDouble();
  // Kategorie f�r die Kanone wiederherstellen
  SetCategory(GetDefCategory(GetID(cannon)),cannon);
  cannon = 0;
  
  // Neue Kanonen suchen
  var i, obj;
  while(obj = Contents(i++) )
  {
    if(obj->~IsCannon() )
    {
      ConnectCannon(obj);
      break;
    }
  }
  return(1);
}

/* Wird von den Kanonen als Turm erkannt */

public func IsCannonTower()
{
  return(1);
}

public func HasCannon()
{
  return (cannon != 0); 
}

/* Zerst�rung */

protected func Destruction()
{
  if(cannon) ReleaseCannon();
  return(_inherited() );
}

/* Abfackeln */

protected func Incineration()
{

  if(cannon) ReleaseCannon();
  ChangeDef(CTW1);
  return(_inherited() );
}

protected func IncinerationEx()
{
  if(cannon) ReleaseCannon();
  ChangeDef(CTW1);
  return(_inherited() );
}
