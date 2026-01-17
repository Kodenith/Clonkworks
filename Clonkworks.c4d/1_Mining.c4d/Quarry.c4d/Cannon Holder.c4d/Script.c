/*-- Neues Script --*/

#strict 2
local cannon;

func Initialize() {
  return(1);
}

public func ConnectCannon(object pCannon)
{
  // Wir rufen die Connect-Funktion der Kanone auf. Somit kann die Kanone
  // eigene Dinge tun um sich anzubauen.
  pCannon->Connect(this());
  SetR(180, pCannon);
  if(GetX() > LandscapeWidth() / 2) SetR(-45, pCannon);
  
  // neue Kategorie für die Kanone um sie in den Hintergrund zu kriegen
  SetCategory(2,pCannon);
  SetObjectOrder(pCannon);

  // Die neue Kanone über enthaltene Objekte informieren
  var i = ContentsCount() - 1, obj;
  while(obj = Contents(--i) )
    if(!obj->~IsCannon() )
      pCannon->~ComEnter(obj);
  
  cannon = pCannon;
  Sound("Connect");
}

// Diese Funktion können die Kanonen aufrufen, um sich an den Geschützturm anzuschrauben,
// wenn es darum geht, ihre Definition zu ändern.
public func ConnectCannonDef(object pCannon, id defChg, string szAction)
{
  if(!szAction) szAction = "Attaching";
  ChangeDef(defChg, pCannon);
  ObjectSetAction(pCannon, szAction, this() ); 
}

// Löst die Kanone vom Geschützturm
public func ReleaseCannon()
{
  if(!cannon) return(0);
  Sound("Connect");
  cannon->ComStopDouble();
  // Kategorie für die Kanone wiederherstellen
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

/* Wird von den Kanonen als Turm erkannt */

public func IsCannonTower()
{
  return(1);
}

public func HasCannon()
{
  return (cannon != 0); 
}

